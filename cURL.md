# Call a Gemini large language model from the command line with cURL

  * **Make sure to replace *YOUR_API_KEY* with your actual API key**  
  (You can generate an API key in [Google AI Studio](aistudio.google.com))
  * Paste the command below into a text editor and change the  
    value after "text" into whatever prompt you like.
  * You should get a response in a similar format.

```
curl "https://generativelanguage.googleapis.com/v1beta/models/gemini-2.0-flash-lite:generateContent?key=YOUR_API_KEY" \
  -H 'Content-Type: application/json' \
  -X POST \
  -d '{
    "contents": [
      {
        "parts": [
          {
            "text": "Explain how AI works in a few words"
          }
        ]
      }
    ]
  }'
```

#### Generate an image according to the prompt and save it in the current folder as "gemini.png":

```
curl "https://generativelanguage.googleapis.com/v1beta/models/gemini-2.0-flash-preview-image-generation:generateContent?key=YOUR_API_KEY" \
  -H 'Content-Type: application/json' \
  -X POST \
  -d '{
    "contents": [
      {
        "parts": [
          {
            "text": "Hi, can you create a group portrait of the attendees of Fidget Camp?"
          }
        ]
      }
    ],
    "generationConfig":{"responseModalities":["TEXT","IMAGE"]}
  }' | grep -o '"data": "[^"]*"' | cut -d'"' -f4 | base64 --decode > gemini.png
```
