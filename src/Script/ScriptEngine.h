#ifndef SCRIPT_SCRIPTENGINE_H
#define SCRIPT_SCRIPTENGINE_H

#include "../Engine.h"
#include <string>
#include <unordered_map>

class Game;
class btRigidBody;
namespace sol {
    class state;
}

class ScriptEngine : public Engine {
    public:
        ScriptEngine(Game* parent);
        virtual ~ScriptEngine();

        virtual void init();
        virtual void frame(double time);
        virtual void reset();
        virtual void changeState(int state);

        void loadFile(const std::string& file, const std::string& className);
        void instanciateScript(const std::string& className, const std::string& args, btRigidBody* body = nullptr);

    private:
        sol::state* _lua;
};

#endif

