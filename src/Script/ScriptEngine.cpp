#include "ScriptEngine.h"

#include <angelscript.h>
#include "thirdparty/scriptstdstring/scriptstdstring.h"
#include "thirdparty/scriptarray/scriptarray.h"
#include "thirdparty/serializer/serializer.h"
#include "../Logger.h"
#include "../Physics/PhysicsEngine.h"
#include <btBulletDynamicsCommon.h>
#include "../Game/Player.h"

static void btVector3Constructor(btVector3* self) {
    new(self) btVector3();
}

static void btVector3Constructor(const btVector3& other, btVector3* self) {
    new(self) btVector3(other);
}

static void btVector3Constructor(float x, float y, float z, btVector3* self) {
    new(self) btVector3(x, y, z);
}

static void btQuaternionConstructor(btQuaternion* self) {
    new(self) btQuaternion();
}

static void btQuaternionConstructor(const btQuaternion& other, btQuaternion* self) {
    new(self) btQuaternion(other);
}

static void btQuaternionConstructor(float x, float y, float z, float w, btQuaternion* self) {
    new(self) btQuaternion(x, y, z, w);
}

static void btQuaternionConstructor(const btVector3& axis, float angle, btQuaternion* self) {
    new(self) btQuaternion(axis, angle);
}

static void btQuaternionConstructor(float yaw, float pitch, float roll, btQuaternion* self) {
    new(self) btQuaternion(yaw, pitch, roll);
}

static void btTransformConstructor(btTransform* self) {
    new(self) btTransform();
}

static void btTransformConstructor(const btTransform& other, btTransform* self) {
    new(self) btTransform();
}

static void btTransformConstructor(const btVector3& v, const btQuaternion& q, btTransform* self) {
    new(self) btTransform(q, v);
}

static void rayResultConstructor(const PhysicsEngine::RayResult& r, PhysicsEngine::RayResult* self) {
    self->collisionObject = r.collisionObject;
    self->hitNormal = r.hitNormal;
    self->hitPoint = r.hitPoint;
    self->rayFrom = r.rayFrom;
    self->rayTo = r.rayTo;
    self->group = r.group;
    self->mask = r.mask;
}

static btRigidBody* rayResultGetBody(PhysicsEngine::RayResult& r) {
    return (btRigidBody*)r.collisionObject;
}

static void print(const std::string& str) {
    printf("%s", str.c_str());
}

struct StringTypeSerializer : public CUserType {
    void Store(CSerializedValue* val, void* ptr) {
        val->SetUserData(new std::string(*(std::string*)ptr));
    }
    void Restore(CSerializedValue* val, void* ptr) {
        std::string* buffer = (std::string*)val->GetUserData();
        *(std::string*)ptr = *buffer;
    }
    void CleanupUserData(CSerializedValue* val) {
        std::string* buffer = (std::string*)val->GetUserData();
        delete buffer;
    }
};

struct ArrayTypeSerializer : public CUserType {
    void Store(CSerializedValue* val, void* ptr) {
        CScriptArray* arr = (CScriptArray*)ptr;
        for(unsigned int i = 0; i < arr->GetSize(); ++i) {
            val->m_children.push_back(new CSerializedValue(val, "", "", arr->At(i), arr->GetElementTypeId()));
        }
    }
    void Restore(CSerializedValue* val, void* ptr) {
        CScriptArray* arr = (CScriptArray*)ptr;
        arr->Resize(val->m_children.size());
        for(size_t i = 0; i < val->m_children.size(); ++i) {
            val->m_children[i]->Restore(arr->At(i), arr->GetElementTypeId());
        }
    }
};

