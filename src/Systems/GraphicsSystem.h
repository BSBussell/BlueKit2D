
// Bee Bussell
// Mar 4, 2023
// GraphicsSystem.h

#ifndef GRAPHICS_SYSTEM_H
#define GRAPHICS_SYSTEM_H

#include <memory>
#include <algorithm>
#include <BML/bWindow.h>

#include "Core/BlueBridge.h"
#include "Components/Window.h"

class Graphics : public BlueSys {
	
public:
	void Init();
	void Update();
	void Close();

};

#endif