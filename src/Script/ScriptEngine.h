#ifndef SCRIPT_SCRIPTENGINE_H
#define SCRIPT_SCRIPTENGINE_H

#include "../Engine.h"
#include <string>
#include <unordered_map>

class Game;
/*
class asSMessageInfo;
class asIScriptEngine;
class asIScriptModule;
class asIScriptContext;
class asIScriptFunction;
class asIObjectType;
*/
class btRigidBody;

class ScriptEngine : public Engine {
    public:
        ScriptEngine(Game* parent);
        virtual ~ScriptEngine();

        virtual void init();
        virtual void frame(double time);
        virtual void reset();
        virtual void changeState(int state);

        void loadFile(const std::string& file, const std::string& className);
        void loadString(const std::string& str, const std::string& section);
        void instanciateScript(const std::string& className, btRigidBody* body = nullptr);

    private:
        int _compile();
        //void _messageCallback(const asSMessageInfo* msg);
        /*
        asIScriptEngine* _engine;
        asIScriptModule* _module;
        asIScriptContext* _ctx;
        asIScriptFunction* _addFunc;
        asIScriptFunction* _frameFunc;
        */
        std::unordered_map<std::string, std::string> _classesCode;
};

#endif

