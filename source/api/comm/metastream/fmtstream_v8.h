/* ***** BEGIN LICENSE BLOCK *****
 * Version: MPL 1.1/GPL 2.0/LGPL 2.1
 *
 * The contents of this file are subject to the Mozilla Public License Version
 * 1.1 (the "License"); you may not use this file except in compliance with
 * the License. You may obtain a copy of the License at
 * http://www.mozilla.org/MPL/
 *
 * Software distributed under the License is distributed on an "AS IS" basis,
 * WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
 * for the specific language governing rights and limitations under the
 * License.
 *
 * The Original Code is COID/comm module.
 *
 * The Initial Developer of the Original Code is
 * Outerra
 * Portions created by the Initial Developer are Copyright (C) 2003
 * the Initial Developer. All Rights Reserved.
 *
 * Contributor(s):
 * Brano Kemen
 *
 * Alternatively, the contents of this file may be used under the terms of
 * either the GNU General Public License Version 2 or later (the "GPL"), or
 * the GNU Lesser General Public License Version 2.1 or later (the "LGPL"),
 * in which case the provisions of the GPL or the LGPL are applicable instead
 * of those above. If you wish to allow use of your version of this file only
 * under the terms of either the GPL or the LGPL, and not to allow others to
 * use your version of this file under the terms of the MPL, indicate your
 * decision by deleting the provisions above and replace them with the notice
 * and other provisions required by the GPL or the LGPL. If you do not delete
 * the provisions above, a recipient may use your version of this file under
 * the terms of any one of the MPL, the GPL or the LGPL.
 *
 * ***** END LICENSE BLOCK ***** */ 
/*
 * Copyright (C) 2012 Outerra. All Rights Reserved.
 */

#ifndef __COID_COMM_FMTSTREAM_V8__HEADER_FILE__
#define __COID_COMM_FMTSTREAM_V8__HEADER_FILE__

#include "fmtstream.h"
#include <v8/v8.h>

#include "metastream.h"
#include "../str.h"

//@file formatting stream for V8 javascript engine

COID_NAMESPACE_BEGIN

////////////////////////////////////////////////////////////////////////////////
class fmtstream_v8;

///Helper class to stream types to/from v8 objects
template<class T>
class v8_streamer
{
public:
    ///Generic generator from a type to its v8 representation
    static v8::Handle<v8::Value> to_v8(const T& v);

    ///Generic parser from v8 type representation
    static bool from_v8( v8::Handle<v8::Value> src, T& res );
};

//@{ Fast v8_streamer specializations for base types

