
// Bee Bussell
// Mar 4, 2024
// GraphicsSystem.cpp

#include "GraphicsSystem.h"

extern std::weak_ptr<BlueBridge> g_WeakBlueBridge;

// Ok so Now I should be able to create a new window entity
// Set up it's components so that it has transform and window and assign those values
// Call init before running loop
// Call update inside loop

void Graphics::Init() {
	
	std::shared_ptr g_BlueBridgePtr = g_WeakBlueBridge.lock();
	if (!g_BlueBridgePtr) {

		perror("GIRL WHERE'D THE BRIDGE GO!!!");
		exit(0);
	}

	for (auto const& entity : BlueEntities) {

		Transform &transform = g_BlueBridgePtr -> GetComponent<Transform>(entity);
		Window &window = g_BlueBridgePtr -> GetComponent<Window>(entity);

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
	
	std::shared_ptr g_BlueBridgePtr = g_WeakBlueBridge.lock();
	if (!g_BlueBridgePtr) {

		perror("GIRL GRAPHICS UPDATE METHOD LOST THE BRIDGE!!!");
		exit(1);
	}

	for (auto const& entity : BlueEntities) {
		//Transform &transform = g_BlueBridge.GetComponent<Transform>(entity);
		Window &window = g_BlueBridgePtr -> GetComponent<Window>(entity);

		window.window -> drawBuffer();
		window.window -> drawRect({0,0,1920,1080}, 255, 255, 255);
	}
	// Draw Buffer command goes here, along with whatever else
}

void Graphics::Close() {

	std::shared_ptr g_BlueBridgePtr = g_WeakBlueBridge.lock();
	if (!g_BlueBridgePtr) {

		perror("GIRL GRAPHICS UPDATE METHOD LOST THE BRIDGE!!!");
		exit(1);
	}

	for (auto const& entity : BlueEntities) {
		//Transform &transform = g_BlueBridge.GetComponent<Transform>(entity);
		Window &window = g_BlueBridgePtr -> GetComponent<Window>(entity);

		
		window.window -> closeWindow();
	}

}