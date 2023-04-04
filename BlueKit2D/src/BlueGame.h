//  Bee Bussell
//  March 27, 2023
//  The Game

#ifndef BLUEKIT2D_BLUEGAME_H
#define BLUEKIT2D_BLUEGAME_H

// System Libraries we want
#include <iostream>
#include <string>
#include <stdint.h>
#include <memory>

#include "BlueKit2D.h"

// BML is our custom wrapper for SDL
#include <BML/BML.h>

// Including Core Files
#include "Core/BlueBridge.h"
#include "Core/BlueSceneManager.h"


// Component Files
#include "Components/Sprite.h"
#include "Components/Transform.h"
#include "Components/Window.h"
#include "Components/PhysicsObject.h"

// System Includes
#include "Systems/SpriteSystem.h"
#include "Systems/PhysicsSystem.h"


class BlueGame {

public:
	BlueGame(std::shared_ptr<BlueScene> scene);
	~BlueGame();

	void Run();

	void AddScene(std::shared_ptr<BlueScene> scene);

	// Set the Windows Title
	void SetTitle(std::string title) { this->title = title; }

	// Set the Windows Size
	void SetWindowSize(bPoint size) { window_dimensions = size; }

	// Set if we should display the FPS
	void SetDisplayFPS(bool display) { display_fps = display; }

	// Target FPS
	int target_fps;

private:

	// Window Title
	std::string title;

	// Window Size
	bPoint window_dimensions;

	// Display FPS
	bool display_fps = true;




	// The Scene Manager
	std::shared_ptr<BlueSceneManager> SceneManager;

	// The Bridge
	std::shared_ptr<BlueBridge> bridge;

	// The Window
	std::shared_ptr<bWindow> window;

	// The Renderer
	std::shared_ptr<bRenderer> renderer;

};


#endif //BLUEKIT2D_BLUEGAME_H
