#ifndef UTILS_H
#define UTILS_H

#include <OgreQuaternion.h>
#include <OgreVector3.h>
#include <LinearMath/btVector3.h>
#include <LinearMath/btQuaternion.h>
#include <pugixml.hpp>

namespace Ogre {
    class SceneNode;
}

class Utils {
    public:
        static constexpr int bit(char x) {
            return 1 << x;
        }

        static inline btVector3 convert(const Ogre::Vector3& vector) {
            return btVector3(vector.x, vector.y, vector.z);
        }

        static inline Ogre::Vector3 convert(const btVector3& vector) {
            return Ogre::Vector3(vector.x(), vector.y(), vector.z());
        }

        static inline btQuaternion convert(const Ogre::Quaternion& quaternion) {
            return btQuaternion(quaternion.x, quaternion.y, quaternion.z, quaternion.w);
        }

        static inline Ogre::Quaternion convert(const btQuaternion& quaternion) {
            return Ogre::Quaternion(quaternion.w(), quaternion.x(), quaternion.y(), quaternion.z());
        }

        static btVector3 getBtVector(pugi::xml_node element);
        static Ogre::Vector3 getOgreVector(pugi::xml_node element);

        static void deleteOgreNode(Ogre::SceneNode* node);
};

#endif

