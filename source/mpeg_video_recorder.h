/*********************************************************************************
*Mpeg Video recorder Plugin X264
**********************************************************************************.
* Copyright (C) 2015  Outerra s.r.o
*
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License
* as published by the Free Software Foundation; either version 2
* of the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
***********************************************************************************/

#pragma once
#include <ot/video_recorder.h>
#include <inttypes.h>
extern "C"{
	#include <x264/x264.h>
}

#define __NPLUGINDEBUG

#include "x264_cfg.h"

#include <comm/str.h>
#include <comm/timer.h>
#include <mp4v2/mp4v2.h>
#include <comm/pthreadx.h>


class mpeg_video_recorder :
	public ot::video_recorder
{
public:
	virtual void on_user_key(bool on) override;
	mpeg_video_recorder();
	~mpeg_video_recorder();
protected:
	virtual void process_frame(const void* data, uints size, uint64 timestamp_ns, uint nbatchframes, bool video_end) override;
	virtual bool initialize(const coid::token& video_folder, int width, int height) override;
private:
	void StopRecording();
	bool InitRecording(int width, int height);
	inline int GetDivisibleBy4(int value);

// member variables
	bool m_IsRecording;
	MP4FileHandle m_hMp4FileHandle;
	MP4TrackId m_iVideoTrackID;

	int m_iWidth;
	int m_iHeight;

	x264_t * m_pEncoder;
	x264_cfg m_oX264Cfg;
	x264_nal_t* m_pLastNals;
	coid::uint64 m_uiLastTimestampNs;
	coid::uint32 m_uiFrameIndex;
	int m_iLastFramesize;

	coid::charstr m_sVideoFolderPath;
#ifdef __PLUGINDEBUG
// debug timer
	coid::nsec_timer m_oPerformanceTimer;
#endif
};

