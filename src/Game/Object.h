#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include <pugixml.hpp>
class ObjectManager;
class ObjectTemplate;
class btRigidBody;
class Logger;
namespace Ogre {
    class SceneNode;
}

class Object {
    public:
        Object(ObjectManager* parent, size_t id, ObjectTemplate* objTemplate);
        ~Object();
        void load(pugi::xml_node root, Logger* log);

    private:
        ObjectManager* _parent;
        size_t _id;
        ObjectTemplate* _template;

        Ogre::SceneNode* _node;
        btRigidBody* _body;
        short _group;
        short _mask;
};

#endif

