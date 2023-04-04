
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


		// Making a sprite entity
		BlueEnt Sprite_Entity = _bridge -> CreateEntity();

		// I build the sprite entity in a block so that the component variable names
		// Can be reused for other entities
		// Also I like being able to collapse the block
		{
			// Setup Transform Component
			Transform loc;
			loc.position = {175, 175, 700, 700};

			// Setup Sprite Component
			Sprite image;
			image.filePath = "resources/MCaniHIGH-Start_walk.json";
			image.context = std::weak_ptr(_bridge->GetComponent<Window>(Window_Entity).window);
			image.layer = 0;

			// Add the two Components
			_bridge->AddComponent(Sprite_Entity, loc);
			_bridge->AddComponent(Sprite_Entity, image);
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
    SpritSystem sprite;

};

#endif // MY_SCENE_H
