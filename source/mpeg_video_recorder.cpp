#include "mpeg_video_recorder.h"
#include <comm/binstream/filestream.h>
#include <comm/metastream/fmtstreamcxx.h>
#include <comm/metastream/metastream.h>
#include <comm/binstream/nullstream.h>
#include <comm/dir.h>
#include <comm/commexception.h>
#include <comm/commtime.h>
#include <comm/timer.h>

#define WARNINGMESSAGE(szMsg)			"warning: (MpegRecorderPlugin) "szMsg
#define ERRORMESSAGE(szMsg)			"error: (MpegRecoredPlugin) "szMsg
#define DEBUGMESSAGE(szMsg)			"debug: (MpegRecoredPlugin) "szMsg

static iref<mpeg_video_recorder> _TR = ot::video_recorder::create(new mpeg_video_recorder);


mpeg_video_recorder::mpeg_video_recorder()
{	
	m_IsRecording = false;
}


mpeg_video_recorder::~mpeg_video_recorder()
{
}

void mpeg_video_recorder::on_user_key(bool on){
	if (!m_IsRecording){
		record(true);
	}
	else{
		record(false);
	}
}

bool mpeg_video_recorder::initialize(const coid::token& video_folder, int width, int height){
	m_sVideoFolderPath = video_folder;
	m_sVideoFolderPath << "\\";
	return InitRecording(width, height);
}

void mpeg_video_recorder::process_frame(const void* data, uints size, uint64 timestamp_ns, uint nbatchframes, bool video_end){

	if (!m_IsRecording){
		// this should't be there but last frame isn't uniqe so i must skip frames sent after "last" frame
		return;
	}

	if (m_iLastFramesize > 0 && m_uiFrameIndex > 0){
		coid::uint64 timeMs = (timestamp_ns - m_uiLastTimestampNs) / 1000000.0;
		MP4Duration frameDuration = timeMs * 90; // 90 000 ticks per seconds in mp4 container
		MP4WriteSample(m_hMp4FileHandle, m_iVideoTrackID, m_pLastNals[0].p_payload, m_iLastFramesize, frameDuration);
		m_uiLastTimestampNs = timestamp_ns;
	}
	else if(m_uiFrameIndex == 0){
		m_uiLastTimestampNs = timestamp_ns;
	}
	else{
		log(WARNINGMESSAGE("Frameskip detected!"));
	}

	uchar * ucharData = (uchar*)data;

	uint width4 = GetDivisibleBy4(m_iWidth);
	uint halfWidth4 = GetDivisibleBy4(m_iWidth >> 1);

	x264_picture_t pic_in,pic_out;
	x264_picture_init(&pic_in);
	pic_in.img.i_csp = X264_CSP_I420;
	pic_in.img.i_plane = 3;
	pic_in.img.plane[0] = ucharData;
	pic_in.img.plane[1] = ucharData + width4;
	pic_in.img.plane[2] = ucharData + 2 * width4 + halfWidth4;
	pic_in.img.i_stride[0] = width4 + halfWidth4;
	pic_in.img.i_stride[2] = pic_in.img.i_stride[1] = (width4 + halfWidth4) * 2;
	
	int i_nals;
	
	m_iLastFramesize = x264_encoder_encode(m_pEncoder, &m_pLastNals, &i_nals, &pic_in, &pic_out);

	m_uiFrameIndex++;

	if (video_end){
		if (m_iLastFramesize > 0){
			MP4Duration frameDuration = 33 * 90;  // 90 000 ticks per seconds in mp4 container
			MP4WriteSample(m_hMp4FileHandle, m_iVideoTrackID, m_pLastNals[0].p_payload, m_iLastFramesize, frameDuration);
		}
		StopRecording();
	}	
}


void mpeg_video_recorder::StopRecording(){
	if (MP4_IS_VALID_FILE_HANDLE(m_hMp4FileHandle)){
		MP4Close(m_hMp4FileHandle);
	}

	m_IsRecording = false;
}

bool mpeg_video_recorder::InitRecording(int width, int height){
	coid::charstr mp4FileName(m_sVideoFolderPath);
	mp4FileName << "capture_";
	mp4FileName << coid::msec_timer::get_time();
	mp4FileName << ".mp4";
	MP4FileHandle fileHandle = MP4Create(mp4FileName.c_str(), MP4FileMode::FILEMODE_CREATE);
	if (!MP4_IS_VALID_FILE_HANDLE(fileHandle)){
		coid::charstr msg;
		msg << ERRORMESSAGE(" ");
		msg << "Can't create Mp4 file! Path: " << mp4FileName;
		log(msg);
		return false;
	}

	m_IsRecording = true;
	m_hMp4FileHandle = fileHandle;
	m_iVideoTrackID = MP4_INVALID_TRACK_ID;

	coid::bifstream cfgFile(m_sVideoFolderPath+"x264.cfg");
	coid::fmtstreamcxx fmt;
	coid::metastream meta(fmt);

	if (cfgFile.is_open()){
		try{
			fmt.bind(cfgFile);
			meta.stream_in(m_oX264Cfg);
		}
		catch (coid::exception& e){

		}
	}
	else{
		log(WARNINGMESSAGE("x264 configuration file does not exists!"));
		fmt.bind(coid::nullstream);
		meta.xstream_in(m_oX264Cfg);
	}

	meta.stream_acknowledge();
	
	x264_param_t param;
	x264_param_default_preset(&param, m_oX264Cfg.m_sPreset.c_str(), "zerolatency");
	param.i_threads = m_oX264Cfg.m_iThreadCount;
	param.i_width = width;
	param.i_height = height;
	param.i_fps_num = 30;
	param.i_fps_den = 1;
	// Intra refres:
	param.i_keyint_max = 30;
	param.b_intra_refresh = 1;
	//Rate control:
	param.rc.i_rc_method = X264_RC_CRF;
	param.rc.f_rf_constant = 25;
	param.rc.f_rf_constant_max = 35;
	//For streaming:
	param.b_repeat_headers = 1;
	param.b_annexb = 1;
	x264_param_apply_profile(&param, "baseline");

	m_pEncoder = x264_encoder_open(&param);

	MP4TrackId videoTrackID = MP4AddH264VideoTrack(m_hMp4FileHandle, 90000, MP4_INVALID_DURATION, width, height, 0, 1, 0, 3);
	if (!MP4_IS_VALID_TRACK_ID(videoTrackID)){
		MP4Close(m_hMp4FileHandle);
		return false;
	}

	m_iVideoTrackID = videoTrackID;

	m_iWidth = width;
	m_iHeight = height;

	m_pLastNals = NULL;
	m_uiLastTimestampNs = 0;
	m_uiFrameIndex = 0;

	return true;
}

int mpeg_video_recorder::GetDivisibleBy4(int value){
	return (value % 4 == 0) ? value : ((value >> 2) + 1) << 2;
}

