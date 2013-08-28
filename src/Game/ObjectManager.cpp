#include "ObjectManager.h"
#include "../Logger.h"
#include "ObjectTemplate.h"
#include "Object.h"
#include "GameEngine.h"
#include "Player.h"
#include "../Script/ScriptEngine.h"
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
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(std::string("data/Maps/" + file).c_str());
	if(result) {
        _log->info("Loading data/Maps/%s", file.c_str());
    } else {
        _log->error("Error when loading data/Maps/%s: %s", file.c_str(), result.description());
        return;
	}

	pugi::xml_node root = doc.child("Map");
	if(root) {
        load(root);
	} else {
        _log->error("/Map does not exists, aborting");
	}
}

void ObjectManager::load(pugi::xml_node root) {
    if(pugi::xml_node templatesNode = root.child("Templates")) {
        for(pugi::xml_node node : templatesNode.children("Template")) {
            _templates.push_back(new ObjectTemplate(this, _getNextTemplateId()));
            _templates.back()->load(node, _log);
        }
    } else {
        _log->error("/Map/Templates does not exists, aborting");
        return;
    }

    if(pugi::xml_node objectsNode = root.child("Objects")) {
        for(pugi::xml_node node : objectsNode.children("Template")) {
            if(pugi::xml_attribute attr = node.attribute("name")) {
                ObjectTemplate* tmpl = _templates[_getIdFromTemplateName(attr.value())];
                for(pugi::xml_node node2 : node.children("Transform")) {
                    _objects.push_back(new Object(this, _getNextObjectId(), tmpl));
                    _objects.back()->load(node2, _log);
                }
            } else {
                _log->error("/Map/Objects/Template/@name does not exists, aborting");
                return;
            }
        }

        if(pugi::xml_node playerNode = objectsNode.child("Player")) {
            if(pugi::xml_node position = playerNode.child("Position")) {
                _parent->getPlayer()->getBody()->getWorldTransform().setOrigin(Utils::getBtVector(position));
            } else {
                _log->error("/Map/Objects/Player/Position does not exists, aborting");
                return;
            }
        } else {
            _log->error("/Map/Objects/Player does not exists, aborting");
            return;
        }
    } else {
        _log->error("/Map/Objects does not exists, aborting");
        return;
    }

    if(pugi::xml_node scriptsNode = root.child("Scripts")) {
        for(pugi::xml_node node : scriptsNode.children("Script")) {
            if(pugi::xml_attribute classAttr = node.attribute("class")) {
                if(pugi::xml_attribute pathAttr = node.attribute("path")) {
                    _parent->getScriptEngine()->loadFile(std::string("data/Scripts/") + pathAttr.value(), classAttr.value());
                    _parent->getScriptEngine()->instanciateScript(classAttr.value());
                } else {
                    _log->error("/Map/Scripts/Script/@path does not exists, aborting");
                    return;
                }
            } else {
                _log->error("/Map/Scripts/Script/@class does not exists, aborting");
                return;
            }
        }
    } //No else: it is not required
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

