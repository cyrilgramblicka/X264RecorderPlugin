
#ifndef __OT_GLM_META_V8_H__
#define __OT_GLM_META_V8_H__

#include "glm_meta.h"
#include <comm/metastream/fmtstream_v8.h>

namespace coid {

#define V8_STREAMER_VEC(T,V8T,CT) \
    template<> class v8_streamer<T##4> {\
    public:\
    static v8::Handle<v8::Object> to_v8(const T##4& v) {\
        v8::Handle<v8::Object> obj = v8::Object::New();\
        obj->Set(v8::String::NewSymbol("x",1), v8::V8T::New(CT(v.x)));\
        obj->Set(v8::String::NewSymbol("y",1), v8::V8T::New(CT(v.y)));\
        obj->Set(v8::String::NewSymbol("z",1), v8::V8T::New(CT(v.z)));\
        obj->Set(v8::String::NewSymbol("w",1), v8::V8T::New(CT(v.w)));\
        return obj;\
    }\
    static bool from_v8( v8::Handle<v8::Value> src, T##4& r ) {\
        if(!src->IsObject()) return false;\
        v8::Local<v8::Object> v = src->ToObject();\
        v8::Handle<v8::Value> vx = v->Get(v8::String::NewSymbol("x",1)); r.x = vx->IsUndefined() ? T(0) : T(vx->V8T##Value());\
        v8::Handle<v8::Value> vy = v->Get(v8::String::NewSymbol("y",1)); r.y = vy->IsUndefined() ? T(0) : T(vy->V8T##Value());\
        v8::Handle<v8::Value> vz = v->Get(v8::String::NewSymbol("z",1)); r.z = vz->IsUndefined() ? T(0) : T(vz->V8T##Value());\
        v8::Handle<v8::Value> vw = v->Get(v8::String::NewSymbol("w",1)); r.w = vw->IsUndefined() ? T(0) : T(vw->V8T##Value());\
        return true;\
    }\
};\
    template<> class v8_streamer<T##3> {\
    public:\
    static v8::Handle<v8::Object> to_v8(const T##3& v) {\
        v8::Handle<v8::Object> obj = v8::Object::New();\
        obj->Set(v8::String::NewSymbol("x",1), v8::V8T::New(CT(v.x)));\
        obj->Set(v8::String::NewSymbol("y",1), v8::V8T::New(CT(v.y)));\
        obj->Set(v8::String::NewSymbol("z",1), v8::V8T::New(CT(v.z)));\
        return obj;\
    }\
    static bool from_v8( v8::Handle<v8::Value> src, T##3& r ) {\
        if(!src->IsObject()) return false;\
        v8::Local<v8::Object> v = src->ToObject();\
        v8::Handle<v8::Value> vx = v->Get(v8::String::NewSymbol("x",1)); r.x = vx->IsUndefined() ? T(0) : T(vx->V8T##Value());\
        v8::Handle<v8::Value> vy = v->Get(v8::String::NewSymbol("y",1)); r.y = vy->IsUndefined() ? T(0) : T(vy->V8T##Value());\
        v8::Handle<v8::Value> vz = v->Get(v8::String::NewSymbol("z",1)); r.z = vz->IsUndefined() ? T(0) : T(vz->V8T##Value());\
        return true;\
    }\
};\
    template<> class v8_streamer<T##2> {\
    public:\
    static v8::Handle<v8::Object> to_v8(const T##2& v) {\
        v8::Handle<v8::Object> obj = v8::Object::New();\
        obj->Set(v8::String::NewSymbol("x",1), v8::V8T::New(CT(v.x)));\
        obj->Set(v8::String::NewSymbol("y",1), v8::V8T::New(CT(v.y)));\
        return obj;\
    }\
    static bool from_v8( v8::Handle<v8::Value> src, T##2& r ) {\
        if(!src->IsObject()) return false;\
        v8::Local<v8::Object> v = src->ToObject();\
        v8::Handle<v8::Value> vx = v->Get(v8::String::NewSymbol("x",1)); r.x = vx->IsUndefined() ? T(0) : T(vx->V8T##Value());\
        v8::Handle<v8::Value> vy = v->Get(v8::String::NewSymbol("y",1)); r.y = vy->IsUndefined() ? T(0) : T(vy->V8T##Value());\
        return true;\
    }\
};


V8_STREAMER_VEC(double, Number, double);
V8_STREAMER_VEC(float, Number, double);

V8_STREAMER_VEC(int, Int32, int);
V8_STREAMER_VEC(short, Int32, int);
V8_STREAMER_VEC(schar, Int32, int);

V8_STREAMER_VEC(uint, Uint32, uint);
V8_STREAMER_VEC(ushort, Uint32, uint);
V8_STREAMER_VEC(uchar, Uint32, uint);

} //namespace coid

#endif //__OT_GLM_META_V8_H__
