#include "Game.h"

#include "Input/InputEngine.h"
#include "Game/GameEngine.h"
#include "Graphics/GraphicsEngine.h"
#include "Physics/PhysicsEngine.h"

#include <OISKeyboard.h>

Game::Game() {
    _input = new InputEngine(this);
    _game = new GameEngine(this);
    _physics = new PhysicsEngine(this);
    _graphics = new GraphicsEngine(this);

    _input->attachEngines(_input, _game, _physics, _graphics);
    _game->attachEngines(_input, _game, _physics, _graphics);
    _physics->attachEngines(_input, _game, _physics, _graphics);
    _graphics->attachEngines(_input, _game, _physics, _graphics);

    _input->init();
    _game->init();
    _physics->init();
    _graphics->init();

    _time = std::chrono::high_resolution_clock::now();
}

Game::~Game() {
    delete _game;
    delete _physics;
    delete _graphics;
}

void Game::run() {
    _running = true;
    changeState(GS_RUNNING);
    while(_running) {
        _lastTime = _time;
        _time = std::chrono::high_resolution_clock::now();
        double elapsedTime = (std::chrono::duration_cast<std::chrono::duration<double>>(_time - _lastTime)).count();

        _input->frame(elapsedTime);
        _game->frame(elapsedTime);
        _physics->frame(elapsedTime);
        _graphics->frame(elapsedTime);

        if(_input->getKeyboard()->isKeyDown(OIS::KC_M)) {
            changeState(GS_MENU);
        }
        if(_input->getKeyboard()->isKeyDown(OIS::KC_R)) {
            changeState(GS_RUNNING);
        }
        if(_input->getKeyboard()->isKeyDown(OIS::KC_ESCAPE) || _input->getKeyboard()->isKeyDown(OIS::KC_Q)) {
            changeState(GS_QUIT);
        }
    }
}

void Game::changeState(int state) {
    _state = state;
    _game->changeState(state);
    _physics->changeState(state);
    _graphics->changeState(state);
    switch(state) {
        case GS_MENU:
            _input->setGrab(false);
            break;
        case GS_RUNNING:
            _input->setGrab(true);
            break;
        case GS_QUIT:
            _running = false;
            break;
        default:
            break;
    }
}

int Game::getState() {
    return _state;
}

