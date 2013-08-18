#include "GameEngine.h"

#include <Ogre.h>
#include <btBulletDynamicsCommon.h>
#include <OISKeyboard.h>
#include "../Input/InputEngine.h"
#include "../Game.h"
#include "../Physics/PhysicsEngine.h"
#include "../Graphics/GraphicsEngine.h"
#include "Player.h"

GameEngine::GameEngine(Game* parent) : Engine(parent) {
    _player = new Player(this);
}

GameEngine::~GameEngine() {
}

void GameEngine::init() {
    btCollisionShape* fixedBoxShape = new btBoxShape(btVector3(25, 0.25, 25));
    Ogre::SceneNode* fixedBoxNode = _graphics->getSceneManager()->getRootSceneNode()->createChildSceneNode();
    fixedBoxNode->attachObject(_graphics->getSceneManager()->createEntity("FixedBox", "GroundTest.mesh"));
    fixedBoxNode->setPosition(0, 0, 0);
    _physics->addRigidBody(0.0, fixedBoxShape, fixedBoxNode);

    btCollisionShape* fallingBoxShape = new btBoxShape(btVector3(1, 1, 1));
    Ogre::SceneNode* fallingBoxNode = _graphics->getSceneManager()->getRootSceneNode()->createChildSceneNode();
    fallingBoxNode->attachObject(_graphics->getSceneManager()->createEntity("FallingBox", "BoxTest.mesh"));
    fallingBoxNode->setPosition(0, 5, -1);
    _physics->addRigidBody(2.0, fallingBoxShape, fallingBoxNode);
    _player->init();
}

void GameEngine::frame(double time) {
    if(!_pause) {
        _player->frame(time);
        if(_input->getKeyboard()->isKeyDown(OIS::KC_UP)) {
            _player->move(btVector3(-1, 0, 0), time);
        }
        if(_input->getKeyboard()->isKeyDown(OIS::KC_DOWN)) {
            _player->move(btVector3(1, 0, 0), time);
        }
        if(_input->getKeyboard()->isKeyDown(OIS::KC_LEFT)) {
            _player->move(btVector3(0, 0, 1), time);
        }
        if(_input->getKeyboard()->isKeyDown(OIS::KC_RIGHT)) {
            _player->move(btVector3(0, 0, -1), time);
        }
    }
}

void GameEngine::reset() {
    _player->reset();
}

void GameEngine::changeState(int state) {
    switch(state) {
        case Game::GS_MENU:
            _pause = true;
            break;
        case Game::GS_RUNNING:
            _pause = false;
            break;
        case Game::GS_QUIT:
            break;
        default:
            break;
    }
}

