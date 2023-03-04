// Bee Bussell
// Sept 17, 2022
// Testing animation class creator.

#include "main.h"

std::weak_ptr<BlueBridge> g_WeakBlueBridge;

int main() {
 
    // Setup BML :3
    BML_Init();
    BlueSceneManager SceneManager;

    auto BasicScene = std::make_shared<Template_Scene>("Basic");
    auto SecondScene = std::make_shared<Second_Scene>("Second");
    
    SceneManager.AddScene(BasicScene);
    SceneManager.AddScene(SecondScene);
    SceneManager.LoadScene("Basic");

    // Our loop variable
    bool run = true;

    bRect dest = {10,10,128,128};
    
    // Audio Component / System
    if (!bSound::openAudio())
        printf(":(");

    //  bSound music; 
    // Music Component
    //bSound::loadMUS("../resources/BLUE-Compress.wav");
    //bSound::playMUS(5);

    
    while(run) {

        
        // Event System (possibly only one maybe?)
        // Event loop
        run = bEvent::eventLoop();

        // Playable System
        if (bEvent::keyDown('W')) {
            dest.y--;
        } 
        if (bEvent::keyDown('S')) {
            dest.y++;
        }
        if (bEvent::keyDown('A')) {
            dest.x--;
            SceneManager.LoadScene("Basic");
        }
        if (bEvent::keyDown('D')) {
            dest.x++;
            SceneManager.LoadScene("Second");
        }
        if (bEvent::keyDown('Q')) {
            run = false;
        }

        SceneManager.Update(0);
        SceneManager.Render();
    }
    //spriteSheet.stopAnimation();
    //
    //window->freeSpriteSheet(spriteSheet);
    
    // Sound component
    bSound::freeMUS();
    bSound::closeAudio();
    
    //window->closeWindow();
    
    BML_Close();
    
    return 0;
}
