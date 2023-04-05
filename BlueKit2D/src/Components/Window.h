
// Bee Bussell
// Mar 4 2023
// A Window


#ifndef WINDOW_H
#define WINDOW_H

#include <BML/bWindow.h>

#include "Transform.h"

struct Window {

	std::shared_ptr<bWindow> window;
	std::string name;	
};

#endif