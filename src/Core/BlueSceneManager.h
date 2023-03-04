
// Bee Bussell
// March 4, 2023
// BlueSceneManager.cpp

#ifndef BLUE_Scene_MANAGER_H
#define BLUE_Scene_MANAGER_H

#include <memory>
#include <unordered_map>

#include "BlueScene.h"
#include "BlueBridge.h"

extern std::weak_ptr<BlueBridge> g_WeakBlueBridge;

class BlueSceneManager {
public:
    //BlueSceneManager(BlueBridge& bridge) : _bridge(bridge) {}

    // Adds a Blue Scene to the StageManager as a week pointer
    void AddScene(std::shared_ptr<BlueScene> scene) {

        std::pair <std::string, std::weak_ptr<BlueScene>> newScene (
            scene -> GetName(),
            std::weak_ptr(scene)
        );

        _scenes.insert(newScene);
    }

    // Loading a new scene
    void LoadScene(std::string sceneName) {



        // Grab the shared_ptr
        auto CurrentScene = _currentScene.lock();
        if (CurrentScene) {
            CurrentScene->Unload();
        }



        _currentScene = _scenes.at(sceneName);
        CurrentScene = _currentScene.lock();



        if (!CurrentScene) {

            perror("GIRL THAT SCENES FAKE AF\n");
            exit(1);
        }

        // Swap the current BlueBridge instance with the one associated with the new scene
        _bridge = CurrentScene->GetBridge();

        if (!_bridge.lock()) {

            perror("Ok so follow it up :3\n");
            exit(0);
        }
        g_WeakBlueBridge = _bridge;
        

        // Alright so we Finished setting up everything :3
        CurrentScene->Load();
        
    }

    void Update(float deltaTime) {

        // Grab it as a shared_ptr
        auto CurrentScene = _currentScene.lock();

        if (CurrentScene) {
            CurrentScene->Update(deltaTime);
        }
    }

    void Render() {

        // Getting the shared_ptr
        auto CurrentScene = _currentScene.lock();

        if (CurrentScene) {
            CurrentScene -> Render();
        }
    }

private:
    std::weak_ptr<BlueBridge> _bridge;
    std::unordered_map<std::string, std::weak_ptr<BlueScene>> _scenes;
    std::weak_ptr<BlueScene> _currentScene;
};

#endif
