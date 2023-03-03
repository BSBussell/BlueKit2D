
// Bee Bussell
// March 3, 2023
// Types Used throughout the Engine

#ifndef BLUETYPES_H
#define BLUETYPES_H

#include <bitset>
#include <cstdint>

using BlueEnt = uint32_t;
using BlueCompType = uint8_t;

const BlueEnt MAX_ENTITIES = 1000;
const BlueCompType MAX_COMPONENTS = 32;

using Signature = std::bitset<MAX_COMPONENTS>;


#endif