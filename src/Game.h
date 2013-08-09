#ifndef GAME_H
#define GAME_H

#include <chrono>

namespace OIS {
    class Mouse;
    class Keyboard;
    class InputManager;
}

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
            GS_RUNNING,
            GS_QUIT
        };

    private:
        std::chrono::high_resolution_clock::time_point _time;
        std::chrono::high_resolution_clock::time_point _lastTime;
        bool running;
        int _state;

        PhysicsEngine* _physics;
        GraphicsEngine* _graphics;

        OIS::InputManager* _inputManager;
        OIS::Mouse* _mouse;
        OIS::Keyboard* _keyboard;
};

#endif

