
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

        bPointF initial_pos;
        bPointF final_pos;
        bPointF delta_pos;

        // Component we want to grab
        PhysicsObject &obj = g_BlueBridgePtr -> GetComponent<PhysicsObject>(entity);
        Transform &trans = g_BlueBridgePtr -> GetComponent<Transform>(entity);

        initial_pos.x = obj.position.x;
        initial_pos.y = obj.position.y;
        

        update_obj_positions(obj, dt);

        check_collisions(entity, dt);
       
        final_pos.x = obj.position.x;
        final_pos.y = obj.position.y;

        delta_pos = final_pos - initial_pos;
       
        bRectF new_pos_float;
        new_pos_float.x = (static_cast<float>(trans.position.x) + delta_pos.x);
        new_pos_float.y = (static_cast<float>(trans.position.y) + delta_pos.y);
        new_pos_float.width = trans.position.width;
        new_pos_float.height = trans.position.height;

        //trans.position = new_pos_float;
        trans.position = obj.position;
        //trans.position.x = new_pos_float.x;
        //trans.position.y = new_pos_float.y;

        // I LOVE DEBUGGING!!!!
        if (entity == 0 && delta_pos.x != 0.0f && false) {
            printf("##############################\n");
            printf("Name: %s\n", obj.name);
            printf("Initial Pos: (%f, %f)\n", initial_pos.x, initial_pos.y);
            printf("Final Pos: (%f, %f)\n", final_pos.x, final_pos.y);
            printf("Delta Pos: (%f, %f)\n", delta_pos.x, delta_pos.y);
            printf("New Pos: (%f, %f)\n", new_pos_float.x, new_pos_float.y);
            printf("Obj Pos: (%f, %f)\n", obj.position.x, obj.position.y);
            printf("Trans: (%u, %u)\n", trans.position.x, trans.position.y);
            printf("##############################\n");
        }
    }
}

void PhysicsSystem::ApplyForce(BlueEnt &ent, Force force) {

    std::shared_ptr g_BlueBridgePtr = g_WeakBlueBridge.lock();
	if (!g_BlueBridgePtr) {

		perror("GIRL WHERE'D THE BRIDGE GO!!!\n ( Error in PhysicsSystem )\n");

		exit(1);
	}

    PhysicsObject &obj = g_BlueBridgePtr -> GetComponent<PhysicsObject>(ent);

    force *= 50.0f;

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
        context -> drawRect(
            g_BlueBridgePtr -> GetComponent<PhysicsObject>(entity).position,
            g_BlueBridgePtr -> GetComponent<PhysicsObject>(entity).render_color
        );

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
    obj.velocity += obj.acceleration * dt;

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
        ent2.render_color = {255, 0, 0, 50};
    }
}

// Resolves collisions between two entities
// Im hoping to make this as generalized as possible
/*
void PhysicsSystem::resolve_collision(PhysicsObject &ent1, PhysicsObject &ent2, float dt) {

    
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
    float restitution = 1.0f;

    // Calculate the impulse magnitude
    float j = -(1 + restitution) * vel_along_normal / (1 / ent1.mass + 1 / ent2.mass);

    // Apply the impulse to the objects
    bPointF impulse = normal * j;

    bPointF correction = normal * 8.0f / (1 / ent1.mass + 1 / ent2.mass);
    bPointF correctionA = correction * (1 / ent1.mass);
    bPointF correctionB = correction * (1 / ent2.mass);

    // Representing Forces
    bPointF initialForce1 = ent1.acceleration;
    bPointF initialForce2 = ent2.acceleration;

    float dotProduct1 = dot_product(initialForce1, normal);
    float dotProduct2 = dot_product(initialForce2, normal);

    bPointF netForce1 = initialForce1 - (normal * dotProduct1);
    bPointF netForce2 = initialForce2 - (normal * dotProduct2);

    // Changing Objects
    if(ent1.type == DYNAMIC) {
        
        ent1.acceleration = (netForce1 + impulse) / ent1.mass;
        ent1.velocity -= impulse;
        ent1.position -= correctionA;
    }
    
    if (ent2.type == DYNAMIC) {
        
        ent2.acceleration = (netForce2 + impulse) / ent2.mass;
        ent2.velocity += impulse;
        ent2.position += correctionB;
    }

}
*/

