
// Bee Bussell
// March 4, 2023
// Simple Scene

#ifndef SIMPLE_SCENE_H
#define SIMPLE_SCENE_H

// Where our BML Types come from
#include <BML/bEvent.h>
#include <BML/bRect.h>
#include <BML/bRenderer.h>

// The Core needed for any scene
#include <BlueKit2D/Core/BlueTypes.h>
#include <BlueKit2D/Core/BlueTypes.h>
#include <BlueKit2D/Core/BlueScene.h>

// Systems used by Scene
#include <BlueKit2D/Systems/PhysicsSystem.h>
#include <BlueKit2D/Systems/SpriteSystem.h>

// Compnents used by Scene
#include <BlueKit2D/Components/Transform.h>
#include <BlueKit2D/Components/Sprite.h>
#include <BlueKit2D/Components/PhysicsObject.h>

/*
    This is how we're gonna be programming Scenes
    _bridge is your connection to the ECS
*/


class Simple_Scene : public BlueScene {

public:

    Simple_Scene(std::string name, std::shared_ptr<bRenderer> context) : 
        BlueScene(name, context) {}

	Simple_Scene(std::string name) :
			BlueScene(name) {}

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
            loc.position = { 164, 364, 350, 350};

			// Setup Sprite Component
			Sprite image;
			image.filePath = "resources/MCaniHIGH-Start_walk.json";
			image.context = _context;
			image.layer = 0;


            PhysicsObject object;
            object.name = "Player";
            object.bounding_box = {static_cast<float>(loc.position.x+100), static_cast<float>(loc.position.y+50), 150, 250};
            object.maxVelocity = {5000, 5000};
            object.maxAcceleration = {1000000000, 1000000000};
            object.friction = 0.25f;
            object.restitution = 0.0f;
            object.mass = 0.1f;
			object.snap = true;
            object.type = ACTOR;
            
            // Add the two Components
            _bridge -> AddComponent(Sprite_Entity, loc);
            _bridge -> AddComponent(Sprite_Entity, image);
            _bridge -> AddComponent(Sprite_Entity, object);

