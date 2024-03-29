
// Bee Bussell
// Feb 27, 2023
// Blue Object

/* 
	Heavily Inspired by:
		https://austinmorlan.com/posts/entity_component_system/
*/


#ifndef BLUE_ENT_H
#define BLUE_ENT_H

#include <array>
#include <queue>

#include "BlueTypes.h"

class BlueEntManager {

/* My professors would kill me if they saw this header file */

// Also, might make this all static, just trying to figure out
// Why would we need to have more than one ent manager
public:
	
	BlueEntManager() {

		for (BlueEnt ent = 0; ent <  MAX_ENTITIES; ent++) {

			_AvailableEnts.push(ent);
		}
	}

	BlueEnt CreateEntity() {

		if (_LivingEntityCount >= MAX_ENTITIES) {

			/*
				If I ever wanna Handle Entity Overflow. . .
			*/
			perror("Too many BlueEnts, calm down girly :3");
			exit(1);
		}



		BlueEnt id = _AvailableEnts.front();
		//printf("Creating Entity: %d\n", id);
		_AvailableEnts.pop();
		_LivingEntityCount++;

		return id;
	}
	void DestroyEntity(BlueEnt entity) {

		//printf("Destroying Entity: %d\n", entity);
		if (_LivingEntityCount >= MAX_ENTITIES) {

			/*
				If I ever wanna Handle Entity Overflow. . .
			*/
			perror("Your BlueEnts are outta range girly");
			exit(1);
		}
		//printf("Destroying Entity: %d\n", entity);
		_Signatures[entity].reset();

		_AvailableEnts.push(entity);
		_LivingEntityCount--;
	}

	void SetSignature(BlueEnt entity, Signature signature) {

		if (_LivingEntityCount >= MAX_ENTITIES) {

			/*
				If I ever wanna Handle Entity Overflow. . .
			*/
			perror("BlueEnts outta range girly\n");
			exit(1);
		}
		_Signatures[entity] = signature;
	}

	Signature GetSignature(BlueEnt entity) {

		if (_LivingEntityCount >= MAX_ENTITIES) {

			/*
				If I ever wanna Handle Entity Overflow. . .
			*/
			perror("BlueEnts outta range girly\n");
			exit(1);
		}
		return _Signatures[entity];		
	}

private:

	// You can tell I reccently had a professor teach me C#,
	// Because I started adding underscores to everything :3

	// Queue of unused entity IDs
	std::queue<BlueEnt> _AvailableEnts{};

	// Array of Signatures where the index corresponds to the BlueEnt ID
	std::array<Signature, MAX_ENTITIES> _Signatures{};

	// Total living entities - used for keeping limits
	Uint32 _LivingEntityCount{};
};


#endif
