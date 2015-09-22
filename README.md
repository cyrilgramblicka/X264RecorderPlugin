If you want to compile source, you will need: Visual Studio 2013, x264 static library, mp4v2 static library and comm static library.

You must compile the static libraries on your own and then it link to the VS project. 

X264 source code: ftp://ftp.videolan.org/pub/x264/snapshots/last_x264.tar.bz2

mp4v2 source code: https://mp4v2.googlecode.com/files/mp4v2-2.0.0.tar.bz2

comm source code is in "source\api\comm" directory. Solution file path: "source\api\comm\_build\msvc\2013\comm.sln". Just compile comm_static project.

<b>Installation instructions:</b>
 
1. Download binary form this link: https://github.com/cyrilgramblicka/X264RecorderPlugin/raw/master/binary/x264_recorder_plugin.dll
2. Place x264_recorder_plugin.dll to Outerra plugins directory (e.g C:\Program Files (x86)\Outerra\Anteworld\plugins)
3. Start Outerra and press F9 to start and/or stop recording.
4. Output video files should be in you outerra data directory in "videos" subfolder. (e.g. C:\Users\(YOURACCOUNT)\Outerra\videos)