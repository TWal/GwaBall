#include "Engine.h"

Engine::Engine(Game *parent) :
    _parent(parent),
    _graphics(nullptr),
    _mouse(nullptr),
    _keyboard(nullptr),
    _pause(false) {
}

Engine::~Engine() {
}

void Engine::attachEngines(GameEngine* game, PhysicsEngine* physics, GraphicsEngine* graphics) {
    _game = game;
    _physics = physics;
    _graphics = graphics;
}

void Engine::attachInput(OIS::Mouse* mouse, OIS::Keyboard* keyboard) {
    _mouse = mouse;
    _keyboard = keyboard;
}

void Engine::togglePause() {
    _pause = !_pause;
}

void Engine::setPause(bool pause) {
    _pause = pause;
}

bool Engine::getPause() {
    return _pause;
}

Game* Engine::parent() {
    return _parent;
}

GameEngine* Engine::getGameEngine() {
    return _game;
}

PhysicsEngine* Engine::getPhysicsEngine() {
    return _physics;
}

GraphicsEngine* Engine::getGraphicsEngine() {
    return _graphics;
}

OIS::Mouse* Engine::getMouse() {
    return _mouse;
}

OIS::Keyboard* Engine::getKeyboard() {
    return _keyboard;
}