ScriptEngine::ScriptEngine(Game* parent) : Engine(parent) {
    _log = new Logger(Logger::INFO, Logger::STDERR | Logger::LOGFILE, "Logs/ScriptEngine.log");
    _engine = asCreateScriptEngine(ANGELSCRIPT_VERSION);
    _engine->SetMessageCallback(asMETHOD(ScriptEngine, _messageCallback), this, asCALL_THISCALL);
    RegisterStdString(_engine);
    RegisterScriptArray(_engine, true);

    _engine->RegisterObjectType("PhysicsEngine", sizeof(PhysicsEngine), asOBJ_REF | asOBJ_NOCOUNT);
    _engine->RegisterObjectType("RigidBody", sizeof(btRigidBody), asOBJ_REF | asOBJ_NOCOUNT);
    _engine->RegisterObjectType("RayResult", sizeof(PhysicsEngine::RayResult), asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS_K);
    _engine->RegisterObjectType("Player", sizeof(Player), asOBJ_REF | asOBJ_NOCOUNT);
    _engine->RegisterObjectType("Vector3", sizeof(btVector3), asOBJ_VALUE | asOBJ_APP_CLASS_CK | asOBJ_POD);
    _engine->RegisterObjectType("Quaternion", sizeof(btQuaternion), asOBJ_VALUE | asOBJ_APP_CLASS_CK | asOBJ_POD);
    _engine->RegisterObjectType("Transform", sizeof(btTransform), asOBJ_VALUE | asOBJ_APP_CLASS_CK | asOBJ_POD);

    _engine->RegisterGlobalFunction("void print(const string& in)", asFUNCTION(print), asCALL_CDECL);

    _engine->RegisterObjectMethod("PhysicsEngine", "RayResult rayTest(const Vector3& in, const Vector3& in, int, float)", asMETHOD(PhysicsEngine, rayTest), asCALL_THISCALL);

    _engine->RegisterObjectMethod("RigidBody", "const Vector3& get_linearVelocity() const", asMETHOD(btRigidBody, getLinearVelocity), asCALL_THISCALL);
    _engine->RegisterObjectMethod("RigidBody", "void set_linearVelocity(const Vector3& in)", asMETHOD(btRigidBody, setLinearVelocity), asCALL_THISCALL);
    _engine->RegisterObjectMethod("RigidBody", "const Vector3& get_angularVelocity() const", asMETHOD(btRigidBody, getAngularVelocity), asCALL_THISCALL);
    _engine->RegisterObjectMethod("RigidBody", "void set_angularVelocity(const Vector3& in)", asMETHOD(btRigidBody, setAngularVelocity), asCALL_THISCALL);
    _engine->RegisterObjectMethod("RigidBody", "const Transform& get_transform() const", asMETHODPR(btRigidBody, getWorldTransform, () const, const btTransform&), asCALL_THISCALL);
    _engine->RegisterObjectMethod("RigidBody", "void set_transform(const Transform& in)", asMETHOD(btRigidBody, setWorldTransform), asCALL_THISCALL);
    _engine->RegisterObjectMethod("RigidBody", "const Vector3& get_gravity() const", asMETHOD(btRigidBody, getGravity), asCALL_THISCALL);
    _engine->RegisterObjectMethod("RigidBody", "void set_gravity(const Vector3& in)", asMETHOD(btRigidBody, setGravity), asCALL_THISCALL);
    _engine->RegisterObjectMethod("RigidBody", "void applyForce(const Vector3& in, const Vector3& in)", asMETHOD(btRigidBody, applyForce), asCALL_THISCALL);
    _engine->RegisterObjectMethod("RigidBody", "void applyCentralForce(const Vector3& in)", asMETHOD(btRigidBody, applyCentralForce), asCALL_THISCALL);
    _engine->RegisterObjectMethod("RigidBody", "void applyImpulse(const Vector3& in, const Vector3& in)", asMETHOD(btRigidBody, applyImpulse), asCALL_THISCALL);
    _engine->RegisterObjectMethod("RigidBody", "void applyCentralImpulse(const Vector3& in)", asMETHOD(btRigidBody, applyCentralImpulse), asCALL_THISCALL);
    _engine->RegisterObjectMethod("RigidBody", "void applyTorque(const Vector3& in)", asMETHOD(btRigidBody, applyTorque), asCALL_THISCALL);
    _engine->RegisterObjectMethod("RigidBody", "void applyTorqueImpulse(const Vector3& in)", asMETHOD(btRigidBody, applyTorqueImpulse), asCALL_THISCALL);

    _engine->RegisterObjectBehaviour("RayResult", asBEHAVE_CONSTRUCT, "void f(const RayResult& in)", asFUNCTION(rayResultConstructor), asCALL_CDECL_OBJLAST);
    _engine->RegisterObjectMethod("RayResult", "RigidBody@ get_body()", asFUNCTION(rayResultGetBody), asCALL_CDECL_OBJFIRST);
    _engine->RegisterObjectProperty("RayResult", "Vector3 hitNormal", asOFFSET(PhysicsEngine::RayResult, hitNormal));
    _engine->RegisterObjectProperty("RayResult", "Vector3 hitPoint", asOFFSET(PhysicsEngine::RayResult, hitPoint));
    _engine->RegisterObjectProperty("RayResult", "Vector3 rayFrom", asOFFSET(PhysicsEngine::RayResult, rayFrom));
    _engine->RegisterObjectProperty("RayResult", "Vector3 rayTo", asOFFSET(PhysicsEngine::RayResult, rayTo));
    _engine->RegisterObjectProperty("RayResult", "int16 group", asOFFSET(PhysicsEngine::RayResult, group));
    _engine->RegisterObjectProperty("RayResult", "int16 mask", asOFFSET(PhysicsEngine::RayResult, mask));

    _engine->RegisterObjectMethod("Player", "RigidBody@ get_body()", asMETHOD(Player, getBody), asCALL_THISCALL);

    _engine->RegisterObjectBehaviour("Vector3", asBEHAVE_CONSTRUCT, "void f()", asFUNCTIONPR(btVector3Constructor, (btVector3*), void), asCALL_CDECL_OBJLAST);
    _engine->RegisterObjectBehaviour("Vector3", asBEHAVE_CONSTRUCT, "void f(const Vector3& in)", asFUNCTIONPR(btVector3Constructor, (const btVector3&, btVector3*), void), asCALL_CDECL_OBJLAST);
    _engine->RegisterObjectBehaviour("Vector3", asBEHAVE_CONSTRUCT, "void f(float, float, float)", asFUNCTIONPR(btVector3Constructor, (float, float, float, btVector3*), void), asCALL_CDECL_OBJLAST);
    _engine->RegisterObjectMethod("Vector3", "const float& get_x() const", asMETHOD(btVector3, getX), asCALL_THISCALL);
    _engine->RegisterObjectMethod("Vector3", "void set_x(float)", asMETHOD(btVector3, setX), asCALL_THISCALL);
    _engine->RegisterObjectMethod("Vector3", "const float& get_y() const", asMETHOD(btVector3, getY), asCALL_THISCALL);
    _engine->RegisterObjectMethod("Vector3", "void set_y(float)", asMETHOD(btVector3, setY), asCALL_THISCALL);
    _engine->RegisterObjectMethod("Vector3", "const float& get_z() const", asMETHOD(btVector3, getZ), asCALL_THISCALL);
    _engine->RegisterObjectMethod("Vector3", "void set_z(float)", asMETHOD(btVector3, setZ), asCALL_THISCALL);
    _engine->RegisterObjectMethod("Vector3", "float length() const", asMETHOD(btVector3, length), asCALL_THISCALL);
    _engine->RegisterObjectMethod("Vector3", "float length2() const", asMETHOD(btVector3, length2), asCALL_THISCALL);
    _engine->RegisterObjectMethod("Vector3", "Vector3 normalized() const", asMETHOD(btVector3, normalized), asCALL_THISCALL);
    _engine->RegisterObjectMethod("Vector3", "Vector3& normalize()", asMETHOD(btVector3, normalize), asCALL_THISCALL);
    _engine->RegisterObjectMethod("Vector3", "float dot(const Vector3& in) const", asMETHOD(btVector3, dot), asCALL_THISCALL);
    _engine->RegisterObjectMethod("Vector3", "Vector3 cross(const Vector3& in) const", asMETHOD(btVector3, cross), asCALL_THISCALL);
    _engine->RegisterObjectMethod("Vector3", "float angle(const Vector3& in) const", asMETHOD(btVector3, angle), asCALL_THISCALL);
    _engine->RegisterObjectMethod("Vector3", "Vector3 lerp(const Vector3& in, const float& in) const", asMETHOD(btVector3, lerp), asCALL_THISCALL);
    _engine->RegisterObjectMethod("Vector3", "Vector3 rotate(const Vector3& in, const float) const", asMETHOD(btVector3, rotate), asCALL_THISCALL);
    _engine->RegisterObjectMethod("Vector3", "Vector3 opAdd(const Vector3& in) const", asFUNCTIONPR(operator+, (const btVector3&, const btVector3&), btVector3), asCALL_CDECL_OBJFIRST);
    _engine->RegisterObjectMethod("Vector3", "Vector3 opSub(const Vector3& in) const", asFUNCTIONPR(operator-, (const btVector3&, const btVector3&), btVector3), asCALL_CDECL_OBJFIRST);
    _engine->RegisterObjectMethod("Vector3", "Vector3 opMul(const Vector3& in) const", asFUNCTIONPR(operator*, (const btVector3&, const btVector3&), btVector3), asCALL_CDECL_OBJFIRST);
    _engine->RegisterObjectMethod("Vector3", "Vector3 opMul(const float& in) const", asFUNCTIONPR(operator*, (const btVector3&, const btScalar&), btVector3), asCALL_CDECL_OBJFIRST);
    _engine->RegisterObjectMethod("Vector3", "Quaternion opMul(const Quaternion& in) const", asFUNCTIONPR(operator*, (const btVector3&, const btQuaternion&), btQuaternion), asCALL_CDECL_OBJFIRST);
    _engine->RegisterObjectMethod("Vector3", "Vector3 opDiv(const Vector3& in) const", asFUNCTIONPR(operator/, (const btVector3&, const btVector3&), btVector3), asCALL_CDECL_OBJFIRST);
    _engine->RegisterObjectMethod("Vector3", "Vector3 opDiv(const float& in) const", asFUNCTIONPR(operator/, (const btVector3&, const btScalar&), btVector3), asCALL_CDECL_OBJFIRST);
    _engine->RegisterObjectMethod("Vector3", "bool opEquals(const Vector3& in) const", asMETHOD(btVector3, operator==), asCALL_THISCALL);

    _engine->RegisterObjectBehaviour("Quaternion", asBEHAVE_CONSTRUCT, "void f()", asFUNCTIONPR(btQuaternionConstructor, (btQuaternion*), void), asCALL_CDECL_OBJLAST);
    _engine->RegisterObjectBehaviour("Quaternion", asBEHAVE_CONSTRUCT, "void f(const Quaternion& in)", asFUNCTIONPR(btQuaternionConstructor, (const btQuaternion&, btQuaternion*), void), asCALL_CDECL_OBJLAST);
    _engine->RegisterObjectBehaviour("Quaternion", asBEHAVE_CONSTRUCT, "void f(float, float, float, float)", asFUNCTIONPR(btQuaternionConstructor, (float, float, float, float, btQuaternion*), void), asCALL_CDECL_OBJLAST);
    _engine->RegisterObjectBehaviour("Quaternion", asBEHAVE_CONSTRUCT, "void f(const Vector3& in, float)", asFUNCTIONPR(btQuaternionConstructor, (const btVector3&, float, btQuaternion*), void), asCALL_CDECL_OBJLAST);
    _engine->RegisterObjectBehaviour("Quaternion", asBEHAVE_CONSTRUCT, "void f(float, float, float)", asFUNCTIONPR(btQuaternionConstructor, (float, float, float, btQuaternion*), void), asCALL_CDECL_OBJLAST);
    _engine->RegisterObjectMethod("Quaternion", "const float& get_x()", asMETHOD(btQuaternion, getX), asCALL_THISCALL);
    _engine->RegisterObjectMethod("Quaternion", "void set_x(float)", asMETHOD(btQuaternion, setX), asCALL_THISCALL);
    _engine->RegisterObjectMethod("Quaternion", "const float& get_y()", asMETHOD(btQuaternion, getY), asCALL_THISCALL);
    _engine->RegisterObjectMethod("Quaternion", "void set_y(float)", asMETHOD(btQuaternion, setY), asCALL_THISCALL);
    _engine->RegisterObjectMethod("Quaternion", "const float& get_z()", asMETHOD(btQuaternion, getZ), asCALL_THISCALL);
    _engine->RegisterObjectMethod("Quaternion", "void set_z(float)", asMETHOD(btQuaternion, setZ), asCALL_THISCALL);
    _engine->RegisterObjectMethod("Quaternion", "const float& get_w()", asMETHOD(btQuaternion, getW), asCALL_THISCALL);
    _engine->RegisterObjectMethod("Quaternion", "void set_w(float)", asMETHOD(btQuaternion, setW), asCALL_THISCALL);
    _engine->RegisterObjectMethod("Quaternion", "float length()", asMETHOD(btQuaternion, length), asCALL_THISCALL);
    _engine->RegisterObjectMethod("Quaternion", "float length2()", asMETHOD(btQuaternion, length2), asCALL_THISCALL);
    _engine->RegisterObjectMethod("Quaternion", "Quaternion& normalize()", asMETHOD(btQuaternion, normalize), asCALL_THISCALL);
    _engine->RegisterObjectMethod("Quaternion", "Quaternion normalized()", asMETHOD(btQuaternion, normalized), asCALL_THISCALL);
    _engine->RegisterObjectMethod("Quaternion", "float dot(const Quaternion& in)", asMETHOD(btQuaternion, dot), asCALL_THISCALL);
    _engine->RegisterObjectMethod("Quaternion", "float getAngle()", asMETHOD(btQuaternion, getAngle), asCALL_THISCALL);
    _engine->RegisterObjectMethod("Quaternion", "float angle(const Quaternion& in)", asMETHOD(btQuaternion, angle), asCALL_THISCALL);
    _engine->RegisterObjectMethod("Quaternion", "void setEuler(const float& in, const float& in, const float& in)", asMETHOD(btQuaternion, setEuler), asCALL_THISCALL);
    _engine->RegisterObjectMethod("Quaternion", "void setEulerRotation(const Vector3& in, const float& in)", asMETHOD(btQuaternion, setRotation), asCALL_THISCALL);
    _engine->RegisterObjectMethod("Quaternion", "Quaternion opAdd(const Quaternion& in)", asMETHOD(btQuaternion, operator+), asCALL_THISCALL);
    _engine->RegisterObjectMethod("Quaternion", "Quaternion opSub(const Quaternion& in)", asMETHODPR(btQuaternion, operator-, (const btQuaternion&) const, btQuaternion), asCALL_THISCALL);
    _engine->RegisterObjectMethod("Quaternion", "Quaternion opNeg()", asMETHODPR(btQuaternion, operator-, () const, btQuaternion), asCALL_THISCALL);
    _engine->RegisterObjectMethod("Quaternion", "Quaternion opMul(const Quaternion& in)", asFUNCTIONPR(operator*, (const btQuaternion&, const btQuaternion&), btQuaternion), asCALL_CDECL_OBJFIRST);
    _engine->RegisterObjectMethod("Quaternion", "Quaternion opMul(const Vector3& in)", asFUNCTIONPR(operator*, (const btQuaternion&, const btVector3&), btQuaternion), asCALL_CDECL_OBJFIRST);
    _engine->RegisterObjectMethod("Quaternion", "Quaternion opMul(const float& in)", asMETHOD(btQuaternion, operator*), asCALL_THISCALL);
    _engine->RegisterObjectMethod("Quaternion", "Quaternion opDiv(const float& in)", asMETHOD(btQuaternion, operator/), asCALL_THISCALL);
    _engine->RegisterObjectMethod("Quaternion", "bool opEquals(const Quaternion& in)", asMETHOD(btQuaternion, operator==), asCALL_THISCALL);

    _engine->RegisterObjectBehaviour("Transform", asBEHAVE_CONSTRUCT, "void f()", asFUNCTIONPR(btTransformConstructor, (btTransform*), void), asCALL_CDECL_OBJLAST);
    _engine->RegisterObjectBehaviour("Transform", asBEHAVE_CONSTRUCT, "void f(const Transform& in)", asFUNCTIONPR(btTransformConstructor, (const btTransform&, btTransform*), void), asCALL_CDECL_OBJLAST);
    _engine->RegisterObjectBehaviour("Transform", asBEHAVE_CONSTRUCT, "void f(Vector3& in, Quaternion& in)", asFUNCTIONPR(btTransformConstructor, (const btVector3&, const btQuaternion&, btTransform*), void), asCALL_CDECL_OBJLAST);
    _engine->RegisterObjectMethod("Transform", "const Vector3& get_origin() const", asMETHODPR(btTransform, getOrigin, () const, const btVector3&), asCALL_THISCALL);
    _engine->RegisterObjectMethod("Transform", "void set_origin(const Vector3& in)", asMETHOD(btTransform, setOrigin), asCALL_THISCALL);
    _engine->RegisterObjectMethod("Transform", "Quaternion get_rotation()", asMETHOD(btTransform, getRotation), asCALL_THISCALL);
    _engine->RegisterObjectMethod("Transform", "void set_rotation(const Quaternion& in)", asMETHOD(btTransform, setRotation), asCALL_THISCALL);
    _engine->RegisterObjectMethod("Transform", "Vector3 opMul(const Vector3& in)", asMETHODPR(btTransform, operator*, (const btVector3&) const, btVector3), asCALL_THISCALL);
    _engine->RegisterObjectMethod("Transform", "Quaternion opMul(const Quaternion& in)", asMETHODPR(btTransform, operator*, (const btQuaternion&) const, btQuaternion), asCALL_THISCALL);
    _engine->RegisterObjectMethod("Transform", "Transform opMul(const Transform& in)", asMETHODPR(btTransform, operator*, (const btTransform&) const, btTransform), asCALL_THISCALL);
    _engine->RegisterObjectMethod("Transform", "bool opEquals(const Transform& in)", asFUNCTIONPR(operator==, (const btTransform&, const btTransform&), bool), asCALL_CDECL_OBJFIRST);

    std::string code =
        "interface Script {\n"
        "  void frame(double time);\n"
        "}\n"
        "array<Script@> __scripts__;\n"
        "void __frame__(double time) {\n"
        "  for(uint i = 0; i < __scripts__.length(); ++i) {\n"
        "    __scripts__[i].frame(time);\n"
        "  }\n"
        "}\n"
        "void __addNewScript__(Script@ script) {\n"
        "  __scripts__.insertLast(script);\n"
        "}\n"
    ;

    _module = _engine->GetModule("Default", asGM_ALWAYS_CREATE);
    loadString(code, "__base__");

    _ctx = _engine->CreateContext();
}

