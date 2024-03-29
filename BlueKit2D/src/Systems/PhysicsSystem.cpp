
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

	//check_collisions(0, dt);

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
    // obj.acceleration *= (1.0f - obj.friction );
    obj.acceleration  = {0.0f, 0.0f};
    obj.velocity     *= (1.0f - obj.friction );

    // Now we Cap the velocity
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


	/*
	bool hasCollisions = true;
	while (hasCollisions) {
		hasCollisions = false;

		for (auto const& entity: BlueEntities) {

			if (entity == ent) {
				continue;
			}

			PhysicsObject &obj1 = g_BlueBridgePtr->GetComponent<PhysicsObject>(ent);
			PhysicsObject &obj2 = g_BlueBridgePtr->GetComponent<PhysicsObject>(entity2);


			for (auto const &entity2: BlueEntities) {
				if (entity1 == entity2) {
					continue;
				}

				PhysicsObject &obj2 = g_BlueBridgePtr->GetComponent<PhysicsObject>(entity2);

				// Check for collisions
				if (obj1.position.intersects(obj2.position)) {

					printf("====================================\n");
            		printf("Collision between %s and %s\n", obj1.name.c_str(), obj2.name.c_str());
					resolve_collision(obj1, obj2, dt);
					printf("====================================\n");
					hasCollisions = true;
				}

			}

		}

	}

	*/

	PhysicsObject &obj = g_BlueBridgePtr -> GetComponent<PhysicsObject>(ent);

    for (auto const &entity: BlueEntities) {

		PhysicsObject &obj2 = g_BlueBridgePtr -> GetComponent<PhysicsObject>(entity);

		// Make sure we don't check collisions with ourselves
        if (ent != entity)
            check_collision(obj, obj2, dt);
    }

}



// Checks for collisions between two entities
bool PhysicsSystem::check_collision(PhysicsObject &ent1, PhysicsObject &ent2, float dt) {

    int iterations = 0;
	bool hadCollision = false;
    // run until the two objects are no longer colliding or we've tried to resolve the collision 10 times
    while ( ent1.position.intersects(ent2.position)) {
		// printf("====================================\n");
        //if (strcmp(ent1.name, "Player") && strcmp(ent2.name, "Player"))
		// printf("Collision between %s and %s\n", ent1.name.c_str(), ent2.name.c_str());
        resolve_collision(ent1, ent2, dt);
        // printf("====================================\n");
        ent2.render_color = {255, 0, 0, 50};
        iterations++;
        if (iterations > 10) {
            // printf("Could not resolve collision between %s and %s\n", ent1.name, ent2.name);
            break;
        }

		hadCollision = true;
    }
	return hadCollision;

    // After contact, most objects will be contacting each other so we need to add it to the list
    
    // ent1.contacts.push_back(&ent2);
    // ent2.contacts.push_back(&ent1);
    
}

