// Bee Bussell
// Sept 17, 2022
// Testing animation class creator.

#include "main.h"

BlueBridge g_BlueBridge;

int main() {
 
    // Setup the Bridge
    g_BlueBridge.Init();
    g_BlueBridge.RegisterComponent<Window>();
    g_BlueBridge.RegisterComponent<Transform>();

    auto graphics = g_BlueBridge.RegisterSystem<Graphics>();    
    
    Signature signature;
    signature.set(g_BlueBridge.GetComponentType<Window>());
    signature.set(g_BlueBridge.GetComponentType<Transform>());
    g_BlueBridge.SetSystemSignature<Graphics>(signature);
    

    bool run = true;
    
    BML_Init();
    
    // Window component
    BlueEnt Window_Entity = g_BlueBridge.CreateEntity();

    Transform box;
    box.position = {0, 0, 1920, 1080};

    Window window;

    g_BlueBridge.AddComponent( Window_Entity, box);
    g_BlueBridge.AddComponent( Window_Entity, window);

    graphics -> BlueEntities.insert(Window_Entity);

    graphics->Init();

/*
    bWindow* window = new bWindow("JSON Test", 0, 0, 800, 450);
    
    // Ok looking at this a year after I developed this, I hate it
    // This should be something that should be set, like maybe a bool?
    // However now that I've peaked at bit more at this function... I get it a bit more...
    window->toggleResizeable();
    window->toggleHardwareRender();
    window->toggleVSync();
    window->toggleHighDPI();

    window->createWindow();
*/ 
    bRect dest = {10,10,128,128};
    
    // Audio Component / System
    if (!bSound::openAudio())
        printf(":(");

    //  bSound music; 
    // Music Component
    //bSound::loadMUS("../resources/BLUE-Compress.wav");
    //bSound::playMUS(5);

    
    // I think this should be something handled in the sheet function
    // SpriteComponent
    //bSheet spriteSheet;
    //readSheetFromJSON(BML_GetPath("../resources /NoelAssets/MCaniHIGH-Start_walk.json").c_str(), spriteSheet);
    //spriteSheet.startAnimation("default");

    //window->initSpriteSheet(spriteSheet);
    
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
        }
        if (bEvent::keyDown('D')) {
            dest.x++;
        }
        if (bEvent::keyDown('Q')) {
            run = false;
        }

        // Sprite Component
        //window->drawRect(dest, 255, 255, 255);
        //window->drawSprite(spriteSheet, dest);
        

        // Window Component
        graphics -> Update(0);
        //run = false;
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
