#ifndef PHYSICS_PHYSICSENGINE_H
#define PHYSICS_PHYSICSENGINE_H

#include "../Engine.h"
#include <unordered_set>

class Game;
class btCollisionConfiguration;
class btDispatcher;
class btBroadphaseInterface;
class btConstraintSolver;
class btDynamicsWorld;
class btRigidBody;
class btCollisionShape;

namespace Ogre {
    class SceneNode;
    class Entity;
}

class PhysicsEngine : public Engine {
    public:
        PhysicsEngine(Game* parent);
        virtual ~PhysicsEngine ();

        virtual void init();
        virtual void frame(double time);
        virtual void reset();
        virtual void changeState(int state);

        enum RIGIDBODYTYPE {
            RBT_CONVEX,
            RBT_TRIMESH
        };

		btRigidBody* addRigidBody(btRigidBody* body);
		btRigidBody* addRigidBody(btRigidBody* body, short group, short mask);
		btRigidBody* addRigidBody(float mass, btCollisionShape* shape, Ogre::SceneNode* node);
		btRigidBody* addRigidBody(float mass, btCollisionShape* shape, Ogre::SceneNode* node, short group, short mask);
		btRigidBody* addRigidBody(float mass, Ogre::Entity* entity, Ogre::SceneNode* node, RIGIDBODYTYPE type);
		btRigidBody* addRigidBody(float mass, Ogre::Entity* entity, Ogre::SceneNode* node, RIGIDBODYTYPE type, short group, short mask);

    private:
        void _construct();
        void _destroy();

        btCollisionConfiguration* _collisionConfiguration;
        btDispatcher* _dispatcher;
        btBroadphaseInterface* _broadphase;
        btConstraintSolver* _constraintSolver;
        btDynamicsWorld* _world;

        std::unordered_set<btRigidBody*> _bodies;
        std::unordered_set<btCollisionShape*> _shapes;
};

#endif

