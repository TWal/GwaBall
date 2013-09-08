#ifndef GAME_OBJECTMANAGER_H
#define GAME_OBJECTMANAGER_H

#include <string>
#include "thirdparty/rapidjson/document.h"
#include <vector>

class GameEngine;
class Logger;
class ObjectTemplate;
class Object;

class ObjectManager {
    public:
        ObjectManager(GameEngine* parent);
        ~ObjectManager();
        void load(const std::string& file);
        void load(const rapidjson::Document& doc);

        void clear();

        GameEngine* parent();
    private:
        size_t _getNextTemplateId();
        size_t _getNextObjectId();
        size_t _getIdFromTemplateName(const std::string& name);
        GameEngine* _parent;
        Logger* _log;
        std::vector<ObjectTemplate*> _templates;
        std::vector<Object*> _objects;
};

#endif

