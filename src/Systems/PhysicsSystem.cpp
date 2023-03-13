
// Bee Bussell
// March 12, 2023
// PhysicsSystem.cpp

#include "PhysicsSystem.h"

extern std::weak_ptr<BlueBridge> g_WeakBlueBridge;

// IDK what we need to do on init ngl
void PhysicsSystem::Init() {

}

void PhysicsSystem::Update(float dt) {

    std::shared_ptr g_BlueBridgePtr = g_WeakBlueBridge.lock();
	if (!g_BlueBridgePtr) {

		perror("GIRL WHERE'D THE BRIDGE GO!!!\n ( Error in SpriteSystem )\n");
		exit(1);
	}

    for (auto const& entity: BlueEntities) {

        // Component we want to grab
        PhysicsObject &obj = g_BlueBridgePtr -> GetComponent<PhysicsObject>(entity);
        Transform &trans = g_BlueBridgePtr -> GetComponent<Transform>(entity);

        update_obj_positions(obj, dt);
        check_collisions(entity);
        trans.position = obj.position;
    }
}

void PhysicsSystem::ApplyForce(BlueEnt &ent, Force force) {

    std::shared_ptr g_BlueBridgePtr = g_WeakBlueBridge.lock();
	if (!g_BlueBridgePtr) {

		perror("GIRL WHERE'D THE BRIDGE GO!!!\n ( Error in PhysicsSystem )\n");

		exit(1);
	}

    PhysicsObject &obj = g_BlueBridgePtr -> GetComponent<PhysicsObject>(ent);

    obj.acceleration.x += force.x;
    obj.acceleration.y += force.y;
}

void PhysicsSystem::Render(std::weak_ptr<bRenderer> _context) {

    std::shared_ptr g_BlueBridgePtr = g_WeakBlueBridge.lock();
    if (!g_BlueBridgePtr) {
        
        perror("GIRL WHERE'D THE BRIDGE GO!!!\n ( Error in PhysicsSystem )\n");
        exit(1);
    }

    for (auto const &entity: BlueEntities) {

        auto context = _context.lock();
        context -> drawRect(g_BlueBridgePtr -> GetComponent<PhysicsObject>(entity).position, 0, 255, 0);

    }
}

void PhysicsSystem::Close() {


}

// Takes and object and updates their position
void PhysicsSystem::update_obj_positions(PhysicsObject &obj, float dt) {

    obj.velocity += obj.acceleration * dt;

    obj.position.x += obj.velocity.x;
    obj.position.y += obj.velocity.y;
}

// Checks for collisions between all entities
void PhysicsSystem::check_collisions(const BlueEnt &ent) {

    std::shared_ptr g_BlueBridgePtr = g_WeakBlueBridge.lock();
    if (!g_BlueBridgePtr) {

        perror("bridge gone yadda yadda\n");
        exit(1);
    }

    PhysicsObject &obj = g_BlueBridgePtr -> GetComponent<PhysicsObject>(ent);

    for (auto const &entity: BlueEntities) {

        if (entity != ent)
            check_collision(obj, g_WeakBlueBridge.lock() -> GetComponent<PhysicsObject>(entity));
    }

}

// Checks for collisions between two entities
void PhysicsSystem::check_collision(PhysicsObject &ent1, PhysicsObject &ent2) {

    if (ent1.position.intersects(ent2.position)) {

        resolve_collision(ent1, ent2);
    }
}

// Resolves collisions between two entities
void PhysicsSystem::resolve_collision(PhysicsObject &ent1, PhysicsObject &ent2) {

    // Calculate the relative velocity of the objects
    bPointF rel_vel = ent2.velocity - ent1.velocity;

    // Calculate the normal vector of the collision
    bPointF normal = ent2.position.center() - ent1.position.center();
    normal.normalize();

    // Calculate the magnitude of the relative velocity along the normal vector
    float vel_along_normal = dot_product(rel_vel, normal);

    // If the objects are moving away from each other, there's no collision to resolve
    if (vel_along_normal > 0) {
        return;
    }

    // Calculate the restitution of the collision
    float restitution = 0.5f; // Choose a value between 0 and 1

    // Calculate the impulse magnitude
    float j = -(1 + restitution) * vel_along_normal;

    // Apply the impulse to the objects
    bPointF impulse = normal * j;

    ent1.velocity -= impulse;
    ent2.velocity += impulse;

}
