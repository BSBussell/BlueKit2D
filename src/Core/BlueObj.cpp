
// Bee Bussell
// Feb 27, 2023
// Blue Object

#include "BlueObj.h"
#include "Transform.h"


BlueObj::BlueObj() {

	pos = {0,0,0,0};
}

BlueObj::~BlueObj() {

}

bRect BlueObj::get_pos() {

	return pos;
}

void BlueObj::set_pos(bRect update) {

	pos = update;
}
