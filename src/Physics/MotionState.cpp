#include "MotionState.h"
#include <OgreSceneNode.h>
#include "../Utils.h"

MotionState::MotionState(Ogre::SceneNode* node) :
    _node(node) {
}

MotionState::~MotionState() {
}

void MotionState::getWorldTransform(btTransform& worldTrans) const {
    worldTrans.setOrigin(Utils::convert(_node->getPosition()));
    worldTrans.setRotation(Utils::convert(_node->getOrientation()));
}

void MotionState::setWorldTransform(const btTransform& worldTrans) {
    if(_node) {
        _node->setOrientation(Utils::convert(worldTrans.getRotation()));
        _node->setPosition(Utils::convert(worldTrans.getOrigin()));
    }
}

