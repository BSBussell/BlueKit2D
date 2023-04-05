
// Bee Bussell
// Mar 4, 2024
// GraphicsSystem.cpp

#include "SpriteSystem.h"

extern std::weak_ptr<BlueBridge> g_WeakBlueBridge;

void SpriteSystem::Init() {

	std::shared_ptr g_BlueBridgePtr = g_WeakBlueBridge.lock();
	if (!g_BlueBridgePtr) {

		perror("GIRL WHERE'D THE BRIDGE GO!!!\n ( Error in SpriteSystem )\n");
		exit(1);
	}

	// Sort the entities by layer
	RefreshLayers();

	for (auto const& entity : BlueEntities) {


		// Grab needed components
		Sprite &sprite = g_BlueBridgePtr -> GetComponent<Sprite>(entity);

    	// BML function for reading ASEPRITE json to bSheet
    	readSheetFromJSON(sprite.filePath.c_str(), sprite.sprite_sheet);
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

	// Yeah this isn't gonna be in the final build haha
	RefreshLayers();
}

void SpriteSystem::Render() {

	std::shared_ptr g_BlueBridgePtr = g_WeakBlueBridge.lock();
	if (!g_BlueBridgePtr) {

		perror("GIRL WHERE'D THE BRIDGE GO!!!\n ( Error in SpriteSystem )\n");
		exit(1);
	}

	//printf("Looping through entities\n");
	for (auto const& entity : BlueEntities) {

		// Grab needed components
		Transform &transform = g_BlueBridgePtr -> GetComponent<Transform>(entity);
		Sprite &sprite = g_BlueBridgePtr -> GetComponent<Sprite>(entity);

		auto context = sprite.context.lock();
		if (context) {
			//printf("Rendering sprite\n");
			context->drawSprite(sprite.sprite_sheet, transform.position);
		} else {
			perror("They deleted your window :0\n");
			exit(1);
		}

		//window->drawSprite(spriteSheet, dest);
	}
	//printf("Done looping through entities\n");
}

void SpriteSystem::Close() {

	std::shared_ptr g_BlueBridgePtr = g_WeakBlueBridge.lock();
	if (!g_BlueBridgePtr) {

		perror("GIRL WHERE'D THE BRIDGE GO!!!\n ( Error in SpriteSystem )\n");
		exit(1);
	}

	for (auto const& entity : BlueEntities) {

		Sprite &sprite = g_BlueBridgePtr -> GetComponent<Sprite>(entity);
		sprite.sprite_sheet.stopAnimation();


		auto context = sprite.context.lock();
		if (context) {
			
			context -> unloadSpriteSheet(sprite.sprite_sheet);

		} else {
			perror("They deleted your window :0\n");
			exit(1);
		}
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

		perror("GIRL WHERE'D THE BRIDGE GO!!!\n ( Error in SpriteSystem )\n");
		exit(1);
	}

	Sprite &spriteA = g_BlueBridgePtr -> GetComponent<Sprite>(a);
	Sprite &spriteB = g_BlueBridgePtr -> GetComponent<Sprite>(b);

	int layer1 = spriteA.layer;
	int layer2 = spriteB.layer;

	return layer1 > layer2;
}
