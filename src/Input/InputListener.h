#ifndef INPUT_INPUTLISTENER_H
#define INPUT_INPUTLISTENER_H

#include <OISKeyboard.h>
#include <OISMouse.h>

class InputListener {
    public:
        virtual ~InputListener();
        virtual void mouseMoved(const OIS::MouseEvent& arg);
        virtual void mousePressed(const OIS::MouseEvent& arg, OIS::MouseButtonID id);
        virtual void mouseReleased(const OIS::MouseEvent& arg, OIS::MouseButtonID id);
        virtual void keyPressed(const OIS::KeyEvent& arg);
        virtual void keyReleased(const OIS::KeyEvent& arg);
};

#endif