// Resolves collisions between two entities
void PhysicsSystem::resolve_collision(PhysicsObject &ent1, PhysicsObject &ent2, float dt) {

	float invMass1 = 1 / ent1.mass;
    float invMass2 = 1 / ent2.mass;

    /*
        Step 1: Calculate the collision normal.

        Ok so collision normal is the vector perpendicular to the collision surface. 
        I tried to be clever for like a week before giving up and just check each vertice
        To find the relative position :D
    */
   

    // Get all vertices of our rectangle

    // Physics Step Backwards
    // This is needed because at the time of collision the object is inside the other object
    ent1.position.x -= ent1.velocity.x * dt;
    ent1.position.y -= ent1.velocity.y * dt;

	// Gimme those vertices
    bPointF ent1Vertices[4] = {
        {ent1.position.x, ent1.position.y},
        {ent1.position.x + ent1.position.width, ent1.position.y},
        {ent1.position.x, ent1.position.y + ent1.position.height},
        {ent1.position.x + ent1.position.width, ent1.position.y + ent1.position.height}
    };

    // Physics Step Forwards
    ent1.position.x += ent1.velocity.x * dt;
    ent1.position.y += ent1.velocity.y * dt;

	// Physics Step Backwards, to get where we were before the collision
	ent2.position.x -= ent2.velocity.x * dt;
	ent2.position.y -= ent2.velocity.y * dt;

	// Grab the vertices of ent2
	bPointF ent2Vertices[4] = {
		{ent2.position.x, ent2.position.y},
		{ent2.position.x + ent2.position.width, ent2.position.y},
		{ent2.position.x, ent2.position.y + ent2.position.height},
		{ent2.position.x + ent2.position.width, ent2.position.y + ent2.position.height}
	};

	// Revert the physics step back
	ent2.position.x += ent2.velocity.x * dt;
	ent2.position.y += ent2.velocity.y * dt;

	bPointF normal_axis;

	// This isn't pretty, but it works and its not horrible

	// Check if every vertice of ent1 is on the left of ent2
	// If so, then the collision normal is the x-axis
	if (ent1Vertices[0].x <= ent2Vertices[0].x
		&& ent1Vertices[1].x <= ent2Vertices[0].x
		&& ent1Vertices[2].x <= ent2Vertices[0].x
		&& ent1Vertices[3].x <= ent2Vertices[0].x) {

//		printf("Pushing Left\n");
		normal_axis = {-1, 0};
	}

	// Check if every vertice of ent1 is on the right of ent2
	// If so, then the collision normal is the x-axis
	else if (ent1Vertices[0].x >= ent2Vertices[1].x
		&& ent1Vertices[1].x >= ent2Vertices[1].x
		&& ent1Vertices[2].x >= ent2Vertices[1].x
		&& ent1Vertices[3].x >= ent2Vertices[1].x) {

//		printf("Pushing Right\n");
		normal_axis = {1, 0};
	}

	// Check if every vertice of ent1 is on the top of ent2
	// If so, then the collision normal is the y-axis
	else if (ent1Vertices[0].y <= ent2Vertices[0].y
		&& ent1Vertices[1].y <= ent2Vertices[0].y
		&& ent1Vertices[2].y <= ent2Vertices[0].y
		&& ent1Vertices[3].y <= ent2Vertices[0].y) {

//		printf("Pushing down\n");
		normal_axis = {0, -1};
	}

	// Check if every vertice of ent1 is on the bottom of ent2
	// If so, then the collision normal is on the y-axis
	else if (ent1Vertices[0].y >= ent2Vertices[2].y
		&& ent1Vertices[1].y >= ent2Vertices[2].y
		&& ent1Vertices[2].y >= ent2Vertices[2].y
		&& ent1Vertices[3].y >= ent2Vertices[2].y) {

//		printf("Pushing up\n");
		normal_axis = {0, 1};
	}
	else {

		// Means object was forced into another object
		// The reason this can happen is because the position is updated here
		// Without caring about the change in position causing a collision

//		printf("No Collision Normal Found\n");

		// Hacky :3
//		if (ent1.type == DYNAMIC) {
//			ent1.position -= ent1.velocity * dt;
//		}
//		if (ent2.type == DYNAMIC) {
//			ent2.position -= ent2.velocity * dt;
//		}

		return;
	}


    /*
        Step 2)
            Calculate relative velocity, the inv mass, and nSpd
    */

    bPointF rel_Velocity = ent2.velocity - ent1.velocity;

//	printf("Normal Axis: %f, %f\n", normal_axis.x, normal_axis.y);
//	printf("Relative Velocity: %f, %f\n", rel_Velocity.x, rel_Velocity.y);
    float nSpd = dot_product(rel_Velocity, normal_axis);

    /*
        Step 2.B
            If Check if objects are actually moving closer togehter, if not, return
    */

//    if (nSpd >= 0) {
//        printf("The Objects are not moving closer together\n");
//        //return;
//    }

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
        Step 6)
            Apply the changes
    */

    if (ent1.type == DYNAMIC) {

		// Reset acceleration
        ent1.acceleration = {0, 0};

        // Step backwards until the two objects are no longer colliding
		ent1.position -= ent1.velocity * dt;

        ent1.velocity -= deltaV1;

		// Cap the velocity
		ent1.velocity.x = fmax(fmin(ent1.velocity.x, ent1.maxVelocity.x), -ent1.maxVelocity.x);
    	ent1.velocity.y = fmax(fmin(ent1.velocity.y, ent1.maxVelocity.y), -ent1.maxVelocity.y);

		// A problem here is that the position is updated, without care for collision
        ent1.position += ent1.velocity * dt;

    }

    if (ent2.type == DYNAMIC) {

		// Reset acceleration
        ent2.acceleration = {0, 0};

        // Step backwards until the two objects are no longer colliding
		ent2.position -= ent2.velocity * dt;

        ent2.velocity += deltaV2;

		ent2.velocity.x = fmax(fmin(ent2.velocity.x, ent2.maxVelocity.x), -ent2.maxVelocity.x);
    	ent2.velocity.y = fmax(fmin(ent2.velocity.y, ent2.maxVelocity.y), -ent2.maxVelocity.y);

        ent2.position += ent2.velocity * dt;
    }

}



/*
        Step 5)
            Penetration resolution :<

            We'll get this working later, for now we just reverse our step and change velocity
	*/

	/*

    bRectF overlap = ent1.position.intersection(ent2.position);
    bPointF overlap_size = {overlap.width, overlap.height};

    // Print the overlap size
    // printf("overlap_size: %f, %f\n", overlap_size.x, overlap_size.y);


    // Since we are only pushing the object out of the rect in one direction
    // Just use the normal direction
    overlap_size.x *= (normal_axis.x);
    overlap_size.y *= -(normal_axis.y);

    //s printf("overlap_pos: %f, %f\n", overlap_size.x, overlap_size.y);

    //float depth = fmin(overlap.width, overlap.height);

    // const float percent = 0.8f;
    // const float slop = 0.01f;

    //bPointF correction = normal_axis * percent * fmax(depth - slop, 0.0f) / (invMass1 + invMass2);

    bPointF correction = overlap_size;

    bPointF deltaPos1 = correction;
    bPointF deltaPos2 = correction;

    // printf("correction: %f, %f\n", correction.x, correction.y);

	// printf("deltaPos1: %f, %f\n", deltaPos1.x, deltaPos1.y);
    // printf("deltaPos2: %f, %f\n", deltaPos2.x, deltaPos2.y);
	*/