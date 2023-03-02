
// Bee Bussell
// Feb 27, 2023
// Blue Object

/*
 * Essentially the Entity for our ECS
 */ 


#ifndef BlueObj_h
#define BlueObj_h

#include "main.h"
#include "Transform.h"
#include <bitset>
#include <cstdint>

using BlueObj = uint32_t;
using ComponentType = uint8_t;

const BlueObj MAX_ENTITIES = 1000;
const ComponentType MAX_COMPONENTS = 32;

using Signature = std::bitset<MAX_COMPONENTS>;

class BlueObjManager {

public:
	BlueObjManager();
	~BlueObjManager();

	BlueObj CreateObj();
	void DestrouObj();

	void SetSignature();
	Signature GetSignature();

private:

	// Queue of unused entity IDs
	std::queue<BlueObj> mAvailableObjs{};

	// Array of Signatures where the index corresponds to the BlueObj ID
	std::array<Signature, MAX_ENTITIES> mSignatures{};

	// Total living entities - used for keeping limits
	uint32_t mLivingEntityCount{};
};



class BlueObj {


public:

	BlueObj();
	~BlueObj();

	bRect get_pos();
	void set_pos(bRect update);

private:

	bRect pos;


};

#endif
