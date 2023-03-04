
// Bee Bussell
// Mar 4, 2024
// GraphicsSystem.cpp

#include "GraphicsSystem.h"

extern BlueBridge g_BlueBridge;

// Ok so Now I should be able to create a new window entity
// Set up it's components so that it has transform and window and assign those values
// Call init before running loop
// Call update inside loop

void Graphics::Init() {
	
	for (auto const& entity : BlueEntities) {

		Transform &transform = g_BlueBridge.GetComponent<Transform>(entity);
		Window &window = g_BlueBridge.GetComponent<Window>(entity);

		bRect position = transform.position;


		// This is where we initialize the window, set any flags, etc.
		// Window component
	    window.window = std::make_shared<bWindow>(window.name.c_str(), position.x, position.y, position.width, position.height);
	    
	    // Ok looking at this a year after I developed this, I hate it
	    // This should be something that should be set, like maybe a bool?
	    // However now that I've peaked at bit more at this function... I get it a bit more...
	    window.window->toggleResizeable();
	    window.window->toggleHardwareRender();
	    window.window->toggleVSync();
	    window.window->toggleHighDPI();

	    window.window->createWindow();
	}

	
}

void Graphics::Update() {
	


	for (auto const& entity : BlueEntities) {
		//Transform &transform = g_BlueBridge.GetComponent<Transform>(entity);
		Window &window = g_BlueBridge.GetComponent<Window>(entity);

		window.window -> drawBuffer();
	}
	// Draw Buffer command goes here, along with whatever else
}