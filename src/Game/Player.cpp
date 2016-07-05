#include "Player.h"
#include "GameEngine.h"
#include "../Graphics/GraphicsEngine.h"
#include "../Physics/PhysicsEngine.h"
#include <Ogre.h>
#include <btBulletDynamicsCommon.h>
#include <cmath>

static const float BALL_RADIUS = 0.5;
static const float DAMPING_PER_SEC_XZ = 0.05;
static const float DAMPING_PER_SEC_Y = 0.75;
static const float SPEED = 10;


Player::Player(GameEngine* parent) :
    _parent(parent),
    _node(nullptr),
    _body(nullptr),
    _shape(nullptr),
    _moveSum(0, 0, 0),
    _moveNb(0) {
}

Player::~Player() {
    _parent->getPhysicsEngine()->deleteRigidBody(_body);
    _parent->getPhysicsEngine()->deleteCollisionShape(_shape);
    Utils::deleteOgreNode(_node);
}

void Player::init() {
    _node = _parent->getGraphicsEngine()->getSceneManager()->getRootSceneNode()->createChildSceneNode();
    _node->attachObject(_parent->getGraphicsEngine()->getSceneManager()->createEntity("Ball", "Ball.mesh"));
    _shape = new btSphereShape(BALL_RADIUS);
    _body = _parent->getPhysicsEngine()->addRigidBody(0.5, _shape, _node, PhysicsEngine::COL_PLAYER, PhysicsEngine::COL_ALL);
}

//vel(t) = speed*ln(1+x)
//=> vel'(t) = speed*exp(-vel(t)/speed)
void Player::move(const btVector3& direction, double time) {
    btVector3 dir = btEqual(direction.length2()-1, SIMD_EPSILON) ? direction : direction.normalized();
    float velocity = (_body->getLinearVelocity() * dir).length();
    float acc = SPEED * exp(-velocity/SPEED);
    _body->applyCentralForce(dir * acc);
    _body->applyTorque(btVector3(dir.z(), dir.y(), -dir.x()) * acc / M_PI);

    _moveSum += dir;
    _moveNb += 1;
}

void Player::frame(double time) {
    //Player is always activated
    if(_body->getActivationState() != 1) {
        _body->setActivationState(1);
    }

    if(_moveNb) {
        _moveSum = btVector3(btFabs(_moveSum.x()), btFabs(_moveSum.y()), btFabs(_moveSum.z()));
        _moveSum /= _moveNb;
    }

    //Custom damping (don't damp the y axis)
    btScalar factorXZ = btPow(DAMPING_PER_SEC_XZ, time);
    btScalar factorY = btPow(DAMPING_PER_SEC_Y, time);
    btVector3 linearFactor = btVector3(factorXZ, factorY, factorXZ);
    linearFactor += _moveSum * (btVector3(1, 1, 1) - linearFactor);
    btVector3 angularFactor = btVector3(factorXZ, factorXZ, factorXZ);
    angularFactor += _moveSum * (btVector3(1, 1, 1) - angularFactor);
    _body->setLinearVelocity(_body->getLinearVelocity() * linearFactor);
    _body->setAngularVelocity(_body->getAngularVelocity() * angularFactor);

    _moveSum = btVector3(0, 0, 0);
    _moveNb = 0;
}

void Player::reset() {
}

Ogre::SceneNode* Player::getSceneNode() {
    return _node;
}

btRigidBody* Player::getBody() {
    return _body;
}

