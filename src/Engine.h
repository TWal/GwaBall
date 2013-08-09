#ifndef ENGINE_H
#define ENGINE_H

namespace OIS {
    class Mouse;
    class Keyboard;
    class InputManager;
}

class Game;
class GameEngine;
class PhysicsEngine;
class GraphicsEngine;

class Engine {
    public:
        Engine(Game* parent);
        virtual ~Engine();

        virtual void init() = 0;
        virtual void frame(double time) = 0;
        virtual void reset() = 0;
        virtual void changeState(int state) = 0;

        void togglePause();
        void setPause(bool pause);
        bool getPause();

        void attachEngines(GameEngine* game, PhysicsEngine* physics, GraphicsEngine* graphics);
        void attachInput(OIS::Mouse* mouse, OIS::Keyboard* keyboard);

        Game* parent();
        GameEngine* getGameEngine();
        PhysicsEngine* getPhysicsEngine();
        GraphicsEngine* getGraphicsEngine();
        OIS::Mouse* getMouse();
        OIS::Keyboard* getKeyboard();

    protected:
        Game* _parent;
        GameEngine* _game;
        PhysicsEngine* _physics;
        GraphicsEngine* _graphics;
        OIS::Mouse* _mouse;
        OIS::Keyboard* _keyboard;
        bool _pause;
};

#endif