void PhysicsSystem::resolve_collision(PhysicsObject &ent1, PhysicsObject &ent2, float dt) {

    /*
        Step 1: Calculate the collision normal.

        Ok so collision normal is the vector perpendicular to the collision surface. 
        Since we just two rectangles, it will be either the x-axis or the y-axis, 
        depending on which side of the rectangles collided.
    */

    bPointF centerVector = ent2.position.center() - ent1.position.center();
    //centerVector.normalize();
    // printf("Center Vector: (%f, %f)\n", centerVector.x, centerVector.y);

    bPointF normal_axis;
    if (abs(centerVector.x) > abs(centerVector.y)) {
        
        if (ent2.position.x < ent1.position.x) {
            printf("Left :3\n");
            normal_axis = {-1, 0};
        } else if (ent2.position.x > ent1.position.x) {
            printf("Right :3\n");
            normal_axis = {1, 0};
        }
    } else {
        if (ent1.position.y > ent2.position.y) {
            printf("Bottom :3\n");
            normal_axis = {0, -1};
        } else if (ent1.position.y < ent2.position.y) {
            printf("Top :3\n");
            normal_axis = {0, 1};
        }
    }

    /*
        Step 2)
            Calculate relative velocity, the inv mass, and nSpd
    */

    bPointF rel_Velocity = ent2.velocity - ent1.velocity;

    float invMass1 = 1 / ent1.mass;
    float invMass2 = 1 / ent2.mass;

    float nSpd = dot_product(rel_Velocity, normal_axis);

    /*
        Step 2.B
            If Check if objects are actually moving closer togehter, if not, return
    */

    if (nSpd >= 0)
        return;


    /*
        Step 3)
            Calculate the impulse from restitution and the scalar
    */

    // Use the greater restitution value (probably)
    float e = (ent1.restitution > ent2.restitution) ? ent1.restitution : ent2.restitution;

    float j = -(1 + e) * nSpd / (invMass1 + invMass2);

    bPointF impulse = normal_axis * j;

    /*
        Step 4)
            Calculate and apply the change in velocity
    */

    bPointF deltaV1 = impulse * invMass1;
    bPointF deltaV2 = impulse * invMass2;

    if (ent1.type == DYNAMIC) {

        ent1.acceleration = {0, 0};
        ent1.position -= ent1.velocity * dt;
        ent1.velocity -= deltaV1;
        ent1.position += ent1.velocity * dt;
    }

    if (ent2.type == DYNAMIC) {

        ent2.acceleration = {0, 0};
        ent2.position -= ent2.velocity * dt;
        ent2.velocity += deltaV2;
        ent2.position += ent2.velocity * dt;
    }

}

// void PhysicsSystem::resolve_collision(PhysicsObject &ent1, PhysicsObject &ent2, float dt) {

//     printf("#-----------------------------#\n");
//     printf("Initial Acceleration of Ent 1: (%f, %f)\n", ent1.acceleration.x, ent1.acceleration.y);
//     printf("Initial Velocity of Ent 1    : (%f, %f)\n", ent1.velocity.x, ent1.velocity.y);
//     printf("Initial Position of Ent 1    : (%f, %f)\n", ent1.position.x, ent1.position.y);

//     printf("Initial Acceleration of Ent 2: (%f, %f)\n", ent2.acceleration.x, ent2.acceleration.y);
//     printf("Initial Velocity of Ent 2    : (%f, %f)\n", ent2.velocity.x, ent2.velocity.y);
//     printf("Initial Position of Ent 2    : (%f, %f)\n", ent2.position.x, ent2.position.y);
//     printf("#-----------------------------#\n");

    
//     /*
//         Step 1: Calculate the collision normal.

//         Ok so collision normal is the vector perpendicular to the collision surface. 
//         Since we just two rectangles, it will be either the x-axis or the y-axis, 
//         depending on which side of the rectangles collided.
//     */

