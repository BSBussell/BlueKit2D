
// Bee Bussell
// March 4, 2023
// BlueScene.h

#ifndef BLUE_SCENE_H
#define BLUE_SCENE_H


#include "Core/BlueSceneManager.h"
#include "Core/BlueBridge.h"
#include "Core/BlueTypes.h"

#include <string>
#include <memory>
#include <vector>


// I Love Circular Dependacies :D
class BlueSceneManager;

// Abstract BlueScene
class BlueScene {
public:

    // THE SCENE IS THE OWNER OF THE BRIDGES
    BlueScene(std::string name, std::shared_ptr<bRenderer> context) : _name(name) {

        // Adding the Scene
        _bridge = std::make_shared<BlueBridge>();
        _context = std::weak_ptr<bRenderer>(context);
    }

    void AddEntity(BlueEnt entity) {
        _entities.push_back(entity);
    }

    std::string GetName() {

        return _name;
    }

    // Used by SceneManager, nothing else plz
    void AssignStage(std::weak_ptr<BlueSceneManager> stage) {

        _stage = stage;
    }

    // Give us your bridge and make it weak!
    std::weak_ptr<BlueBridge> GetBridge() {

        return std::weak_ptr<BlueBridge>(_bridge);
    }

    /*
       ##  Methods You Override   ##
       #############################
       #############################
    */
   
    // Destroyer
    virtual ~BlueScene() {}

    // Called when you are switching in/out
    virtual void Load() = 0;
    virtual void Unload() = 0;

    // Physics/Input
    virtual void Update(float deltaTime) = 0;
    
    // Self Explanatory
    virtual void Render() = 0;




protected:

    // This is where you need to register all the componets your Scene needs
    virtual void _Register_Components() = 0;

    // This is where you need to register all the Systems your scene needs
    virtual void _Register_Systems() = 0;

    std::string _name;

    // The entities in our scene
    std::vector<BlueEnt> _entities;

    // The Scenes ECS instance shared because bridges are stored with the scene
    std::shared_ptr<BlueBridge> _bridge;
    
    // The window our scene will exist on 
    std::weak_ptr<bRenderer> _context;

    // The SceneManager that our scene exists in :3
    std::weak_ptr<BlueSceneManager> _stage;

};

#endif // BLUE_SCENE_H