#define V8_STREAMER(T,V8T,CT) \
template<> class v8_streamer<T> { public: \
    static v8::Handle<v8::Value> to_v8(const T& v) { return v8::V8T::New(CT(v)); } \
    static bool from_v8( v8::Handle<v8::Value> src, T& res ) { res = (T)src->V8T##Value(); return true; } \
}

V8_STREAMER(int8,  Int32, int32);
V8_STREAMER(int16, Int32, int32);
V8_STREAMER(int32, Int32, int32);
V8_STREAMER(int64, Number, double);     //can lose data in conversion

V8_STREAMER(uint8,  Uint32, uint32);
V8_STREAMER(uint16, Uint32, uint32);
V8_STREAMER(uint32, Uint32, uint32);
V8_STREAMER(uint64, Number, double);    //can lose data in conversion

#ifdef SYSTYPE_WIN
# ifdef SYSTYPE_32
V8_STREAMER(ints,  Int32,  int32);
V8_STREAMER(uints, Uint32, uint32);
# else //SYSTYPE_64
V8_STREAMER(int,   Number, double);
V8_STREAMER(uint,  Number, double);
# endif
#elif defined(SYSTYPE_32)
V8_STREAMER(long,  Int32,  int32);
V8_STREAMER(ulong, Uint32, uint32);
#endif

V8_STREAMER(float,  Number, double);
V8_STREAMER(double, Number, double);

V8_STREAMER(bool, Boolean, bool);

///Date/time
template<> class v8_streamer<timet> {
public:
    static v8::Handle<v8::Value> to_v8( const timet& v ) {
        return v8::Date::New(double(v.t));
    }
    
    static bool from_v8( v8::Handle<v8::Value> src, timet& res ) {
        res = timet((int64)src->NumberValue());
        return true;
    }
};

///Strings
template<> class v8_streamer<charstr> {
public:
    static v8::Handle<v8::Value> to_v8(const charstr& v) {
        return v8::String::New(v.ptr(), v.len());
    }
    
    static v8::Handle<v8::Value> to_v8(const token& v) {
        return v8::String::New(v.ptr(), v.len());
    }
    
    static v8::Handle<v8::Value> to_v8(const char* v) {
        return v8::String::New(v);
    }
    
    static bool from_v8( v8::Handle<v8::Value> src, charstr& res ) {
        if(src->IsUndefined() || src->IsNull())
            return false;
        v8::String::Utf8Value str(src);
        res.set_from(*str, str.length());
        return true;
    }
};

template<> class v8_streamer<token> {
public:
    static v8::Handle<v8::Value> to_v8(const token& v) {
        return v8::String::New(v.ptr(), v.len());
    }
};

///Helper to fill dynarray from V8 array
template<class T>
inline bool v8_write_dynarray( v8::Handle<v8::Value> src, dynarray<T>& a )
{
    if(!src->IsArray())
        return false;

    v8::Local<v8::Object> obj = src->ToObject();
    uint n = obj->Get(v8::String::NewSymbol("length"))->Uint32Value();
    a.alloc(n);

    for(uint i=0; i<n; ++i) {
        v8_streamer<T>::from_v8(obj->Get(i), a[i]);
    }

    return true;
}

///Generic dynarray<T> partial specialization
template<class T> class v8_streamer<dynarray<T>> {
public:
    static v8::Handle<v8::Value> to_v8(const dynarray<T>& v) {
        uint n = (uint)v.size();
        v8::Local<v8::Array> a = v8::Array::New(n);
        
        for(uint i=0; i<n; ++i) {
            a->Set(i, v8_streamer<T>::to_v8(v[i]));
        }

        return a;
    }
    
    static bool from_v8( v8::Handle<v8::Value> src, dynarray<T>& res ) {
        return v8_write_dynarray(src, res);
    }
};



///Helper class to stream types to/from v8 objects for volatile data (ifc_volatile)
template<class T>
class v8_streamer_volatile : public v8_streamer<T>
{
public:
    ///Generic generator from a type to its v8 representation
    static v8::Handle<v8::Value> to_v8(const T& v) {
        return v8_streamer<T>::to_v8(v);
    }

    ///Generic parser from v8 type representation
    static bool from_v8( v8::Handle<v8::Value> src, T& res ) {
        return v8_streamer<T>::from_v8(src, res);
    }

    static void cleanup( v8::Handle<v8::Value> val ) {}
};

///Helper to map typed array from C++ to V8
template<class T>
inline v8::Handle<v8::Value> v8_map_typed_array( const T* ptr, uint count, v8::ExternalArrayType type )
{
    v8::Handle<v8::Object> a = v8::Object::New();
    a->SetIndexedPropertiesToExternalArrayData((void*)ptr, type, count);
    a->Set(v8::String::NewSymbol("length", 6), v8::Uint32::NewFromUnsigned(count));

    return a;
}

///Macro for direct array mapping to V8
#define V8_STREAMER_MAPARRAY(T, V8EXT) \
template<> class v8_streamer_volatile<dynarray<T>> : public v8_streamer<dynarray<T>> {\
public:\
    static v8::Handle<v8::Value> to_v8(const dynarray<T>& v) { \
        return v8_map_typed_array(v.ptr(), (uint)v.size(), V8EXT); \
    } \
 \
    static bool from_v8( v8::Handle<v8::Value> src, dynarray<T>& res ) { \
        return v8_write_dynarray(src, res); \
    } \
    static void cleanup( v8::Handle<v8::Value> val ) { \
        val->ToObject()->SetIndexedPropertiesToExternalArrayData(0, V8EXT, 0); \
    } \
};

V8_STREAMER_MAPARRAY(int8, v8::kExternalByteArray)
V8_STREAMER_MAPARRAY(uint8, v8::kExternalUnsignedByteArray)
V8_STREAMER_MAPARRAY(int16, v8::kExternalShortArray)
V8_STREAMER_MAPARRAY(uint16, v8::kExternalUnsignedShortArray)
V8_STREAMER_MAPARRAY(int32, v8::kExternalIntArray)
V8_STREAMER_MAPARRAY(uint32, v8::kExternalUnsignedIntArray)
V8_STREAMER_MAPARRAY(float, v8::kExternalFloatArray)
V8_STREAMER_MAPARRAY(double, v8::kExternalDoubleArray)

#ifdef SYSTYPE_WIN
# ifdef SYSTYPE_32
V8_STREAMER_MAPARRAY(ints, v8::kExternalIntArray)
V8_STREAMER_MAPARRAY(uints, v8::kExternalUnsignedIntArray)
# else //SYSTYPE_64
V8_STREAMER_MAPARRAY(int, v8::kExternalIntArray)
V8_STREAMER_MAPARRAY(uint, v8::kExternalUnsignedIntArray)
# endif
#elif defined(SYSTYPE_32)
V8_STREAMER_MAPARRAY(long, v8::kExternalIntArray)
V8_STREAMER_MAPARRAY(ulong, v8::kExternalUnsignedIntArray)
#endif



///V8 values
template<typename V> class v8_streamer<v8::Handle<V>> { public: \
    typedef v8::Handle<V> T; \
    static v8::Handle<v8::Value> to_v8(T v) { return v; } \
    static bool from_v8( v8::Handle<v8::Value> src, T& res ) { res = src.Cast<T>(src); return true; } \
};

//dummy metastream operator for v8::Handle
template<typename V>
inline metastream& operator || (metastream& m, v8::Handle<V>) { RASSERT(0); return m; }

//@}


////////////////////////////////////////////////////////////////////////////////
///token wrapper for v8
class v8_token : public v8::String::ExternalAsciiStringResource
{
    token _tok;

public:

    ~v8_token() {}

    v8_token()
    {}
    v8_token( const token& tok ) : _tok(tok)
    {}

    void set( const token& tok ) {
        _tok = tok;
    }

    const char* data() const { return _tok.ptr(); }

    size_t length() const { return _tok.len(); }

protected:

    void Dispose() {}
};

////////////////////////////////////////////////////////////////////////////////
class fmtstream_v8 : public fmtstream
{
protected:

    ///Stack entry
    struct entry
    {
        v8::Local<v8::Object> object;
        v8::Handle<v8::Value> value;
        v8::Local<v8::Array> array;

        uint element;
        token key;

        entry() : element(0)
        {}
    };

    dynarray<entry> _stack;
    entry* _top;

public:
    fmtstream_v8()
    {
        _stack.reserve(32, true);

        _top = _stack.alloc(1);
        _top->element = 0;
    }

    ~fmtstream_v8() {}


    ///Set v8 Value to read from
    void set(v8::Handle<v8::Value> val) {
        _top = _stack.alloc(1);
        _top->value = val;
        
        if(val->IsObject())
            _top->object = val->ToObject();
    }

    ///Get a v8 Value result from streaming
    v8::Handle<v8::Value> get()
    {
        DASSERT(_stack.size() == 1);
        if(!_top) return v8::Null();

        v8::Handle<v8::Value> v = _top->value;
        _top = _stack.alloc(1);
        _top->element = 0;

        return v;
    }


    virtual token fmtstream_name() { return "fmtstream_v8"; }

    virtual void fmtstream_file_name( const token& file_name )
    {
        //_tokenizer.set_file_name(file_name);
    }

    ///Return formatting stream error (if any) and current line and column for error reporting purposes
    //@param err [in] error text
    //@param err [out] final (formatted) error text with line info etc.
    virtual void fmtstream_err( charstr& err )
    {
    }

    void acknowledge( bool eat=false )
    {
        _top = _stack.alloc(1);
        _top->element = 0;
    }

    void flush()
    {
        _top = _stack.alloc(1);
        _top->element = 0;
    }


    ////////////////////////////////////////////////////////////////////////////////
    opcd write_key( const token& key, int kmember )
    {
        if(kmember > 0) {
            //attach the previous property
            _top->object->Set(v8::String::New(_top->key.ptr(), _top->key.len()), _top->value);
        }

        _top->key = key;
        return 0;
    }

    opcd read_key( charstr& key, int kmember, const token& expected_key )
    {
        if(_top->object.IsEmpty())
            return ersNO_MORE;

        //looks up the variable in the current object
        _top->value = _top->object->Get(v8::String::NewSymbol(expected_key.ptr(), expected_key.len()));

        if(_top->value->IsUndefined())
            return ersNO_MORE;

        key = expected_key;
        return 0;
    }

    ////////////////////////////////////////////////////////////////////////////////
    opcd write( const void* p, type t )
    {
        if( t.is_array_start() )
        {
            /*if( t.type == type::T_BINARY )
                DASSERT(0);
            else if( t.type == type::T_CHAR )
                //_bufw << "\\\"";
                ;//postpone writing until the escape status is known
            else
                _bufw << char('[');*/
            if(t.type != type::T_BINARY && t.type != type::T_CHAR) {
                _top->array = v8::Array::New((int)t.get_count(p));
                _top->element = 0;
            }
        }
        else if( t.is_array_end() )
        {
            if(t.type != type::T_BINARY && t.type != type::T_CHAR)
                _top->value = _top->array;
        }
        else if( t.type == type::T_STRUCTEND )
        {
            //attach the last property
            if(_top->key)
                _top->object->Set(v8::String::New(_top->key.ptr(), _top->key.len()), _top->value);

            DASSERT(_stack.size() > 1);
            _top[-1].value = _top->object;
            _top = _stack.pop();
        }
        else if( t.type == type::T_STRUCTBGN )
        {
            _top = _stack.push();
            _top->object = v8::Object::New();
        }
        else
        {
            switch( t.type )
            {
                case type::T_INT:
                    switch( t.get_size() )
                    {
                    case 1: _top->value = v8_streamer<int8>() .to_v8(*(int8*)p); break;
                    case 2: _top->value = v8_streamer<int16>().to_v8(*(int16*)p); break;
                    case 4: _top->value = v8_streamer<int32>().to_v8(*(int32*)p); break;
                    case 8: _top->value = v8_streamer<int64>().to_v8(*(int64*)p); break;
                    }
                    break;

                case type::T_UINT:
                    switch( t.get_size() )
                    {
                    case 1: _top->value = v8_streamer<uint8>() .to_v8(*(uint8*)p); break;
                    case 2: _top->value = v8_streamer<uint16>().to_v8(*(uint16*)p); break;
                    case 4: _top->value = v8_streamer<uint32>().to_v8(*(uint32*)p); break;
                    case 8: _top->value = v8_streamer<uint64>().to_v8(*(uint64*)p); break;
                    }
                    break;

                case type::T_CHAR: {

                    if( !t.is_array_element() ) {
                        _top->value = v8::String::New((const char*)p, 1);
                    }
                    else {
                        DASSERT(0); //should not get here - optimized in write_array
                    }

                } break;

                /////////////////////////////////////////////////////////////////////////////////////
                case type::T_FLOAT:
                    switch( t.get_size() ) {
                    case 4: _top->value = v8_streamer<float>().to_v8(*(float*)p); break;
                    case 8: _top->value = v8_streamer<double>().to_v8(*(double*)p); break;
                    }
                    break;

                /////////////////////////////////////////////////////////////////////////////////////
                case type::T_BOOL:
                    _top->value = v8::Boolean::New(*(bool*)p);
                break;

                /////////////////////////////////////////////////////////////////////////////////////
                case type::T_TIME: {
                    _top->value = v8_streamer<timet>().to_v8(*(timet*)p);
                } break;

                /////////////////////////////////////////////////////////////////////////////////////
                case type::T_ANGLE: {
                    _top->value = v8_streamer<double>().to_v8(*(double*)p); break;
                } break;

                /////////////////////////////////////////////////////////////////////////////////////
                case type::T_ERRCODE:
                    {
                        opcd e = (const opcd::errcode*)p;
                        _top->value = v8::Integer::New(e.code());
                    }
                break;

                /////////////////////////////////////////////////////////////////////////////////////
                case type::T_BINARY: {
                    _bufw.reset();
                    write_binary( p, t.get_size() );

                    _top->value = v8::String::New((const char*)_bufw.ptr(), _bufw.len());
                } break;

                case type::T_COMPOUND:
                    break;

                default: return ersSYNTAX_ERROR "unknown type"; break;
            }
        }

        return 0;
    }


    /////////////////////////////////////////////////////////////////////////////////////////////////////
    opcd read( void* p, type t )
    {
        opcd e=0;
        if( t.is_array_start() )
        {
            if( t.type == type::T_BINARY ) {
                DASSERT(0);
            }
            else if( t.type == type::T_CHAR ) {
                v8::String::Utf8Value text(_top->value);
                if(*text == 0)
                    e = ersSYNTAX_ERROR "expected string";
                else
                    t.set_count(text.length(), p);
            }
            else if( t.type == type::T_KEY ) {
                DASSERT(0);
            }
            else
            {
                if(!_top->value->IsArray())
                    e = ersSYNTAX_ERROR "expected array";
                else {
                    _top->array = v8::Local<v8::Array>(v8::Array::Cast(*_top->value));
                    _top->element = 0;
                }
            }
        }
        else if( t.is_array_end() )
        {
        }
        else if( t.type == type::T_STRUCTEND )
        {
            _top = _stack.pop();
        }
        else if( t.type == type::T_STRUCTBGN )
        {
            bool undef = _top->value->IsUndefined();
            if(!undef && !_top->value->IsObject())
                e = ersSYNTAX_ERROR "expected object";
            else {
                entry* le = _stack.add(1);
                _top = le-1;

                le->object = _top->value->ToObject();
                _top = le;
            }
        }
        else
        {
            switch( t.type )
            {
                case type::T_INT:
                    {
                        try {
                            switch( t.get_size() )
                            {
                            case 1: v8_streamer<int8>() .from_v8(_top->value, *(int8*)p ); break;
                            case 2: v8_streamer<int16>().from_v8(_top->value, *(int16*)p); break;
                            case 4: v8_streamer<int32>().from_v8(_top->value, *(int32*)p); break;
                            case 8: v8_streamer<int64>().from_v8(_top->value, *(int64*)p); break;
                            }
                        }
                        catch(v8::Exception) {
                            e = ersSYNTAX_ERROR "expected number";
                        }
                    }
                    break;
                
                case type::T_UINT:
                    {
                        try {
                            switch( t.get_size() )
                            {
                            case 1: v8_streamer<uint8>() .from_v8(_top->value, *(uint8*)p ); break;
                            case 2: v8_streamer<uint16>().from_v8(_top->value, *(uint16*)p); break;
                            case 4: v8_streamer<uint32>().from_v8(_top->value, *(uint32*)p); break;
                            case 8: v8_streamer<uint64>().from_v8(_top->value, *(uint64*)p); break;
                            }
                        }
                        catch(v8::Exception) {
                            e = ersSYNTAX_ERROR "expected number";
                        }
                    }
                    break;

                case type::T_KEY:
                    return ersUNAVAILABLE;// "should be read as array";
                    break;

                case type::T_CHAR:
                    {
                        if( !t.is_array_element() )
                        {
                            v8::String::Utf8Value str(_top->value);

                            if(*str)
                                *(char*)p = str.length() ? **str : 0;
                            else
                                e = ersSYNTAX_ERROR "expected string";
                        }
                        else
                            //this is optimized in read_array(), non-continuous containers not supported
                            e = ersNOT_IMPLEMENTED;
                    }
                    break;
                
                case type::T_FLOAT:
                    {
                        try {
                            switch( t.get_size() )
                            {
                            case 4: v8_streamer<float>().from_v8(_top->value, *(float*)p); break;
                            case 8: v8_streamer<double>().from_v8(_top->value, *(double*)p); break;
                            }
                        } catch(v8::Exception) {
                            e = ersSYNTAX_ERROR "expected number";
                        }
                    }
                    break;

                /////////////////////////////////////////////////////////////////////////////////////
                case type::T_BOOL: {
                        try {
                            *(bool*)p = _top->value->BooleanValue();
                        } catch(v8::Exception) {
                            e = ersSYNTAX_ERROR "expected boolean";
                        }
                    } break;

                /////////////////////////////////////////////////////////////////////////////////////
                case type::T_TIME: {
                    try {
                        *(timet*)p = timet((int64)_top->value->NumberValue());
                    } catch(v8::Exception) {
                        return ersSYNTAX_ERROR "expected time";
                    }
                } break;

                /////////////////////////////////////////////////////////////////////////////////////
                case type::T_ANGLE: {
                    double a;
                    if(_top->value->IsString()) {
                        v8::String::Utf8Value str(_top->value);
                        token tok(*str, str.length());
                        a = tok.toangle();
                    }
                    else {
                        try {
                            a = _top->value->NumberValue();
                        } catch(v8::Exception) {
                            e = ersSYNTAX_ERROR "expected angle";
                        }
                    }

                    if(!e)
                        *(double*)p = a;
                } break;

                /////////////////////////////////////////////////////////////////////////////////////
                case type::T_ERRCODE: {
                    int64 v;
                    try {
                        v = _top->value->IntegerValue();
                        opcd e;
                        e.set(uint(v));

                        *(opcd*)p = e;
                    }
                    catch(v8::Exception) {
                        e = ersSYNTAX_ERROR "expected number";
                    }
                } break;

                /////////////////////////////////////////////////////////////////////////////////////
                case type::T_BINARY: {
                    try {
                        v8::String::Utf8Value str(_top->value);
                        token tok(*str, str.length());

                        uints i = charstrconv::hex2bin( tok, p, t.get_size(), ' ' );
                        if(i>0)
                            return ersMISMATCHED "not enough array elements";
                        tok.skip_char(' ');
                        if( !tok.is_empty() )
                            return ersMISMATCHED "more characters after array elements read";
                    } catch(v8::Exception) {
                        return ersSYNTAX_ERROR "expected hex string";
                    }
                } break;


                /////////////////////////////////////////////////////////////////////////////////////
                case type::T_COMPOUND:
                    break;


                default:
                    return ersSYNTAX_ERROR "unknown type";
                    break;
            }
        }

        return e;
    }


    virtual opcd write_array_separator( type t, uchar end )
    {
        if(_top->element > 0)
            _top->array->Set(_top->element-1, _top->value);
        ++_top->element;

        return 0;
    }

    virtual opcd read_array_separator( type t )
    {
        if(_top->element >= _top->array->Length())
            return ersNO_MORE;

        _top->value = _top->array->Get(_top->element++);
        return 0;
    }

    virtual opcd write_array_content( binstream_container_base& c, uints* count )
    {
        type t = c._type;
        uints n = c.count();
        //c.set_array_needs_separators();

        if( t.type != type::T_CHAR  &&  t.type != type::T_KEY && t.type != type::T_BINARY )
            return write_compound_array_content(c,count);

        opcd e;
        //optimized for character and key strings
        if( c.is_continuous()  &&  n != UMAXS )
        {
            token tok;

            if( t.type == type::T_BINARY ) {
                _bufw.reset();
                e = write_binary( c.extract(n), n );
                tok = _bufw;
            }
            else {
                tok.set( (const char*)c.extract(n), n );
            }

            if(!e) {
                _top->value = v8::String::New(tok.ptr(), tok.len());
                *count = n;
            }
        }
        else {
            RASSERT(0); //non-continuous string containers not supported here
            e = ersNOT_IMPLEMENTED;
            //e = write_compound_array_content(c,count);
        }

        return e;
    }

    virtual opcd read_array_content( binstream_container_base& c, uints n, uints* count )
    {
        type t = c._type;
        
        if( t.type != type::T_CHAR  &&  t.type != type::T_BINARY ) {
            uint na = _top->array->Length();

            if(n == UMAXS)
                n = na;
            else if(n != na)
                return ersMISMATCHED "array size";

            return read_compound_array_content(c,n,count);
        }

        //if(!_top->value->IsString())
        //    return ersSYNTAX_ERROR "expected string";

        v8::String::Utf8Value str(_top->value);
        token tok(*str, str.length());

        opcd e=0;
        if( t.type == type::T_BINARY )
            e = read_binary(tok,c,n,count);
        else
        {
            if( n != UMAXS  &&  n != tok.len() )
                e = ersMISMATCHED "array size";
            else if( c.is_continuous() )
            {
                xmemcpy( c.insert(n), tok.ptr(), tok.len() );

                *count = tok.len();
            }
            else
            {
                const char* p = tok.ptr();
                uints nc = tok.len();
                for(; nc>0; --nc,++p )
                    *(char*)c.insert(1) = *p;

                *count = nc;
            }
        }

        return e;
    }


protected:
};

////////////////////////////////////////////////////////////////////////////////
///
struct v8_streamer_context
{
    metastream meta;
    fmtstream_v8 fmtv8;

    v8_streamer_context() {
        meta.bind_formatting_stream(fmtv8);
    }

    void reset() {
        meta.stream_reset(true, false);
    }
};

////////////////////////////////////////////////////////////////////////////////
template<bool ISENUM, class T>
struct v8_enum_helper {
    int operator << (const T&) const { return 0; }
    T operator >> (int) const { return T(); }
};

template<class T>
struct v8_enum_helper<true,T> {
    int operator << (const T& v) const { return int(v); }
    T operator >> (int v) const { return T(v); }
};

template<class T>
inline v8::Handle<v8::Value> v8_streamer<T>::to_v8(const T& v)
{
    v8_enum_helper<std::is_enum<T>::value, T> en;
    if(std::is_enum<T>::value)
        return v8::Int32::New(en << v);

    auto& streamer = THREAD_SINGLETON(v8_streamer_context);
    streamer.meta.xstream_out(v);
    return streamer.fmtv8.get();
}


template<class T>
inline bool v8_streamer<T>::from_v8( v8::Handle<v8::Value> src, T& res )
{
    v8_enum_helper<std::is_enum<T>::value, T> en;
    if(std::is_enum<T>::value)
        res = en >> src->Int32Value();
    else {
        auto& streamer = THREAD_SINGLETON(v8_streamer_context);
        streamer.fmtv8.set(src);
        streamer.meta.xstream_in(res);
    }
    return true;
}



COID_NAMESPACE_END

#endif  // ! __COID_COMM_FMTSTREAM_V8__HEADER_FILE__
