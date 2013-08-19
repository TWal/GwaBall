#ifndef GRAPHICS_GRAPHICSENGINE_H
#define GRAPHICS_GRAPHICSENGINE_H

#include "../Engine.h"
#include <OgreWindowEventUtilities.h>

namespace Ogre {
    class Root;
    class RenderWindow;
    class SceneManager;
    class Viewport;
    class Camera;
}

class Game;
class CameraManager;

class GraphicsEngine : public Engine, private Ogre::WindowEventListener {
    public:
        GraphicsEngine(Game* parent);
        virtual ~GraphicsEngine();

        virtual void init();
        virtual void frame(double time);
        virtual void reset();
        virtual void changeState(int state);

        Ogre::RenderWindow* getRenderWindow();
        Ogre::SceneManager* getSceneManager();
        CameraManager* getCameraManager();

    private:
        virtual void windowMoved(Ogre::RenderWindow* rw);
        virtual void windowResized(Ogre::RenderWindow* rw);
        bool _render(double time);
        Ogre::Root* _root;
        Ogre::RenderWindow* _renderWindow;
        Ogre::SceneManager* _smgr;
        Ogre::Viewport* _viewport;
        CameraManager* _cameraMgr;
};

#endif


