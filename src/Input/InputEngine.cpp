#include "InputEngine.h"
#include <OIS.h>
#include <sstream>
#include "../Graphics/GraphicsEngine.h"
#include "InputListener.h"
#include <OgreRenderWindow.h>

InputEngine::InputEngine(Game* parent) : Engine(parent) {
    _grab = false;
}

InputEngine::~InputEngine() {
    _destroy();
}

void InputEngine::_create() {
    OIS::ParamList pl;
    size_t windowHnd = 0;
    std::ostringstream windowHndStr;
    _graphics->getRenderWindow()->getCustomAttribute("WINDOW", &windowHnd);
    windowHndStr << windowHnd;
    pl.insert(std::make_pair("WINDOW", windowHndStr.str()));
#if defined OIS_WIN32_PLATFORM
    std::string grabStr1 = _grab ? "DISCL_FOREGROUND" : "DISCL_BACKGROUND";
    std::string grabStr2 = _grab ? "DISCL_EXCLUSIVE": "DISCL_NONEXCLUSIVE";
    pl.insert(std::make_pair("w32_mouse", grabStr1));
    pl.insert(std::make_pair("w32_mouse", grabStr2));
    pl.insert(std::make_pair("w32_keyboard", grabStr1));
    pl.insert(std::make_pair("w32_keyboard", grabStr2));
#elif defined OIS_LINUX_PLATFORM
    std::string grabStr = _grab ? "true" : "false";
    pl.insert(std::make_pair("x11_mouse_grab", grabStr));
    pl.insert(std::make_pair("x11_mouse_hide", grabStr));
    pl.insert(std::make_pair("x11_keyboard_grab", grabStr));
#endif

    _inputManager = OIS::InputManager::createInputSystem(pl);
    _mouse = (OIS::Mouse*)_inputManager->createInputObject(OIS::OISMouse, true);
    _keyboard = (OIS::Keyboard*)_inputManager->createInputObject(OIS::OISKeyboard, true);

    unsigned int width, height, depth;
    int top, left;
    _graphics->getRenderWindow()->getMetrics(width, height, depth, left, top);
    const OIS::MouseState& ms = _mouse->getMouseState();
    ms.width = width;
    ms.height = height;

    _mouse->setEventCallback(this);
    _keyboard->setEventCallback(this);
}

void InputEngine::_destroy() {
    if(_inputManager) {
        _inputManager->destroyInputObject(_mouse);
        _inputManager->destroyInputObject(_keyboard);
        OIS::InputManager::destroyInputSystem(_inputManager);
    }
}

void InputEngine::init() {
    _create();
}

void InputEngine::frame(double time) {
    _mouse->capture();
    _keyboard->capture();
}

void InputEngine::reset() {
    _destroy();
    _create();
}

void InputEngine::changeState(int state) {
}


void InputEngine::setGrab(bool grab) {
    _grab = grab;
    _destroy();
    _create();
}

void InputEngine::addListener(InputListener* listener) {
    _listeners.insert(listener);
}

void InputEngine::removeListener(InputListener* listener) {
    _listeners.erase(listener);
}

OIS::Keyboard* InputEngine::getKeyboard() {
    return _keyboard;
}

OIS::Mouse* InputEngine::getMouse() {
    return _mouse;
}


bool InputEngine::mouseMoved(const OIS::MouseEvent& arg) {
    for(InputListener* l : _listeners) {
        l->mouseMoved(arg);
    }
    return true;
}

bool InputEngine::mousePressed(const OIS::MouseEvent& arg, OIS::MouseButtonID id) {
    for(InputListener* l : _listeners) {
        l->mousePressed(arg, id);
    }
    return true;
}

bool InputEngine::mouseReleased(const OIS::MouseEvent& arg, OIS::MouseButtonID id) {
    for(InputListener* l : _listeners) {
        l->mouseReleased(arg, id);
    }
    return true;
}

bool InputEngine::keyPressed(const OIS::KeyEvent& arg) {
    for(InputListener* l : _listeners) {
        l->keyPressed(arg);
    }
    return true;
}

bool InputEngine::keyReleased(const OIS::KeyEvent& arg) {
    for(InputListener* l : _listeners) {
        l->keyReleased(arg);
    }
    return true;
}

