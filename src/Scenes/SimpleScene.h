
// Bee Bussell
// March 4, 2023
// Example Scene

#ifndef SIMPLE_SCENE_H
#define SIMPLE_SCENE_H

// Where our BML Types come from
#include <BML/bEvent.h>
#include <BML/bRect.h>
#include <BML/bRenderer.h>

// The Core needed for any scene
#include "Core/BlueBridge.h"
#include "Core/BlueTypes.h"
#include "Core/BlueScene.h"

// Systems used by Scene
#include "Systems/PhysicsSystem.h"
#include "Systems/SpriteSystem.h"

// Compnents used by Scene
#include "Components/Transform.h"
#include "Components/Sprite.h"
#include "Components/PhysicsObject.h"

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
            // Sprite image;
            // image.filePath = "../user/resources/MCaniHIGH-Start_walk.json";
            // image.context = _context;
            // image.layer = 0;

            PhysicsObject object;
            object.name = "Player";
            object.position = {static_cast<float>(loc.position.x+50), static_cast<float>(loc.position.y+50), 250, 250};
            object.maxVelocity = {1000000000, 1000000000};
            object.maxAcceleration = {1000000000, 1000000000};
            object.surfaceFriction = 0.25f;
            object.restitution = 0.001f;
            object.mass = 1.0f;
            object.type = DYNAMIC;
            
            // Add the two Components
            _bridge -> AddComponent(Sprite_Entity, loc);
            //_bridge -> AddComponent(Sprite_Entity, image);
            _bridge -> AddComponent(Sprite_Entity, object);

            _entities.push_back(Sprite_Entity);
        }

        // Creating Some Physics Entities
        
        {

            BlueEnt physics_entity = _bridge -> CreateEntity();

            Transform loc;
            loc.position = { 946, 700, 200, 700};

            PhysicsObject object;
            object.name = "Wall";
            object.position = loc.position;
            // object.maxVelocity = {10000, 10000};
            // object.maxAcceleration = {20, 20};
            object.surfaceFriction = 0.05f;
            object.restitution = 0.002f;
            object.mass = 1.0f;
            object.type = STATIC;

            _bridge -> AddComponent(physics_entity, loc);
            _bridge -> AddComponent(physics_entity, object);

            _entities.push_back(physics_entity);

        }
        
        
        {

            BlueEnt physics_entity = _bridge -> CreateEntity();

            Transform loc;
            loc.position = { 146, 1350, 50, 50};

            PhysicsObject object;
            object.name = "Box";
            object.position = loc.position;
            // object.maxVelocity = {10000, 10000};
            // object.maxAcceleration = {200000, 2000000};
            object.surfaceFriction = 0.05f;
            object.restitution = 1.0f;
            object.mass = 1.0f;
            object.type = STATIC;

            _bridge -> AddComponent(physics_entity, loc);
            _bridge -> AddComponent(physics_entity, object);

            _entities.push_back(physics_entity);

        }

        {

            BlueEnt physics_entity = _bridge -> CreateEntity();

            Transform loc;
            loc.position = { 1846, 1150, 250, 250};

            PhysicsObject object;
            object.name = "Sponge";
            object.position = loc.position;
            object.surfaceFriction = 0.05f;
            object.restitution = 1.0f;
            object.mass = 1.0f;
            object.type = STATIC;

            _bridge -> AddComponent(physics_entity, loc);
            _bridge -> AddComponent(physics_entity, object);

            _entities.push_back(physics_entity);

        }
        /**/
        {

            BlueEnt physics_entity = _bridge -> CreateEntity();

            Transform loc;
            loc.position = { 0, 1400, 2560, 200};

            PhysicsObject object;
            object.name = "Floor";
            object.position = loc.position;
            object.maxVelocity = {0, 0};
            object.maxAcceleration = {20, 20};
            object.surfaceFriction = 0.05f;
            object.restitution = 0.002f;
            object.mass = 1.0f;
            object.type = STATIC;

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

        float scalar = 200.0f;
        Force force = {0,0};

        if (bEvent::keyDown('W')) {

            force += {0,-scalar};

        }  
        if (bEvent::keyDown('A')) {

            
            force += {-scalar,0};

        }
        if (bEvent::keyDown('S')) {

            
            force += {0,scalar};

        }
        if (bEvent::keyDown('D')) {

            
            force += {scalar,0};

        }
        


        
        physics -> ApplyForce(player, force);

        // // Loop through all entities
        for (auto entity : _entities) {

            // Get the Transform Component
            Transform& transform = _bridge -> GetComponent<Transform>(entity);

            // Get the Physics Component
            PhysicsObject& physicsObject = _bridge -> GetComponent<PhysicsObject>(entity);

            // Update the Transform Component
            if (physicsObject.type == DYNAMIC) {
                
                if (physicsObject.position.x < 1150) {
                // Apply Gravity
                    physics -> ApplyForce(entity, {0, 50.8f});
                    printf("Velocity: %f, %f\n", physicsObject.velocity.x, physicsObject.velocity.y);
                }
            }
        }
        
        
        //bPointF x = _bridge -> GetComponent<PhysicsObject>(player).acceleration;
        //printf("Acceleration: %f, %f\n", x.x, x.y);

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
