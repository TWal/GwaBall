#include "JsonUtils.h"

btVector3 JsonUtils::getBtVector(const rapidjson::Value& val) {
    if(val.IsArray() && val.Size() == 3 && val[0u].IsNumber() && val[1u].IsNumber() && val[2u].IsNumber()) {
        return btVector3(val[0u].GetDouble(), val[1u].GetDouble(), val[2u].GetDouble());
    } else {
        return btVector3(NAN, NAN, NAN);
    }
}

Ogre::Vector3 JsonUtils::getOgreVector(const rapidjson::Value& val) {
    if(val.IsArray() && val.Size() == 3 && val[0u].IsNumber() && val[1u].IsNumber() && val[2u].IsNumber()) {
        return Ogre::Vector3(val[0u].GetDouble(), val[1u].GetDouble(), val[2u].GetDouble());
    } else {
        return Ogre::Vector3(NAN, NAN, NAN);
    }
}

