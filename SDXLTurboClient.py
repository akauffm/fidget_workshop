# A minimal client to interact with a model served via Pinokio
# The prediction part will be specific to the app, click on the
# API tab at the bottom of the Gradio UI to see the particulars.

from gradio_client import Client

client = Client("http://127.0.0.1:7860/")
result = client.predict(
		"A pig with wings",	# str  in 'Prompt' Textbox component
		None,	# str (filepath on your computer (or URL) of image) in 'image' Image component
		api_name="/predict"
)
print(result)
