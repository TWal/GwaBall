#include "CameraManager.h"
#include "GraphicsEngine.h"
#include "../Game/GameEngine.h"
#include "../Physics/PhysicsEngine.h"
#include "../Game/Player.h"
#include <OgreSceneNode.h>
#include <OgreCamera.h>
#include <OgreSceneManager.h>
#include <OgreRay.h>
#include <iostream>

const static int PIXEL_NUMBER_HALF_TURN = 300;
const static int WHEEL_MAGIC = -120;
const static float WHEEL_DISTANCE = 0.5;

CameraManager::CameraManager(GraphicsEngine* parent) : _parent(parent) {
    _camera = _parent->getSceneManager()->createCamera("Camera");
    _camera->setFOVy(Ogre::Degree(90));
    _camera->setNearClipDistance(0.01);
    _camera->setFarClipDistance(1000);
    _toPlayerNode = _parent->getSceneManager()->getRootSceneNode()->createChildSceneNode();
    _yawNode = _toPlayerNode->createChildSceneNode();
    _pitchNode = _yawNode->createChildSceneNode();
    _distanceNode = _pitchNode->createChildSceneNode();
    _distanceNode->attachObject(_camera);
    _distance = 5;
    _distanceNode->setPosition(0, 0, _distance);
    _camera->lookAt(0, 0, 0);
}

void CameraManager::update() {
    _distanceNode->setPosition(Ogre::Vector3(0, 0, _distance));
    Player* player = _parent->getGameEngine()->getPlayer();
    if(player && player->getSceneNode()) {
        _toPlayerNode->setPosition(player->getSceneNode()->getPosition());
        //TODO: avoid magic constants, 0.25 is the half of the player's radius
        PhysicsEngine::RayResult result = _parent->getPhysicsEngine()->rayTest(Converter::convert(player->getSceneNode()->getPosition()), Converter::convert(_distanceNode->_getDerivedPosition()), PhysicsEngine::COL_NOPLAYER, 0.25);
        if(result.collisionObject) {
            _distanceNode->_setDerivedPosition(Converter::convert(result.hitPoint));
            //Avoid graphical bugs
            _distanceNode->setPosition(_distanceNode->getPosition() + _distanceNode->getPosition().normalisedCopy() * -0.1);
        }
    }
}

void CameraManager::mouseMoved(const OIS::MouseEvent& arg) {
    _yawNode->yaw(Ogre::Radian(arg.state.X.rel * M_PI / PIXEL_NUMBER_HALF_TURN));
    _pitchNode->pitch(Ogre::Radian(arg.state.Y.rel * M_PI / PIXEL_NUMBER_HALF_TURN));
    //Limit pitch
    float pitchAngle = 2 * Ogre::Degree(Ogre::Math::ACos(_pitchNode->getOrientation().w)).valueDegrees();
    float pitchAngleSign = _pitchNode->getOrientation().x;
    if (pitchAngle > 90.0f) {
        if (pitchAngleSign > 0) {
            _pitchNode->setOrientation(Ogre::Quaternion(Ogre::Math::Sqrt(0.5f), Ogre::Math::Sqrt(0.5f), 0, 0));
        } else if (pitchAngleSign < 0) {
            _pitchNode->setOrientation(Ogre::Quaternion(Ogre::Math::Sqrt(0.5f), -Ogre::Math::Sqrt(0.5f), 0, 0));
        }
    }

    if((_distance - 0.01) < FLT_EPSILON) {
        _distance = 0;
    }
    _distance += arg.state.Z.rel * WHEEL_DISTANCE / WHEEL_MAGIC;
    if(_distance < FLT_EPSILON) {
        _distance = 0.01;
    }
}

Ogre::Camera* CameraManager::getCamera() {
    return _camera;
}

Ogre::Vector3 CameraManager::getLookingDirection() {
    Player* player = _parent->getGameEngine()->getPlayer();
    if(player && player->getSceneNode()) {
        return player->getSceneNode()->getPosition() - _distanceNode->_getDerivedPosition();
    } else {
        return Ogre::Vector3::ZERO;
    }
}

