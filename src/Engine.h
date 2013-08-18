#ifndef ENGINE_H
#define ENGINE_H

class Game;
class InputEngine;
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

        void attachEngines(InputEngine* input, GameEngine* game, PhysicsEngine* physics, GraphicsEngine* graphics);

        Game* parent();
        InputEngine* getInputEngine();
        GameEngine* getGameEngine();
        PhysicsEngine* getPhysicsEngine();
        GraphicsEngine* getGraphicsEngine();

    protected:
        Game* _parent;
        InputEngine* _input;
        GameEngine* _game;
        PhysicsEngine* _physics;
        GraphicsEngine* _graphics;
        bool _pause;
};

#endif

