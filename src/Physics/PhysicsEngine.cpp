#include "PhysicsEngine.h"

#include <btBulletDynamicsCommon.h>
#include <BulletCollision/Gimpact/btGImpactCollisionAlgorithm.h>
#include <cfloat>

#include "PhysicsHelper.h"

PhysicsEngine::PhysicsEngine(Game* parent) : Engine(parent) {
    _construct();
}

PhysicsEngine::~PhysicsEngine () {
    _destroy();
}

void PhysicsEngine::_construct() {
    _collisionConfiguration = new btDefaultCollisionConfiguration();
    _dispatcher = new btCollisionDispatcher(_collisionConfiguration);
    _broadphase = new btDbvtBroadphase();
    _constraintSolver = new btSequentialImpulseConstraintSolver();
    _world = new btDiscreteDynamicsWorld(_dispatcher, _broadphase, _constraintSolver, _collisionConfiguration);
    btGImpactCollisionAlgorithm::registerAlgorithm((btCollisionDispatcher*)_dispatcher);
    _world->setGravity(btVector3(0, -9.81, 0));
}

void PhysicsEngine::_destroy() {
    delete _world;
    delete _collisionConfiguration;
    delete _dispatcher;
    delete _broadphase;
    delete _constraintSolver;
}

void PhysicsEngine::frame(double time) {
    if(!_pause) {
        _world->stepSimulation(time);
    }
}

void PhysicsEngine::reset() {
    _destroy();
    _construct();
}

void PhysicsEngine::changeState(int state) {
}


btRigidBody* PhysicsEngine::addRigidBody(btRigidBody* body) {
    _bodies.insert(body);
    _shapes.insert(body->getCollisionShape());
	_world->addRigidBody(body);
    return body;
}

btRigidBody* PhysicsEngine::addRigidBody(btRigidBody* body, short group, short mask) {
    _bodies.insert(body);
    _shapes.insert(body->getCollisionShape());
	_world->addRigidBody(body, group, mask);
    return body;
}

btRigidBody* PhysicsEngine::addRigidBody(float mass, btCollisionShape* shape, Ogre::SceneNode* node) {
	return addRigidBody(PhysicsHelper::createRigidBody(mass, shape, node));
}

btRigidBody* PhysicsEngine::addRigidBody(float mass, btCollisionShape* shape, Ogre::SceneNode* node, short group, short mask) {
	return addRigidBody(PhysicsHelper::createRigidBody(mass, shape, node), group, mask);
}

btRigidBody* PhysicsEngine::addRigidBody(float mass, Ogre::Entity* entity, Ogre::SceneNode* node, RIGIDBODYTYPE type) {
    btCollisionShape* shape = nullptr;
    if(type == RBT_TRIMESH) {
        if(mass < FLT_EPSILON) {
            shape = PhysicsHelper::createBvhTriangleMeshShape(entity);
        } else {
            shape = PhysicsHelper::createGImpactMeshShape(entity);
        }
    } else {
        shape = PhysicsHelper::createConvexHullShape(entity);
    }
    return addRigidBody(mass, shape, node);
}

btRigidBody* PhysicsEngine::addRigidBody(float mass, Ogre::Entity* entity, Ogre::SceneNode* node, RIGIDBODYTYPE type, short group, short mask) {
    btCollisionShape* shape = nullptr;
    if(type == RBT_TRIMESH) {
        if(mass < FLT_EPSILON) {
            shape = PhysicsHelper::createBvhTriangleMeshShape(entity);
        } else {
            shape = PhysicsHelper::createGImpactMeshShape(entity);
        }
    } else {
        shape = PhysicsHelper::createConvexHullShape(entity);
    }
    return addRigidBody(mass, shape, node, group, mask);
}

