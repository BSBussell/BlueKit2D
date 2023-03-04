// Bee Bussell
// Sept 17, 2022
// Testing animation class creator.

#include "main.h"

BlueBridge g_BlueBridge;

int main() {
 
    // Setup BML :3
    BML_Init();

    // Our loop variable
    bool run = true;

    // Setup the Bridge
    g_BlueBridge.Init();


    // Registering Components
    g_BlueBridge.RegisterComponent<Window>();
    g_BlueBridge.RegisterComponent<Transform>();
    g_BlueBridge.RegisterComponent<Sprite>();

    // Register Graphics System
    auto graphics = g_BlueBridge.RegisterSystem<Graphics>();    
    
    // Give System Required Components of Entities
    {
        Signature signature;
        signature.set(g_BlueBridge.GetComponentType<Window>());
        signature.set(g_BlueBridge.GetComponentType<Transform>());
        g_BlueBridge.SetSystemSignature<Graphics>(signature);
    }

    // Register Sprite System
    auto sprites = g_BlueBridge.RegisterSystem<SpriteSystem>();

    // Tell the Sprite System what components to look for :3
    {
        Signature signature;
        signature.set(g_BlueBridge.GetComponentType<Sprite>());
        signature.set(g_BlueBridge.GetComponentType<Transform>());
        g_BlueBridge.SetSystemSignature<SpriteSystem>(signature);
    }
    
    // Making the Window Entity
    BlueEnt Window_Entity = g_BlueBridge.CreateEntity();

    // Component Transform and Window for the entity
    Transform box;
    box.position = {0, 0, 1920, 1080};

    Window window;
    window.name = "BlueKit2D ECS Integration";

    // Adding the components to the entity
    g_BlueBridge.AddComponent( Window_Entity, box);
    g_BlueBridge.AddComponent( Window_Entity, window);


    // Call the graphics systems initialization
    graphics->Init();

    // Draw a fuck ton of animated sprites (this is a stress test :3)
    for (int i = 0; i < 10; i++) {
    for (int j = 0; j < 10; j++) {
        // Making the sprite entity
        BlueEnt Sprite_Entity = g_BlueBridge.CreateEntity();

        // Setup Transform Component
        Transform loc;
        loc.position = { 175*i, 175*j, 700, 700};

        // Setup Sprite Component
        Sprite image;
        image.filePath = "../user/resources/MCaniHIGH-Start_walk.json";
        image.context = std::weak_ptr(g_BlueBridge.GetComponent<Window>(Window_Entity).window);
        image.layer = i*(j+1);

        // Add the two Components
        g_BlueBridge.AddComponent(Sprite_Entity, loc);
        g_BlueBridge.AddComponent(Sprite_Entity, image);
    }
    }
    // Initialize
    sprites->Init();

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
        }
        if (bEvent::keyDown('D')) {
            dest.x++;
        }
        if (bEvent::keyDown('Q')) {
            run = false;
        }

        // Sprite Component
        //window->drawRect(dest, 255, 255, 255);
        

        // Window Component
        graphics -> Update(0);

        // Update Sprites
        sprites -> Update();
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
