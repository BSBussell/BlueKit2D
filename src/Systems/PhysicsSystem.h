
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

class PhysicsSystem : public BlueSys {

public:

    // Variables All Systems should have :3
    void Init();
    void Update(float dt);
    void Render(std::weak_ptr<bRenderer> _context);
    void Close();

    void ApplyForce(BlueEnt &ent, Force force);

private:

    // Applies velocity to position and acceleration to velocity
    void update_obj_positions(PhysicsObject &obj, float dt);

    // Checks for collisions between this obj and others
    void check_collisions(const BlueEnt &ent, float dt);

    // Checks for collisions between two Physics Objects
    bool check_collision(PhysicsObject &ent1, PhysicsObject &ent2, float dt);

    // Resolves collisions between two Physics Objects
    void resolve_collision(PhysicsObject &ent1, PhysicsObject &ent2, float dt);

};

#endif