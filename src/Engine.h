#ifndef ENGINE_H
#define ENGINE_H

class Game;
class Logger;
class InputEngine;
class GameEngine;
class ScriptEngine;
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

        void attachEngines(InputEngine* input, GameEngine* game, ScriptEngine* script, PhysicsEngine* physics, GraphicsEngine* graphics);

        Game* parent();
        InputEngine* getInputEngine();
        GameEngine* getGameEngine();
        ScriptEngine* getScriptEngine();
        PhysicsEngine* getPhysicsEngine();
        GraphicsEngine* getGraphicsEngine();

    protected:
        Game* _parent;
        Logger* _log;
        InputEngine* _input;
        GameEngine* _game;
        ScriptEngine* _script;
        PhysicsEngine* _physics;
        GraphicsEngine* _graphics;
        bool _pause;
};

#endif

