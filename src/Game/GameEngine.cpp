#include "GameEngine.h"

#include <Ogre.h>
#include <btBulletDynamicsCommon.h>
#include <OISKeyboard.h>
#include "../Input/InputEngine.h"
#include "../Game.h"
#include "../Physics/PhysicsEngine.h"
#include "../Graphics/GraphicsEngine.h"
#include "../Graphics/CameraManager.h"
#include "Player.h"
#include "../Utils.h"
#include "../Logger.h"
#include "ObjectManager.h"

GameEngine::GameEngine(Game* parent) : Engine(parent) {
    _log = new Logger(Logger::INFO, Logger::STDERR | Logger::LOGFILE, "Logs/GameEngine.log");
    _log->info("Creating player");
    _objmgr = new ObjectManager(this);
    _player = new Player(this);
}

GameEngine::~GameEngine() {
}

void GameEngine::init() {
    _player->init();
    _objmgr->load("Test.xml");
}

void GameEngine::frame(double time) {
    if(!_pause) {
        btVector3 lookingDir = Converter::convert(_graphics->getCameraManager()->getLookingDirection());
        lookingDir.setY(0);
        _player->frame(time);
        if(_input->getKeyboard()->isKeyDown(OIS::KC_UP)) {
            //_player->move(btVector3(-1, 0, 0), time);
            _player->move(lookingDir, time);
        }
        if(_input->getKeyboard()->isKeyDown(OIS::KC_DOWN)) {
            _player->move(-lookingDir, time);
        }
        if(_input->getKeyboard()->isKeyDown(OIS::KC_LEFT)) {
            _player->move(lookingDir.cross(btVector3(0, -9.81, 0)), time);
        }
        if(_input->getKeyboard()->isKeyDown(OIS::KC_RIGHT)) {
            _player->move(-lookingDir.cross(btVector3(0, -9.81, 0)), time);
        }
    }
}

void GameEngine::reset() {
    _player->reset();
}

void GameEngine::changeState(int state) {
    switch(state) {
        case Game::GS_MENU:
            _pause = true;
            break;
        case Game::GS_RUNNING:
            _pause = false;
            break;
        case Game::GS_QUIT:
            break;
        default:
            break;
    }
}

Player* GameEngine::getPlayer() {
    return _player;
}