ScriptEngine::~ScriptEngine() {
    delete _log;
    _ctx->Release();
    _engine->Release();
}


void ScriptEngine::init() {
}

void ScriptEngine::frame(double time) {
    _ctx->Prepare(_frameFunc);
    _ctx->SetArgDouble(0, time);
    _ctx->Execute();
}

void ScriptEngine::reset() {
}

void ScriptEngine::changeState(int state) {
}

void ScriptEngine::_messageCallback(const asSMessageInfo* msg) {
    if(msg->type == asMSGTYPE_ERROR) {
        _log->error("%s (%d, %d) : %s", msg->section, msg->row, msg->col, msg->message);
    } else if(msg->type == asMSGTYPE_WARNING) {
        _log->warning("%s (%d, %d) : %s", msg->section, msg->row, msg->col, msg->message);
    } else if(msg->type == asMSGTYPE_INFORMATION) {
        _log->info("%s (%d, %d) : %s", msg->section, msg->row, msg->col, msg->message);
    }
}

void ScriptEngine::loadFile(const std::string& file, const std::string& className) {
    std::string s;
    if(FILE* f = fopen(file.c_str(), "r")) {
        char buf[1024];
        while(size_t len = fread(buf, 1, sizeof(buf), f)) {
            s.insert(s.end(), buf, buf + len);
        }
        fclose(f);
    } else {
        _log->error("Cannot read file \"%s\"", file.c_str());
        return;
    }

    loadString(s, className);

    asIObjectType* objType = _engine->GetObjectTypeById(_module->GetTypeIdByDecl(className.c_str()));
    if(!objType) {
        _log->error("Class \"%s\" is not defined");
        return;
    }
}

