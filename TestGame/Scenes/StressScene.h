
// Bee Bussell
// March 4, 2023
// Example Scene

#ifndef STRESS_SCENE_H
#define STRESS_SCENE_H

#include <cstdlib>

// Only the BlueKit2D Headers we need for a scene
#include <BlueKit2D/Core/BlueBridge.h>
#include <BlueKit2D/Core/BlueTypes.h>
#include <BlueKit2D/Core/BlueScene.h>

/*
    This is how we're gonna be programming Scenes
    _bridge is your connection to the ECS
*/


class Stress_Scene : public BlueScene {

public:

    Stress_Scene(std::string name, std::shared_ptr<bRenderer> context) : 
        BlueScene(name, context) {}

	Stress_Scene(std::string name) :
		BlueScene(name) {}

    void Load() override {

        // ALWAYS DO THIS
        _bridge -> Init();

        // Loading Essentials :3
        _Register_Components();
        _Register_Systems();

        // Draw a fuck ton of animated sprites (this is a stress test :3)
        // 119 x 45 = 5355 animated sprites at once
        for (Uint32 i = 0; i < 119; i++) 
        for (Uint32 j = 0; j < 45; j++) {
            // Making the sprite entity
            BlueEnt Sprite_Entity = _bridge -> CreateEntity();

            // Setup Transform Component
            Transform loc;
            loc.position = { 32*i, 45*j, 64, 64};

            // Setup Sprite Component
            Sprite image;

			// Ok So FOR SOME REASON, our running directory is TestGame/
            image.filePath = "resources/MCaniHIGH-Start_walk.json";
            image.context = _context;
            image.layer = i*(j+1);

            // Add the two Components
            _bridge -> AddComponent(Sprite_Entity, loc);
            _bridge -> AddComponent(Sprite_Entity, image);

            AddEntity(Sprite_Entity);
        }

        // Initialize Sprites
        sprites->Init();

    }

    void Unload() override {

        sprites -> Close();

        for (auto &entity: _entities) {

            _bridge -> DestroyEntity(entity);
        }

        _entities.clear();

    }

    bool Update(float deltaTime) override {
        // implementation

        // Lol this'd be a nightmare of operations
        //sprites -> Update();
        if (bEvent::keyDown('D')) {
            
            _stage.lock() -> LoadScene("init");
			return false;
        }

		return true;
    }

    void Render() override {
        // implementation
		printf("Rendering Stress Scene\n");
        sprites -> Render();
    }

private:

    // Register Components we want to use
    void _Register_Components() override {
        
        // Registering Components
        _bridge -> RegisterComponent<Transform>();
        _bridge -> RegisterComponent<Sprite>();
    }

    // Register Systems we want to use
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
    std::shared_ptr<SpriteSystem> sprites;

};

#endif
