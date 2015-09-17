
#ifndef __INTERGEN_GENERATED__video_recorder_JS_H__
#define __INTERGEN_GENERATED__video_recorder_JS_H__

//@file Javascript interface file for video_recorder interface generated by intergen

#include "video_recorder.h"

#include <comm/intergen/ifc.js.h>
#include <comm/token.h>

namespace ot {
namespace js {

class video_recorder
{
public:

    //@param scriptpath path to js script to bind to
    static iref<ot::video_recorder> create( const script_handle& script, const coid::token& bindvar = coid::token(), v8::Handle<v8::Context>* ctx=0 )
    {
        typedef iref<ot::video_recorder> (*fn_bind)(const script_handle&, const coid::token&, v8::Handle<v8::Context>*);
        static fn_bind binder = 0;
        static const coid::token ifckey = "ot::js::video_recorder.create@creator";
        
        if(!binder)
            binder = reinterpret_cast<fn_bind>(
                coid::interface_register::get_interface_creator(ifckey));

        if(!binder)
            throw coid::exception("interface binder inaccessible: ") << ifckey;

        return binder(script, bindvar, ctx);
    }
};

} //namespace js
} //namespace


#endif //__INTERGEN_GENERATED__video_recorder_JS_H__