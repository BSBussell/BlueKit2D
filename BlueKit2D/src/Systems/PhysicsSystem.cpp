
// Bee Bussell
// March 12, 2023
// PhysicsSystem.cpp

#include "PhysicsSystem.h"

extern std::weak_ptr<BlueBridge> g_WeakBlueBridge;

// IDK what we need to do on init ngl
void PhysicsSystem::Init() {

	std::shared_ptr g_BlueBridgePtr = g_WeakBlueBridge.lock();
	if (!g_BlueBridgePtr) {

		perror("GIRL YOU LOST YOUR THING\n");
		exit(1);
	}




	// Loop through all entities
	for (auto const& entity : BlueEntities) {

		PhysicsObject &obj = g_BlueBridgePtr -> GetComponent<PhysicsObject>(entity);

		// That way we can access the entity from the component hahaha
		// Nothing can go wrong here
		obj.id = entity;
	}

}

void PhysicsSystem::Update(float dt) {

	// TODO: Make Update work with fixed time steps

    std::shared_ptr g_BlueBridgePtr = g_WeakBlueBridge.lock();
	if (!g_BlueBridgePtr) {

		perror("GIRL WHERE'D THE BRIDGE GO!!!\n ( Error in SpriteSystem )\n");
		exit(1);
	}

    for (auto const& entity: BlueEntities) {

		// Used for translating the transform component
        bPointF initial_pos;
        bPointF delta_pos;

        // Component we want to grab
        PhysicsObject &obj = g_BlueBridgePtr -> GetComponent<PhysicsObject>(entity);
        Transform &trans = g_BlueBridgePtr -> GetComponent<Transform>(entity);

		// Storing the initial position
        initial_pos.x = obj.position.x;
        initial_pos.y = obj.position.y;
        
		// Update the objects position
        update_obj_positions(obj, dt);

		// Check for collisions
        check_collisions(entity, dt);

		// Find the change in position
		delta_pos.x = obj.position.x - initial_pos.x;
		delta_pos.y = obj.position.y - initial_pos.y;

		// Apply that change to the transform component
		trans.position.x += static_cast<Uint32>(round(delta_pos.x));
		trans.position.y += static_cast<Uint32>(round(delta_pos.y));


    }
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

bool PhysicsSystem::IsOnFloor(BlueEnt &ent) {

	std::shared_ptr g_BlueBridgePtr = g_WeakBlueBridge.lock();
	if (!g_BlueBridgePtr) {

		perror("GIRL WHERE'D THE BRIDGE GO!!!\n ( Error in PhysicsSystem )\n");

		exit(1);
	}

	PhysicsObject &obj = g_BlueBridgePtr -> GetComponent<PhysicsObject>(ent);

	// Loop through all the contacts
	for (auto const& contact_ent : obj.contacts) {

		// Get the obj
		PhysicsObject &contact = g_BlueBridgePtr -> GetComponent<PhysicsObject>(contact_ent);

		// Get the normal of the contact
		bPointF normal = find_collision_normal(obj, contact);



		// If the contact is on the floor
		if (normal.y == 1.0f) {

			return true;
		}
	}

	return false;

}

bool PhysicsSystem::IsOnWall(BlueEnt &ent) {

	std::shared_ptr g_BlueBridgePtr = g_WeakBlueBridge.lock();
	if (!g_BlueBridgePtr) {

		perror("GIRL WHERE'D THE BRIDGE GO!!!\n ( Error in PhysicsSystem )\n");

		exit(1);
	}

	PhysicsObject &obj = g_BlueBridgePtr -> GetComponent<PhysicsObject>(ent);

	// Loop through all the contacts
	for (auto const& contact_ent : obj.contacts) {

		// Get the obj
		PhysicsObject &contact = g_BlueBridgePtr->GetComponent<PhysicsObject>(contact_ent);

		// Get the normal of the contact
		bPointF normal = find_collision_normal(obj, contact);

		// If the contact is on the wall
		if (normal.x == 1.0f || normal.x == -1.0f) {

			return true;
		}
	}
	return false;
}

void PhysicsSystem::SnapToFloor(BlueEnt &ent) {

	std::shared_ptr g_BlueBridgePtr = g_WeakBlueBridge.lock();
	if (!g_BlueBridgePtr) {

		perror("GIRL WHERE'D THE BRIDGE GO!!!\n ( Error in PhysicsSystem )\n");

		exit(1);
	}

	PhysicsObject &obj = g_BlueBridgePtr -> GetComponent<PhysicsObject>(ent);

	// Loop through all the contacts
	for (auto const& contact_ent : obj.contacts) {

		// Get the obj
		PhysicsObject &contact = g_BlueBridgePtr -> GetComponent<PhysicsObject>(contact_ent);

		// Get the normal of the contact
		bPointF normal = find_collision_normal(obj, contact);


		// If the contact is on the floor
		if (normal.y == 1.0f) {

			// Snap to the floor
			obj.position.y = contact.position.y - obj.position.height;

			// We only need one contact
			break;
		}
	}
}

// Make the entity snap to the wall
void PhysicsSystem::SnapToWall(BlueEnt &ent) {

	std::shared_ptr g_BlueBridgePtr = g_WeakBlueBridge.lock();
	if (!g_BlueBridgePtr) {

		perror("GIRL WHERE'D THE BRIDGE GO!!!\n ( Error in PhysicsSystem )\n");

		exit(1);
	}

	PhysicsObject &obj = g_BlueBridgePtr -> GetComponent<PhysicsObject>(ent);

	// Loop through all the contacts
	for (auto const& contact_ent : obj.contacts) {

		// Get the obj
		PhysicsObject &contact = g_BlueBridgePtr->GetComponent<PhysicsObject>(contact_ent);

		// Get the normal of the contact
		bPointF normal = find_collision_normal(obj, contact);

		// If the contact is on the wall
		if (normal.x == 1.0f || normal.x == -1.0f) {


			if (normal.x == 1.0f) {
				// Snap to the wall
				obj.position.x = contact.position.x - obj.position.width;
			}
			else {
				// Snap to the wall
				obj.position.x = contact.position.x + contact.position.width;
			}

			break;
		}
	}
}

float PhysicsSystem::GetWallNormal(BlueEnt &ent) {

	std::shared_ptr g_BlueBridgePtr = g_WeakBlueBridge.lock();
	if (!g_BlueBridgePtr) {

		perror("GIRL WHERE'D THE BRIDGE GO!!!\n ( Error in PhysicsSystem )\n");

		exit(1);
	}

	PhysicsObject &obj = g_BlueBridgePtr -> GetComponent<PhysicsObject>(ent);

	// Loop through all the contacts
	for (auto const& contact_ent : obj.contacts) {

		// Get the obj
		PhysicsObject &contact = g_BlueBridgePtr->GetComponent<PhysicsObject>(contact_ent);

		// Get the normal of the contact
		bPointF normal = find_collision_normal(obj, contact);



		// If the contact is on the wall
		if (normal.x == 1.0f || normal.x == -1.0f) {

			printf("Normal: %f\n", normal.x);
			return (normal.x);
		}
	}

	return 0.0f;
}


// Takes and object and updates their position
void PhysicsSystem::update_obj_positions(PhysicsObject &obj, float dt) {

    // Cap the acceleration
    obj.acceleration.x = fmax(fmin(obj.acceleration.x, obj.maxAcceleration.x), -obj.maxAcceleration.x);
    obj.acceleration.y = fmax(fmin(obj.acceleration.y, obj.maxAcceleration.y), -obj.maxAcceleration.y);

    // Update the velocity
    obj.velocity += obj.acceleration * dt;

    // Apply friction(decay) to velocity and reset acceleration
    // Bee's 1st law of motion: An object in motion will eventually rest
    obj.acceleration  = {0.0f, 0.0f};
    obj.velocity     *= (1.0f - obj.friction );

    // Now we Cap the velocity
    obj.velocity.x = fmax(fmin(obj.velocity.x, obj.maxVelocity.x), -obj.maxVelocity.x);
    obj.velocity.y = fmax(fmin(obj.velocity.y, obj.maxVelocity.y), -obj.maxVelocity.y);

    // Update the position
    obj.position.x += obj.velocity.x * dt;
    obj.position.y += obj.velocity.y * dt;


}

void PhysicsSystem::snap(PhysicsObject &obj) {

	std::shared_ptr g_BlueBridgePtr = g_WeakBlueBridge.lock();
	if (!g_BlueBridgePtr) {

		perror("GIRL WHERE'D THE BRIDGE GO!!!\n ( Error in PhysicsSystem )\n");

		exit(1);
	}

	if (obj.type == SOLID)
		return;

	// Loop through all the contacts
	for (auto const& contact_ent : obj.contacts) {

		// Get the obj
		PhysicsObject &contact = g_BlueBridgePtr->GetComponent<PhysicsObject>(contact_ent);

		// Get the normal of the contact
		bPointF normal = find_collision_normal(obj, contact);

		// If the contact is on the wall
		if (normal.x == 1.0f || normal.x == -1.0f) {


			if (normal.x == 1.0f) {
				// Snap to the wall
				obj.position.x = contact.position.x - obj.position.width;
			} else {
				// Snap to the wall
				obj.position.x = contact.position.x + contact.position.width;
			}

		}

		// If the contact is on the floor
		if (normal.y == 1.0f) {

			// Snap to the floor
			obj.position.y = contact.position.y - obj.position.height;

		}

		// If the contact is on the ceiling
		if (normal.y == -1.0f) {

			// Snap to the floor
			obj.position.y = contact.position.y + contact.position.height;

		}

	}

}

// Checks for collisions between all entities
void PhysicsSystem::check_collisions(const BlueEnt &ent, float dt) {

	std::shared_ptr g_BlueBridgePtr = g_WeakBlueBridge.lock();
    if (!g_BlueBridgePtr) {

        perror("bridge gone yadda yadda\n");
        exit(1);
    }


	PhysicsObject &obj = g_BlueBridgePtr -> GetComponent<PhysicsObject>(ent);

	if (obj.type == SOLID)
		return;



    for (auto const &entity: BlueEntities) {

		PhysicsObject &obj2 = g_BlueBridgePtr -> GetComponent<PhysicsObject>(entity);

		// Make sure we don't check collisions with ourselves
        if (ent != entity) {

			// Check if the two objects are in contact
			check_contact(obj, obj2);

			// Check if the two objects are colliding
            check_collision(obj, obj2, dt);
		}
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
}

void PhysicsSystem::check_contact(PhysicsObject &ent1, PhysicsObject &ent2) {

	// TODO: Make this something that the can be changed with a setter
	const float tolerance = 0.1f;

	// Make bRect for ent2 adding a tolerance
	bRectF ent2_expanded = ent2.position;
	ent2_expanded.x -= tolerance;
	ent2_expanded.y -= tolerance;
	ent2_expanded.width += tolerance * 2;
	ent2_expanded.height += tolerance * 2;

	// Check if ent1 is inside ent2_expanded
	if (ent2_expanded.intersects(ent1.position)) {

		ent1.contacts.push_back(ent2.id);
		ent2.contacts.push_back(ent1.id);

	} else {
		// printf("No contact between %s and %s\n", ent1.name, ent2.name);
		// Remove ent1 from ent2's contacts
		ent2.contacts.erase(std::remove(ent2.contacts.begin(), ent2.contacts.end(), ent1.id), ent2.contacts.end());

		// Remove ent2 from ent1's contacts
		ent1.contacts.erase(std::remove(ent1.contacts.begin(), ent1.contacts.end(), ent2.id), ent1.contacts.end());

		// Trusting Copilot with this one :3
	}

}

// Finds the collision normal between two objects
bPointF PhysicsSystem::find_collision_normal(const PhysicsObject &ent1, const PhysicsObject &ent2) {

	bRectF intersection = ent1.position.intersection(ent2.position);

	// Find the difference in coordinates between the two objects
    float left_diff = abs(intersection.x + intersection.width - ent1.position.x);
    float right_diff = abs(ent1.position.x + ent1.position.width - intersection.x);
    float up_diff = abs(intersection.y + intersection.height - ent1.position.y);
    float down_diff = abs(ent1.position.y + ent1.position.height - intersection.y);

	// Find the smallest difference
    float min_diff = std::min({left_diff, right_diff, up_diff, down_diff});

    if (min_diff == left_diff) {
        return {-1, 0};
    } else if (min_diff == right_diff) {
        return {1, 0};
    } else if (min_diff == up_diff) {
        return {0, -1};
    } else {
        return {0, 1};
    }
}


// Resolves collisions between two entities
void PhysicsSystem::resolve_collision(PhysicsObject &ent1, PhysicsObject &ent2, float dt) {



    /*
        Step 1: Calculate the collision normal.

        Ok so collision normal is the vector perpendicular to the collision surface.
    */

    // Find the collision normal
	bPointF normal_axis = find_collision_normal(ent1, ent2);

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


	// This has false positives sometimes so I'm disabling it for now
    if (nSpd >= 0) {
        printf("The Objects are not moving closer together\n");
      	return;
    }
	*/

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

    if (ent1.type == ACTOR) {

		// Reset acceleration
        ent1.acceleration = {0, 0};

        // Step backwards until the two objects are no longer colliding
		ent1.position -= ent1.velocity * dt;

        ent1.velocity -= deltaV1;
		update_obj_positions(ent1, dt);

		if (ent1.snap)
			snap(ent1);

    }

    if (ent2.type == ACTOR) {

		// Reset acceleration
        ent2.acceleration = {0, 0};

        // Step backwards until the two objects are no longer colliding
		ent2.position -= ent2.velocity * dt;

        ent2.velocity += deltaV2;
		update_obj_positions(ent2, dt);

		if (ent2.snap)
			snap(ent2);
    }

}