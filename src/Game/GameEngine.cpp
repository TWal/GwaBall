#include "GameEngine.h"

#include <Ogre.h>
#include <btBulletDynamicsCommon.h>
#include "../Physics/PhysicsEngine.h"
#include "../Graphics/GraphicsEngine.h"

GameEngine::GameEngine(Game* parent) : Engine(parent) {
}

GameEngine::~GameEngine() {
}

void GameEngine::init() {
    btCollisionShape* shape = new btBoxShape(btVector3(1, 1, 1));
    Ogre::SceneNode* fixedBoxNode = _graphics->getSceneManager()->getRootSceneNode()->createChildSceneNode();
    fixedBoxNode->attachObject(_graphics->getSceneManager()->createEntity("FixedBox", "BoxTest.mesh"));
    fixedBoxNode->setPosition(0, 0, 0);
    _physics->addRigidBody(0.0, shape, fixedBoxNode);

    Ogre::SceneNode* fallingBoxNode = _graphics->getSceneManager()->getRootSceneNode()->createChildSceneNode();
    fallingBoxNode->attachObject(_graphics->getSceneManager()->createEntity("FallingBox", "BoxTest.mesh"));
    fallingBoxNode->setPosition(0, 5, -1);
    _physics->addRigidBody(2.0, shape, fallingBoxNode);
}

void GameEngine::frame(double time) {
}

void GameEngine::reset() {
}

void GameEngine::changeState(int state) {
}

