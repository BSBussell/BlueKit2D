
// Bee Bussell
// March 12, 2023
// The Physics Object

#ifndef PHYSICS_OBJ_H
#define PHYSICS_OBJ_H

// Where our bRects and bPoints come from
#include <BML/bRect.h>

#include <vector>

enum BodyType {
    STATIC, // Not Moved by Collisions
    DYNAMIC // Can be moved
};

// @brief A Physics Object
struct PhysicsObject {

    // @brief The position of the object
    bRectF position;

    // @brief The velocity of the object
    bPointF velocity = {0, 0};

    // @brief The acceleration of the object
    bPointF acceleration = {0, 0};

    // @brief The friction of the object(0.0 - 1.0)
    float friction = 0.0f;

    // @brief The maximum velocity of the object
    bPointF maxVelocity = {100, 100};

    // @brief The maximum acceleration of the object
    bPointF maxAcceleration = {100, 100};

	// @brief The restitution of the object the higher the more bouncy :3
    float restitution = 1.0f;

    // @brief The mass of the object
    float mass = 1.0f;

    // @brief The type of body
	// @note STATIC bodies are not moved by collisions
    BodyType type = STATIC;

    // @brief The name of the object for internal
    std::string name = "PhysicsObject";

    // @brief The color used when rendering collision boxes
    SDL_Color render_color = {0, 0, 255, 50};

    // @brief The PhysicObjects we are contacting
    std::vector<std::weak_ptr<PhysicsObject>> contacts;

};

// @brief A Force contains direction and magnitude
typedef bPointF Force;

#endif