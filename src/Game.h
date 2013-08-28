#ifndef GAME_H
#define GAME_H

#include <chrono>

class InputEngine;
class GameEngine;
class ScriptEngine;
class PhysicsEngine;
class GraphicsEngine;
class Logger;

class Game {
    public:
        Game();
        ~Game();
        void run();
        void changeState(int state);
        int getState();

        enum GAMESTATE {
            GS_MENU,
            GS_RUNNING,
            GS_QUIT
        };

    private:
        std::chrono::high_resolution_clock::time_point _time;
        std::chrono::high_resolution_clock::time_point _lastTime;
        bool _running;
        int _state;

        InputEngine* _input;
        GameEngine* _game;
        ScriptEngine* _script;
        PhysicsEngine* _physics;
        GraphicsEngine* _graphics;

        Logger* _log;
};

#endif

