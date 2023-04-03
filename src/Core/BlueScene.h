
// Bee Bussell
// March 4, 2023
// BlueScene.h

#ifndef BLUE_SCENE_H
#define BLUE_SCENE_H


#include "BlueSceneManager.h"
#include "BlueBridge.h"
#include "BlueTypes.h"

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

	BlueScene(std::string name) : _name(name) {

		// Adding the Scene
		_bridge = std::make_shared<BlueBridge>();
	}

    void AddEntity(BlueEnt entity) {
        _entities.push_back(entity);
    }

    std::string GetName() {

        return _name;
    }

	void AssignContext(std::shared_ptr<bRenderer> context) {

		_context = std::weak_ptr<bRenderer>(context);
	}

    // Used by SceneManager, nothing else plz
    void AssignStage(std::weak_ptr<BlueSceneManager> stage) {

        _stage = stage;
    }

    // Give us your bridge and make it weak!
    std::weak_ptr<BlueBridge> GetBridge() {

        return std::weak_ptr<BlueBridge>(_bridge);
    }

	std::shared_ptr<bRenderer> GetContext() {

		// Check if the context is expired
		if (_context.expired()) {
			fprintf(stderr, "%s's context is expired\n", _name.c_str());
			exit(1);
		}

		return _context.lock();
	}

	std::shared_ptr<BlueSceneManager> GetStage() {

		// Check if the context is expired
		if (_stage.expired()) {
			fprintf(stderr, "%s's stage is expired\n", _name.c_str());
			exit(1);
		}

		return _stage.lock();
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
    virtual bool Update(float deltaTime) = 0;
    
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
    
    // The renderer our scene will exist on
    std::weak_ptr<bRenderer> _context;

    // The SceneManager that our scene exists in :3
    std::weak_ptr<BlueSceneManager> _stage;

};

#endif // BLUE_SCENE_H
