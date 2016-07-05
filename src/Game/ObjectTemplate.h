#ifndef GAME_OBJECTTEMPLATE_H
#define GAME_OBJECTTEMPLATE_H

#include <string>
#include "thirdparty/rapidjson/document.h"

class ObjectManager;
class btCollisionShape;
class Logger;

class ObjectTemplate {
    public:
        ObjectTemplate(ObjectManager* parent, size_t id, const std::string& name);
        ~ObjectTemplate();

        void load(const rapidjson::Value& val, Logger* log);

        size_t getId();
        const std::string& getName();
        const std::string& getEntityPath();
        float getMass();
        btCollisionShape* getShape();
        bool hasScript();
        const std::string& getScriptClass();
    private:
        btCollisionShape* _getCollisionShapeFromValue(const rapidjson::Value& val, Logger* log);
        ObjectManager* _parent;
        size_t _id;
        std::string _name;

        std::string _entityPath;

        float _mass;
        btCollisionShape* _shape;

        std::string _scriptClass;
        std::string _scriptPath;
};

#endif

