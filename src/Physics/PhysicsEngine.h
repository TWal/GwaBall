#ifndef PHYSICS_PHYSICSENGINE_H
#define PHYSICS_PHYSICSENGINE_H

#include "../Engine.h"
#include <unordered_set>
#include "../Utils.h"

class Game;
class btCollisionConfiguration;
class btDispatcher;
class btBroadphaseInterface;
class btConstraintSolver;
class btDynamicsWorld;
class btRigidBody;
class btCollisionShape;
class btCollisionObject;

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

        enum COLLISIONGROUP {
            COL_NOTHING = 0,
            COL_STATIC = Utils::bit(0),
            COL_DYNAMIC = Utils::bit(1),
            COL_PLAYER = Utils::bit(2),
            COL_RAYTEST = Utils::bit(3),
            COL_ALL = ~COL_NOTHING,
            COL_NOSTATIC = ~COL_STATIC,
            COL_NODYNAMIC = ~COL_DYNAMIC,
            COL_NOPLAYER = ~COL_PLAYER,
            COL_NORAYTEST = ~COL_RAYTEST
        };

        struct RayResult {
            const btCollisionObject* collisionObject;
            btVector3 hitNormal;
            btVector3 hitPoint;
            btVector3 rayFrom;
            btVector3 rayTo;
            short group;
            short mask;
        };

        btRigidBody* addRigidBody(btRigidBody* body);
        btRigidBody* addRigidBody(btRigidBody* body, short group, short mask);
        btRigidBody* addRigidBody(float mass, btCollisionShape* shape, Ogre::SceneNode* node);
        btRigidBody* addRigidBody(float mass, btCollisionShape* shape, Ogre::SceneNode* node, short group, short mask);
        btRigidBody* addRigidBody(float mass, Ogre::Entity* entity, Ogre::SceneNode* node, RIGIDBODYTYPE type);
        btRigidBody* addRigidBody(float mass, Ogre::Entity* entity, Ogre::SceneNode* node, RIGIDBODYTYPE type, short group, short mask);
        void registerCollisionShape(btCollisionShape* shape);
        void deleteRigidBody(btRigidBody* body, bool deleteShape = false);
        void deleteCollisionShape(btCollisionShape* shape);
        RayResult rayTest(const btVector3& from, const btVector3& to, int filter, float radius);

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

