#include "Utils.h"
#include <OgreSceneNode.h>
#include <OgreSceneManager.h>

btVector3 Utils::getBtVector(const rapidjson::Value& val) {
    if(val.IsArray() && val.Size() == 3 && val[0u].IsNumber() && val[1u].IsNumber() && val[2u].IsNumber()) {
        return btVector3(val[0u].GetDouble(), val[1u].GetDouble(), val[2u].GetDouble());
    } else {
        return btVector3(NAN, NAN, NAN);
    }
}

Ogre::Vector3 Utils::getOgreVector(const rapidjson::Value& val) {
    if(val.IsArray() && val.Size() == 3 && val[0u].IsNumber() && val[1u].IsNumber() && val[2u].IsNumber()) {
        return Ogre::Vector3(val[0u].GetDouble(), val[1u].GetDouble(), val[2u].GetDouble());
    } else {
        return Ogre::Vector3(NAN, NAN, NAN);
    }
}

void Utils::deleteOgreNode(Ogre::SceneNode* node) {
    Ogre::SceneNode::ObjectIterator it = node->getAttachedObjectIterator();
    while(it.hasMoreElements()) {
        node->getCreator()->destroyMovableObject(it.getNext());
    }
    node->getCreator()->destroySceneNode(node);
}

