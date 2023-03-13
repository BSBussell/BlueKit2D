
// Bee Bussell
// March 12, 2023
// The Physics Object

#ifndef PHYSICS_OBJ_H
#define PHYSICS_OBJ_H

// Where our bRects and bPoints come from
#include <BML/bRect.h>

// @brief A Physics Object
struct PhysicsObject {

    bRectF position;

    bPointF velocity = {0, 0};
    bPointF acceleration = {0, 0};

};

// @brief A Force contains direction and magnitude
typedef bPointF Force;

#endif