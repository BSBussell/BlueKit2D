
// Bee Bussell
// Mar 4, 2024
// GraphicsSystem.cpp

#include <algorithm>
#include <BML/bSheet.h>
#include "SpriteSystem.h"

extern BlueBridge g_BlueBridge;

void SpriteSystem::Init() {

	// Sort the entities by layer
	RefreshLayers();

	for (auto const& entity : BlueEntities) {


		// Grab needed components
		//Transform &transform = g_BlueBridge.GetComponent<Transform>(entity);
		Sprite &sprite = g_BlueBridge.GetComponent<Sprite>(entity);

    	readSheetFromJSON(BML_GetPath(sprite.filePath).c_str(), sprite.sprite_sheet);
    	sprite.sprite_sheet.startAnimation("default");

    	// Initializes the spritesheet onto the given context
    	auto context = sprite.context.lock();
		if (context) {
    		context->initSpriteSheet(sprite.sprite_sheet);
		} else {
			perror("They Deleted your window :0\n");
			exit(1);
		}
	}
}

void SpriteSystem::Update() {

	for (auto const& entity : BlueEntities) {

		// Grab needed components
		Transform &transform = g_BlueBridge.GetComponent<Transform>(entity);
		Sprite &sprite = g_BlueBridge.GetComponent<Sprite>(entity);

		auto context = sprite.context.lock();
		if (context) {
			
			context->drawSprite(sprite.sprite_sheet, transform.position);
		} else {
			perror("They deleted your window :0\n");
			exit(1);
		}

		//window->drawSprite(spriteSheet, dest);
	}
}

void SpriteSystem::RefreshLayers() {

	// Sort the entities by layer
	std::vector<BlueEnt> Sorted(BlueEntities.begin(), BlueEntities.end());
	std::sort(Sorted.begin(),Sorted.end(), SpriteSystem::compare_layer);
	BlueEntities = std::set<BlueEnt>(Sorted.begin(), Sorted.end());
}


bool SpriteSystem::compare_layer(const BlueEnt &a, const BlueEnt &b) {

	Sprite &spriteA = g_BlueBridge.GetComponent<Sprite>(a);
	Sprite &spriteB = g_BlueBridge.GetComponent<Sprite>(b);

	int layer1 = spriteA.layer;
	int layer2 = spriteB.layer;

	return layer1 > layer2;
}
