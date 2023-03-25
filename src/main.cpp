// Bee Bussell
// Sept 17, 2022
// Testing animation class creator.

#include "main.h"

std::weak_ptr<BlueBridge> g_WeakBlueBridge;

int main() {
 
    const int TARGET_FPS = 60;
    const float FRAME_TIME = 1000.0f / float(TARGET_FPS); // in milliseconds
	const float UPDATE_INTERVAL = 1.0f; // update once per second
    float last_update_time = 0.0f;


    // Setup BML :3
    BML_Init();

    // Our Scene Manager
    auto SceneManager = std::make_shared<BlueSceneManager>() ;

    // Create our bWindow
    auto window = std::make_shared<bWindow>(
        "BlueKit2D Physics Test",
        0, 0, 2560, 1600
    );
    
    // Set Flags
    window->toggleResizeable();
    window->toggleHardwareRender();
    window->toggleVSync();
    window->toggleHighDPI();

    // Load the window
    std::shared_ptr<bRenderer> renderer(window->createWindow(), [](bRenderer*){});
    renderer -> background(255, 255, 255, 255);

    // Create our scenes and tell them they render to the window
    auto BasicScene = std::make_shared<Stress_Scene>("Stress", renderer);
    auto SimpleScene = std::make_shared<Simple_Scene>("Simple", renderer);
    
    // Add the scenes to the manager
    SceneManager -> AddScene(BasicScene);
    SceneManager -> AddScene(SimpleScene);

    // Load in our starting scenes
    SceneManager -> LoadScene("Simple");

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
    while(run) {

        

        // Event System (possibly only one maybe?)
        // Event loop
        run = bEvent::eventLoop();


        // Deltatime Calculations
        current = SDL_GetTicks();
	    deltaTime = (current - lastUpdate) / 1000.0f;

        // Call Updates, handles inputs and such
        SceneManager -> Update(deltaTime);
    
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
            char title[100];
            sprintf(title, "BlueKit2D Physics Test | FPS: %.2f", fps);
            window->setWindowTitle(title);

            // reset the last update time
            last_update_time = current_time;
        }

        start = SDL_GetTicks();

    }
    
    // Sound component
    // bSound::freeMUS();
    bSound::closeAudio();
        
    // Close the window
    // We do it this way because of smart pointer jank
    window.reset();

    BML_Close();
    
    return 0;
}
