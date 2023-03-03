
// Bee Bussell
// March 3, 2023
// Blue System

/* 
	Heavily Inspired by:
		https://austinmorlan.com/posts/entity_component_system/
*/

#ifndef BLUESYS_H
#define BLUESYS_H

#include <set>

#include "Core/BlueTypes.h"


// Every System will inherit from this and have a list of entities called
// BlueEntities
class BlueSys {

public:
	std::set<BlueEnt> BlueEntities;

}

#endif