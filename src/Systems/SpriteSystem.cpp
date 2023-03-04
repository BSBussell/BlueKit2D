
// Bee Bussell
// Mar 4, 2024
// GraphicsSystem.cpp

#include "SpriteSystem.h"

extern std::weak_ptr<BlueBridge> g_WeakBlueBridge;

void SpriteSystem::Init() {

	std::shared_ptr g_BlueBridgePtr = g_WeakBlueBridge.lock();
	if (!g_BlueBridgePtr) {

		perror("GIRL WHERE'D THE BRIDGE GO!!!");
		exit(1);
	}

	// Sort the entities by layer
	RefreshLayers();

	for (auto const& entity : BlueEntities) {


		// Grab needed components
		//Transform &transform = g_BlueBridge.GetComponent<Transform>(entity);
		Sprite &sprite = g_BlueBridgePtr -> GetComponent<Sprite>(entity);

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

	std::shared_ptr g_BlueBridgePtr = g_WeakBlueBridge.lock();
	if (!g_BlueBridgePtr) {

		perror("GIRL WHERE'D THE BRIDGE GO!!!");
		exit(1);
	}

	for (auto const& entity : BlueEntities) {

		// Grab needed components
		Transform &transform = g_BlueBridgePtr -> GetComponent<Transform>(entity);
		Sprite &sprite = g_BlueBridgePtr -> GetComponent<Sprite>(entity);

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

	std::shared_ptr g_BlueBridgePtr = g_WeakBlueBridge.lock();
	if (!g_BlueBridgePtr) {

		perror("GIRL WHERE'D THE BRIDGE GO!!!");
		exit(1);
	}

	Sprite &spriteA = g_BlueBridgePtr -> GetComponent<Sprite>(a);
	Sprite &spriteB = g_BlueBridgePtr -> GetComponent<Sprite>(b);

	int layer1 = spriteA.layer;
	int layer2 = spriteB.layer;

	return layer1 > layer2;
}
