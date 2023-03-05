
// Bee Bussell
// Mar 4, 2023
// SpriteSystem.h

#ifndef SPRITE_SYSTEM_H
#define SPRITE_SYSTEM_H

#include <memory>
#include <BML/BML.h>

#include "Core/BlueBridge.h"
#include "Components/Sprite.h"
#include "Components/Transform.h"



class SpriteSystem : public BlueSys {
	
public:

	// Variables all systems should have :3
	void Init();
	void Update();
	void Render();
	void Close();

	// Call this as rarely as possible
	void RefreshLayers();


	SpriteSystem() {}

private:

	static bool compare_layer(const BlueEnt &a, const BlueEnt &b);

	//std::shared_ptr<BlueBridge> get_bridge();
};

#endif