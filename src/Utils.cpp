#include "Utils.h"
#include <OgreSceneNode.h>
#include <OgreSceneManager.h>

void Utils::deleteOgreNode(Ogre::SceneNode* node) {
    Ogre::SceneNode::ObjectIterator it = node->getAttachedObjectIterator();
    while(it.hasMoreElements()) {
        node->getCreator()->destroyMovableObject(it.getNext());
    }
    node->getCreator()->destroySceneNode(node);
}

