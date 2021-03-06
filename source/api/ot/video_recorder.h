
#ifndef __INTERGEN_GENERATED__video_recorder_H__
#define __INTERGEN_GENERATED__video_recorder_H__

//@file Interface file for video_recorder interface generated by intergen

#include <comm/commexception.h>
#include <comm/intergen/ifc.h>

#include <ot/location_cfg.h>

class framebuffer;


namespace ot {

////////////////////////////////////////////////////////////////////////////////

class video_recorder
    : public intergen_interface
{
public:

    // --- interface methods ---

    ///Turn the recording on/off
    //@return previous recording state. Also returns true if the built-in recording is active
    bool record( bool on );

    ///Post a log message with the following recognized prefixes: error:,warning:,info:,dbg:,perf:
    void log( const coid::token& text );


protected:
    // --- interface events (callbacks from host to client) ---
    // ---       overload these to handle host events       ---
    
    friend class ::framebuffer;

    ///Initialize recording
    //@param video_folder path to user's configured video directory
    //@param width image width
    //@param height image height
    //@return true if recording should start
    virtual bool initialize( const coid::token& video_folder, int width, int height ){return true;}

    ///Invoked when user presses the video recording key. Allows the plugin to intercept that and launch its own recording.
    //@note runs from a different thread than process_frame, so it should be used only for calling record()
    virtual void on_user_key( bool on ) {}

    /**Process YUV420 frame data
      @param data planar YUV420 frame data, ordered as interleaved rows of Y-U-Y-V
      @param size size of the data in bytes
      @param width effective width of the screen
      @param height effective height of the screen
      @param timestamp_ns frame render time [ns]
      @param nbatchframes 1 + the number of skipped/missed frames
      @param video_end true if the video capture ends with this frame

      The format is as follows: for two rows of image data
      - Y0 row data, bytes: width aligned to 4
      - U row data, bytes: width/2 aligned to 4
      - Y1 row data, bytes: width aligned to 4
      - V row data, bytes: width/2 aligned to 4
      An odd number of rows is extended by one.

      Strides:
        int w4 = (w+3)&~3
        int u4 = (w/2+3)&~3
        int h2 = (h+1)&~1

        Y stride:   w4 + u4
        U,V stride: 2*w4 + 2*u4
    **/
    virtual void process_frame( const void* data, uints size, uint64 timestamp_ns, uint nbatchframes, bool video_end ) {}

    //@return true if plugin handles the capture
    virtual bool capture_screen( const coid::token& path, uint width, uint height, uint pitch, const void* data, uints size, const ot::location_info& info ){return false;}

    virtual void force_bind_script_events() {}

public:
    // --- creators ---

    static iref<video_recorder> create() {
        return create<video_recorder>(0);
    }

    template<class T>
    static iref<T> create( T* _subclass_ );

    // --- internal helpers ---

    virtual ~video_recorder() {
        if(_cleaner) _cleaner(this,0);
    }

    static const int HASHID = 2798060792;
    
    int intergen_hash_id() const override { return HASHID; }
    
    const coid::token& intergen_interface_name() const override {
        static const coid::token _name = "ot::video_recorder";
        return _name;
    }

    const coid::token& intergen_default_creator() const override {
        static const coid::token _dc("ot::video_recorder.create@2798060792");
        return _dc;
    }

    void* intergen_wrapper_js() const override {
        static void* _js_create_wrapper=0;
        if(_js_create_wrapper)
            return _js_create_wrapper;

        static const coid::token _js_wrapper_key = "ot::js::video_recorder@wrapper";
        _js_create_wrapper = coid::interface_register::get_interface_creator(_js_wrapper_key);
        return _js_create_wrapper;
    }
    
private:
    // --- host helpers to check presence of handlers in scripts ---

    virtual bool is_bound_initialize() { return true; }
    virtual bool is_bound_on_user_key() { return true; }
    virtual bool is_bound_process_frame() { return true; }
    virtual bool is_bound_capture_screen() { return true; }
    
protected:

    typedef void (*cleanup_fn)(video_recorder*, intergen_interface*);
    cleanup_fn _cleaner;

    video_recorder() : _cleaner(0)
    {}
};

////////////////////////////////////////////////////////////////////////////////
template<class T>
inline iref<T> video_recorder::create( T* _subclass_ )
{
    typedef iref<T> (*fn_creator)(video_recorder*);

    static fn_creator create = 0;
    static const coid::token ifckey = "ot::video_recorder.create@2798060792";

    if(!create)
        create = reinterpret_cast<fn_creator>(
            coid::interface_register::get_interface_creator(ifckey));

    if(!create)
        throw coid::exception("interface creator inaccessible: ") << ifckey;

    return create(_subclass_);
}


inline bool video_recorder::record( bool on )
{ return VT_CALL(bool,(bool),0)(on); }

inline void video_recorder::log( const coid::token& text )
{ return VT_CALL(void,(const coid::token&),1)(text); }

} //namespace

#endif //__INTERGEN_GENERATED__video_recorder_H__
