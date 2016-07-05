#ifndef GAME_JSONUTILS_H
#define GAME_JSONUTILS_H

#include <OgreVector3.h>
#include <LinearMath/btVector3.h>
#include "thirdparty/rapidjson/document.h"

namespace JsonUtils {
    btVector3 getBtVector(const rapidjson::Value& val);
    Ogre::Vector3 getOgreVector(const rapidjson::Value& val);
}

#endif

