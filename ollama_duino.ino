/*
  Ollama API Client for Arduino

  This sketch demonstrates how to send a request to a local Ollama API
  using an Arduino with WiFi capabilities. It is adapted from a Google
  Gemini client sketch.

  It performs an HTTP POST request with a JSON body and parses the
  JSON response to extract the generated content. This sketch assumes your
  Ollama instance is accessible on your local network.

  This sketch will work with:
  - MKR1010, Nano 33 IoT, Uno R4 WiFi (with WiFiNINA or WiFiS3 library)
  - ESP32-based boards (with WiFi.h library)

  Required Libraries:
  - ArduinoHttpClient: http://librarymanager/All#ArduinoHttpClient
  - Arduino_JSON: http://librarymanager/All#Arduino_JSON

  To make sure Ollama is exposed to the network on a Mac,
  run `launchctl setenv OLLAMA_HOST "0.0.0.0"`
  For other operating systems, see:
  https://aident.ai/blog/how-to-expose-ollama-service-api-to-network

*/

// Step 1: Choose the correct WiFi library for your board
#include <WiFiNINA.h>  // For MKR WiFi 1010, Nano 33 IoT, etc.
// #include <WiFiS3.h>      // For Uno R4 WiFi
// #include <WiFi.h>        // For ESP32 boards like Nano ESP32

// Step 2: Include required libraries
#include <ArduinoHttpClient.h>
#include <Arduino_JSON.h>
#include "arduino_secrets.h"  // Contains your WiFi credentials and Ollama Server IP

// --- Ollama Configuration ---
// The IP address of the computer running Ollama.
// This MUST be defined in your arduino_secrets.h file.
// Example: const char OLLAMA_SERVER_IP[] = "192.168.1.123";
const char server[] = "192.168.1.10";
const int port = 11434;  // Default Ollama API port (HTTP)

// The model to use for the generation
String ollamaModel = "gemma3:4b";

// The prompt
String prompt = "Who do you think attends Fidget Camp? Answer in one sentence.";

// Repetition configuration
// Set to true to make the API call only once in setup().
// Set to false to make the API call repeatedly in loop().
const bool RUN_ONCE = true;
long lastRequestTime = 0;
const long requestInterval = 30000;  // 30 seconds between requests

// Initialize the WiFi and HTTP clients
WiFiClient wifiClient;
HttpClient client = HttpClient(wifiClient, server, port);

void setup() {
  Serial.begin(115200);
  // Wait for the serial monitor to open (optional)
  while (!Serial)
    ;

  // Connect to WiFi
  connectToWiFi();

  // If RUN_ONCE is true, make the call here and we're done.
  if (RUN_ONCE) {
    makeApiRequest();
  }
}

void loop() {
  // Send a request every `requestInterval` milliseconds
  if (!RUN_ONCE) {
    if (millis() - lastRequestTime > requestInterval) {
      makeApiRequest();
      lastRequestTime = millis();
    }
  }
}

void connectToWiFi() {
  Serial.print("Attempting to connect to WiFi: ");
  Serial.println(SECRET_SSID);

  while (WiFi.status() != WL_CONNECTED) {
    WiFi.begin(SECRET_SSID, SECRET_PASS);
    delay(2000);
    Serial.print(".");
  }

  Serial.println("\nConnected to WiFi!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}

void makeApiRequest() {
  Serial.println("\n========================================");
  Serial.println("Making API request to Ollama...");

  // 1. Construct the API route
  String route = "/api/generate";

  // 2. Construct the JSON body based on Ollama's format
  // We set "stream" to false to get the full response at once.
  String requestBody = R"({"model":")" + ollamaModel + R"(", "prompt":")" + prompt + R"(", "stream": false})";

  // 3. Set the content type header
  String contentType = "application/json";

  Serial.println("Sending POST request to: http://" + String(server) + ":" + String(port) + route);
  Serial.println("Request body: " + requestBody);

  // 4. Make the POST request
  client.post(route, contentType, requestBody);

  // 5. Read the response
  int statusCode = client.responseStatusCode();
  String responseBody = client.responseBody();

  Serial.println("\n--- Response ---");
  Serial.print("Status code: ");
  Serial.println(statusCode);
  Serial.print("Full Response Body:\n");
  Serial.println(responseBody);

  // 6. Parse the JSON response to extract the generated text
  if (statusCode == 200) {
    JSONVar responseObject = JSON.parse(responseBody);

    // Check if JSON parsing was successful
    if (JSON.typeof(responseObject) == "undefined") {
      Serial.println("JSON parsing failed! The response may be too large for the Arduino's memory.");
      return;
    }

    // Navigate the JSON structure to get the text
    // The path for Ollama is simply: response
    if (responseObject.hasOwnProperty("response")) {
      String generatedText = (const char*)responseObject["response"];
      Serial.println("\n--- Extracted Text ---");
      Serial.println(generatedText);
    } else {
      Serial.println("\nCould not find 'response' key in the JSON. The response might be an error message.");
    }
  } else {
    Serial.println("Request failed. Check your Ollama server IP and network connection.");
  }
  Serial.println("========================================");
}
