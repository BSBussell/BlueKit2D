
// Bee Bussell
// March 3, 2023
// Blue System Manager

/* 
	Heavily Inspired by:
		https://austinmorlan.com/posts/entity_component_system/
*/

#ifndef BLUE_SYS_MANAGER_H
#define BLUE_SYS_MANAGER_H

#include "Core/BlueSys.h"
#include "Core/BlueTypes.h"

#include <memory>
#include <unordered_map>


// How we manage systems
class BlueSysManager {

public:
	
	template<typename T>
	std::shared_ptr<T> RegisterSystem() {

		const char *typeName = typeid(T).name();

		if (_Systems.find(typeName) != _Systems.end()) {

			perror("Girly, you can't register a system more than once💅😇");
			exit(1);
		}

		auto system = std::make_shared<T>();
		_Systems.insert({typeName, system});
		return system;
	}

	template<typename T>
	void SetSignature(Signature signature) {

		const char *typeName = typeid(T).name();

		if (_Systems.find(typeName) == _Systems.end()) {

			perror("Girl you gotta register the system first.");
			exit(1);
		}

		_Signatures.insert({typeName, signature});
	}

	void EntityDestroyed(BlueEnt entity)
	{
		// Erase a destroyed entity from all system lists
		// mEntities is a set so no check needed
		for (auto const& pair : _Systems)
		{
			auto const& system = pair.second;

			system -> BlueEntities.erase(entity);
		}
	}

	void EntitySignatureChanged(BlueEnt entity, Signature entitySignature)
	{
		// Notify each system that an entity's signature changed
		for (auto const& pair : _Systems)
		{
			auto const& type = pair.first;
			auto const& system = pair.second;
			auto const& systemSignature = _Signatures[type];

			// Entity signature matches system signature - insert into set
			if ((entitySignature & systemSignature) == systemSignature)
			{
				system->BlueEntities.insert(entity);
			}
			// Entity signature does not match system signature - erase from set
			else
			{
				system->BlueEntities.erase(entity);
			}
		}
	}


private:

	// Map from system type string pointer to a signature
	std::unordered_map <const char*, Signature> _Signatures{};

	// Map from system type string pointer to a system pointer
	std::unordered_map <const char*, std::shared_ptr<BlueSys> > _Systems{};

};

#endif