#ifndef GAME_PLAYER_H
#define GAME_PLAYER_H

#include <LinearMath/btVector3.h>
class GameEngine;
class btRigidBody;
class btCollisionShape;
namespace Ogre {
    class SceneNode;
}

class Player {
    public:
        Player(GameEngine* parent);
        ~Player();

        void init();
        void move(const btVector3& direction, double time);
        void frame(double time);
        void reset();

        Ogre::SceneNode* getSceneNode();
        btRigidBody* getBody();

    private:
        float _getNFromVel(float vel);
        float _getAccFromN(float N);
        GameEngine* _parent;
        Ogre::SceneNode* _node;
        btRigidBody* _body;
        btCollisionShape* _shape;
        btVector3 _moveSum;
        int _moveNb;
};

#endif

