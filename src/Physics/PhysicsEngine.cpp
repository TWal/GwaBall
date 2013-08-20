#include "PhysicsEngine.h"

#include <btBulletDynamicsCommon.h>
#include <BulletCollision/Gimpact/btGImpactCollisionAlgorithm.h>
#include <cfloat>

#include "../Game.h"
#include "PhysicsHelper.h"
#include "../Logger.h"

PhysicsEngine::PhysicsEngine(Game* parent) : Engine(parent) {
    _log = new Logger(Logger::INFO, Logger::STDERR | Logger::LOGFILE, "Logs/PhysicsEngine.log");
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

void PhysicsEngine::init() {
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
    switch(state) {
        case Game::GS_MENU:
            _pause = true;
            break;
        case Game::GS_RUNNING:
            _pause = false;
            break;
        case Game::GS_QUIT:
            break;
        default:
            break;
    }
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
        if(mass <= FLT_EPSILON) {
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
        if(mass <= FLT_EPSILON) {
            shape = PhysicsHelper::createBvhTriangleMeshShape(entity);
        } else {
            shape = PhysicsHelper::createGImpactMeshShape(entity);
        }
    } else {
        shape = PhysicsHelper::createConvexHullShape(entity);
    }
    return addRigidBody(mass, shape, node, group, mask);
}

PhysicsEngine::RayResult PhysicsEngine::rayTest(const btVector3& from, const btVector3& to, int filter, float radius) {
    if(radius <= FLT_EPSILON) {
        btCollisionWorld::ClosestRayResultCallback callback(from, to);
        callback.m_collisionFilterGroup = COL_RAYTEST;
        callback.m_collisionFilterMask = filter;

        _world->rayTest(from, to, callback);
        RayResult result;
        result.collisionObject = callback.m_collisionObject;
        result.group = callback.m_collisionFilterGroup;
        result.mask = callback.m_collisionFilterMask;
        result.hitPoint = callback.m_hitPointWorld;
        result.hitNormal = callback.m_hitNormalWorld;
        result.rayFrom = callback.m_rayFromWorld;
        result.rayTo = callback.m_rayToWorld;

        return result;
    } else {
        btSphereShape sphere(radius);

        btTransform transFrom;
        transFrom.setIdentity();
        transFrom.setOrigin(from + ((to - from).normalize() * (radius + 0.001)));

        btTransform transTo;
        transTo.setIdentity();
        transTo.setOrigin(to);

        btCollisionWorld::ClosestConvexResultCallback callback(from, to);
        callback.m_collisionFilterGroup = COL_RAYTEST;
        callback.m_collisionFilterMask = filter;
        _world->convexSweepTest(&sphere, transFrom, transTo, callback);

        RayResult result;
        result.collisionObject = callback.m_hitCollisionObject;
        result.group = callback.m_collisionFilterGroup;
        result.mask = callback.m_collisionFilterMask;
        result.hitPoint = callback.m_hitPointWorld;
        result.hitNormal = callback.m_hitNormalWorld;
        result.rayFrom = callback.m_convexFromWorld;
        result.rayTo = callback.m_convexToWorld;

        return result;
    }
}
