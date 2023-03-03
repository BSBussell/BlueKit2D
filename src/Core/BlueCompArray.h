
// Bee Bussell
// March 3, 2023
// Blue Component Array

/* 
	Heavily Inspired by:
		https://austinmorlan.com/posts/entity_component_system/
*/

#ifndef BLUE_COMP_ARRAY_H
#define BLUE_COMP_ARRAY_H

#include <stdint.h>
#include <unordered_map>

#include "Core/BlueTypes.h"

// Virtual Inheritance
class _blue_comp_array {

public:
	virtual ~_blue_comp_array() = default;
	virtual void BlueEntDestroyed(BlueEnt entity) = 0;
};

/*
 * So essentially this template allows use to create a component array for each unique component
 */

template<typename T>
class BlueCompArray : public _blue_comp_array {

public:

	/*
		InsertComponent() Essentially adds the component to the array of components and manages the maps

		@param BlueEnt entity: The entity the component belongs to
		@param T component: The component struct we're attatching
	 */
	void InsertComponent(BlueEnt entity, T component) {

		if (_EntityToIndexMap.find(entity) == _EntityToIndexMap.end()) {

			perror("Girly, you already added this component...");
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

	/*
		RemoveData() Essentially just removes the component from the entity

		@param BlueEnt entity: the entity the component is removed from
	 */
	void RemoveComponent(BlueEnt entity) {

		if (_EntityToIndexMap.find(entity) == _EntityToIndexMap.end()) {

			perror("Girly, you gotta add the component before you can remove it...")
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

	/*
		GetData() Returns the component given an entity

		@param BlueEnt entity: the entity we're grabbing
	 */
	T& GetData(BlueEnt entity) {

		if (_EntityToIndexMap.find(entity) == _EntityToIndexMap.end()) {

			perror("Girl, you trynna get info about a component you don't have");
			exit(1);
		}

		// Return a reference to the BlueEnt's component
		return _ComponentArray[_EntityToIndexMap[entity]];
	}

	/*
		BlueEntDestroyed() overrides the initial method and catches when an entity is destroyed

		@param BlueEnt entity: The entity that was destroyed
	 */
	void BlueEntDestroyed(BlueEnt entity) override {

		if (_EntityToIndexMap.find(entity) != _EntityToIndexMap.end()) {

			RemoveComponent(entity);
		}
	}

private:

	// Packed array of components
	// Set to a specified max amount, matching the maximum number
	std::array<T, MAX_ENTITIES> _ComponentArray;

	std::unordered_map<BlueEnt, size_t> _EntityToIndexMap;

	std::unordered_map<size_t, BlueEnt> _IndexToEntityMap;

	// Total size of the valid entries in the array
	size_t _Size;
};

#endif