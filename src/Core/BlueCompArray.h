
// Bee Bussell
// March 3, 2023
// Component Array

/* 
	Heavily Inspired by:
		https://austinmorlan.com/posts/entity_component_system/
*/

#ifndef COMPARRAY_H
#define COMPARRAY_H

#include "BlueTypes.h"

// Virtual Inheritance
class _blue_comp_array {

public:
	virtual ~_blue_comp_array() = default;
	virtual void BlueEntDestroyed(BlueEnt entity) = 0;
};

template<typename T>
class BlueCompArray : public _blue_comp_array {

public:

	void InsertComponent(BlueEnt entity, T component) {

		if (_EntityToIndexMap.find(entity) == _EntityToIndexMap.end()) {

			perror("Girly, you already added this component...")
			exit(1);
		}

		// Setting our components index to be the size
		size_t newIndex = _Size;

		// Add the new index to the entity to index map
		_EntityToIndexMap[entity] = newIndex;

		// Add the entity to the index to entity map
		_IndexToEntityMap[newIndex] = entity;

		// Add the component to the component array
		_ComponentArray[newIndex] = component;
		_Size++;
	}

	void RemoveData(BlueEnt entity) {

		if (_EntityToIndexMap.find(entity) == _EntityToIndexMap.end()) {

			perror("Girly, you gotta add the component first!...")
			exit(1);
		}

		// Copy element at end into deleted element's place to maintain density
		size_t indexOfRemovedBluEnt = _EntityToIndexMap[entity];
		size_t indexOfLastElement = _Size - 1;
		_ComponentArray[indexOfRemovedBluEnt] = _ComponentArray[indexOfLastElement];

		// Update map to point to moved spot
		BlueEnt finalEnt = _IndexToEntityMap[indexOfLastElement];
		_EntityToIndexMap[finalEnt] = indexOfRemovedBluEnt;
		_IndexToEntityMap[indexOfRemovedBluEnt] = finalEnt;

		// Actually remove the entity
		_EntityToIndexMap.erase(entity);
		_IndexToEntityMap.erase(indexOfLastElement);

		_Size--;
	}

private:

	// Packed array of components
	// Set to a specified max amount, matching the maximum number
	// IDK why we use std::array
	std::array<T, MAX_ENTITIES> _ComponentArray;

	std::unordered_map<BlueEnt, size_t> _EntityToIndexMap;

	std::unordered_map<size_t, BlueEnt> _IndexToEntityMap;

	// Total size of the valid entries in the array
	size_t _Size;
}

#endif