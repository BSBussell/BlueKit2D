
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
        check_collisions(entity, dt);
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

    // I Love Physics 101
    obj.acceleration.x += force.x / obj.mass;
    obj.acceleration.y += force.y / obj.mass;
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

    // Cap the acceleration
    obj.acceleration.x = fmax(fmin(obj.acceleration.x, obj.maxAcceleration.x), -obj.maxAcceleration.x);
    obj.acceleration.y = fmax(fmin(obj.acceleration.y, obj.maxAcceleration.y), -obj.maxAcceleration.y);

    // Update the velocity
    obj.velocity += obj.acceleration;

    // Apply friction(decay) to acceleration and velocity
    // Bee's 1st law of motion:
    // An object in motion will eventually rest
    obj.acceleration *= (1.0f - obj.surfaceFriction );
    obj.velocity     *= (1.0f - obj.surfaceFriction );

    // Cap the velocity
    obj.velocity.x = fmax(fmin(obj.velocity.x, obj.maxVelocity.x), -obj.maxVelocity.x);
    obj.velocity.y = fmax(fmin(obj.velocity.y, obj.maxVelocity.y), -obj.maxVelocity.y);

    // Update the position
    obj.position.x += obj.velocity.x * dt;
    obj.position.y += obj.velocity.y * dt;

}

// Checks for collisions between all entities
void PhysicsSystem::check_collisions(const BlueEnt &ent, float dt) {

    std::shared_ptr g_BlueBridgePtr = g_WeakBlueBridge.lock();
    if (!g_BlueBridgePtr) {

        perror("bridge gone yadda yadda\n");
        exit(1);
    }

    PhysicsObject &obj = g_BlueBridgePtr -> GetComponent<PhysicsObject>(ent);

    for (auto const &entity: BlueEntities) {

        if (entity != ent)
            check_collision(obj, g_WeakBlueBridge.lock() -> GetComponent<PhysicsObject>(entity), dt);
    }

}

// Checks for collisions between two entities
void PhysicsSystem::check_collision(PhysicsObject &ent1, PhysicsObject &ent2, float dt) {

    if (ent1.position.intersects(ent2.position)) {

        
        resolve_collision(ent1, ent2, dt);
    }
}

// Resolves collisions between two entities
// THE BOUNCY ONE

void PhysicsSystem::resolve_collision(PhysicsObject &ent1, PhysicsObject &ent2, float dt) {

// Calculate the relative velocity of the objects
    bPointF rel_vel = ent2.velocity - ent1.velocity;

    // Calculate the normal vector of the collision
    bPointF normal = ent2.position.center() - ent1.position.center();
    normal.normalize();

    // Calculate the tangent vector of the collision
    bPointF tangent = {-normal.y, normal.x};

    // Calculate the magnitude of the relative velocity along the normal and tangent vectors
    float vel_along_normal = dot_product(rel_vel, normal);
    float vel_along_tangent = dot_product(rel_vel, tangent);

    // If the objects are moving away from each other, there's no collision to resolve
    if (vel_along_normal > 0) {
        return;
    }

    // Calculate the restitution of the collision
    float restitution = fmin(ent1.restitution, ent2.restitution); // Choose a value between 0 and 1

    // Calculate the impulse magnitude along the normal vector
    float j_n = -(1 + restitution) * vel_along_normal;

    // Calculate the impulse magnitude along the tangent vector
    float j_t = -ent1.surfaceFriction * vel_along_tangent;

    // Calculate the total impulse
    bPointF impulse = (normal * j_n) + (tangent * j_t);

    // Update the velocity of the objects using the impulse
    ent1.velocity -= impulse / ent1.mass;
    ent2.velocity += impulse / ent2.mass;

    // printf("ENT1 Velocity: %f, %f\n", ent1.velocity.x, ent1.velocity.y);
    // printf("ENT2 Veloocity: %f, %f\n", ent2.velocity.x, ent2.velocity.y);
    // printf("ENT1 Acceleration: %f, %f\n", ent1.acceleration.x, ent1.acceleration.y);
    // printf("ENT2 Acceleration: %f, %f\n", ent2.acceleration.x, ent2.acceleration.y);
}


/*
void PhysicsSystem::resolve_collision(PhysicsObject &ent1, PhysicsObject &ent2) {

    // Calculate the relative velocity of the objects
    bPointF rel_vel = ent2.velocity - ent1.velocity;

    // Calculate the normal vector of the collision
    bPointF normal = rel_vel;
    normal.normalize();
    normal = normal * -1;

    // Calculate the magnitude of the relative velocity along the normal vector
    float vel_along_normal = dot_product(rel_vel, normal);

    // If the objects are moving away from each other, there's no collision to resolve
    if (vel_along_normal > 0) {
        return;
    }

    // Calculate the restitution of the collision
    float restitution = 0.8f; // Choose a value between 0 and 1

    // Calculate the impulse magnitude
    float j = -(1 + restitution) * vel_along_normal;

    // Apply the impulse to ent1
    bPointF impulse = normal * j;

    // Updae ent1
    ent1.velocity -= impulse;
    ent1.position.x -= impulse.x;
    ent1.position.y -= impulse.y;
}
*/

