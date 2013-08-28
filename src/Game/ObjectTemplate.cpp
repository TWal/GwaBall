#include "ObjectTemplate.h"
#include "../Logger.h"
#include <btBulletCollisionCommon.h>
#include <BulletCollision/Gimpact/btGImpactShape.h>
#include <OgreEntity.h>
#include <OgreSceneManager.h>
#include "ObjectManager.h"
#include "GameEngine.h"
#include "../Physics/PhysicsHelper.h"
#include "../Physics/PhysicsEngine.h"
#include "../Graphics/GraphicsEngine.h"
#include "../Script/ScriptEngine.h"
#include "../Utils.h"

ObjectTemplate::ObjectTemplate(ObjectManager* parent, size_t id) :
    _parent(parent),
    _id(id),
    _name(""),
    _entityPath(""),
    _mass(0.f),
    _shape(nullptr) {
}

ObjectTemplate::~ObjectTemplate() {
    _parent->parent()->getPhysicsEngine()->deleteCollisionShape(_shape);
}

void ObjectTemplate::load(const std::string& file, Logger* log) {
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(std::string(file).c_str());
    if(result) {
        log->info("Loading %s", file.c_str());
    } else {
        log->error("Error when loading %s: %s", file.c_str(), result.description());
        return;
    }

    if(pugi::xml_node root = doc.child("Template")) {
        load(root, log);
    } else {
        log->error("/Template does not exists, aborting");
    }
}

void ObjectTemplate::load(pugi::xml_node root, Logger* log) {
    if(pugi::xml_attribute name = root.attribute("name")) {
        _name = name.value();
    } else {
        log->error("/Template/@name does not exists, aborting");
    }

    if(pugi::xml_node graphicsNode = root.child("Graphics")) {
        if(pugi::xml_node prop = graphicsNode.child("Properties")) {
            if(pugi::xml_attribute attr = prop.attribute("mesh")) {
                _entityPath = attr.value();
            } else {
                log->error("/Template/Graphics/Properties/@mesh does not exists, aborting");
                return;
            }
        } else {
            log->error("/Template/Graphics/Properties does not exists, aborting");
            return;
        }
    } else {
        log->error("/Template/Graphics does not exists, aborting");
        return;
    }

    if(pugi::xml_node physicsNode = root.child("Physics")) {
        if(pugi::xml_node shape = physicsNode.child("Shape")) {
            if(!(_shape = _getCollisionShapeFromElement(shape))) {
                log->error("/Template/Physics/Shape is malformed, aborting");
                return;
            }
        } else {
            log->error("/Template/Physics/Shape does not exists, aborting");
        }
        if(pugi::xml_node prop = physicsNode.child("Properties")) {
            if(pugi::xml_attribute attr = prop.attribute("mass")) {
                if(isnan(_mass = attr.as_float(NAN))) {
                    log->error("/Template/Physics/Properties/@mass is malformed, aborting");
                    return;
                }
            } else {
                log->error("/Template/Physics/Properties/@mass does not exists, aborting");
                return;
            }
        } else {
            log->error("/Template/Physics/Properties does not exists, aborting");
            return;
        }
    } else {
        log->error("/Template/Physics does not exists, aborting");
        return;
    }

    if(pugi::xml_node scriptNode = root.child("Script")) {
        if(pugi::xml_attribute classAttr = scriptNode.attribute("class")) {
            if(pugi::xml_attribute pathAttr = scriptNode.attribute("path")) {
                _scriptClass = classAttr.value();
                _scriptPath = pathAttr.value();
                _parent->parent()->getScriptEngine()->loadFile("data/Scripts/" + _scriptPath, _scriptClass);
            } else {
                log->error("/Template/Script/@path does not exists, aborting");
                return;
            }
        } else {
            log->error("/Template/Script/@class does not exists, aborting");
            return;
        }
    }
}

size_t ObjectTemplate::getId() {
    return _id;
}

const std::string& ObjectTemplate::getName() {
    return _name;
}

btCollisionShape* ObjectTemplate::getShape() {
    return _shape;
}

float ObjectTemplate::getMass() {
    return _mass;
}

const std::string& ObjectTemplate::getEntityPath() {
    return _entityPath;
}

bool ObjectTemplate::hasScript() {
    return !(_scriptClass.empty() || _scriptPath.empty());
}

const std::string& ObjectTemplate::getScriptClass() {
    return _scriptClass;
}

btCollisionShape* ObjectTemplate::_getCollisionShapeFromElement(pugi::xml_node element) {
    std::string type = element.attribute("type").value();
    if(type == "box") {
        return new btBoxShape(Utils::getBtVector(element.child("HalfExtends")));
    } else if(type == "convex") {
        std::string path;
        if(element.child("Mesh") && element.child("Mesh").attribute("path")) {
            path = _entityPath;
        } else {
            path = _entityPath;
        }
        Ogre::Entity* entity = _parent->parent()->getGraphicsEngine()->getSceneManager()->createEntity("__shape__", path);
        btCollisionShape* shape = PhysicsHelper::createConvexHullShape(entity);
        _parent->parent()->getGraphicsEngine()->getSceneManager()->destroyEntity(entity);
        return shape;
    } else if(type == "mesh") {
        std::string path;
        if(element.child("Mesh") && element.child("Mesh").attribute("path")) {
            path = element.child("Mesh").attribute("path").value();
        } else {
            path = _entityPath;
        }
        Ogre::Entity* entity = _parent->parent()->getGraphicsEngine()->getSceneManager()->createEntity("__shape__", path);
        btCollisionShape* shape;
        if(_mass <= FLT_EPSILON) {
            shape = PhysicsHelper::createBvhTriangleMeshShape(entity);
        } else {
            shape = PhysicsHelper::createGImpactMeshShape(entity);
        }
        _parent->parent()->getGraphicsEngine()->getSceneManager()->destroyEntity(entity);
        return shape;
    }
    return nullptr;
}
