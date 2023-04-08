
// Bee Bussell
// March 3, 2023
// Blue System

/* 
	Heavily Inspired by:
		https://austinmorlan.com/posts/entity_component_system/
*/

#ifndef BLUE_SYS_H
#define BLUE_SYS_H

#include "Core/BlueTypes.h"
#include <set>


// Every System will inherit from this and have a list of entities called
// BlueEntities
class BlueSys {

public:
	std::set<BlueEnt> BlueEntities;


};

#endif