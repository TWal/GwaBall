#ifndef GRAPHICS_CAMERAMANAGER_H
#define GRAPHICS_CAMERAMANAGER_H

#include "../Input/InputListener.h"
#include <OgreVector3.h>

class GraphicsEngine;
namespace Ogre {
    class Camera;
    class SceneNode;
    class RaySceneQuery;
}

class CameraManager : public InputListener {
    public:
        CameraManager(GraphicsEngine* parent);
        void update();
        virtual void mouseMoved(const OIS::MouseEvent& arg);
        Ogre::Camera* getCamera();
        Ogre::Vector3 getLookingDirection();

    private:
        GraphicsEngine* _parent;
        Ogre::SceneNode* _toPlayerNode;
        Ogre::SceneNode* _yawNode;
        Ogre::SceneNode* _pitchNode;
        Ogre::SceneNode* _distanceNode;
        Ogre::Camera* _camera;
        float _distance;
};

#endif

