
// Bee Bussell
// March 3, 2023
// Blue Component Manager

/* 
	Heavily Inspired by:
		https://austinmorlan.com/posts/entity_component_system/
*/

#ifndef BLUE_COMP_MGMT_H
#define BLUE_COMP_MGMT_H

#include <memory>
#include <unordered_map>

#include "Core/BlueTypes.h"
#include "Core/BlueCompArray.h"

class BlueCompManager {

public:
	template<typename T>
	void RegisterComponent() {

		const char *typeName = typeid(T).name();

		if (_ComponentTypes.find(typeName) != _ComponentTypes.end()) {

			perror("Girl, stop trynna register a component twice");
			exit(1);
		}

		// Add this component type to the component type map
		_ComponentTypes.insert({typeName, _NextComponentType});

		// Create a BlueCompArray pointer and add it to the comp arrays map
		_ComponentArrays.insert({typeName, std::make_shared<BlueCompArray<T>>()});

		// Increment the value so that the next component registered will be different
		_NextComponentType++;

	}

	template<typename T>
	BlueCompType GetComponentType() {

		const char *typeName = typeid(T).name();

		if (_ComponentTypes.find(typeName) == _ComponentTypes.end()) {

			perror("Girl, I think you could just call register Component maybe...");
			exit(1);
		}

		// Return the components type - used for creating signatures
		return _ComponentTypes[typeName];
	}

	template<typename T>
	void AddComponent(BlueEnt entity, T component) {

		// I absolutly hate thi
		GetComponentArray<T>() -> InsertComponent(entity,component);
	}

	template<typename T>
	void RemoveComponent(BlueEnt entity) {

		GetComponentArray<T>() -> RemoveComponent(entity);
	}

	template<typename T>
	T& GetComponent(BlueEnt entity) {

		return GetComponentArray<T>() -> GetData(entity);
	}

private:

	// Map from c-string to a component type
	std::unordered_map<const char*, BlueCompType> _ComponentTypes{};

	// Map from c-string to component array
	std::unordered_map<const char*, std::shared_ptr<_blue_comp_array>> _ComponentArrays{};

	// The component type to be assigned to the next registered component - starting at 0
	BlueCompType _NextComponentType{};

	template<typename T>
	std::shared_ptr<BlueCompArray<T>> GetComponentArray()
	{
		const char* typeName = typeid(T).name();
		if (_ComponentTypes.find(typeName) == _ComponentTypes.end()) {

			perror("Girly, you gotta register this component before you can use it 💅😇");
			exit(1);
		}

		return std::static_pointer_cast<ComponentArray<T>>(mComponentArrays[typeName]);
	}

};

#endif