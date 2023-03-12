// Bee Bussell
// Sept 17, 2022
// Testing animation class creator.

#include "main.h"

std::weak_ptr<BlueBridge> g_WeakBlueBridge;

int main() {
 
    // Setup BML :3
    BML_Init();

    // Our Scene Manager
    auto SceneManager = std::make_shared<BlueSceneManager>() ;

    // Create our bWindow
    auto window = std::make_shared<bWindow>(
        "BlueKit2D Optimization Test",
        0, 0, 2560, 1600
    );
    
    // Set Flags
    window->toggleResizeable();
    window->toggleHardwareRender();
    window->toggleVSync();
    window->toggleHighDPI();

    // Load the window
    std::shared_ptr<bRenderer> renderer(window->createWindow(), [](bRenderer*){});
    //bRenderer *renderer = window->createWindow();
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
    bSound::loadMUS("../resources/BLUE-Compress.wav");
    bSound::playMUS(5);

    
    while(run) {

        
        // Event System (possibly only one maybe?)
        // Event loop
        run = bEvent::eventLoop();

        // Call Updates, handles inputs and such
        SceneManager -> Update(0);
    
        // Clear the renderer
        renderer -> clearBuffer();

        // Queue up things on the buffer
        SceneManager -> Render();
        //renderer -> drawRect({0,0,100,100}, 255, 0, 0 );

        // Draw Draw the buffer
        renderer -> presentBuffer();
    }
    //spriteSheet.stopAnimation();
    //
    //window->freeSpriteSheet(spriteSheet);
    
    // Sound component
    bSound::freeMUS();
    bSound::closeAudio();
    
    printf("Closing Window\n");
    
    // Close the window
    // We do it this way because of smart pointer jank
    window.reset();

    printf("Closing BML\n");
    BML_Close();
    
    return 0;
}
