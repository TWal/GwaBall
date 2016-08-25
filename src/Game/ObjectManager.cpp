#include "ObjectManager.h"
#include "../Logger.h"
#include "ObjectTemplate.h"
#include "Object.h"
#include "GameEngine.h"
#include "Player.h"
#include "../Script/ScriptEngine.h"
#include "JsonUtils.h"
#include <BulletDynamics/Dynamics/btRigidBody.h>

ObjectManager::ObjectManager(GameEngine* parent) {
    _parent = parent;
    _log = new Logger(Logger::INFO, Logger::STDERR | Logger::LOGFILE, "Logs/ObjectManager.log");
}

ObjectManager::~ObjectManager() {
    delete _log;
    clear();
}

void ObjectManager::load(const std::string& file) {
    if(FILE* f = fopen(("data/Maps/" + file).c_str(), "rb")) {
        char* str = nullptr;
        fseek(f, 0, SEEK_END);
        long size = ftell(f);
        fseek(f, 0, SEEK_SET);
        str = new char[size+1]; //+1 for the ending \0
        fread(str, size, 1, f);
        fclose(f);
        str[size] = 0;

        rapidjson::Document doc;
        doc.ParseInsitu<0>(str);
        if(!doc.HasParseError()) {
            load(doc);
        } else {
            _log->error("Parsing error in file data/Maps/%s:%u: %s", file.c_str(), doc.GetErrorOffset(), doc.GetParseError());
        }
        delete[] str;
    } else {
        _log->error("Could not read file data/Maps/%s", file.c_str());
    }
}

void ObjectManager::load(const rapidjson::Document& doc) {
    if(doc.IsObject()) {
        if(const rapidjson::Value::Member* mtemplates = doc.FindMember("templates")) {
            const rapidjson::Value& templates = mtemplates->value;
            if(templates.IsObject()) {
                for(rapidjson::Value::ConstMemberIterator it = templates.MemberBegin(); it != templates.MemberEnd(); ++it) {
                    _templates.push_back(new ObjectTemplate(this, _getNextTemplateId(), it->name.GetString()));
                    _templates.back()->load(it->value, _log);
                }
            } else {
                _log->error("$.templates must be an object, aborting");
                return;
            }
        }

        if(const rapidjson::Value::Member* mobjects = doc.FindMember("objects")) {
            const rapidjson::Value& objects = mobjects->value;
            if(objects.IsObject()) {
                for(rapidjson::Value::ConstMemberIterator it = objects.MemberBegin(); it != objects.MemberEnd(); ++it) {
                    ObjectTemplate* tmpl = _templates[_getIdFromTemplateName(it->name.GetString())];
                    const rapidjson::Value& instances = it->value;
                    if(instances.IsArray()) {
                        for(rapidjson::Value::ConstValueIterator it2 = instances.Begin(); it2 != instances.End(); ++it2) {
                            _objects.push_back(new Object(this, _getNextObjectId(), tmpl));
                            _objects.back()->load(*it2, _log);
                        }
                    } else {
                        _log->error("$.objects.%s must be an array, aborting", it->name.GetString());
                        return;
                    }
                }
            } else {
                _log->error("$.objects must be an object, aborting");
                return;
            }
        }

        if(const rapidjson::Value::Member* mscripts = doc.FindMember("scripts")) {
            const rapidjson::Value& scripts = mscripts->value;
            if(scripts.IsArray()) {
                for(rapidjson::Value::ConstValueIterator it = scripts.Begin(); it != scripts.End(); ++it) {
                    //TODO: error handling...
                    std::string file = it->FindMember("file")->value.GetString();
                    std::string className = it->FindMember("class")->value.GetString();
                    std::string args = "";
                    if(const rapidjson::Value::Member* margs = it->FindMember("args")) {
                        args = margs->value.GetString();
                    }
                    _parent->getScriptEngine()->loadFile(std::string("data/Scripts/") + file, className);
                    _parent->getScriptEngine()->instanciateScript(className, args);
                }
            } else {
                _log->error("$.scripts must be an array, aborting");
                return;
            }
        }

        if(const rapidjson::Value::Member* mplayer = doc.FindMember("player")) {
            if(mplayer->value.IsObject()) {
                if(const rapidjson::Value::Member* mpos = mplayer->value.FindMember("pos")) {
                    btVector3 pos = JsonUtils::getBtVector(mpos->value);
                    if(!isnan(pos.x())) {
                        _parent->getPlayer()->getBody()->getWorldTransform().setOrigin(pos);
                    } else {
                        _log->error("$.player.pos is malformed, aborting");
                        return;
                    }
                } else {
                    _log->error("$.player.pos does not exists, aborting");
                    return;
                }
            } else {
                _log->error("$.player must be an object, aborting");
                return;
            }
        }
    } else {
        _log->error("$ must be an object");
    }
}

void ObjectManager::clear() {
    for(Object* obj : _objects) {
        delete obj;
    }
    _objects.clear();

    for(ObjectTemplate* tmpl : _templates) {
        delete tmpl;
    }
    _templates.clear();
}

GameEngine* ObjectManager::parent() {
    return _parent;
}

size_t ObjectManager::_getNextTemplateId() {
    return _templates.size();
}

size_t ObjectManager::_getNextObjectId() {
    return _objects.size();
}

size_t ObjectManager::_getIdFromTemplateName(const std::string& name) {
    for(size_t i = 0; i < _templates.size(); ++i) {
        if(_templates[i]->getName() == name) {
            return i;
        }
    }
    return -1;
}

