#ifndef INPUT_INPUTENGINE_H
#define INPUT_INPUTENGINE_H

#include "../Engine.h"
#include <unordered_set>
#include <OISKeyboard.h>
#include <OISMouse.h>

class InputListener;

class InputEngine : public Engine, public OIS::MouseListener, public OIS::KeyListener {
    public:
        InputEngine(Game* parent);
        virtual ~InputEngine();
        virtual void init();
        virtual void frame(double time);
        virtual void reset();
        virtual void changeState(int state);
        void setGrab(bool grab);
        bool getGrab();
        void addListener(InputListener* listener);
        void removeListener(InputListener* listener);

        OIS::Keyboard* getKeyboard();
        OIS::Mouse* getMouse();

    private:
        virtual bool mouseMoved(const OIS::MouseEvent& arg);
        virtual bool mousePressed(const OIS::MouseEvent& arg, OIS::MouseButtonID id);
        virtual bool mouseReleased(const OIS::MouseEvent& arg, OIS::MouseButtonID id);
        virtual bool keyPressed(const OIS::KeyEvent& arg);
        virtual bool keyReleased(const OIS::KeyEvent& arg);

        void _create();
        void _destroy();
        OIS::InputManager* _inputManager;
        OIS::Mouse* _mouse;
        OIS::Keyboard* _keyboard;
        std::unordered_set<InputListener*> _listeners;
        bool _grab;
};

#endif

