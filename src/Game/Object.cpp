#include "Object.h"
#include <OgreEntity.h>
#include <OgreSceneNode.h>
#include <OgreSceneManager.h>
#include "JsonUtils.h"
#include "../Graphics/GraphicsEngine.h"
#include "../Physics/PhysicsEngine.h"
#include "../Script/ScriptEngine.h"
#include "GameEngine.h"
#include "ObjectManager.h"
#include "../Logger.h"
#include "ObjectTemplate.h"

Object::Object(ObjectManager* parent, size_t id, ObjectTemplate* objTemplate) :
    _parent(parent),
    _id(id),
    _template(objTemplate),
    _node(nullptr),
    _body(nullptr) {
}

Object::~Object() {
    Utils::deleteOgreNode(_node);
    _parent->parent()->getPhysicsEngine()->deleteRigidBody(_body);
}

void Object::load(const rapidjson::Value& val, Logger* log) {
    _node = _parent->parent()->getGraphicsEngine()->getSceneManager()->getRootSceneNode()->createChildSceneNode();
    _node->attachObject(_parent->parent()->getGraphicsEngine()->getSceneManager()->createEntity(_template->getName() + std::to_string(_id), _template->getEntityPath()));

    if(const rapidjson::Value::Member* mpos = val.FindMember("pos")) {
        Ogre::Vector3 pos = JsonUtils::getOgreVector(mpos->value);
        if(!isnan(pos.x)) {
            _node->setPosition(pos);
        } else {
            log->error("$.objects.%s[%u].pos is malformed, aborting", _template->getName().c_str(), _id);
            return;
        }
    } else {
        log->error("$.objects.%s[%u].pos does not exists, aborting", _template->getName().c_str(), _id);
        return;
    }

    if(const rapidjson::Value::Member* mrot = val.FindMember("rot")) {
        Ogre::Vector3 rot = JsonUtils::getOgreVector(mrot->value);
        if(!isnan(rot.x)) {
            btQuaternion rotation = btQuaternion::getIdentity();
            rotation.setEuler(Ogre::Math::DegreesToRadians(rot.y), Ogre::Math::DegreesToRadians(rot.x), Ogre::Math::DegreesToRadians(rot.z));
            _node->setOrientation(Utils::convert(rotation));
        } else {
            log->error("$.objects.%s[%u].rot is malformed, aborting", _template->getName().c_str(), _id);
            return;
        }
    } else {
        log->error("$.objects.%s[%u].rot does not exists, aborting", _template->getName().c_str(), _id);
        return;
    }

    std::string scriptArgs = "";
    if(const rapidjson::Value::Member* mscript = val.FindMember("scriptArgs")) {
        if(mscript->value.IsString()) {
            scriptArgs = mscript->value.GetString();
            if(!_template->hasScript()) {
                log->error("$.objects.%s[%u].scriptArgs exists but $.templates.%s.script does not exists", _template->getName().c_str(), _id, _template->getName().c_str());
            }
        } else {
            log->error("$.objects.%s[%u].scriptArgs is not a string", _template->getName().c_str(), _id);
        }
    }

    _group = _template->getMass() <= FLT_EPSILON ? PhysicsEngine::COL_STATIC : PhysicsEngine::COL_DYNAMIC;
    _mask = _template->getMass() <= FLT_EPSILON ? PhysicsEngine::COL_NOSTATIC : PhysicsEngine::COL_ALL;

    _body = _parent->parent()->getPhysicsEngine()->addRigidBody(_template->getMass(), _template->getShape(), _node, _group, _mask);

    if(_template->hasScript()) {
        _parent->parent()->getScriptEngine()->instanciateScript(_template->getScriptClass(), scriptArgs, _body);
    }
}