            _entities.push_back(Sprite_Entity);
        }

        // Creating Some Physics Entities
        
        {

            BlueEnt physics_entity = _bridge -> CreateEntity();

            Transform loc;
            loc.position = { 1146, 128, 450, 450};

            PhysicsObject object;
            object.name = "Big Block";
            object.bounding_box = loc.position;
            object.maxVelocity = {10000, 10000};
            object.maxAcceleration = {20, 20};
            object.friction = 0.05f;
            object.restitution = 0.0f;
            object.mass = 100.0f;
            object.type = SOLID;

            _bridge -> AddComponent(physics_entity, loc);
            _bridge -> AddComponent(physics_entity, object);

            _entities.push_back(physics_entity);

        }
        
        
        {

            BlueEnt physics_entity = _bridge -> CreateEntity();

            Transform loc;
            loc.position = { 1726, 1028, 50, 50};

            PhysicsObject object;
            object.name = "Box";
            object.bounding_box = loc.position;
            object.maxVelocity = {10000, 10000};
            object.maxAcceleration = {200000, 2000000};
            object.friction = 0.05f;
            object.restitution = 0.0f;
            object.mass = 0.750f;
            object.type = SOLID;

            _bridge -> AddComponent(physics_entity, loc);
            _bridge -> AddComponent(physics_entity, object);

            _entities.push_back(physics_entity);

        }

        {

            BlueEnt physics_entity = _bridge -> CreateEntity();

            Transform loc;
            loc.position = { 2046, 428, 250, 250};

            PhysicsObject object;
            object.name = "Sponge";
            object.bounding_box = loc.position;
            object.friction = 0.05f;
            object.restitution = 0.0f;
            object.mass = 0.25f;
            object.type = SOLID;

            _bridge -> AddComponent(physics_entity, loc);
            _bridge -> AddComponent(physics_entity, object);

            _entities.push_back(physics_entity);

        }

        {

            BlueEnt physics_entity = _bridge -> CreateEntity();

            Transform loc;
            loc.position = { 246, 428, 100, 10000};

            PhysicsObject object;
            object.name = "Rect";
            object.bounding_box = loc.position;
            object.friction = 0.5f;
            object.restitution = 0.0f;
            object.mass = 0.1f;
            object.type = SOLID;

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
            object.bounding_box = loc.position;
            object.maxVelocity = {0, 0};
            object.maxAcceleration = {20, 20};
            object.friction = 0.0000004f;
            object.restitution = 0.0f;
            object.mass = 10.0f;
            object.type = SOLID;

            _bridge -> AddComponent(physics_entity, loc);
            _bridge -> AddComponent(physics_entity, object);

            _entities.push_back(physics_entity);

        }
        

        // Initialize Sprites
        sprites->Init();

		// Initialize Physics
		physics->Init();

    }

    // Called when a scene is being unloaded
    void Unload() override {
        // implementation
        // HAHA what you want me to clean???
        sprites -> Close();

		for (auto &entity: _entities) {

			_bridge -> DestroyEntity(entity);
		}

		_entities.clear();
    }

    bool Update(float deltaTime) override {

		// implementation
        BlueEnt player = _entities.front();


		force = {0,0};

		float gravity = 25.8f;

        float scalar = 15.0f;

        if (bEvent::keyDown(Uint8(44)) ) {

			gravity = 1.8f;
			if (physics -> IsOnFloor(player)) {

				force += {0, -450};
			}


        }

		// Wall Jump
		if (physics -> IsOnWall(player) && bEvent::keyJustDown(Uint8(44))) {

			if (bEvent::keyDown('A') || bEvent::keyDown('D')) {



				// Can't wall from the floor
				if (!physics -> IsOnFloor(player)) {

					float x = physics->GetWallNormal(player);

					//printf("x: %f\n", x);
					x = x * -200.0f;

					force += { x, -350 };

				}
			}

		}

        if (bEvent::keyDown('A')) {

            
            force += {-scalar,0};



        }
        // if (bEvent::keyDown('S')) {

            
        //     force += {0,scalar};

        // }
        if (bEvent::keyDown('D')) {

            
            force += {scalar,0};

        }


		if (bEvent::keyDown('p')) {


			// Change Scene to Stress Scene
			_stage.lock() -> LoadScene("stress");
			return false;


		}
        


        if (gravity == 15.8f) {

			printf("Player expereincing normal gravity\n");
		} else {

//			printf("Player expereincing low gravity\n");
		}

		//printf("Force: %f, %f\n", force.x, force.y);
        physics -> ApplyForce(player, force);

		if (!physics ->IsOnFloor(player)) {

//			printf("Player is in the air\n");
			_bridge -> GetComponent<PhysicsObject>(player).friction = 0.15f;
			_bridge -> GetComponent<PhysicsObject>(player).maxVelocity = {3500, 5000};
			//_bridge -> GetComponent<PhysicsObject>(player).maxAcceleration = {2000000000, 2000000000};
		} else {

//			printf("Player is on the ground\n");
			//physics ->SnapToFloor(player);
			_bridge -> GetComponent<PhysicsObject>(player).friction = 0.25f;
			_bridge -> GetComponent<PhysicsObject>(player).maxVelocity = {5000, 5000};
			//_bridge -> GetComponent<PhysicsObject>(player).maxAcceleration = {1000000000, 1000000000};
		}

		if (physics -> IsOnWall(player)) {

			//physics ->SnapToWall(player);
		}

        // Loop through all entities
        for (auto entity : _entities) {

            // Get the Transform Component
            Transform& transform = _bridge -> GetComponent<Transform>(entity);

            // Get the Physics Component
            PhysicsObject& physicsObject = _bridge -> GetComponent<PhysicsObject>(entity);

            // Update the Transform Component
            if (physicsObject.type == ACTOR) {
                
                // Apply Gravity
				if (!physics -> IsOnFloor(entity))
                	physics -> ApplyForce(entity, {0, gravity});
            }
        }
        
        
        //bPointF x = _bridge -> GetComponent<PhysicsObject>(player).acceleration;
        //printf("Acceleration: %f, %f\n", x.x, x.y);

        physics -> Update(deltaTime);
        sprites -> Update();

		return true;
        
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

	Force force;

};

#endif