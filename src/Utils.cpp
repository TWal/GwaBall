#include "Utils.h"

btVector3 XmlUtils::getBtVector(pugi::xml_node element) {
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

Ogre::Vector3 XmlUtils::getOgreVector(pugi::xml_node element) {
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

