# Ollama
 1. Install Ollama from https://ollama.com/download
 2. Check out available [models](https://ollama.com/search)
 3. ```ollama pull MODEL_NAME``` e.g.: ```ollama pull gemma3:4b```
 4. ```ollama run MODEL_NAME``` e.g.: ```ollama run gemma3:4b```
 5. Try using a model to describe an image (not all models can do this):  
```ollama run gemma3:4b 'Describe any people you see in this image path/to/image'```
 6. Try running ```curl http://localhost:11434/api/generate -d '{"model": "gemma3:4b", "prompt":"Why is the sky blue? Answer in one sentence.", "stream":false}'```
 7. If you want Ollama to be accessible on your network, on a Mac run ```launchctl setenv OLLAMA_HOST "0.0.0.0"``` from the command line and restart the Ollama application. For other operating systems, see:
(https://aident.ai/blog/how-to-expose-ollama-service-api-to-network)
 8. Call it on my machine (replace **MY_IP** with whatever my IP happens to be): ```curl http://MY_IP:11434/api/generate -d '{ "model": "llama3.2", "prompt":"Who should attend Fidget Camp? Answer in one sentence.", "stream":false }'```
