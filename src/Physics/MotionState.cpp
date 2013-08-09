#include "MotionState.h"
#include <OgreSceneNode.h>
#include "../Utils.h"

MotionState::MotionState(Ogre::SceneNode* node) :
    _node(node) {
}

MotionState::~MotionState() {
}

void MotionState::getWorldTransform(btTransform& worldTrans) const {
    worldTrans.setOrigin(Converter::convert(_node->getPosition()));
    worldTrans.setRotation(Converter::convert(_node->getOrientation()));
}

void MotionState::setWorldTransform(const btTransform& worldTrans) {
    if(_node) {
        _node->setOrientation(Converter::convert(worldTrans.getRotation()));
        _node->setPosition(Converter::convert(worldTrans.getOrigin()));
    }
}

