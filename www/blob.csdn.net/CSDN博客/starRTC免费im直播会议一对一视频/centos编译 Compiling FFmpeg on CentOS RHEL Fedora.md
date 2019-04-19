# centos编译 Compiling FFmpeg on CentOS  RHEL  Fedora - starRTC免费im直播会议一对一视频 - CSDN博客
2017年05月17日 15:23:08[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：261
This guide is based on a minimal installation of the latest CentOS release, and will provide a local, non-system installation of FFmpeg with support for several external encoding libraries. These
 instructions should also work for recent Red Hat Enterprise Linux (RHEL) and Fedora. This is a non-invasive guide and undoing all steps
 is simple and is shown at the end of this page.
## Get the Dependencies
These are required compiling, but you can remove them when you are done if you prefer (except `make`; it should be installed
 by default and many things depend on it).
# yum install autoconf automake cmake freetype-devel gcc gcc-c++ git libtool make mercurial nasm pkgconfig zlib-devel bzip2 
In your home directory make a new directory to put all of the source code into:mkdir ~/ffmpeg_sources
## Compilation & Installation
Note: If you do not require certain encoders you may skip the relevant section and then remove the appropriate `./configure` option
 in FFmpeg. For example, if libvorbis is not needed, then skip that section and then remove `--enable-libvorbis` from the [Install
 FFmpeg](https://trac.ffmpeg.org/wiki/CompilationGuide/Centos#FFmpeg) section.
### Yasm
Yasm is an assembler used by x264 and FFmpeg.
cd ~/ffmpeg_sources
git clone --depth 1 git://github.com/yasm/yasm.git
cd yasm
autoreconf -fiv
./configure --prefix="$HOME/ffmpeg_build" --bindir="$HOME/bin"
make && make install
make distclean
### libx264
H.264 video encoder. See the [H.264 Encoding Guide](https://trac.ffmpeg.org/wiki/Encode/H.264) for more information and usage examples.
Requires `ffmpeg` to be configured with `--enable-gpl``--enable-libx264`.
cd ~/ffmpeg_sources
git clone --depth 1 git://git.videolan.org/x264
cd x264
PKG_CONFIG_PATH="$HOME/ffmpeg_build/lib/pkgconfig"./configure --prefix="$HOME/ffmpeg_build" --bindir="$HOME/bin" --enable-staticmake && make installmake distclean
### libx265
H.265/HEVC video encoder. See the [H.265 Encoding Guide](https://trac.ffmpeg.org/wiki/Encode/H.265) for more information
 and usage examples.
Requires `ffmpeg` to be configured with `--enable-gpl``--enable-libx265`.
cd ~/ffmpeg_sources
hg clone https://bitbucket.org/multicoreware/x265
cd ~/ffmpeg_sources/x265/build/linux
cmake -G "Unix Makefiles" -DCMAKE_INSTALL_PREFIX="$HOME/ffmpeg_build" -DENABLE_SHARED:bool=off ../../source
make
make install
### libfdk_aac
AAC audio encoder.
Requires `ffmpeg` to be configured with `--enable-libfdk-aac` (and `--enable-nonfree` if
 you also included `--enable-gpl`).
cd ~/ffmpeg_sources
git clone --depth 1 git://git.code.sf.net/p/opencore-amr/fdk-aac
cd fdk-aac
autoreconf -fiv
./configure --prefix="$HOME/ffmpeg_build" --disable-shared
make && make install && make distclean
### libmp3lame
MP3 audio encoder.
Requires `ffmpeg` to be configured with `--enable-libmp3lame`.
cd ~/ffmpeg_sources
curl -L -O http://downloads.sourceforge.net/project/lame/lame/3.99/lame-3.99.5.tar.gz
tar xzvf lame-3.99.5.tar.gz
cd lame-3.99.5
./configure --prefix="$HOME/ffmpeg_build" --bindir="$HOME/bin" --disable-shared --enable-nasm
make
make install
make distclean
### libopus
Opus audio decoder and encoder.
Requires `ffmpeg` to be configured with `--enable-libopus`.
cd ~/ffmpeg_sources
git clone git://git.opus-codec.org/opus.git
cd opus
autoreconf -fiv
./configure --prefix="$HOME/ffmpeg_build" --disable-shared
make
make install
make distclean
### libogg
Ogg bitstream library. Required by [libtheora](https://trac.ffmpeg.org/wiki/CompilationGuide/Centos#libtheora) and [libvorbis](https://trac.ffmpeg.org/wiki/CompilationGuide/Centos#libvorbis).
cd ~/ffmpeg_sources
curl -O http://downloads.xiph.org/releases/ogg/libogg-1.3.2.tar.gz
tar xzvf libogg-1.3.2.tar.gz
cd libogg-1.3.2
./configure --prefix="$HOME/ffmpeg_build" --disable-shared
make
make install
make distclean
### libvorbis
Vorbis audio encoder. Requires [libogg](https://trac.ffmpeg.org/wiki/CompilationGuide/Centos#libogg).
Requires `ffmpeg` to be configured with `--enable-libvorbis`.
cd ~/ffmpeg_sources
curl -O http://downloads.xiph.org/releases/vorbis/libvorbis-1.3.4.tar.gz
tar xzvf libvorbis-1.3.4.tar.gz
cd libvorbis-1.3.4
LDFLAGS="-L$HOME/ffmeg_build/lib" CPPFLAGS="-I$HOME/ffmpeg_build/include" ./configure --prefix="$HOME/ffmpeg_build" --with-ogg="$HOME/ffmpeg_build" --disable-shared
make
make install
make distclean
### libvpx
VP8/VP9 video encoder.
Requires `ffmpeg` to be configured with `--enable-libvpx`.
地址要改为[https://github.com/webmproject/libvpx/](https://github.com/webmproject/libvpx/)git clone[https://github.com/webmproject/libvpx](https://github.com/webmproject/libvpx)
cd ~/ffmpeg_sources
git clone --depth 1 https://chromium.googlesource.com/webm/libvpx.git
cd libvpx
./configure --prefix="$HOME/ffmpeg_build" --disable-examples
make
make install
make clean
### FFmpeg
cd ~/ffmpeg_sources
git clone --depth 1 git://source.ffmpeg.org/ffmpeg
cd ffmpeg
PKG_CONFIG_PATH="$HOME/ffmpeg_build/lib/pkgconfig" ./configure --prefix="$HOME/ffmpeg_build" --extra-cflags="-I$HOME/ffmpeg_build/include" --extra-ldflags="-L$HOME/ffmpeg_build/lib" --bindir="$HOME/bin" --pkg-config-flags="--static" --enable-gpl --enable-nonfree --enable-libfdk-aac --enable-libfreetype --enable-libmp3lame --enable-libopus --enable-libvorbis --enable-libvpx --enable-libx264 --enable-libx265
make && make install && make distclean
hash -r
Compilation is now complete and `ffmpeg` (also `ffprobe`, `ffserver`, `lame`,
 and `x264`) should now be ready to use. The rest of this guide shows how to update or remove FFmpeg.
编译完成后，会：
Tip: Keep the `ffmpeg_sources` directory
 and all contents if you intend to [update](https://trac.ffmpeg.org/wiki/CompilationGuide/Centos#Updating) as shown below. Otherwise you can delete this directory.
## Updating
Development of FFmpeg is active and an occasional update can give you new features and bug fixes. First, remove the old files and then update the dependencies:
rm -rf ~/ffmpeg_build ~/bin/{ffmpeg,ffprobe,ffserver,lame,vsyasm,x264,x265,yasm,ytasm}
# yum install autoconf automake cmake gcc gcc-c++ git libtool make mercurial nasm pkgconfig zlib-devel
### Update Yasm
cd ~/ffmpeg_sources/yasm
make distclean
git pull
Then run `./configure`, `make`,
 and `make install` as shown in the [Install
 yasm](https://trac.ffmpeg.org/wiki/CompilationGuide/Centos#yasm) section.
### Update x264
cd ~/ffmpeg_sources/x264
make distclean
git pull
Then run `./configure`, `make`,
 and `make install` as shown in the [Install
 x264](https://trac.ffmpeg.org/wiki/CompilationGuide/Centos#x264) section.
### Update x265
cd ~/ffmpeg_sources/x265
rm -rf ~/ffmpeg_sources/x265/build/linux/*
hg update
cd ~/ffmpeg_sources/x265/build/linux
Then run `cmake`, `make`,
 and `make install` as shown in the [Install
 x265](https://trac.ffmpeg.org/wiki/CompilationGuide/Centos#x265) section.
### Update libfdk_aac
cd ~/ffmpeg_sources/fdk_aac
make distclean
git pull
Then run `./configure`, `make`,
 and `make install` as shown in the [Install
 libfdk_aac](https://trac.ffmpeg.org/wiki/CompilationGuide/Centos#libfdk_aac) section.
### Update libvpx
cd ~/ffmpeg_sources/libvpx
make clean
git pull
Then run `./configure`, `make`,
 and `make install` as shown in the [Install
 libvpx](https://trac.ffmpeg.org/wiki/CompilationGuide/Centos#libvpx) section.
### Update FFmpeg
cd ~/ffmpeg_sources/ffmpeg
make distclean
git pull
Then run `./configure`, `make`,
 and `make install` as shown in the [Install
 FFmpeg](https://trac.ffmpeg.org/wiki/CompilationGuide/Centos#FFmpeg) section.
## Reverting changes made by this guide
rm -rf ~/ffmpeg_build ~/ffmpeg_sources ~/bin/{ffmpeg,ffprobe,ffserver,lame,vsyasm,x264,yasm,ytasm}
# yum erase autoconf automake cmake gcc gcc-c++ git libtool mercurial nasm pkgconfig zlib-devel
hash -r
## If You Need Help
Feel free to ask your questions at the #ffmpeg IRC channel or the [​ffmpeg-user](http://ffmpeg.org/contact.html) mailing list.
## Also See
- [H.264 Video Encoding Guide](https://trac.ffmpeg.org/wiki/Encode/H.264)
- [AAC Audio Encoding Guide](https://trac.ffmpeg.org/wiki/Encode/AAC)
