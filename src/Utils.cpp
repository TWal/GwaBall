#include "Utils.h"
#include <OgreSceneNode.h>
#include <OgreSceneManager.h>

btVector3 Utils::getBtVector(pugi::xml_node element) {
    btVector3 result(0, 0, 0);

	if(element.attribute("x")) {
        result.setX(element.attribute("x").as_float());
    }
	if(element.attribute("y")) {
        result.setY(element.attribute("y").as_float());
    }
	if(element.attribute("z")) {
        result.setZ(element.attribute("z").as_float());
    }

	return result;
}

Ogre::Vector3 Utils::getOgreVector(pugi::xml_node element) {
    Ogre::Vector3 result(0, 0, 0);

	if(element.attribute("x")) {
        result.x = element.attribute("x").as_float();
    }
	if(element.attribute("y")) {
        result.y = element.attribute("y").as_float();
    }
	if(element.attribute("z")) {
        result.z = element.attribute("z").as_float();
    }

	return result;
}

void Utils::deleteOgreNode(Ogre::SceneNode* node) {
    Ogre::SceneNode::ObjectIterator it = node->getAttachedObjectIterator();
    while(it.hasMoreElements()) {
        node->getCreator()->destroyMovableObject(it.getNext());
    }
    node->getCreator()->destroySceneNode(node);
}

