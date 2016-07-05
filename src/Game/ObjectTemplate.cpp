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
#include "JsonUtils.h"

ObjectTemplate::ObjectTemplate(ObjectManager* parent, size_t id, const std::string& name) :
    _parent(parent),
    _id(id),
    _name(name),
    _entityPath(""),
    _mass(0.f),
    _shape(nullptr) {
}

ObjectTemplate::~ObjectTemplate() {
    _parent->parent()->getPhysicsEngine()->deleteCollisionShape(_shape);
}

void ObjectTemplate::load(const rapidjson::Value& val, Logger* log) {
    if(const rapidjson::Value::Member* mmesh = val.FindMember("mesh")) {
        if(mmesh->value.IsString()) {
            _entityPath = mmesh->value.GetString();
        } else {
            log->error("$.templates.%s.mesh must be a string, aborting", _name.c_str());
            return;
        }
    } else {
        log->error("$.templates.%s.mesh does not exists, aborting", _name.c_str());
        return;
    }

    if(const rapidjson::Value::Member* mmass = val.FindMember("mass")) {
        if(mmass->value.IsNumber()) {
            _mass = mmass->value.GetDouble();
        } else {
            log->error("$.templates.%s.mass must be a number, aborting", _name.c_str());
            return;
        }
    } else {
        log->error("$.templates.%s.mass does not exists, aborting", _name.c_str());
        return;
    }

    if(const rapidjson::Value::Member* mshape = val.FindMember("shape")) {
        if(mshape->value.IsObject()) {
            if(!(_shape = _getCollisionShapeFromValue(mshape->value, log))) {
                log->error("$.templates.%s.shape is malformed, aborting", _name.c_str());
                return;
            }
        } else {
            log->error("$.templates.%s.shape must be an object, aborting", _name.c_str());
            return;
        }
    } else {
        log->error("$.templates.%s.shape does not exists, aborting", _name.c_str());
        return;
    }

    if(const rapidjson::Value::Member* mscript = val.FindMember("script")) {
        const rapidjson::Value& script = mscript->value;
        if(script.IsObject()) {
            if(const rapidjson::Value::Member* mclassName = script.FindMember("class")) {
                if(mclassName->value.IsString()) {
                    if(const rapidjson::Value::Member* mpath = script.FindMember("path")) {
                        if(mpath->value.IsString()) {
                            _scriptClass = mclassName->value.GetString();
                            _scriptPath = mpath->value.GetString();
                            _parent->parent()->getScriptEngine()->loadFile("data/Scripts/" + _scriptPath, _scriptClass);
                        } else {
                            log->error("$.templates.%s.script.path must be a string, aborting", _name.c_str());
                            return;
                        }
                    } else {
                        log->error("$.templates.%s.script.path does not exists, aborting", _name.c_str());
                        return;
                    }
                } else {
                    log->error("$.templates.%s.script.class must be a string, aborting", _name.c_str());
                    return;
                }
            } else {
                log->error("$.templates.%s.script.class does not exists, aborting", _name.c_str());
                return;
            }
        } else {
            log->error("$.templates.%s.script must be an object, aborting", _name.c_str());
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

btCollisionShape* ObjectTemplate::_getCollisionShapeFromValue(const rapidjson::Value& val, Logger* log) {
    if(const rapidjson::Value::Member* mtype = val.FindMember("type")) {
        if(mtype->value.IsString()) {
            std::string type = mtype->value.GetString();
            if(type == "box") {
                if(const rapidjson::Value::Member* mhalfExtends = val.FindMember("size")) {
                    btVector3 extends = JsonUtils::getBtVector(mhalfExtends->value);
                    if(!isnan(extends.x())) {
                        return new btBoxShape(extends/2);
                    } else {
                        log->error("$.templates.%s.shape.size is malformed, aborting", _name.c_str());
                        return nullptr;
                    }
                } else {
                    log->error("$.templates.%s.shape.size does not exists, aborting", _name.c_str());
                    return nullptr;
                }
            } else if(type == "convex" || type == "mesh") {
                std::string path;
                if(const rapidjson::Value::Member* mpath = val.FindMember("path")) {
                    if(mpath->value.IsString()) {
                        path = mpath->value.GetString();
                    } else {
                        path = _entityPath;
                    }
                } else {
                    path = _entityPath;
                }
                Ogre::Entity* entity = _parent->parent()->getGraphicsEngine()->getSceneManager()->createEntity("__shape__", path);
                btCollisionShape* shape;
                if(type[0] == 'c') {
                    shape = PhysicsHelper::createConvexHullShape(entity);
                } else {
                    if(_mass <= FLT_EPSILON) {
                        shape = PhysicsHelper::createBvhTriangleMeshShape(entity);
                    } else {
                        shape = PhysicsHelper::createGImpactMeshShape(entity);
                    }
                }
                _parent->parent()->getGraphicsEngine()->getSceneManager()->destroyEntity(entity);
                return shape;
            } else {
                log->error("$.templates.%s.shape.type (\"%s\") is not recognised, aborting", _name.c_str(), type.c_str());
                return nullptr;
            }
        } else {
            log->error("$.templates.%s.shape.type must be a string, aborting", _name.c_str());
            return nullptr;
        }
    } else {
        log->error("$.templates.%s.shape.type does not exists, aborting", _name.c_str());
        return nullptr;
    }
}
