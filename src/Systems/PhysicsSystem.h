
// Bee Bussell
// March 12, 2023
// PhysicsSystem.h

#ifndef PHYSICS_SYSTEM_H
#define PHYSICS_SYSTEM_H

#include <memory>
#include <array>
#include <vector>

#include <BML/bRenderer.h>

#include "Core/BlueBridge.h"
#include "Components/PhysicsObject.h"
#include "Components/Transform.h"


/*
    All colliders are axis-aligned bounding boxes (AABBs)
    All collider positions, widths, and heights are floats
    Except for special circumstances, Actors and Solids will never overlap
    Solids do not interact with other Solids
*/

class PhysicsSystem : public BlueSys {

public:

    // Variables All Systems should have :3
    void Init();
    void Update(float dt);
    void Render(std::weak_ptr<bRenderer> _context);
    void Close();

    void ApplyForce(BlueEnt &ent, Force force);
	void ApplyForce(PhysicsObject &obj, Force force);

	bool IsOnFloor(BlueEnt &ent);
	bool IsOnWall(BlueEnt &ent);

	void SnapToFloor(BlueEnt &ent);
	void SnapToWall(BlueEnt &ent);

	// Pretty much find our relative position
	// Will be needed for things like wall jumping
	float GetWallNormal(BlueEnt &ent);

private:

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

	void check_contact(PhysicsObject &ent1, PhysicsObject &ent2);

    // Resolves collisions between two Physics Objects
    void resolve_collision(PhysicsObject &ent1, PhysicsObject &ent2, float dt);

};

#endif