void ScriptEngine::loadString(const std::string& str, const std::string& section) {
    _classesCode[section] = str;
    _compile();
}

void ScriptEngine::instanciateScript(const std::string& className, btRigidBody* body) {
    asIObjectType* objType = _engine->GetObjectTypeById(_module->GetTypeIdByDecl(className.c_str()));
    if(!objType) {
        _log->error("Class \"%s\" is not defined");
        return;
    }
    asIScriptFunction* factory = objType->GetFactoryByDecl((className + "@ " + className + "(" + (body ? "RigidBody@" : "") + ")").c_str());
    _ctx->Prepare(factory);
    if(body) {
        _ctx->SetArgObject(0, body);
    }
    _ctx->Execute();
    asIScriptObject* obj = (asIScriptObject*)_ctx->GetReturnObject();
    obj->AddRef(); //Increase the reference counter, otherwise obj will be garbage-collected at the next _ctx->Prepare(...)
    _ctx->Prepare(_addFunc);
    _ctx->SetArgObject(0, obj);
    _ctx->Execute();
    obj->Release();
}

int ScriptEngine::_compile() {
    CSerializer backup;
    backup.AddUserType(new StringTypeSerializer(), "string");
    backup.AddUserType(new ArrayTypeSerializer(), "array");
    backup.Store(_module);

    for(auto& it : _classesCode) {
        _module->AddScriptSection(it.first.c_str(), it.second.c_str(), it.second.size());
    }
    int result = _module->Build();

    backup.Restore(_module);
    _addFunc = _module->GetFunctionByDecl("void __addNewScript__(Script@)");
    _frameFunc = _module->GetFunctionByDecl("void __frame__(double)");

    return result;
}
