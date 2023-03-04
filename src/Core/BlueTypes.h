
// Bee Bussell
// March 3, 2023
// Types Used throughout the Engine

#ifndef BLUE_TYPES_H
#define BLUE_TYPES_H

#include <bitset>
#include <cstdint>

using BlueEnt = uint32_t;
using BlueCompType = uint8_t;

const BlueEnt MAX_ENTITIES = 1024;
const BlueCompType MAX_COMPONENTS = 32;

using Signature = std::bitset<MAX_COMPONENTS>;


#endif