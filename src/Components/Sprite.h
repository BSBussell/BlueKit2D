
// Bee Bussell
// Mar 4 2023
// The Sprite


#ifndef SPRITE_H
#define SPRITE_H

#include <BML/bRect.h>
#include <memory>

#include "Components/Transform.h"
#include "Components/Window.h"

struct Sprite {

	bSheet sprite_sheet;
	
	std::weak_ptr<bWindow> window;
};

#endif