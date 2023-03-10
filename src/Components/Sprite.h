
// Bee Bussell
// Mar 4 2023
// The Sprite


#ifndef SPRITE_H
#define SPRITE_H

#include <BML/bRect.h>
#include <memory>

#include "Components/Transform.h"

struct Sprite {

	std::string filePath;

	bSheet sprite_sheet;
	
	// Which layer to render on
	int layer;

	std::weak_ptr<bRenderer> context;
};

#endif