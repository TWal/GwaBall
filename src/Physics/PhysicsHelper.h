#ifndef PHYSICS_PHYSICSHELPER
#define PHYSICS_PHYSICSHELPER

namespace Ogre {
    class SceneNode;
    class Entity;
}

class btRigidBody;
class btCollisionShape;
class btConvexHullShape;
class btTriangleMesh;
class btGImpactMeshShape;
class btBvhTriangleMeshShape;

class PhysicsHelper {
    public:
        static btRigidBody* createRigidBody(float mass, btCollisionShape* shape, Ogre::SceneNode* node);
        static btConvexHullShape* createConvexHullShape(Ogre::Entity* entity);
        static btTriangleMesh* createTriangleMeshShape(Ogre::Entity* entity);
        static btGImpactMeshShape* createGImpactMeshShape(Ogre::Entity* entity);
        static btBvhTriangleMeshShape* createBvhTriangleMeshShape(Ogre::Entity* entity);
};

#endif

