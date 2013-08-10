#include "GraphicsEngine.h"

#include <OgreRoot.h>
#include <OgreRenderWindow.h>
#include <OgreSceneManager.h>
#include <OgreViewport.h>
#include <OgreCamera.h>
#include <OgreResourceManager.h>
#include <OgreException.h>
#include <OgreConfigFile.h>
#include <OgreWindowEventUtilities.h>
#include "../Game.h"

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

        _camera = _smgr->createCamera("Camera");
        _camera->setFOVy(Ogre::Degree(90));
        _camera->setNearClipDistance(1);
        _camera->setFarClipDistance(1000);
        _camera->setPosition(5, 3, 0);
        _camera->lookAt(0, 0, 0);
        _viewport = _renderWindow->addViewport(_camera);
        _viewport->setBackgroundColour(Ogre::ColourValue(1.f, 1.f, 1.f, 1.f));
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
}

bool GraphicsEngine::_render(double time) {
    return _root->renderOneFrame(_pause ? 0.f : time);
}

Ogre::RenderWindow* GraphicsEngine::getRenderWindow() {
    return _renderWindow;
}

Ogre::SceneManager* GraphicsEngine::getSceneManager() {
    return _smgr;
}
