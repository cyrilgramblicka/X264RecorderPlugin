If you want to compile source, you will need: Visual Studio 2013, x264 static library, mp4v2 static library and comm static library.

You must compile the static libraries on your own and then it link to the VS project. 

X264 source code: ftp://ftp.videolan.org/pub/x264/snapshots/last_x264.tar.bz2

mp4v2 source code: https://mp4v2.googlecode.com/files/mp4v2-2.0.0.tar.bz2

comm source code is in "source\api\comm" directory. Solution file path: "source\api\comm\_build\msvc\2013\comm.sln". Just compile comm_static project.

The plugin dll file must be placed in outerra plugins directory. 