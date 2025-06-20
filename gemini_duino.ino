/*
  Google Gemini API Client for Arduino

  This sketch demonstrates how to send a request to the Google Gemini API
  using an Arduino with WiFi capabilities. It is based on the provided
  curl command and adapts examples from the tigoe-wifi_examples library.

  It performs an HTTPS POST request with a JSON body and parses the
  JSON response to extract the generated content.

  This sketch will work with:
  - MKR1010, Nano 33 IoT, Uno R4 WiFi (with WiFiNINA or WiFiS3 library)
  - ESP32-based boards (with WiFi.h library)

  Required Libraries:
  - ArduinoHttpClient: http://librarymanager/All#ArduinoHttpClient
  - Arduino_JSON: http://librarymanager/All#Arduino_JSON
*/

// Step 1: Choose the correct WiFi library for your board
#include <WiFiNINA.h>  // For MKR WiFi 1010, Nano 33 IoT, etc.
// #include <WiFiS3.h>      // For Uno R4 WiFi
// #include <WiFi.h>        // For ESP32 boards like Nano ESP32

// Step 2: Include required libraries
#include <ArduinoHttpClient.h>
#include <Arduino_JSON.h>
#include "arduino_secrets.h"  // Contains your WiFi credentials and API Key

// WiFi and network configuration
const char server[] = "generativelanguage.googleapis.com";
const int port = 443;  // HTTPS port

// Repetition configuration
// Set to true to make the API call only once in setup().
// Set to false to make the API call repeatedly in loop().
const bool RUN_ONCE = true;
long lastRequestTime = 0;
const long requestInterval = 30000;  // 30 seconds between requests

// Initialize the WiFi and HTTP clients
WiFiSSLClient wifiClient;
HttpClient client = HttpClient(wifiClient, server, port);

void setup() {
  Serial.begin(9600);
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
  Serial.println("Making API request to Google Gemini...");

  // 1. Construct the API route
  String route = "/v1beta/models/gemini-2.0-flash:generateContent?key=";
  route += SECRET_API_KEY;

  // 2. Construct the JSON body
  // Using a C++ raw string literal (R"()") makes it easy to write JSON
  // without escaping every quote character.
  String prompt = "Describe the most beautiful sunset imaginable in three sentences.";
  String requestBody = R"({"contents":[{"parts":[{"text":")" + prompt + R"("}]}]})";

  // 3. Set the content type header
  String contentType = "application/json";

  Serial.println("Sending POST request to: " + String(server) + route);
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
      Serial.println("JSON parsing failed!");
      return;
    }

    // Navigate the JSON structure to get the text
    // The path is: candidates -> [0] -> content -> parts -> [0] -> text
    if (responseObject.hasOwnProperty("candidates")) {
      String generatedText = (const char*)responseObject["candidates"][0]["content"]["parts"][0]["text"];
      Serial.println("\n--- Extracted Text ---");
      Serial.println(generatedText);
    } else {
      Serial.println("\nCould not find 'candidates' in the response. It might be an error message.");
      Serial.print("Error: ");
      Serial.println((const char*)responseObject["error"]["message"]);
    }
  } else {
    Serial.println("Request failed. Check your API key and network connection.");
  }
  Serial.println("========================================");
}
