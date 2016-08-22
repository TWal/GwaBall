#include "ScriptEngine.h"

#include "../Logger.h"
#include "../Physics/PhysicsEngine.h"
#include <btBulletDynamicsCommon.h>
#include "../Game/Player.h"
#include "../Game/GameEngine.h"
#include "../Game.h"
#include "thirdparty/sol2/sol.hpp"

ScriptEngine::ScriptEngine(Game* parent) : Engine(parent) {
    _log = new Logger(Logger::INFO, Logger::STDERR | Logger::LOGFILE, "Logs/ScriptEngine.log");
    _lua = new sol::state();
    _lua->open_libraries(sol::lib::base, sol::lib::table, sol::lib::math);
    _lua->new_usertype<PhysicsEngine>("PhysicsEngine",
        "rayTest", &PhysicsEngine::rayTest
    );
    _lua->new_usertype<GameEngine>("GameEngine",
        "player", sol::readonly_property(&GameEngine::getPlayer)
    );
    _lua->new_usertype<btRigidBody>("RigidBody",
        "linearVelocity", sol::property(&btRigidBody::getLinearVelocity, &btRigidBody::setLinearVelocity),
        "angularVelocity", sol::property(&btRigidBody::getAngularVelocity, &btRigidBody::setAngularVelocity),
        "transform", sol::property((const btTransform& (btRigidBody::*)() const)&btRigidBody::getWorldTransform, &btRigidBody::setWorldTransform),
        "gravity", sol::property(&btRigidBody::getGravity, &btRigidBody::setGravity),
        "applyForce", &btRigidBody::applyForce,
        "applyCentralForce", &btRigidBody::applyCentralForce,
        "applyImpulse", &btRigidBody::applyImpulse,
        "applyCentralImpulse", &btRigidBody::applyCentralImpulse,
        "applyTorque", &btRigidBody::applyTorque,
        "applyTorqueImpulse", &btRigidBody::applyTorqueImpulse
    );
    _lua->new_usertype<PhysicsEngine::RayResult>("RayResult",
        "hasHit", sol::readonly_property([](PhysicsEngine::RayResult& self) { return self.collisionObject != nullptr; }),
        "body", sol::readonly_property([](PhysicsEngine::RayResult& self) { return static_cast<const btRigidBody*>(self.collisionObject); }),
        "hitNormal", &PhysicsEngine::RayResult::hitNormal,
        "hitPoint", &PhysicsEngine::RayResult::hitPoint
    );
    _lua->new_enum("CollisionGroup",
        "nothing", PhysicsEngine::COL_NOTHING,
        "static", PhysicsEngine::COL_STATIC,
        "dynamic", PhysicsEngine::COL_DYNAMIC,
        "player", PhysicsEngine::COL_PLAYER,
        "raytest", PhysicsEngine::COL_RAYTEST,
        "all", PhysicsEngine::COL_ALL,
        "noStatic", PhysicsEngine::COL_NOSTATIC,
        "noDynamic", PhysicsEngine::COL_NODYNAMIC,
        "noPlayer", PhysicsEngine::COL_NOPLAYER,
        "noRaytest", PhysicsEngine::COL_NORAYTEST
    );
    _lua->new_usertype<Player>("Player",
        "body", sol::readonly_property(&Player::getBody)
    );
    _lua->new_usertype<btVector3>("Vec3",
        sol::constructors<sol::types<>, sol::types<const btScalar&, const btScalar&, const btScalar&>>(),
        "x", sol::property(&btVector3::getX, &btVector3::setX),
        "y", sol::property(&btVector3::getY, &btVector3::setY),
        "z", sol::property(&btVector3::getZ, &btVector3::setZ),
        "length", sol::readonly_property(&btVector3::length),
        "length2", sol::readonly_property(&btVector3::length2),
        "normalized", sol::readonly_property(&btVector3::normalized),
        "dot", &btVector3::dot,
        "cross", &btVector3::cross,
        "angle", &btVector3::angle,
        "lerp", &btVector3::lerp,
        "rotate", &btVector3::rotate,
        "__add", static_cast<btVector3 (*)(const btVector3&, const btVector3&)>(&operator+),
        "__sub", static_cast<btVector3 (*)(const btVector3&, const btVector3&)>(&operator-),
        "__mul", sol::overload(
            static_cast<btVector3 (*)(const btVector3&, const btVector3&)>(&operator*),
            static_cast<btVector3 (*)(const btVector3&, const btScalar&)>(&operator*),
            static_cast<btVector3 (*)(const btScalar&, const btVector3&)>(&operator*),
            static_cast<btQuaternion (*)(const btQuaternion&, const btVector3&)>(&operator*),
            static_cast<btQuaternion (*)(const btVector3&, const btQuaternion&)>(&operator*)
        ),
        "__div", sol::overload(
            static_cast<btVector3 (*)(const btVector3&, const btVector3&)>(&operator/),
            static_cast<btVector3 (*)(const btVector3&, const btScalar&)>(&operator/)
        ),
        "__eq", static_cast<bool (btVector3::*)(const btVector3&) const>(&btVector3::operator==)
    );
    _lua->new_usertype<btQuaternion>("Quaternion",
        sol::constructors<sol::types<>, sol::types<const btScalar&, const btScalar&, const btScalar&>, sol::types<const btVector3&, const btScalar&>, sol::types<const btScalar&, const btScalar&, const btScalar&, const btScalar&>>(),
        "length", sol::readonly_property(&btQuaternion::length),
        "length2", sol::readonly_property(&btQuaternion::length2),
        "x", sol::property(&btQuaternion::getX, &btQuaternion::setX),
        "y", sol::property(&btQuaternion::getY, &btQuaternion::setY),
        "z", sol::property(&btQuaternion::getZ, &btQuaternion::setZ),
        "w", sol::property(&btQuaternion::getW, &btQuaternion::setW),
        "normalized", sol::readonly_property(&btQuaternion::normalized),
        "dot", &btQuaternion::dot,
        "angle", sol::readonly_property(&btQuaternion::getAngle),
        "axis", sol::readonly_property(&btQuaternion::getAxis),
        "__add", static_cast<btQuaternion (btQuaternion::*)(const btQuaternion&) const>(&btQuaternion::operator+),
        "__sub", static_cast<btQuaternion (btQuaternion::*)(const btQuaternion&) const>(&btQuaternion::operator-),
        "__mul", sol::overload(
            static_cast<btQuaternion (*)(const btQuaternion&, const btQuaternion&)>(&operator*),
            static_cast<btQuaternion (btQuaternion::*)(const btScalar&) const>(&btQuaternion::operator*)
        ),
        "__div", static_cast<btQuaternion (btQuaternion::*)(const btScalar&) const>(&btQuaternion::operator/)
    );
    _lua->new_usertype<btTransform>("Transform",
        sol::constructors<sol::types<>, sol::types<const btQuaternion&, const btVector3&>>(),
        "origin", sol::property((const btVector3& (btTransform::*)() const)&btTransform::getOrigin, &btTransform::setOrigin),
        "rotation", sol::property(&btTransform::getRotation, &btTransform::setRotation),
        "__mul", sol::overload(
            static_cast<btVector3 (btTransform::*)(const btVector3&) const>(&btTransform::operator*),
            static_cast<btQuaternion (btTransform::*)(const btQuaternion&) const>(&btTransform::operator*),
            static_cast<btTransform (btTransform::*)(const btTransform&) const>(&btTransform::operator*)
        ),
        "__eq", static_cast<bool (*)(const btTransform&, const btTransform&)>(&operator==)
    );

    //class system, from http://lua-users.org/wiki/SimpleLuaClasses
    _lua->script("function class(base, init)\n"
                 "    local c = {}\n"
                 "    if not init and type(base) == 'function' then\n"
                 "        init = base\n"
                 "        base = nil\n"
                 "    elseif type(base) == 'table' then\n"
                 "        for i,v in pairs(base) do\n"
                 "            c[i] = v\n"
                 "        end\n"
                 "        c._base = base\n"
                 "    end\n"
                 "    c.__index = c\n"
                 "    local mt = {}\n"
                 "    mt.__call = function(class_tbl, ...)\n"
                 "        local obj = {}\n"
                 "        setmetatable(obj,c)\n"
                 "        if class_tbl.init then\n"
                 "            class_tbl.init(obj,...)\n"
                 "        else\n"
                 "            if base and base.init then\n"
                 "                base.init(obj, ...)\n"
                 "            end\n"
                 "        end\n"
                 "        return obj\n"
                 "    end\n"
                 "    c.init = init\n"
                 "    c.is_a = function(self, klass)\n"
                 "        local m = getmetatable(self)\n"
                 "        while m do\n"
                 "            if m == klass then return true end\n"
                 "            m = m._base\n"
                 "        end\n"
                 "        return false\n"
                 "    end\n"
                 "    setmetatable(c, mt)\n"
                 "    return c\n"
                 "end\n"
    );

    _lua->script("__scripts = {}\n"
                 "function frame(time)\n"
                 "    for i, v in ipairs(__scripts) do\n"
                 "        v:frame(time)\n"
                 "    end\n"
                 "end\n"
    );
}

ScriptEngine::~ScriptEngine() {
    delete _log;
    delete _lua;
}


void ScriptEngine::init() {
    try {
        (*_lua)["physics"] = _physics;
        (*_lua)["game"] = _game;
    } catch(std::exception& e) {
        _log->fatal("Couldn't set global variables `physics` and `game`: %s", e.what());
    }
}

void ScriptEngine::frame(double time) {
    if(!_pause) {
        try {
            (*_lua)["frame"](time);
        } catch(std::exception& e) {
            _log->fatal("Couldn't step frame: %s", e.what());
        }
    }
}

void ScriptEngine::reset() {
}

void ScriptEngine::changeState(int state) {
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

void ScriptEngine::loadFile(const std::string& file, const std::string& className) {
    _lua->script_file(file);
}

void ScriptEngine::instanciateScript(const std::string& className, const std::string& args, btRigidBody* body) {
    try {
        if(body) {
            (*_lua)["__body"] = body;
        }
        _lua->script("table.insert(__scripts, " + className + "(" + (body ? "__body" : "") + (body && !args.empty() ? ", " : "") + args + "))");
    } catch(std::exception& e) {
        _log->fatal("Couldn't create %s with arguments '%s': %s", className.c_str(), args.c_str(), e.what());
    }
}

