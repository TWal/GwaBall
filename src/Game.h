#ifndef GAME_H
#define GAME_H

#include <chrono>

namespace OIS {
    class Mouse;
    class Keyboard;
    class InputManager;
}

class GameEngine;
class PhysicsEngine;
class GraphicsEngine;

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
        void _createInput(bool grab);
        void _deleteInput();
        std::chrono::high_resolution_clock::time_point _time;
        std::chrono::high_resolution_clock::time_point _lastTime;
        bool _running;
        int _state;

        GameEngine* _game;
        PhysicsEngine* _physics;
        GraphicsEngine* _graphics;

        OIS::InputManager* _inputManager;
        OIS::Mouse* _mouse;
        OIS::Keyboard* _keyboard;
};

#endif

