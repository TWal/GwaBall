#ifndef GAME_GAMEENGINE_H
#define GAME_GAMEENGINE_H

#include "../Engine.h"

class Game;
class btRigidBody;
namespace Ogre {
    class Entity;
    class SceneNode;
}

class GameEngine : public Engine {
    public:
        GameEngine(Game* parent);
        virtual ~GameEngine();

        virtual void init();
        virtual void frame(double time);
        virtual void reset();
        virtual void changeState(int state);

    private:
};

#endif

