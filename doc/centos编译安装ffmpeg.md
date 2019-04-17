# centos编译安装ffmpeg

此文档参考ffmpeg官方文档。

## 第一步、安装依赖包

相信我，走了这一步后面才会畅通无阻。

```
yum install autoconf automake bzip2 cmake freetype-devel gcc gcc-c++ git libtool make mercurial pkgconfig zlib-devel x264-devel
```

## 第二步、创建文件夹ffmpeg_sources

```
mkdir ~/ffmpeg_sources
```

## 第三步、编译汇编语言NASM

```
cd ~/ffmpeg_sources
curl -O -L http://www.nasm.us/pub/nasm/releasebuilds/2.13.02/nasm-2.13.02.tar.bz2
tar xjvf nasm-2.13.02.tar.bz2
cd nasm-2.13.02
./autogen.sh
./configure --prefix="$HOME/ffmpeg_build" --bindir="$HOME/bin"
make
make install
```

## 第四步、编译安装汇编语言Yasm

```
cd ~/ffmpeg_sources
curl -O -L http://www.tortall.net/projects/yasm/releases/yasm-1.3.0.tar.gz
tar xzvf yasm-1.3.0.tar.gz
cd yasm-1.3.0
./configure --prefix="$HOME/ffmpeg_build" --bindir="$HOME/bin"
make
make install
```

## 第五步、编译安装libx264编码器

```
cd ~/ffmpeg_sources
git clone --depth 1 http://git.videolan.org/git/x264
cd x264
PKG_CONFIG_PATH="$HOME/ffmpeg_build/lib/pkgconfig" ./configure --prefix="$HOME/ffmpeg_build" --bindir="$HOME/bin" --enable-static
make
make install
```

## 第六步、编译安装libx265编码器

```
cd ~/ffmpeg_sources
hg clone https://bitbucket.org/multicoreware/x265
cd ~/ffmpeg_sources/x265/build/linux
cmake -G "Unix Makefiles" -DCMAKE_INSTALL_PREFIX="$HOME/ffmpeg_build" -DENABLE_SHARED:bool=off ../../source
make
make install
```

## 第七步、编译安装libfdk_aac编码器

```
cd ~/ffmpeg_sources
git clone --depth 1 https://github.com/mstorsjo/fdk-aac
cd fdk-aac
autoreconf -fiv
./configure --prefix="$HOME/ffmpeg_build" --disable-shared
make
make install
```

## 第八步、编译安装libmp3lame编码器

```
cd ~/ffmpeg_sources
curl -O -L http://downloads.sourceforge.net/project/lame/lame/3.100/lame-3.100.tar.gz
tar xzvf lame-3.100.tar.gz
cd lame-3.100
./configure --prefix="$HOME/ffmpeg_build" --bindir="$HOME/bin" --disable-shared --enable-nasm
make
make install
```

**注意** 这个http://downloads.sourceforge.net/project/lame/lame/3.100/lame-3.100.tar.gz 可能会下载失败，可以先下载到本地再上传到服务器~/ffmpeg_sources文件夹里边去。

## 第九步、编译安装libopus编码器

```
cd ~/ffmpeg_sources
curl -O -L https://archive.mozilla.org/pub/opus/opus-1.2.1.tar.gz
tar xzvf opus-1.2.1.tar.gz
cd opus-1.2.1
./configure --prefix="$HOME/ffmpeg_build" --disable-shared
make
make install
```

## 第十步、编译安装libogg编码器

```
cd ~/ffmpeg_sources
curl -O -L http://downloads.xiph.org/releases/ogg/libogg-1.3.3.tar.gz
tar xzvf libogg-1.3.3.tar.gz
cd libogg-1.3.3
./configure --prefix="$HOME/ffmpeg_build" --disable-shared
make
make install
```

## 第十一步、编译安装libvorbis编码器

