
// Bee Bussell
// March 12, 2023
// PhysicsSystem.h

#ifndef PHYSICS_SYSTEM_H
#define PHYSICS_SYSTEM_H

#include <memory>
#include <array>
#include <vector>

#include <BML/bRenderer.h>

#include "../Core/BlueBridge.h"
#include "../Components/PhysicsObject.h"
#include "../Components/Transform.h"


/*
    All colliders are axis-aligned bounding boxes (AABBs)
    All collider positions, widths, and heights are floats
    Except for special circumstances, Actors and Solids will never overlap
    Solids do not interact with other Solids

    A child class of PhysicsSystem can be made to add unique collision resolvers
*/



class PhysicsSystem : public BlueSys {

public:

    // Variables All Systems should have :3
    void Init();

	// @brief Updates the physics, applies forces, and checks for collisions
    void Update(float dt);

	// @brief Draws the bounding boxes of all entities
    void Render(std::weak_ptr<bRenderer> _context);

	// @brief Closes the physics system
    void Close();

	// @brief Applies a force to an entity
    void ApplyForce(BlueEnt &ent, Force force);
	void ApplyForce(PhysicsObject &obj, Force force);

	// @brief Bool checks if an entity is on the floor
	bool IsOnFloor(BlueEnt &ent);

	// @brief Bool checks if an entity is on a wall
	bool IsOnWall(BlueEnt &ent);

	// @brief Snaps an entity to the floor
	void SnapToFloor(BlueEnt &ent);

	// @brief Snaps an entity to the wall
	void SnapToWall(BlueEnt &ent);

	// Pretty much find our relative position
	// Will be needed for things like wall jumping
	float GetWallNormal(BlueEnt &ent);

protected:

    // Applies velocity to position and acceleration to velocity
    void update_obj_positions(PhysicsObject &obj, float dt);

	// Snaps object for colliders
	void snap(PhysicsObject &obj);

    // Checks for collisions between this obj and others
    void check_collisions(const BlueEnt &ent, float dt);

	// Find the collision normal of two objects
	bPointF find_collision_normal(const PhysicsObject &ent1, const PhysicsObject &ent2);

    // Checks for collisions between two Physics Objects
    bool check_collision(PhysicsObject &ent1, PhysicsObject &ent2, float dt);

	// Checks if two objects are in contact using a threshold
	void check_contact(PhysicsObject &ent1, PhysicsObject &ent2);

    // Resolves collisions between two Physics Objects
    void resolve_collision(PhysicsObject &ent1, PhysicsObject &ent2, float dt);

	float current_step = 0.0f;
	float goal_step = 1.0f / 60.0f;

};

#endif