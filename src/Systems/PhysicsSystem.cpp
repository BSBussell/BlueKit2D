
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

    force *= 100.0f;

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
    // obj.acceleration *= (1.0f - obj.surfaceFriction );
    obj.acceleration  = {0.0f, 0.0f};
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

    int iterations = 0;
    // run until the two objects are no longer colliding or we've tried to resolve the collision 10 times
    while ( ent1.position.intersects(ent2.position)) {
        resolve_collision(ent1, ent2, dt);
        ent2.render_color = {255, 0, 0, 50};
        iterations++;
        if (iterations > 10) {
            // printf("Could not resolve collision between %s and %s\n", ent1.name, ent2.name);
            break;
        }
    }

    // After contact, most objects will be contacting each other so we need to add it to the list
    
    // ent1.contacts.push_back(&ent2);
    // ent2.contacts.push_back(&ent1);
    
}

// Resolves collisions between two entities
void PhysicsSystem::resolve_collision(PhysicsObject &ent1, PhysicsObject &ent2, float dt) {

    /*
        Step 1: Calculate the collision normal.

        Ok so collision normal is the vector perpendicular to the collision surface. 
        Since we just two rectangles, it will be either the x-axis or the y-axis, 
        depending on which side of the rectangles collided.
    */
   

    // Get all vertices of our rectangle

    // Physics Step Backwards
    // This is needed because at the time of collision the object is inside the other object
    ent1.position.x -= ent1.velocity.x * dt;
    ent1.position.y -= ent1.velocity.y * dt;

    bPointF ent1Vertices[4] = {
        {ent1.position.x, ent1.position.y},
        {ent1.position.x + ent1.position.width, ent1.position.y},
        {ent1.position.x, ent1.position.y + ent1.position.height},
        {ent1.position.x + ent1.position.width, ent1.position.y + ent1.position.height}
    };

    // Physics Step Forwards
    ent1.position.x += ent1.velocity.x * dt;
    ent1.position.y += ent1.velocity.y * dt;

    // bPointF normal_axis;

    // Check if all vertices of ent1 are on the left or right of ent2
    bool xAxis = false;
    for (int i = 0; i < 4; i++) {

        // Ok so if we want to check if a point is to the left or the right of a rectangle,
        // All we have to do is check if the x coordinate of the point is less than the x coordinate of other rectangle
        if (ent1Vertices[i].x < ent2.position.x || ent1Vertices[i].x > ent2.position.x + ent2.position.width) {
            xAxis = true;
        } else {
            xAxis = false;
            break;
        }
    }

    bPointF normal_axis;
    // Check if all vertices of ent1 are on the top or bottom of ent2
    if (xAxis) {  
        if (ent2.position.x < ent1.position.x) {
            // printf("Pushing left\n");
            normal_axis = {-1, 0};
        } else if (ent2.position.x > ent1.position.x) {
            // printf("Pushing right\n");
            normal_axis = {1, 0};
        }
    } else {
        if (ent1.position.y > ent2.position.y) {
            // printf("Pushing up\n");
            normal_axis = {0, -1};
        } else if (ent1.position.y < ent2.position.y) {
            // printf("Pushing down\n");
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
            Calculate the change in velocity
    */

    bPointF deltaV1 = impulse * invMass1;
    bPointF deltaV2 = impulse * invMass2;



    /*
        Step 5)
            Penetration resolution :<
    */

    bRectF overlap = ent1.position.intersection(ent2.position);
    bPointF overlap_size = {overlap.width, overlap.height};

    overlap_size.x *= normal_axis.x;
    overlap_size.y *= normal_axis.y;

    // printf("overlap_pos: %f, %f\n", overlap_size.x, overlap_size.y);

    //float depth = fmin(overlap.width, overlap.height);

    const float percent = 0.8f;
    const float slop = 0.01f;

    //bPointF correction = normal_axis * percent * fmax(depth - slop, 0.0f) / (invMass1 + invMass2);
    
    bPointF correction = overlap_size * percent;

    bPointF deltaPos1 = correction;
    bPointF deltaPos2 = correction;

    // printf("correction: %f, %f\n", correction.x, correction.y);

    // printf("deltaPos1: %f, %f\n", deltaPos1.x, deltaPos1.y);
    // printf("deltaPos2: %f, %f\n", deltaPos2.x, deltaPos2.y);

    /*
        Step 6)
            Apply the changes
    */

    if (ent1.type == DYNAMIC) {

        ent1.acceleration = {0, 0};

        // Step backwards until the two objects are no longer colliding
        
        ent1.position -= ent1.velocity * dt;
        //ent1.position -= deltaPos1;

        ent1.velocity -= deltaV1;
        ent1.position += ent1.velocity * dt;
    }

    if (ent2.type == DYNAMIC) {

        ent2.acceleration = {0, 0};

        // Step backwards until the two objects are no longer colliding
        
        ent2.position -= ent2.velocity * dt;
        //ent2.position += deltaPos2;

        ent2.velocity += deltaV2;
        ent2.position += ent2.velocity * dt;
    }

}
