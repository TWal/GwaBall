#include "Engine.h"

Engine::Engine(Game *parent) :
    _parent(parent),
    _input(nullptr),
    _game(nullptr),
    _physics(nullptr),
    _graphics(nullptr),
    _pause(false) {
}

Engine::~Engine() {
}

void Engine::attachEngines(InputEngine* input, GameEngine* game, PhysicsEngine* physics, GraphicsEngine* graphics) {
    _input = input;
    _game = game;
    _physics = physics;
    _graphics = graphics;
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

InputEngine* Engine::getInputEngine() {
    return _input;
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

