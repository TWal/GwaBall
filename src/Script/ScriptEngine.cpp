#include "ScriptEngine.h"

/*
#include <angelscript.h>
#include "../Game.h"
#include "thirdparty/angelscript/scriptstdstring/scriptstdstring.h"
#include "thirdparty/angelscript/scriptarray/scriptarray.h"
#include "thirdparty/angelscript/serializer/serializer.h"
#include "thirdparty/angelscript/scriptmath/scriptmath.h"
#include "thirdparty/angelscript/scriptmath/scriptmathcomplex.h"
*/
#include "../Logger.h"
#include "../Physics/PhysicsEngine.h"
#include <btBulletDynamicsCommon.h>
#include "../Game/Player.h"
#include "../Game/GameEngine.h"
/*

struct Vec3Wrapper {
    float x, y, z;
    Vec3Wrapper() : x(0.0), y(0.0), z(0.0) {}
    Vec3Wrapper(const Vec3Wrapper& v) : x(v.x), y(v.y), z(v.z) {}
    Vec3Wrapper(float pX, float pY, float pZ) : x(pX), y(pY), z(pZ) {}
};
static inline btVector3 convert(const Vec3Wrapper& v) { return btVector3(v.x, v.y, v.z); }
static inline Vec3Wrapper convert(const btVector3& v) { return {v.x(), v.y(), v.z()}; }
struct QuatWrapper {
    float x, y, z, w;
    QuatWrapper() : x(0.0), y(0.0), z(0.0), w(0.0) {}
    QuatWrapper(const QuatWrapper& q) : x(q.x), y(q.y), z(q.z), w(q.w) {}
    QuatWrapper(float pX, float pY, float pZ, float pW) : x(pX), y(pY), z(pZ), w(pW) {}
};
static inline btQuaternion convert(const QuatWrapper& q) { return btQuaternion(q.x, q.y, q.z, q.w); }
static inline QuatWrapper convert(const btQuaternion& q) { return {q.x(), q.y(), q.z(), q.w()}; }
struct TransWrapper {
    Vec3Wrapper v;
    QuatWrapper q;
    TransWrapper() : v(), q() {}
    TransWrapper(const TransWrapper& t) : v(t.v), q(t.q) {}
    TransWrapper(const Vec3Wrapper& pV, const QuatWrapper& pQ) : v(pV), q(pQ) {}
};
static inline btTransform convert(const TransWrapper& t) { return btTransform(convert(t.q), convert(t.v)); }
static inline TransWrapper convert(const btTransform& t) { return TransWrapper(convert(t.getOrigin()), convert(t.getRotation())); }
struct RayResultWrapper {
    btRigidBody* body;
    Vec3Wrapper hitNormal;
    Vec3Wrapper hitPoint;
    RayResultWrapper(btRigidBody* b, const Vec3Wrapper& n, const Vec3Wrapper& p) : body(b), hitNormal(n), hitPoint(p) {}
    RayResultWrapper(const RayResultWrapper& r) : body(r.body), hitNormal(r.hitNormal), hitPoint(r.hitPoint) {}
};
static inline RayResultWrapper convert(const PhysicsEngine::RayResult& r) { return RayResultWrapper((btRigidBody*)r.collisionObject, convert(r.hitNormal), convert(r.hitPoint)); }

static void vec3Constructor(Vec3Wrapper* self) { new(self) Vec3Wrapper(); }
static void vec3Constructor(const Vec3Wrapper& other, Vec3Wrapper* self) { new(self) Vec3Wrapper(other); }
static void vec3Constructor(float x, float y, float z, Vec3Wrapper* self) { new(self) Vec3Wrapper(x, y, z); }
static float vec3Length(const Vec3Wrapper& v) { return convert(v).length(); }
static float vec3Length2(const Vec3Wrapper& v) { return convert(v).length2(); }
static Vec3Wrapper vec3Normalized(const Vec3Wrapper& v) { return convert(convert(v).normalized()); }
static float vec3Dot(const Vec3Wrapper& v, const Vec3Wrapper& o) { return convert(v).dot(convert(o)); }
static Vec3Wrapper vec3Cross(const Vec3Wrapper& v, const Vec3Wrapper& o) { return convert(convert(v).cross(convert(o))); }
static float vec3Angle(const Vec3Wrapper& v, const Vec3Wrapper& o) { return convert(v).angle(convert(o)); }
static Vec3Wrapper vec3Lerp(const Vec3Wrapper& v, const Vec3Wrapper& o, float f) { return convert(convert(v).lerp(convert(o), f)); }
static Vec3Wrapper vec3Rotate(const Vec3Wrapper& v, const Vec3Wrapper& o, float f) { return convert(convert(v).rotate(convert(o), f)); }
static Vec3Wrapper vec3OpAdd(const Vec3Wrapper& v, const Vec3Wrapper& o) { return convert(convert(v) + convert(o)); }
static Vec3Wrapper vec3OpSub(const Vec3Wrapper& v, const Vec3Wrapper& o) { return convert(convert(v) - convert(o)); }
static Vec3Wrapper vec3OpMul(const Vec3Wrapper& v, const Vec3Wrapper& o) { return convert(convert(v) * convert(o)); }
static Vec3Wrapper vec3OpMul(const Vec3Wrapper& v, float f) { return convert(convert(v) * f); }
static QuatWrapper vec3OpMul(const Vec3Wrapper& v, const QuatWrapper& q) { return convert(convert(v) * convert(q)); }
static Vec3Wrapper vec3OpDiv(const Vec3Wrapper& v, const Vec3Wrapper& o) { return convert(convert(v) / convert(o)); }
static Vec3Wrapper vec3OpDiv(const Vec3Wrapper& v, float f) { return convert(convert(v) / f); }
static bool vec3OpEquals(const Vec3Wrapper& v, const Vec3Wrapper& o) { return convert(v) == convert(o); }

static void quatConstructor(QuatWrapper* self) { new(self) QuatWrapper(); }
static void quatConstructor(const QuatWrapper& other, QuatWrapper* self) { new(self) QuatWrapper(other); }
static void quatConstructor(float x, float y, float z, float w, QuatWrapper* self) { new(self) QuatWrapper(x, y, z, w); }
static void quatConstructor(const Vec3Wrapper& v, float f, QuatWrapper* self) { new(self) QuatWrapper(convert(btQuaternion(convert(v), f))); }
static void quatConstructor(float y, float p, float r, QuatWrapper* self) { new(self) QuatWrapper(convert(btQuaternion(y, p, r))); }
static float quatLength(const QuatWrapper& q) { return convert(q).length(); }
static float quatLength2(const QuatWrapper& q) { return convert(q).length2(); }
static QuatWrapper quatNormalized(const QuatWrapper& q) { return convert(convert(q).normalized()); }
static float quatDot(const QuatWrapper& q, const QuatWrapper& o) { return convert(q).dot(convert(o)); }
static float quatAngle(const QuatWrapper& q, const QuatWrapper& o) { return convert(q).angle(convert(o)); }
static float quatGetAngle(const QuatWrapper& q) { return convert(q).getAngle(); }
static QuatWrapper quatOpAdd(const QuatWrapper& q, const QuatWrapper& o) { return convert(convert(q) + convert(o)); }
static QuatWrapper quatOpSub(const QuatWrapper& q, const QuatWrapper& o) { return convert(convert(q) - convert(o)); }
static QuatWrapper quatOpNeg(const QuatWrapper& q) { return convert(-convert(q)); }
static QuatWrapper quatOpMul(const QuatWrapper& q, const QuatWrapper& o) { return convert(convert(q) * convert(o)); }
static QuatWrapper quatOpMul(const QuatWrapper& q, const Vec3Wrapper& o) { return convert(convert(q) * convert(o)); }
static QuatWrapper quatOpMul(const QuatWrapper& q, float f) { return convert(convert(q) * f); }
static QuatWrapper quatOpDiv(const QuatWrapper& q, float f) { return convert(convert(q) / f); }
static bool quatOpEquals(const QuatWrapper& q, const QuatWrapper& o) { return convert(q) == convert(o); }

static void transConstructor(TransWrapper* self) { new(self) TransWrapper(); }
static void transConstructor(const TransWrapper& t, TransWrapper* self) { new(self) TransWrapper(t); }
static void transConstructor(const Vec3Wrapper& v, const QuatWrapper& q, TransWrapper* self) { new(self) TransWrapper(v, q); }
static Vec3Wrapper transOpMul(const TransWrapper& t, const Vec3Wrapper& v) { return convert(convert(t) * convert(v)); }
static QuatWrapper transOpMul(const TransWrapper& t, const QuatWrapper& q) { return convert(convert(t) * convert(q)); }
static TransWrapper transOpMul(const TransWrapper& t, const TransWrapper& o) { return convert(convert(t) * convert(o)); }
static bool transOpEquals(const TransWrapper& t, const TransWrapper& o) { return convert(t) == convert(o); }

static Vec3Wrapper rigidBodyGetLinearVel(const btRigidBody* b) { return convert(b->getLinearVelocity()); }
static void rigidBodySetLinearVel(btRigidBody* b, const Vec3Wrapper& v) { b->setLinearVelocity(convert(v)); }
static Vec3Wrapper rigidBodyGetAngularVel(const btRigidBody* b) { return convert(b->getAngularVelocity()); }
static void rigidBodySetAngularVel(btRigidBody* b, const Vec3Wrapper& v) { b->setAngularVelocity(convert(v)); }
static TransWrapper rigidBodyGetTransform(const btRigidBody* b) { return convert(b->getWorldTransform()); }
static void rigidBodySetTransform(btRigidBody* b, const TransWrapper& t) { b->setWorldTransform(convert(t)); }
static Vec3Wrapper rigidBodyGetGravity(btRigidBody* b) { return convert(b->getGravity()); }
static void rigidBodySetGravity(btRigidBody* b, const Vec3Wrapper& v) { b->setGravity(convert(v)); }
static void rigidBodyApplyForce(btRigidBody* b, const Vec3Wrapper& v1, const Vec3Wrapper& v2) { b->applyForce(convert(v1), convert(v2)); }
static void rigidBodyApplyCentralForce(btRigidBody* b, const Vec3Wrapper& v) { b->applyCentralForce(convert(v)); }
static void rigidBodyApplyImpulse(btRigidBody* b, const Vec3Wrapper& v1, const Vec3Wrapper& v2) { b->applyImpulse (convert(v1), convert(v2)); }
static void rigidBodyApplyCentralImpulse(btRigidBody* b, const Vec3Wrapper& v) { b->applyCentralImpulse(convert(v)); }
static void rigidBodyApplyTorque(btRigidBody* b, const Vec3Wrapper& v) { b->applyTorque(convert(v)); }
static void rigidBodyApplyTorqueImpulse(btRigidBody* b, const Vec3Wrapper& v) { b->applyTorqueImpulse(convert(v)); }

static void rayResultConstructor(const RayResultWrapper& r, RayResultWrapper* self) { new(self) RayResultWrapper(r); }

static RayResultWrapper physicsEngineRayTest(PhysicsEngine* phy, const Vec3Wrapper& v1, const Vec3Wrapper& v2, int i, float f) { return convert(phy->rayTest(convert(v1), convert(v2), i, f)); }

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
*/

