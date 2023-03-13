
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

    Simple_Scene(std::string name, std::shared_ptr<bRenderer> context) : 
        BlueScene(name, context) {}

    // This is called when a scene is first started up
    void Load() override {

        // ALWAYS DO THIS
        _bridge -> Init();

        // Loading Essentials :3
        _Register_Components();
        _Register_Systems();

        // Creating Sprite Entity
        {
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

            PhysicsObject object;
            object.position = loc.position;
            object.maxVelocity = {1000, 1000};
            object.maxAcceleration = {1000, 1000};
            object.surfaceFriction = 0.5f;
            object.restitution = 0.5f;
            
            // Add the two Components
            _bridge -> AddComponent(Sprite_Entity, loc);
            _bridge -> AddComponent(Sprite_Entity, image);
            _bridge -> AddComponent(Sprite_Entity, object);

            _entities.push_back(Sprite_Entity);
        }

        // Creating Physics Entity
        {

            BlueEnt physics_entity = _bridge -> CreateEntity();

            Transform loc;
            loc.position = { 946, 128, 100, 700};

            PhysicsObject object;
            object.position = loc.position;
            object.maxVelocity = {0, 0};
            object.maxAcceleration = {20, 20};
            object.surfaceFriction = 1.0f;
            object.restitution = 0.5f;
            object.mass = 1000.0f;

            _bridge -> AddComponent(physics_entity, loc);
            _bridge -> AddComponent(physics_entity, object);

            _entities.push_back(physics_entity);

        }

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
        BlueEnt player = _entities.front();

        if (bEvent::keyDown('W')) {

            physics -> ApplyForce(player, {0,-50});

        }  
        if (bEvent::keyDown('A')) {

            physics -> ApplyForce(player, {-50,0});

        } 
        if (bEvent::keyDown('S')) {

            physics -> ApplyForce(player, {0,50});

        } 
        if (bEvent::keyDown('D')) {

            physics -> ApplyForce(player, {50,0});

        }
        
        physics -> Update(deltaTime);
        sprites -> Update();
        
    }

    void Render() override {
        // implementation
        physics -> Render(_context);
        sprites -> Render();
    }

private:
    void _Register_Components() override {
        
        // Registering Components
        _bridge -> RegisterComponent<Transform>();
        _bridge -> RegisterComponent<Sprite>();
        _bridge -> RegisterComponent<PhysicsObject>();

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

        physics = _bridge -> RegisterSystem<PhysicsSystem>();

        // Tell the Physics System what components to look for :3
        {
            Signature signature;
            signature.set(_bridge -> GetComponentType<PhysicsObject>());
            _bridge -> SetSystemSignature<SpriteSystem>(signature);
        }

    }

    // List our Systems
    std::shared_ptr<SpriteSystem> sprites;
    std::shared_ptr<PhysicsSystem> physics;

};

#endif
