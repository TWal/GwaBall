#include "Game.h"

#include "Input/InputEngine.h"
#include "Game/GameEngine.h"
#include "Script/ScriptEngine.h"
#include "Graphics/GraphicsEngine.h"
#include "Physics/PhysicsEngine.h"
#include "Logger.h"

#include <OISKeyboard.h>

Game::Game() {
    _log = new Logger(Logger::INFO, Logger::STDERR | Logger::LOGFILE, "Logs/Game.log");
    _log->info("Creating engines");
    _input = new InputEngine(this);
    _game = new GameEngine(this);
    _script = new ScriptEngine(this);
    _physics = new PhysicsEngine(this);
    _graphics = new GraphicsEngine(this);

    _log->info("Attaching engines");
    _input->attachEngines(_input, _game, _script, _physics, _graphics);
    _game->attachEngines(_input, _game, _script, _physics, _graphics);
    _script->attachEngines(_input, _game, _script, _physics, _graphics);
    _physics->attachEngines(_input, _game, _script, _physics, _graphics);
    _graphics->attachEngines(_input, _game, _script, _physics, _graphics);

    _log->info("Initializing engines");
    _input->init();
    _game->init();
    _script->init();
    _physics->init();
    _graphics->init();

    _log->info("Initializing clock");
    _time = std::chrono::high_resolution_clock::now();

    _game->loadLevel("Test.xml");
    _state = -1;
}

Game::~Game() {
    _log->info("Deleting engines");
    delete _log;
    delete _input;
    delete _game;
    delete _script;
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
        _script->frame(elapsedTime);
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
    _log->info("Changing state from %d to %d", _state, state);
    _state = state;
    _game->changeState(state);
    _script->changeState(state);
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

