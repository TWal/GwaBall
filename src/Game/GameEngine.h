#ifndef GAME_GAMEENGINE_H
#define GAME_GAMEENGINE_H

#include "../Engine.h"

class Game;
class btRigidBody;
namespace Ogre {
    class Entity;
    class SceneNode;
}
class ObjectManager;
class Player;

class GameEngine : public Engine {
    public:
        GameEngine(Game* parent);
        virtual ~GameEngine();

        virtual void init();
        virtual void frame(double time);
        virtual void reset();
        virtual void changeState(int state);

        Player* getPlayer();

    private:
        ObjectManager* _objmgr;
        Player* _player;
};

#endif

