# Pinokio
1. Visit (https://pinokio-home.netlify.app/) and download Pinokio. (On a Mac you have to take the app out of Quarantine using Sentinel, follow the directions included in the .dmg file)
2. Currently, the discover tab is broken in the app, so use the 1-click install button on the website.
3. All Pinokio apps live inside the api folder inside the Pinokio folder in your home directory. To remove an app, simply delete it's folder. Everything is self-contained.
4. Most apps use gradio, Hugging Face's UI framework, to create a user interface. You can make any app accessible from anywhere simply by navigating to its app.py file in its folder, finding the ```demo.launch()``` line, and changing it to ```demo.launch(share=True)```
