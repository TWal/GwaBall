#ifndef GAME_OBJECTTEMPLATE_H
#define GAME_OBJECTTEMPLATE_H

#include <pugixml.hpp>
#include <string>

class ObjectManager;
class btCollisionShape;
class Logger;

class ObjectTemplate {
    public:
        ObjectTemplate(ObjectManager* parent, size_t id);
        ~ObjectTemplate();

        void load(const std::string& file, Logger* log);
        void load(pugi::xml_node root, Logger* log);

        size_t getId();
        const std::string& getName();
        const std::string& getEntityPath();
        float getMass();
        btCollisionShape* getShape();
    private:
        btCollisionShape* _getCollisionShapeFromElement(pugi::xml_node element);
        ObjectManager* _parent;
        size_t _id;
        std::string _name;

        std::string _entityPath;

        float _mass;
        btCollisionShape* _shape;
};

#endif