ScriptEngine::ScriptEngine(Game* parent) : Engine(parent) {
    //_log = new Logger(Logger::INFO, Logger::STDERR | Logger::LOGFILE, "Logs/ScriptEngine.log");
    /*
    _engine = asCreateScriptEngine(ANGELSCRIPT_VERSION);
    _engine->SetMessageCallback(asMETHOD(ScriptEngine, _messageCallback), this, asCALL_THISCALL);
    RegisterStdString(_engine);
    RegisterScriptArray(_engine, true);
    RegisterScriptMath(_engine);
    RegisterScriptMathComplex(_engine);

    _engine->RegisterObjectType("PhysicsEngine", sizeof(PhysicsEngine), asOBJ_REF | asOBJ_NOCOUNT);
    _engine->RegisterObjectType("GameEngine", sizeof(GameEngine), asOBJ_REF | asOBJ_NOCOUNT);
    _engine->RegisterObjectType("RigidBody", sizeof(btRigidBody), asOBJ_REF | asOBJ_NOCOUNT);
    _engine->RegisterObjectType("RayResult", sizeof(RayResultWrapper), asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS_K);
    _engine->RegisterObjectType("Player", sizeof(Player), asOBJ_REF | asOBJ_NOCOUNT);
    _engine->RegisterObjectType("Vec3", sizeof(Vec3Wrapper), asOBJ_VALUE | asOBJ_APP_CLASS_CK | asOBJ_POD);
    _engine->RegisterObjectType("Quaternion", sizeof(QuatWrapper), asOBJ_VALUE | asOBJ_APP_CLASS_CK | asOBJ_POD);
    _engine->RegisterObjectType("Transform", sizeof(btTransform), asOBJ_VALUE | asOBJ_APP_CLASS_CK | asOBJ_POD);

    _engine->RegisterGlobalFunction("void print(const string& in)", asFUNCTION(print), asCALL_CDECL);

    _engine->RegisterObjectMethod("PhysicsEngine", "RayResult rayTest(const Vec3& in, const Vec3& in, int, float)", asFUNCTION(physicsEngineRayTest), asCALL_CDECL_OBJFIRST);

    _engine->RegisterObjectMethod("GameEngine", "Player@ get_player()", asMETHOD(GameEngine, getPlayer), asCALL_THISCALL);

    _engine->RegisterObjectMethod("RigidBody", "Vec3 get_linearVelocity() const", asFUNCTION(rigidBodyGetLinearVel), asCALL_CDECL_OBJFIRST);
    _engine->RegisterObjectMethod("RigidBody", "void set_linearVelocity(const Vec3& in)", asFUNCTION(rigidBodySetLinearVel), asCALL_CDECL_OBJFIRST);
    _engine->RegisterObjectMethod("RigidBody", "Vec3 get_angularVelocity() const", asFUNCTION(rigidBodyGetAngularVel), asCALL_CDECL_OBJFIRST);
    _engine->RegisterObjectMethod("RigidBody", "void set_angularVelocity(const Vec3& in)", asFUNCTION(rigidBodySetAngularVel), asCALL_CDECL_OBJFIRST);
    _engine->RegisterObjectMethod("RigidBody", "Transform get_transform() const", asFUNCTION(rigidBodyGetTransform), asCALL_CDECL_OBJFIRST);
    _engine->RegisterObjectMethod("RigidBody", "void set_transform(const Transform& in)", asFUNCTION(rigidBodySetTransform), asCALL_CDECL_OBJFIRST);
    _engine->RegisterObjectMethod("RigidBody", "Vec3 get_gravity() const", asFUNCTION(rigidBodyGetGravity), asCALL_CDECL_OBJFIRST);
    _engine->RegisterObjectMethod("RigidBody", "void set_gravity(const Vec3& in)", asFUNCTION(rigidBodySetGravity), asCALL_CDECL_OBJFIRST);
    _engine->RegisterObjectMethod("RigidBody", "void applyForce(const Vec3& in, const Vec3& in)", asFUNCTION(rigidBodyApplyForce), asCALL_CDECL_OBJFIRST);
    _engine->RegisterObjectMethod("RigidBody", "void applyCentralForce(const Vec3& in)", asFUNCTION(rigidBodyApplyCentralForce), asCALL_CDECL_OBJFIRST);
    _engine->RegisterObjectMethod("RigidBody", "void applyImpulse(const Vec3& in, const Vec3& in)", asFUNCTION(rigidBodyApplyImpulse), asCALL_CDECL_OBJFIRST);
    _engine->RegisterObjectMethod("RigidBody", "void applyCentralImpulse(const Vec3& in)", asFUNCTION(rigidBodyApplyCentralImpulse), asCALL_CDECL_OBJFIRST);
    _engine->RegisterObjectMethod("RigidBody", "void applyTorque(const Vec3& in)", asFUNCTION(rigidBodyApplyTorque), asCALL_CDECL_OBJFIRST);
    _engine->RegisterObjectMethod("RigidBody", "void applyTorqueImpulse(const Vec3& in)", asFUNCTION(rigidBodyApplyTorqueImpulse), asCALL_CDECL_OBJFIRST);

    _engine->RegisterObjectBehaviour("RayResult", asBEHAVE_CONSTRUCT, "void f(const RayResult& in)", asFUNCTION(rayResultConstructor), asCALL_CDECL_OBJLAST);
    _engine->RegisterObjectProperty("RayResult", "RigidBody@ body", asOFFSET(RayResultWrapper, body));
    _engine->RegisterObjectProperty("RayResult", "Vec3 hitNormal", asOFFSET(RayResultWrapper, hitNormal));
    _engine->RegisterObjectProperty("RayResult", "Vec3 hitPoint", asOFFSET(RayResultWrapper, hitPoint));

    _engine->RegisterObjectMethod("Player", "RigidBody@ get_body()", asMETHOD(Player, getBody), asCALL_THISCALL);

    _engine->RegisterObjectBehaviour("Vec3", asBEHAVE_CONSTRUCT, "void f()", asFUNCTIONPR(vec3Constructor, (Vec3Wrapper*), void), asCALL_CDECL_OBJLAST);
    _engine->RegisterObjectBehaviour("Vec3", asBEHAVE_CONSTRUCT, "void f(const Vec3& in)", asFUNCTIONPR(vec3Constructor, (const Vec3Wrapper&, Vec3Wrapper*), void), asCALL_CDECL_OBJLAST);
    _engine->RegisterObjectBehaviour("Vec3", asBEHAVE_CONSTRUCT, "void f(float, float, float)", asFUNCTIONPR(vec3Constructor, (float, float, float, Vec3Wrapper*), void), asCALL_CDECL_OBJLAST);
    _engine->RegisterObjectProperty("Vec3", "float x", asOFFSET(Vec3Wrapper, x));
    _engine->RegisterObjectProperty("Vec3", "float y", asOFFSET(Vec3Wrapper, y));
    _engine->RegisterObjectProperty("Vec3", "float z", asOFFSET(Vec3Wrapper, z));
    _engine->RegisterObjectMethod("Vec3", "float length() const", asFUNCTION(vec3Length), asCALL_CDECL_OBJFIRST);
    _engine->RegisterObjectMethod("Vec3", "float length2() const", asFUNCTION(vec3Length2), asCALL_CDECL_OBJFIRST);
    _engine->RegisterObjectMethod("Vec3", "Vec3 normalized() const", asFUNCTION(vec3Normalized), asCALL_CDECL_OBJFIRST);
    _engine->RegisterObjectMethod("Vec3", "float dot(const Vec3& in) const", asFUNCTION(vec3Dot), asCALL_CDECL_OBJFIRST);
    _engine->RegisterObjectMethod("Vec3", "Vec3 cross(const Vec3& in) const", asFUNCTION(vec3Cross), asCALL_CDECL_OBJFIRST);
    _engine->RegisterObjectMethod("Vec3", "float angle(const Vec3& in) const", asFUNCTION(vec3Angle), asCALL_CDECL_OBJFIRST);
    _engine->RegisterObjectMethod("Vec3", "Vec3 lerp(const Vec3& in, float) const", asFUNCTION(vec3Lerp), asCALL_CDECL_OBJFIRST);
    _engine->RegisterObjectMethod("Vec3", "Vec3 rotate(const Vec3& in, float) const", asFUNCTION(vec3Rotate), asCALL_CDECL_OBJFIRST);
    _engine->RegisterObjectMethod("Vec3", "Vec3 opAdd(const Vec3& in) const", asFUNCTIONPR(vec3OpAdd, (const Vec3Wrapper&, const Vec3Wrapper&), Vec3Wrapper), asCALL_CDECL_OBJFIRST);
    _engine->RegisterObjectMethod("Vec3", "Vec3 opSub(const Vec3& in) const", asFUNCTIONPR(vec3OpSub, (const Vec3Wrapper&, const Vec3Wrapper&), Vec3Wrapper), asCALL_CDECL_OBJFIRST);
    _engine->RegisterObjectMethod("Vec3", "Vec3 opMul(const Vec3& in) const", asFUNCTIONPR(vec3OpMul, (const Vec3Wrapper&, const Vec3Wrapper&), Vec3Wrapper), asCALL_CDECL_OBJFIRST);
    _engine->RegisterObjectMethod("Vec3", "Vec3 opMul(float) const", asFUNCTIONPR(vec3OpMul, (const Vec3Wrapper&, float), Vec3Wrapper), asCALL_CDECL_OBJFIRST);
    _engine->RegisterObjectMethod("Vec3", "Quaternion opMul(const Quaternion& in) const", asFUNCTIONPR(vec3OpMul, (const Vec3Wrapper&, const QuatWrapper&), QuatWrapper), asCALL_CDECL_OBJFIRST);
    _engine->RegisterObjectMethod("Vec3", "Vec3 opDiv(const Vec3& in) const", asFUNCTIONPR(vec3OpDiv, (const Vec3Wrapper&, const Vec3Wrapper&), Vec3Wrapper), asCALL_CDECL_OBJFIRST);
    _engine->RegisterObjectMethod("Vec3", "Vec3 opDiv(float) const", asFUNCTIONPR(vec3OpDiv, (const Vec3Wrapper&, float), Vec3Wrapper), asCALL_CDECL_OBJFIRST);
    _engine->RegisterObjectMethod("Vec3", "bool opEquals(const Vec3& in) const", asFUNCTION(vec3OpEquals), asCALL_CDECL_OBJFIRST);

    _engine->RegisterObjectBehaviour("Quaternion", asBEHAVE_CONSTRUCT, "void f()", asFUNCTIONPR(quatConstructor, (QuatWrapper*), void), asCALL_CDECL_OBJLAST);
    _engine->RegisterObjectBehaviour("Quaternion", asBEHAVE_CONSTRUCT, "void f(const Quaternion& in)", asFUNCTIONPR(quatConstructor, (const QuatWrapper&, QuatWrapper*), void), asCALL_CDECL_OBJLAST);
    _engine->RegisterObjectBehaviour("Quaternion", asBEHAVE_CONSTRUCT, "void f(float, float, float, float)", asFUNCTIONPR(quatConstructor, (float, float, float, float, QuatWrapper*), void), asCALL_CDECL_OBJLAST);
    _engine->RegisterObjectBehaviour("Quaternion", asBEHAVE_CONSTRUCT, "void f(const Vec3& in, float)", asFUNCTIONPR(quatConstructor, (const Vec3Wrapper&, float, QuatWrapper*), void), asCALL_CDECL_OBJLAST);
    _engine->RegisterObjectBehaviour("Quaternion", asBEHAVE_CONSTRUCT, "void f(float, float, float)", asFUNCTIONPR(quatConstructor, (float, float, float, QuatWrapper*), void), asCALL_CDECL_OBJLAST);
    _engine->RegisterObjectProperty("Quaternion", "float x", asOFFSET(QuatWrapper, x));
    _engine->RegisterObjectProperty("Quaternion", "float y", asOFFSET(QuatWrapper, y));
    _engine->RegisterObjectProperty("Quaternion", "float z", asOFFSET(QuatWrapper, z));
    _engine->RegisterObjectProperty("Quaternion", "float w", asOFFSET(QuatWrapper, x));
    _engine->RegisterObjectMethod("Quaternion", "float length() const", asFUNCTION(quatLength), asCALL_CDECL_OBJFIRST);
    _engine->RegisterObjectMethod("Quaternion", "float length2() const", asFUNCTION(quatLength2), asCALL_CDECL_OBJFIRST);
    _engine->RegisterObjectMethod("Quaternion", "Quaternion normalized() const", asFUNCTION(quatNormalized), asCALL_CDECL_OBJFIRST);
    _engine->RegisterObjectMethod("Quaternion", "float dot(const Quaternion& in) const", asFUNCTION(quatDot), asCALL_CDECL_OBJFIRST);
    _engine->RegisterObjectMethod("Quaternion", "float getAngle() const", asFUNCTION(quatGetAngle), asCALL_CDECL_OBJFIRST);
    _engine->RegisterObjectMethod("Quaternion", "float angle(const Quaternion& in) const", asFUNCTION(quatAngle), asCALL_CDECL_OBJFIRST);
    _engine->RegisterObjectMethod("Quaternion", "Quaternion opAdd(const Quaternion& in) const", asFUNCTION(quatOpAdd), asCALL_CDECL_OBJFIRST);
    _engine->RegisterObjectMethod("Quaternion", "Quaternion opSub(const Quaternion& in) const", asFUNCTION(quatOpSub), asCALL_CDECL_OBJFIRST);
    _engine->RegisterObjectMethod("Quaternion", "Quaternion opNeg() const", asFUNCTION(quatOpNeg), asCALL_CDECL_OBJFIRST);
    _engine->RegisterObjectMethod("Quaternion", "Quaternion opMul(const Quaternion& in) const", asFUNCTIONPR(quatOpMul, (const QuatWrapper&, const QuatWrapper&), QuatWrapper), asCALL_CDECL_OBJFIRST);
    _engine->RegisterObjectMethod("Quaternion", "Quaternion opMul(const Vec3& in) const", asFUNCTIONPR(quatOpMul, (const QuatWrapper&, const Vec3Wrapper&), QuatWrapper), asCALL_CDECL_OBJFIRST);
    _engine->RegisterObjectMethod("Quaternion", "Quaternion opMul(float) const", asFUNCTIONPR(quatOpMul, (const QuatWrapper&, float), QuatWrapper), asCALL_CDECL_OBJFIRST);
    _engine->RegisterObjectMethod("Quaternion", "Quaternion opDiv(float) const", asFUNCTION(quatOpDiv), asCALL_CDECL_OBJFIRST);
    _engine->RegisterObjectMethod("Quaternion", "bool opEquals(const Quaternion& in) const", asFUNCTION(quatOpEquals), asCALL_CDECL_OBJFIRST);

    _engine->RegisterObjectBehaviour("Transform", asBEHAVE_CONSTRUCT, "void f()", asFUNCTIONPR(transConstructor, (TransWrapper*), void), asCALL_CDECL_OBJLAST);
    _engine->RegisterObjectBehaviour("Transform", asBEHAVE_CONSTRUCT, "void f(const Transform& in)", asFUNCTIONPR(transConstructor, (const TransWrapper&, TransWrapper*), void), asCALL_CDECL_OBJLAST);
    _engine->RegisterObjectBehaviour("Transform", asBEHAVE_CONSTRUCT, "void f(Vec3& in, Quaternion& in)", asFUNCTIONPR(transConstructor, (const Vec3Wrapper&, const QuatWrapper&, TransWrapper*), void), asCALL_CDECL_OBJLAST);
    _engine->RegisterObjectProperty("Transform", "Vec3 pos", asOFFSET(TransWrapper, v));
    _engine->RegisterObjectProperty("Transform", "Quaternion rot", asOFFSET(TransWrapper, q));
    _engine->RegisterObjectMethod("Transform", "Vec3 opMul(const Vec3& in) const", asFUNCTIONPR(transOpMul, (const TransWrapper&, const Vec3Wrapper&), Vec3Wrapper), asCALL_CDECL_OBJFIRST);
    _engine->RegisterObjectMethod("Transform", "Quaternion opMul(const Quaternion& in) const", asFUNCTIONPR(transOpMul, (const TransWrapper&, const QuatWrapper&), QuatWrapper), asCALL_CDECL_OBJFIRST);
    _engine->RegisterObjectMethod("Transform", "Transform opMul(const Transform& in) const", asFUNCTIONPR(transOpMul, (const TransWrapper&, const TransWrapper&), TransWrapper), asCALL_CDECL_OBJFIRST);
    _engine->RegisterObjectMethod("Transform", "bool opEquals(const Transform& in) const", asFUNCTION(transOpEquals), asCALL_CDECL_OBJFIRST);

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
    */
}

