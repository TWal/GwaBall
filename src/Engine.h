#ifndef ENGINE_H
#define ENGINE_H

namespace OIS {
    class Mouse;
    class Keyboard;
    class InputManager;
}

class Game;
class GraphicsEngine;
class PhysicsEngine;

class Engine {
    public:
        Engine(Game* parent);
        virtual ~Engine();

        virtual void frame(double time) = 0;
        virtual void reset() = 0;
        virtual void changeState(int state) = 0;

        void togglePause();
        void setPause(bool pause);
        bool getPause();

        void attachEngines(PhysicsEngine* physics, GraphicsEngine* graphics);
        void attachInput(OIS::Mouse* mouse, OIS::Keyboard* keyboard);

        Game* parent();
        PhysicsEngine* getPhysicsEngine();
        GraphicsEngine* getGraphicsEngine();
        OIS::Mouse* getMouse();
        OIS::Keyboard* getKeyboard();

    protected:
        Game* _parent;
        PhysicsEngine* _physics;
        GraphicsEngine* _graphics;
        OIS::Mouse* _mouse;
        OIS::Keyboard* _keyboard;
        bool _pause;
};

#endif

