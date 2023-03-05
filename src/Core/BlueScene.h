
// Bee Bussell
// March 4, 2023
// BlueScene.h


#ifndef BLUE_SCENE_H
#define BLUE_SCENE_H

#include "BlueBridge.h"
#include "BlueTypes.h"

#include <string>
#include <memory>
#include <vector>


// Abstract BlueScene
class BlueScene {
public:

    // THE SCENE IS THE OWNER OF THE BRIDGES
    BlueScene(std::string name) : _name(name) {

        printf("Creating a brand new ECS instance\n");
        _bridge = std::make_shared<BlueBridge>();
    }

    void AddEntity(BlueEnt entity) {
        _entities.push_back(entity);
    }

    std::string GetName() {

        return _name;
    }

    // Give us your bridge and make it weak!
    std::weak_ptr<BlueBridge> GetBridge() {

        return std::weak_ptr<BlueBridge>(_bridge);
    }

    // ##  Methods You Override   ##
    // #############################
    // #############################

    // Destroyer
    virtual ~BlueScene() {}

    // Called when you are switching in/out
    virtual void Load() = 0;
    virtual void Unload() = 0;

    // Physics/Input
    virtual void Update(float deltaTime) = 0;
    
    // Self Explanatory
    virtual void Render() = 0;


    std::vector<BlueEnt> _entities;
    std::shared_ptr<BlueBridge> _bridge;

private:

    // This is where you need to register all the componets your Scene needs
    virtual void _Register_Components() = 0;

    // This is where you need to register all the Systems your scene needs
    virtual void _Register_Systems() = 0;

    std::string _name;
    // std::shared_ptr<BlueBridge> _bridge;
    // std::vector<BlueEnt> _entities;
};

#endif // BLUE_SCENE_H
