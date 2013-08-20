#include "Object.h"
#include <OgreEntity.h>
#include <OgreSceneNode.h>
#include <OgreSceneManager.h>
#include "../Utils.h"
#include "../Graphics/GraphicsEngine.h"
#include "../Physics/PhysicsEngine.h"
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
}

void Object::load(pugi::xml_node root, Logger* log) {
    _node = _parent->parent()->getGraphicsEngine()->getSceneManager()->getRootSceneNode()->createChildSceneNode();
    _node->attachObject(_parent->parent()->getGraphicsEngine()->getSceneManager()->createEntity(_template->getName() + std::to_string(_id), _template->getEntityPath()));
    if(pugi::xml_node pos = root.child("Position")) {
        _node->setPosition(XmlUtils::getOgreVector(pos));
    } else {
        log->error("/Transform/Position does not exists, aborting");
        return;
    }

    if(pugi::xml_node rotNode = root.child("Rotation")) {
        Ogre::Vector3 rot = XmlUtils::getOgreVector(rotNode);
        btQuaternion rotation = btQuaternion::getIdentity();
        rotation.setEuler(Ogre::Math::DegreesToRadians(rot.y), Ogre::Math::DegreesToRadians(rot.x), Ogre::Math::DegreesToRadians(rot.z));
        _node->setOrientation(Converter::convert(rotation));
    }

    _group = _template->getMass() <= FLT_EPSILON ? PhysicsEngine::COL_STATIC : PhysicsEngine::COL_DYNAMIC;
    _mask = _template->getMass() <= FLT_EPSILON ? PhysicsEngine::COL_NOSTATIC : PhysicsEngine::COL_ALL;

    _body = _parent->parent()->getPhysicsEngine()->addRigidBody(_template->getMass(), _template->getShape(), _node, _group, _mask);
}

