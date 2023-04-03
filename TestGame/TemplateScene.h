
// Bee Bussell
// March 4, 2023
// Example Scene

#ifndef TEMPLATE_SCENE_H
#define TEMPLATE_SCENE_H

#include "Core/BlueBridge.h"
#include "Core/BlueTypes"
#include "Core/BlueScene.h"

/*
    This is how we're gonna be programming Scenes
    _bridge is your connection to the ECS
*/


class Template : public BlueScene {

public:
    Template(std::string name) : BlueScene(name) {}

    void Load() override {

        // Loading Essentials :3
        _Register_Components();
        _Register_Systems();

        // Making the Window Entity
        BlueEnt Window_Entity = _bridge -> CreateEntity();

        // Component Transform and Window for the entity
        Transform box;
        box.position = {0, 0, 1920, 1080};

        Window window;
        window.name = "BlueKit2D ECS Integration";

        // Adding the components to the entity
        _bridge -> AddComponent( Window_Entity, box);
        _bridge -> AddComponent( Window_Entity, window);


        // Call the graphics systems initialization
        graphics->Init();

        // Draw a fuck ton of animated sprites (this is a stress test :3)
        for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            // Making the sprite entity
            BlueEnt Sprite_Entity = _bridge -> CreateEntity();

            // Setup Transform Component
            Transform loc;
            loc.position = { 175*i, 175*j, 700, 700};

            // Setup Sprite Component
            Sprite image;
            image.filePath = "../TestGame/resources/MCaniHIGH-Start_walk.json";
            image.context = std::weak_ptr(_bridge -> GetComponent<Window>(Window_Entity).window);
            image.layer = i*(j+1);

            // Add the two Components
            _bridge -> AddComponent(Sprite_Entity, loc);
            _bridge -> AddComponent(Sprite_Entity, image);
        }
        }
        // Initialize Sprites
        sprites->Init();

    }

    void Unload() override {
        // implementation
        // HAHA what you want me to clean???
    }

    void Update(float deltaTime) override {
        // implementation
        graphics -> Update();
        sprites -> Update();
    }

    void Render() override {
        // implementation
    }

private:
    void _Register_Components() override {
        
        // Registering Components
        _bridge -> RegisterComponent<Window>();
        _bridge -> RegisterComponent<Transform>();
        _bridge -> RegisterComponent<Sprite>();
    }

    void _Register_Systems() override {
        // implementation

        // Register Graphics System
        graphics = _bridge -> RegisterSystem<Graphics>();    
        
        // Give System Required Components of Entities
        {
            Signature signature;
            signature.set(_bridge -> GetComponentType<Window>());
            signature.set(_bridge -> GetComponentType<Transform>());
            _bridge -> SetSystemSignature<Graphics>(signature);
        }

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

    // Our Systems
    Graphics graphics;
    SpritSystem sprite;

};

#endif // MY_SCENE_H
