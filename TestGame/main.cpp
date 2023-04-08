// Bee Bussell
// Sept 17, 2022
// Testing animation class creator.

#include "main.h"

std::weak_ptr<BlueBridge> g_WeakBlueBridge;

int main() {

	auto SimpleScene = std::make_shared<Simple_Scene>("init");
	auto StressfulScene = std::make_shared<Stress_Scene>("stress");
	BlueGame game(SimpleScene, "Physics Test");
	game.AddScene(StressfulScene);
	game.SetDisplayFPS(true);
	game.target_fps = 60;
	game.Run();
	return 0;
}