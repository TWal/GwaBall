#include "Game.h"

#include <OIS.h>
#include <sstream>
#include "Graphics/GraphicsEngine.h"
#include "Physics/PhysicsEngine.h"
#include <OgreRenderWindow.h>
//
//Just for tests
#include <Ogre.h>
#include <btBulletDynamicsCommon.h>

Game::Game() {
    _physics = new PhysicsEngine(this);
    _graphics = new GraphicsEngine(this);

    _physics->attachEngines(_physics, _graphics);
    _graphics->attachEngines(_physics, _graphics);

    OIS::ParamList pl;
    size_t windowHnd = 0;
    std::ostringstream windowHndStr;

    _graphics->getRenderWindow()->getCustomAttribute("WINDOW", &windowHnd);
    windowHndStr << windowHnd;
    pl.insert(std::make_pair("WINDOW", windowHndStr.str()));
    pl.insert(std::make_pair(std::string("x11_mouse_grab"), std::string("false")));
    pl.insert(std::make_pair(std::string("x11_mouse_hide"), std::string("false")));
    pl.insert(std::make_pair(std::string("x11_keyboard_grab"), std::string("false")));
    _inputManager = OIS::InputManager::createInputSystem(pl);

    _mouse = (OIS::Mouse*)_inputManager->createInputObject(OIS::OISMouse, true);
    _keyboard = (OIS::Keyboard*)_inputManager->createInputObject(OIS::OISKeyboard, true);

    unsigned int width, height, depth;
    int top, left;

    _graphics->getRenderWindow()->getMetrics(width, height, depth, left, top);

    const OIS::MouseState& ms = _mouse->getMouseState();
    ms.width = width;
    ms.height = height;


    //Test
    btCollisionShape* shape = new btBoxShape(btVector3(1, 1, 1));
    Ogre::SceneNode* fixedBoxNode = _graphics->getSceneManager()->getRootSceneNode()->createChildSceneNode();
    fixedBoxNode->attachObject(_graphics->getSceneManager()->createEntity("FixedBox", "BoxTest.mesh"));
    fixedBoxNode->setPosition(0, 0, 0);
    _physics->addRigidBody(0.0, shape, fixedBoxNode);

    Ogre::SceneNode* fallingBoxNode = _graphics->getSceneManager()->getRootSceneNode()->createChildSceneNode();
    fallingBoxNode->attachObject(_graphics->getSceneManager()->createEntity("FallingBox", "BoxTest.mesh"));
    fallingBoxNode->setPosition(0, 5, -1);
    _physics->addRigidBody(2.0, shape, fallingBoxNode);
    _time = std::chrono::high_resolution_clock::now();
}

Game::~Game() {
    _inputManager->destroyInputObject(_mouse);
    _inputManager->destroyInputObject(_keyboard);
    OIS::InputManager::destroyInputSystem(_inputManager);
}

void Game::run() {
    running = true;
    changeState(GS_RUNNING);
    while(running) {
        _lastTime = _time;
        _time = std::chrono::high_resolution_clock::now();
        double elapsedTime = (std::chrono::duration_cast<std::chrono::duration<double>>(_time - _lastTime)).count();

        _keyboard->capture();
        _mouse->capture();

        _physics->frame(elapsedTime);
        _graphics->frame(elapsedTime);

        if(_keyboard->isKeyDown(OIS::KC_ESCAPE)) {
            changeState(GS_QUIT);
        }
    }
}

void Game::changeState(int state) {
    _state = state;
    _graphics->changeState(state);
    if(_state == GS_QUIT) {
        running = false;
    }
}

int Game::getState() {
    return _state;
}

