#include "PhysicsHelper.h"

#include <Ogre.h>
#include <btBulletDynamicsCommon.h>
#include <BulletCollision/Gimpact/btGImpactShape.h>

#include "MotionState.h"
#include "../Utils.h"

btRigidBody* PhysicsHelper::createRigidBody(float mass, btCollisionShape* shape, Ogre::SceneNode* node) {
    btVector3 inertia;
    shape->calculateLocalInertia(mass, inertia);
    MotionState* motionState = new MotionState(node);
    return new btRigidBody(mass, motionState, shape, inertia);
}

btConvexHullShape* PhysicsHelper::createConvexHullShape(Ogre::Entity* entity) {
    Ogre::MeshPtr mesh = entity->getMesh();
    btConvexHullShape* convexHull = new btConvexHullShape();
    for(int i = 0; i < mesh->getNumSubMeshes(); ++i) {
        Ogre::SubMesh* subMesh = mesh->getSubMesh(i);

        Ogre::VertexData* vertexData = subMesh->vertexData;
        const Ogre::VertexElement* posElement = vertexData->vertexDeclaration->findElementBySemantic(Ogre::VES_POSITION);

        Ogre::HardwareVertexBufferSharedPtr vBuffer = vertexData->vertexBufferBinding->getBuffer(posElement->getSource());
        unsigned char* vertex = static_cast<unsigned char*>(vBuffer->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));

        Ogre::Real* current = 0;

        for(size_t j = 0; j < vertexData->vertexCount; ++j, vertex += vBuffer->getVertexSize()) {
            posElement->baseVertexPointerToElement(vertex, &current);
            convexHull->addPoint(btVector3(current[0], current[1], current[2]));
        }

        vBuffer->unlock();
    }

    return convexHull;
}

btTriangleMesh* PhysicsHelper::createTriangleMeshShape(Ogre::Entity* entity) {
    Ogre::MeshPtr mesh = entity->getMesh();
    btTriangleMesh* triangleMesh = new btTriangleMesh();
    for(int i = 0; i < mesh->getNumSubMeshes(); ++i) {
        Ogre::SubMesh* subMesh = mesh->getSubMesh(i);

        Ogre::VertexData* vertexData = subMesh->vertexData;
        Ogre::IndexData* indexData = subMesh->indexData;
        const Ogre::VertexElement* posElement = vertexData->vertexDeclaration->findElementBySemantic(Ogre::VES_POSITION);

        Ogre::HardwareVertexBufferSharedPtr vBuffer = vertexData->vertexBufferBinding->getBuffer(posElement->getSource());
        Ogre::HardwareIndexBufferSharedPtr iBuffer = indexData->indexBuffer;

        unsigned char* vertex = static_cast<unsigned char*>(vBuffer->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));

        Ogre::Real* vector1 = 0;
        Ogre::Real* vector2 = 0;
        Ogre::Real* vector3 = 0;

        bool use32bitindexes = (iBuffer->getType() == Ogre::HardwareIndexBuffer::IT_32BIT);

        if(use32bitindexes) {
            unsigned long* index = static_cast<unsigned long*>(iBuffer->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));
            for(size_t j = 0; j < indexData->indexCount; j+=3) {
                posElement->baseVertexPointerToElement(vertex + index[j] * vBuffer->getVertexSize(), &vector1);
                posElement->baseVertexPointerToElement(vertex + index[j + 1] * vBuffer->getVertexSize(), &vector2);
                posElement->baseVertexPointerToElement(vertex + index[j + 2] * vBuffer->getVertexSize(), &vector3);
                triangleMesh->addTriangle(btVector3(vector1[0], vector1[1], vector1[2]), btVector3(vector2[0], vector2[1], vector2[2]), btVector3(vector3[0], vector3[1], vector3[2]));
            }
        } else {
            unsigned short* index = static_cast<unsigned short*>(iBuffer->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));
            for(size_t j = 0; j < indexData->indexCount; j+=3) {
                posElement->baseVertexPointerToElement(vertex + index[j] * vBuffer->getVertexSize(), &vector1);
                posElement->baseVertexPointerToElement(vertex + index[j + 1] * vBuffer->getVertexSize(), &vector2);
                posElement->baseVertexPointerToElement(vertex + index[j + 2] * vBuffer->getVertexSize(), &vector3);
                triangleMesh->addTriangle(btVector3(vector1[0], vector1[1], vector1[2]), btVector3(vector2[0], vector2[1], vector2[2]), btVector3(vector3[0], vector3[1], vector3[2]));
            }
        }
        vBuffer->unlock();
        iBuffer->unlock();
    }

    return triangleMesh;
}

btGImpactMeshShape* PhysicsHelper::createGImpactMeshShape(Ogre::Entity* entity) {
    btGImpactMeshShape* gimpactShape = new btGImpactMeshShape(createTriangleMeshShape(entity));
    gimpactShape->updateBound();
    return gimpactShape;
}

btBvhTriangleMeshShape* PhysicsHelper::createBvhTriangleMeshShape(Ogre::Entity* entity) {
    return new btBvhTriangleMeshShape(createTriangleMeshShape(entity), false, true);
}

