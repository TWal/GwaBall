#include "GraphicsEngine.h"

#include <OgreRoot.h>
#include <OgreRenderWindow.h>
#include <OgreSceneManager.h>
#include <OgreViewport.h>
#include <OgreCamera.h>
#include <OgreResourceManager.h>
#include <OgreException.h>
#include <OgreConfigFile.h>
#include "../Game.h"
#include "CameraManager.h"
#include "../Input/InputEngine.h"

GraphicsEngine::GraphicsEngine(Game* parent) : Engine(parent) {
    try {
        _root = new Ogre::Root();
        if(!_root->restoreConfig()) {
            if(!_root->showConfigDialog()) {
                delete _root;
                return;
            }
        }
        _renderWindow = _root->initialise(true, "GwaBall");
        _smgr = _root->createSceneManager(Ogre::ST_GENERIC);

        Ogre::ConfigFile cf;
        cf.load("resources.cfg");
        Ogre::ConfigFile::SectionIterator sectionIterator = cf.getSectionIterator();
        while (sectionIterator.hasMoreElements()) {
            Ogre::ConfigFile::SettingsMultiMap* settings = sectionIterator.getNext();
            for (auto it : *settings) {
                Ogre::ResourceGroupManager::getSingleton().addResourceLocation(it.second, it.first);
            }
        }
        Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
        _cameraMgr = new CameraManager(this);
        _viewport = _renderWindow->addViewport(_cameraMgr->getCamera());
        _viewport->setBackgroundColour(Ogre::ColourValue(1.f, 1.f, 1.f, 1.f));
        Ogre::WindowEventUtilities::addWindowEventListener(_renderWindow, this);
    } catch(Ogre::Exception& e) {
        std::cerr << "An Ogre exception occured: " << e.getFullDescription() << "." << std::endl;
    } catch(std::exception& e) {
        std::cerr << "An exception occured: " << e.what() << "." << std::endl;
    }
}

GraphicsEngine::~GraphicsEngine() {
    delete _root;
}

void GraphicsEngine::init() {
}

void GraphicsEngine::frame(double time) {
    _cameraMgr->update();
    bool renderResult = _render(time);
    Ogre::WindowEventUtilities::messagePump();
    if(_renderWindow->isClosed() || !renderResult) {
        _parent->changeState(Game::GS_QUIT);
    }
}

void GraphicsEngine::reset() {
    _smgr->clearScene();
}

void GraphicsEngine::changeState(int state) {
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
    if(_pause) {
        _input->removeListener(_cameraMgr);
    } else {
        _input->addListener(_cameraMgr);
    }
}

bool GraphicsEngine::_render(double time) {
    return _root->renderOneFrame(_pause ? 0.f : time);
}

void GraphicsEngine::windowMoved(Ogre::RenderWindow* rw) {
    _input->setGrab(_input->getGrab());
}

void GraphicsEngine::windowResized(Ogre::RenderWindow* rw) {
    _input->setGrab(_input->getGrab());
}

Ogre::RenderWindow* GraphicsEngine::getRenderWindow() {
    return _renderWindow;
}

Ogre::SceneManager* GraphicsEngine::getSceneManager() {
    return _smgr;
}

CameraManager* GraphicsEngine::getCameraManager() {
    return _cameraMgr;
}
