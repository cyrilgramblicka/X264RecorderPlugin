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

#include <comm/metastream/metastream.h>
#include <comm/str.h>

struct x264_cfg{
	coid::uint8 m_iThreadCount;
	coid::charstr m_sPreset;
};

coid::metastream& operator || (coid::metastream & meta, x264_cfg & cfg){
	return meta.compound("x264_cfg", [&](){
		meta.member("ThreadCount", cfg.m_iThreadCount, 1);
		meta.member("Preset",cfg.m_sPreset,"ultrafast");
	});
}