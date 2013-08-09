#ifndef PHYSICS_MOTIONSTATE
#define PHYSICS_MOTIONSTATE

#include <LinearMath/btMotionState.h>
#include <LinearMath/btTransform.h>

namespace Ogre {
    class SceneNode;
}

class MotionState : public btMotionState {
    public:
        MotionState(Ogre::SceneNode* node);
        virtual ~MotionState();
        virtual void getWorldTransform(btTransform& worldTrans) const;
        virtual void setWorldTransform(const btTransform& worldTrans);

    protected:
        Ogre::SceneNode* _node;
};

#endif

