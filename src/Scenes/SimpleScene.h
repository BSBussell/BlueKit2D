
// Bee Bussell
// March 4, 2023
// Example Scene

#ifndef SIMPLE_SCENE_H
#define SIMPLE_SCENE_H

#include "Core/BlueBridge.h"
#include "Core/BlueTypes.h"
#include "Core/BlueScene.h"

/*
    This is how we're gonna be programming Scenes
    _bridge is your connection to the ECS
*/


class Simple_Scene : public BlueScene {

public:

    Simple_Scene(std::string name, std::shared_ptr<bWindow> context) : 
        BlueScene(name, context) {}

    // This is called when a scene is first started up
    void Load() override {

        // ALWAYS DO THIS
        _bridge -> Init();

        // Loading Essentials :3
        _Register_Components();
        _Register_Systems();

        // Making the sprite entity
        BlueEnt Sprite_Entity = _bridge -> CreateEntity();

        // Setup Transform Component
        Transform loc;
        loc.position = { 64, 64, 700, 700};

        // Setup Sprite Component
        Sprite image;
        image.filePath = "../user/resources/MCaniHIGH-Start_walk.json";
        image.context = _context;
        image.layer = 0;

        // Add the two Components
        _bridge -> AddComponent(Sprite_Entity, loc);
        _bridge -> AddComponent(Sprite_Entity, image);

        // Initialize Sprites
        sprites->Init();

    }

    // Called when a scene is being unloaded
    void Unload() override {
        // implementation
        // HAHA what you want me to clean???
        sprites -> Close();
    }

    void Update(float deltaTime) override {
        // implementation
        
        if (bEvent::keyDown('A')) {

            // TODO: Implement a checker
            _stage.lock() -> LoadScene("Stress");
        }
        sprites -> Update();
        
    }

    void Render() override {
        // implementation
        sprites -> Render();
    }

private:
    void _Register_Components() override {
        
        // Registering Components
        _bridge -> RegisterComponent<Transform>();
        _bridge -> RegisterComponent<Sprite>();
    }

    void _Register_Systems() override {
        // implementation

        // Register Sprite System
        sprites = _bridge -> RegisterSystem<SpriteSystem>();

        // Tell the Sprite System what components to look for :3
        {
            Signature signature;
            signature.set(_bridge -> GetComponentType<Sprite>());
            signature.set(_bridge -> GetComponentType<Transform>());
            _bridge -> SetSystemSignature<SpriteSystem>(signature);
        }
    }

    // List our Systems
    std::shared_ptr<SpriteSystem> sprites;

};

#endif
