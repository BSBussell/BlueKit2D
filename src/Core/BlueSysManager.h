
// Bee Bussell
// March 3, 2023
// Blue System Manager

/* 
	Heavily Inspired by:
		https://austinmorlan.com/posts/entity_component_system/
*/

#ifndef BLUE_SYS_MANAGER_H
#define BLUE_SYS_MANAGER_H

#include <set>

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

			perror("Girl you trynna register a system more than once");
			exit(1);
		}

		auto system = std::make_shared<T>();
		_Systems.insert({typeName, system});
		return system;
	}

	template<typename T>
	void SetSignature(Signature signature) {

		const char *typeName = typeid(t).name();
	}

private:

	// Map from system type string pointer to a signature
	std::unordered_map<const char*, Signature> _Signatures{};

	// Map from system type string pointer to a system pointer
	std::unordered_map<const char*, std::shared_ptr<BlueSys>> _Systems{};

}

#endif