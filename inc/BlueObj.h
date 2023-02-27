
// Bee Bussell
// Feb 27, 2023
// Blue Object

#ifndef BlueObj_h
#define BlueObj_h

#include "main.h"

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
