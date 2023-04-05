//  Bee Bussell
//  March 27, 2023
//  The Game

#include "BlueGame.h"

BlueGame::BlueGame(std::shared_ptr<BlueScene> scene, std::string title) {



	// Setup BML :3
	BML_Init();


	// Our Scene Manager
	SceneManager = std::make_shared<BlueSceneManager>() ;

	// Create our bWindow
	this -> title = title;
	window = std::make_shared<bWindow>(
			this -> title.c_str(), 0, 0, 2560, 1600
	);

	// Set Flags
	window->toggleResizeable();
	window->toggleHardwareRender();
	window->toggleVSync();
	window->toggleHighDPI();

	// Load the window
	std::shared_ptr<bRenderer> context(window->createWindow(), [](bRenderer*){});
	renderer = context;
	renderer -> background(255, 255, 255, 255);

	// Assign Context to the Scene
	scene -> AssignContext(renderer);

	// Add the scenes to the manager
	SceneManager -> AddScene(scene);

	// Load in our starting scenes
	SceneManager -> LoadScene("init");

	// Set the target FPS
	target_fps = 60;

}

BlueGame::~BlueGame() {

	// HAHAH you expect me to clean up after myself?!
	///... fine

	// Close the window
	// We do it this way because of smart pointer jank
	window.reset();

	BML_Close();
}

void BlueGame::Run() {

	const int TARGET_FPS = target_fps;
	float FRAME_TIME = 1000.0f / float(TARGET_FPS); // in milliseconds
	float UPDATE_INTERVAL = 1.0f; // update once per second
	float last_update_time = 0.0f;


	// Our loop variable
	bool run = true;

	// Audio Component / System
	if (!bSound::openAudio())
		printf(":(");

	// bSound music;
	// Music Component
	// bSound::loadMUS("../resources/BLUE-Compress.wav");
	// bSound::playMUS(5);

	Uint32 current;
	Uint32 lastUpdate = SDL_GetTicks();
	float deltaTime = 0.0f;
	Uint32 start = SDL_GetTicks();

	while (run) {
		// Event System (possibly only one maybe?)
		// Event loop
		run = bEvent::eventLoop();


		// Deltatime Calculations
		current = SDL_GetTicks();
		deltaTime = (current - lastUpdate) / 1000.0f;

		// Call Updates, handles inputs and such
		if (SceneManager -> Update(deltaTime) == false)
			continue;

		// Set update time
		lastUpdate = current;

		// Clear the renderer
		renderer -> clearBuffer();

		// Queue up things on the buffer
		SceneManager -> Render();

		// Draw the buffer
		renderer -> presentBuffer();

		// Frame Rate Calculations
		Uint32 end = SDL_GetTicks();
		Uint32 elapsed = end - start;
		if (elapsed < FRAME_TIME) {
			SDL_Delay(FRAME_TIME - elapsed);
		}


		// Calculate FPS for display
		float fps = 1000.0f / (SDL_GetTicks() - start);
		// check if enough time has passed since the last title update
		float current_time = SDL_GetTicks() / 1000.0f; // convert to seconds
		if (current_time - last_update_time >= UPDATE_INTERVAL) {
			// update the window title with the current fps
			char FPS_title[100];
			sprintf(FPS_title, "%s | FPS: %.2f", title.c_str(), fps);
			window->setWindowTitle(FPS_title);

			// reset the last update time
			last_update_time = current_time;
		}

		start = SDL_GetTicks();
	}

}

void BlueGame::AddScene(std::shared_ptr<BlueScene> scene) {

	// Assign Context to the Scene and add it to the manager
	scene -> AssignContext(renderer);
	SceneManager->AddScene(scene);
}
