#ifndef UTILS_H
#define UTILS_H

#include <OgreQuaternion.h>
#include <OgreVector3.h>
#include <LinearMath/btVector3.h>
#include <LinearMath/btQuaternion.h>

namespace Ogre {
    class SceneNode;
}

namespace Utils {
    constexpr int bit(char x) {
        return 1 << x;
    }

    inline btVector3 convert(const Ogre::Vector3& vector) {
        return btVector3(vector.x, vector.y, vector.z);
    }

    inline Ogre::Vector3 convert(const btVector3& vector) {
        return Ogre::Vector3(vector.x(), vector.y(), vector.z());
    }

    inline btQuaternion convert(const Ogre::Quaternion& quaternion) {
        return btQuaternion(quaternion.x, quaternion.y, quaternion.z, quaternion.w);
    }

    inline Ogre::Quaternion convert(const btQuaternion& quaternion) {
        return Ogre::Quaternion(quaternion.w(), quaternion.x(), quaternion.y(), quaternion.z());
    }

    void deleteOgreNode(Ogre::SceneNode* node);
}

#endif