ScriptEngine::~ScriptEngine() {
    delete _log;
    //_ctx->Release();
    //_engine->Release();
}


void ScriptEngine::init() {
    //_engine->RegisterGlobalProperty("PhysicsEngine@ physics", &_physics);
    //_engine->RegisterGlobalProperty("GameEngine@ game", &_game);
}

void ScriptEngine::frame(double time) {
    if(!_pause) {
        //_ctx->Prepare(_frameFunc);
        //_ctx->SetArgDouble(0, time);
        //_ctx->Execute();
    }
}

void ScriptEngine::reset() {
}

void ScriptEngine::changeState(int state) {
    /*
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
    */
}

/*
void ScriptEngine::_messageCallback(const asSMessageInfo* msg) {
    if(msg->type == asMSGTYPE_ERROR) {
        _log->error("%s (%d, %d) : %s", msg->section, msg->row, msg->col, msg->message);
    } else if(msg->type == asMSGTYPE_WARNING) {
        _log->warning("%s (%d, %d) : %s", msg->section, msg->row, msg->col, msg->message);
    } else if(msg->type == asMSGTYPE_INFORMATION) {
        _log->info("%s (%d, %d) : %s", msg->section, msg->row, msg->col, msg->message);
    }
}
*/

void ScriptEngine::loadFile(const std::string& file, const std::string& className) {
    /*
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
    */
}

void ScriptEngine::loadString(const std::string& str, const std::string& section) {
    /*
    _classesCode[section] = str;
    _compile();
    */
}

void ScriptEngine::instanciateScript(const std::string& className, btRigidBody* body) {
    /*
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
    */
}

int ScriptEngine::_compile() {
    /*
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
    */
}