```
cd ~/ffmpeg_sources
curl -O -L http://downloads.xiph.org/releases/vorbis/libvorbis-1.3.5.tar.gz
tar xzvf libvorbis-1.3.5.tar.gz
cd libvorbis-1.3.5
./configure --prefix="$HOME/ffmpeg_build" --with-ogg="$HOME/ffmpeg_build" --disable-shared
make
make install
```

## 第十二步、编译安装libtheora编码器

```
cd ~/ffmpeg_sources
curl -O -L https://ftp.osuosl.org/pub/xiph/releases/theora/libtheora-1.1.1.tar.gz
tar xzvf libtheora-1.1.1.tar.gz
cd libtheora-1.1.1
./configure --prefix="$HOME/ffmpeg_build" --with-ogg="$HOME/ffmpeg_build" --disable-shared
make
make install
```

## 第十三步、编译安装libvpx编码器

```
cd ~/ffmpeg_sources
git clone --depth 1 https://chromium.googlesource.com/webm/libvpx.git
cd libvpx
./configure --prefix="$HOME/ffmpeg_build" --disable-examples --disable-unit-tests --enable-vp9-highbitdepth --as=yasm
make
make install
```

## 第十四步、编译安装libfreetype

```
wget "https://download.savannah.gnu.org/releases/freetype/freetype-2.9.1.tar.gz"
tar zxvf freetype-2.9.1.tar.gz
cd freetype-2.9.1/
./configure --prefix=/usr/local --disable-static
make
make install
```

## 第十五步、编译安装fribidi

```
wget "https://github.com/fribidi/fribidi/releases/download/v1.0.4/fribidi-1.0.4.tar.bz2"
tar xf fribidi-1.0.4.tar.bz2
cd fribidi-1.0.4
./configure --prefix=/usr/local/ --enable-shared
make
make install

如果报错 No package 'uuid' found 则按如下步骤解决：
yum -y install libuuid-devel
如果报错 WARNING: 'gperf' is missing on your system. 则按如下步骤解决：
wget http://mirrors.ustc.edu.cn/gnu/gperf/gperf-3.1.tar.gz
tar zxvf gperf-3.1.tar.gz
cd gperf-3.1
./configure
make
make install
```

## 第十六步、编译安装fontconfig

```
export PKG_CONFIG_PATH=/usr/local/lib/pkgconfig:$PKG_CONFIG_PATH
wget "https://www.freedesktop.org/software/fontconfig/release/fontconfig-2.13.0.tar.gz"
tar zxvf fontconfig-2.13.0.tar.gz
cd fontconfig-2.13.0
./configure --prefix=/usr/local/ --enable-shared --enable-libxml2
make
make install
```

## 第十七步、编译安装libass

```
wget https://github.com/libass/libass/releases/download/0.14.0/libass-0.14.0.tar.xz
tar xvf libass-0.14.0.tar.xz
cd libass-0.14.0
./configure --prefix=/usr/local/ --disable-static
make
make install
```

## 第十八步、编译配置安装ffmpeg

```
cd ~/ffmpeg_sources
curl -O -L https://ffmpeg.org/releases/ffmpeg-snapshot.tar.bz2
tar xjvf ffmpeg-snapshot.tar.bz2
cd ffmpeg
PATH="$HOME/bin:$PATH" PKG_CONFIG_PATH="$HOME/ffmpeg_build/lib/pkgconfig" ./configure \
  --prefix="$HOME/ffmpeg_build" \
  --pkg-config-flags="--static" \
  --extra-cflags="-I$HOME/ffmpeg_build/include" \
  --extra-ldflags="-L$HOME/ffmpeg_build/lib" \
  --extra-libs=-lpthread \
  --extra-libs=-lm \
  --bindir="$HOME/bin" \
  --enable-gpl \
  --enable-libfdk_aac \
  --enable-libfreetype \
  --enable-libmp3lame \
  --enable-libopus \
  --enable-libvorbis \
  --enable-libtheora \
  --enable-libvpx \
  --enable-libx264 \
  --enable-libx265 \
  --enable-nonfree \
  --enable-libass
make
make install
hash -r
```