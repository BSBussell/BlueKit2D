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
        "BlueKit2D Scene Management",
        0, 0, 1920, 1080
    );
    
    // Set Flags
    window->toggleResizeable();
    window->toggleHardwareRender();
    window->toggleVSync();
    //window->toggleHighDPI();

    // Load the window
    window->createWindow();

    // Create our scenes and tell them they render to the window
    auto BasicScene = std::make_shared<Stress_Scene>("Stress", window);
    auto SimpleScene = std::make_shared<Simple_Scene>("Simple", window);
    
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

    
    while(run) {

        
        // Event System (possibly only one maybe?)
        // Event loop
        run = bEvent::eventLoop();

        // Playable System



        SceneManager -> Update(0);
        SceneManager -> Render();

        // Draw rendered things to window
        window -> drawBuffer();
        window -> drawRect({0,0,1920,1080}, 255, 255, 255);
    }
    //spriteSheet.stopAnimation();
    //
    //window->freeSpriteSheet(spriteSheet);
    
    // Sound component
    bSound::freeMUS();
    bSound::closeAudio();
    
    window->closeWindow();
    
    BML_Close();
    
    return 0;
}