//     bPointF centerVector = ent2.position.center() - ent1.position.center();
//     float absX = abs(centerVector.x);
//     float absY = abs(centerVector.y);


//     // Ok so if i remember from EF157, the normal is like the force
//     // Pushing the object away from the surface
//     // ex, if you're standing on the floor and gravity is pulling you down
//     // the normal is the force the floor is pushing you up with
//     bPointF normal_axis;

//     // Possible Issues: If you sneak through a perfect corner this'll get fucky
//     // If we're further away from the x axis center
//     if (absX > absY) {

//         // Normal force only applies on the x axis
//         if (centerVector.x < 0) {
//             printf("Right\n");
//             normal_axis = {1, 0};
//         } else {
//             printf("Left\n");
//             normal_axis = {-1, 0};
//         }
    
//     // if we're further away from the y axis / if we're equal lol
//     } else {

//         // Normal force only applies on the y axis
//         if (centerVector.y < 0) {
//             printf("Bottom :3\n");
//             normal_axis = {0, -1};
//         } else {
//             printf("Top\n");
//             normal_axis = {0, 1};
//         }
//     }

//     // Force Acting Against Each Object
//     bPointF normal_force1;
//     normal_force1.x = ent1.mass * ent1.acceleration.x * normal_axis.x;
//     normal_force1.y = ent1.mass * ent1.acceleration.y * normal_axis.y;
//     bPointF normal_force2;
//     normal_force2.x = ent2.mass * ent2.acceleration.x * normal_axis.y;
//     normal_force2.y = ent2.mass * ent2.acceleration.y * normal_axis.y;

//     /* 
//         Step 2: Calculate the impulse of the collision.
        
//         Ok so from the impulse of a collision is the
//         force acting on an object multiplied by the time of the collision.
        
//     */

//     Force forces_1 = ent1.acceleration * ent1.mass;
//     Force forces_2 = ent2.acceleration * ent2.mass;

//     printf("#-----------------------------#\n");
//     printf("Normal Force 1: (%f, %f)\n", normal_force1.x, normal_force1.y);
//     printf("Normal Force 2: (%f, %f)\n", normal_force2.x, normal_force2.y);
//     printf("#-----------------------------#\n");

//     Force net_force1 = forces_1 + normal_force1;
//     Force net_force2 = forces_2 + normal_force2;

//     // Impulse is net_force * time
//     bPointF impulse1 = net_force1 * dt;
//     bPointF impulse2 = net_force2 * dt;

//     /*
//         Step 3: Use the impulse to calculate the acceleration of the two rectangles.
//         From there you can calculate the velocity and position of the rectangles.
//     */
    
//     if (ent1.type == DYNAMIC) {
//         //ent1.position -= ent1.velocity * dt;
//         ent1.acceleration = impulse1 / ent1.mass;
//         ent1.velocity += ent1.acceleration * dt;
//         ent1.position -= ent1.velocity * dt;
//         ent1.acceleration = {0, 0};
//     }

//     if (ent2.type == DYNAMIC) {
//         //ent2.position -= ent2.velocity * dt;
//         ent2.acceleration = impulse2 / ent2.mass;
//         ent2.velocity += ent2.acceleration * dt;
//         ent2.position -= ent2.velocity * dt;
//         ent2.acceleration = {0, 0};
//     }

//     printf("#                             #\n");
//     printf("Final Acceleration of Ent 1: (%f, %f)\n", ent1.acceleration.x, ent1.acceleration.y);
//     printf("Final Velocity of Ent 1    : (%f, %f)\n", ent1.velocity.x, ent1.velocity.y);
//     printf("Final Position of Ent 1    : (%f, %f)\n", ent1.position.x, ent1.position.y);

//     printf("Final Acceleration of Ent 2: (%f, %f)\n", ent2.acceleration.x, ent2.acceleration.y);
//     printf("Final Velocity of Ent 2    : (%f, %f)\n", ent2.velocity.x, ent2.velocity.y);
//     printf("Final Position of Ent 2    : (%f, %f)\n", ent2.position.x, ent2.position.y);
//     printf("#-----------------------------#\n");
    
// }


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

