
// Bee Bussell
// March 3, 2023
// Types Used throughout the Engine

#ifndef BLUE_TYPES_H
#define BLUE_TYPES_H

#include <bitset>
#include <cstdint>
#include <SDL2/SDL.h> 

using BlueEnt = Uint32;
using BlueCompType = Uint8;

const BlueEnt MAX_ENTITIES = 6000;
const BlueCompType MAX_COMPONENTS = 32;

using Signature = std::bitset<MAX_COMPONENTS>;


#endif