
// Bee Bussell
// March 3, 2023
// Bridges the ECS

/* 
	Heavily Inspired by:
		https://austinmorlan.com/posts/entity_component_system/
*/

#ifndef BLUE_BRIDGE_H
#define BLUE_BRIDGE_H

#include <memory>

#include "BlueTypes.h"
#include "BlueCompManager.h"
#include "BlueEntManager.h"
#include "BlueSysManager.h"


// Every System will inherit from this and have a list of entities called
// BlueEntities
class BlueBridge {

public:
	
	void Init() {

		// Pointer to each manager
		_ComponentManager = std::make_unique<BlueCompManager>();
		_EntityManager = std::make_unique<BlueEntManager>();
		_SystemManager = std::make_unique<BlueSysManager>();
	}

	// Entity Methods
	BlueEnt CreateEntity() {

		return _EntityManager->CreateEntity();
	}

	void DestroyEntity(BlueEnt entity) {

		_EntityManager->DestroyEntity(entity);
		_ComponentManager->EntityDestroyed(entity);
		_SystemManager->EntityDestroyed(entity);
	}

	// Component Methods
	template<typename T>
	void RegisterComponent() {
		_ComponentManager->RegisterComponent<T>();
	}

	template<typename T>
	void AddComponent(BlueEnt entity, T component) {

		_ComponentManager -> AddComponent<T>(entity, component);

		auto signature = _EntityManager -> GetSignature(entity);
		signature.set(_ComponentManager -> GetComponentType<T>(), true);
		_EntityManager -> SetSignature(entity, signature);

		_SystemManager -> EntitySignatureChanged(entity, signature);
	}

	template<typename T>
	void RemoveComponent(BlueEnt entity)
	{
		_ComponentManager->RemoveComponent<T>(entity);

		auto signature = _EntityManager->GetSignature(entity);
		signature.set(_ComponentManager->GetComponentType<T>(), false);
		_EntityManager->SetSignature(entity, signature);

		_SystemManager->EntitySignatureChanged(entity, signature);
	}

	template<typename T>
	T& GetComponent(BlueEnt entity)
	{
		return _ComponentManager->GetComponent<T>(entity);
	}

	template<typename T>
	BlueCompType GetComponentType()
	{
		return _ComponentManager->GetComponentType<T>();
	}


	// System Methods
	template<typename T>
	std::shared_ptr<T> RegisterSystem() {

		return _SystemManager->RegisterSystem<T>();
	}

	template<typename T>
	void SetSystemSignature(Signature signature) {

		_SystemManager->SetSignature<T>(signature);
	}

private:

	std::unique_ptr<BlueCompManager> _ComponentManager;
	std::unique_ptr<BlueEntManager> _EntityManager;
	std::unique_ptr<BlueSysManager> _SystemManager;
};

//BlueBridge g_BlueBridge

#endif