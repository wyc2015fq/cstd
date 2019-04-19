# Ubuntu 下用NDK编译移植 ffmpeg 2.0 (配置最新版x264) 到android平台 - 建建的博客 - CSDN博客
2017年06月08日 11:30:39[纪建](https://me.csdn.net/u013898698)阅读数：301
个人分类：[Android](https://blog.csdn.net/u013898698/article/category/6941721)

将x264配置到ffmpeg中需要先编译x264,生成静态库或动态库。因为264的静态库本身不大（我编译完成后是1.1M）且考虑到平台移植问题，这里选择的是编译生成静态库。
准备，新建工作空间 
（1）创建总目录FFmpeg-[Android](http://lib.csdn.net/base/android)
eg: mkdir workspace --> cd workspace --> mkdir FFmpeg-Android --> cd FFmpeg-Android 
（2）创建保存x264静态库的目录 android-x264
（在FFmpeg-Android目录下）mkdir 264 --> cd 264
1、编译x264
（1）去官网：[http://www.videolan.org/developers/x264.html](http://www.videolan.org/developers/x264.html) 下载最新的264源代码。这里下载的是2013年10月份出的版本0.14。将压缩包解压缩到264FFmpeg-Android/264目录下，
（2）编写脚本文件：
**[cpp]**[view
 plain](http://blog.csdn.net/chance_yin/article/details/17711003#)[copy](http://blog.csdn.net/chance_yin/article/details/17711003#)
- export NDK=$NDK_HOME  
- export PREBUILT=$NDK/toolchains/arm-linux-androideabi-4.6/prebuilt  
- export PLATFORM=$NDK/platforms/android-19/arch-arm   
- export PREFIX=../../android-x264  
- 
- ./configure --prefix=$PREFIX \  
- --enable-static \  
- --enable-pic \  
- --disable-asm \  
- --disable-cli \  
- --host=arm-linux \  
- --cross-prefix=$PREBUILT/linux-x86_64/bin/arm-linux-androideabi- \  
- --sysroot=$PLATFORM  
注意：这里涉及到路径的变量需要根据你们自己的情况来调整。
PREBUILT 变量保存的是NDK交叉编译链的路径，
PLATFORM 变量保存的是NDK进行链接时查找库文件的路径（arch-arm目录下保存的是各种andorid平台下的c库头文件和静态以及动态库）
PREFIX 变量保存的是编译生成的静态库保存的路径
（3）运行脚本，结果如下图
![](https://img-blog.csdn.net/20131231111641687?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvY2hhbmNlX3lpbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
（4）执行完刚才的脚本后，会在当前目录生成配置文件，这里还需要修改以下几个文件，原因是：编译成功后在eclipse下运行会出现 "cannot locate symbol 'log2f' " 的错误
修改1、config.h
---- #define HAVE_LOG2F 1
+++#define HAVE_LOG2F 0
修改2、encoder/encoder.c 、 encoder/ratecontrol.c 、encoder/analyse.c 、encoder/set.c
在文件开头处添加 
+++ #include <math.h>
+++  //<!-- add by chance_yin 2013.12.27
+++  #undef log2f(x) 
+++  #undef log2f
+++  #define log2f(x) (logf(x)*1.4426950408889634f)
+++  // end add by chance_yin-->
（5）在当前目录下执行： make , make install
完成后，就可以在FFmpeg-Android的android-x264目录下生成头文件和静态库文件了，至此，x264编译完成。
![](https://img-blog.csdn.net/20131231132000531?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvY2hhbmNlX3lpbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
2、编译FFmpeg 2.0 ，下载地址：[http://ffmpeg.org/olddownload.html](http://ffmpeg.org/olddownload.html)
（1）将压缩包解压缩到FFmpeg-Android的根目录下
（2）编写脚本，这里将配置和编译过程分成两个脚本来写，主要是配置脚本完成后，还要手动的去修改ffmpeg目录下的config.h文件
配置脚本：
**[cpp]**[view
 plain](http://blog.csdn.net/chance_yin/article/details/17711003#)[copy](http://blog.csdn.net/chance_yin/article/details/17711003#)
- #!/bin/bash
- 
- DEST=`pwd`/build/ffmpeg && rm -rf $DEST  
- SOURCE=`pwd`/ffmpeg  
- 
- 
- if [ -d ffmpeg ]; then  
-   cd ffmpeg  
- else
-   echo "can not find ffmpeg source code"
-   exit 1  
- fi  
- 
- if [ "$PIPESTATUS" != "0" ] ; then  
-     echo "error in FFmpeg-Android.sh : 14 "
- fi  
- 
- 
- TOOLCHAIN=/tmp/vplayer  
- SYSROOT=$TOOLCHAIN/sysroot/  
- $NDK_HOME/build/tools/make-standalone-toolchain.sh --platform=android-19 --install-dir=$TOOLCHAIN  
- 
- export PATH=$TOOLCHAIN/bin:$PATH  
- export CC="arm-linux-androideabi-gcc"
- export LD=arm-linux-androideabi-ld  
- export AR=arm-linux-androideabi-ar  
- 
- #CFLAGS="-O3 -Wall -mthumb -pipe -fpic -fasm \
- #  -finline-limit=300 -ffast-math \
- #  -fstrict-aliasing \
- #  -fmodulo-sched -fmodulo-sched-allow-regmoves \
- #  -Wno-psabi -Wa,--noexecstack \
- #  -DANDROID -DNDEBUG"
- 
- CFLAGS="-Os -fPIC -marm"
- 
- FFMPEG_FLAGS="--target-os=linux \  
-   --arch=arm \  
-   --sysroot=$SYSROOT \  
-   --enable-cross-compile \  
-   --cross-prefix=arm-linux-androideabi- \  
-   --enable-shared \  
-   --enable-static \  
-   --disable-symver \  
-   --disable-doc \  
-   --disable-ffplay \  
-   --disable-ffmpeg \  
-   --disable-ffprobe \  
-   --disable-ffserver \  
-   --disable-avdevice \  
-   --disable-avfilter \  
-   --disable-filters \  
-   --disable-devices \  
-   --disable-pthreads \  
-   --disable-everything \  
-   --enable-gpl \  
-   --enable-muxers \  
-   --enable-encoders \  
-   --enable-protocols  \  
-   --enable-parsers \  
-   --enable-demuxers \  
-   --enable-decoders \  
-   --enable-bsfs \  
-   --enable-network \  
-   --enable-swscale  \  
-   --enable-libx264 \  
-   --enable-encoder=libx264 \  
-   --enable-decoder=h264 \  
-   --enable-muxer=h264 \  
-   --enable-demuxer=h264 \  
-   --disable-demuxer=sbg \  
-   --disable-demuxer=dts \  
-   --disable-parser=dca \  
-   --disable-decoder=dca \  
-   --extra-libs=-lx264 \  
-   --enable-asm \  
-   --enable-version3"  
- 
- VERSION=armv7  
- cd $SOURCE  
- 
- EXTRA_CFLAGS="-I../android-x264/include -march=armv7-a"
- EXTRA_LDFLAGS="-L../android-x264/lib"
- 
- PREFIX="$DEST/$VERSION" && mkdir -p $PREFIX  
- FFMPEG_FLAGS="$FFMPEG_FLAGS --prefix=$PREFIX"
- 
- sh  ./configure $FFMPEG_FLAGS --extra-cflags="$CFLAGS $EXTRA_CFLAGS" --extra-ldflags="$EXTRA_LDFLAGS" | tee $PREFIX/configuration.txt  
- if [ "$?" = "0" ] ; then   
-     echo "./configure success in shell-script"
- else
-     echo "./configure erroe in shell-script"
-     exit 1  
- fi  
- cp config.* $PREFIX  
我们编译ffmpeg的过程是这样的，将交叉编译链拷贝整个拷贝一份到系统的临时目录下/tmp的vplayer中（交叉编译链的结构其实蛮有意思，我在另一篇博客中会好好分析）
DEST ， 生成的ffmpeg共享库libffmpeg.so保存的路径
SOURCE， ffmpeg源代码的路径 
TOOLCHAIN ，用来保存交叉编译链的目录路径
SYSROOT ，    用来保存交叉编译练进行链接步骤时查找库文件的路径
EXTRA_CFLAGS，用来帮助我们将264集成到ffmpeg时，查找264头文件的路径
EXTRA_LDFLAGS，ffmpeg查找264静态库的路径
其他的变量估计都是见名知意的了，不明白的可以给我留言。
OK，编写完脚本后，先执行 config-ffmpeg-android.sh 脚本，对ffmpeg进行配置
PS. 如果执行配置脚本时，遇到诸如 ” c compiler failed ....“ 之类的错误，多半是你配置脚本中某个变量的路径错了。
配置脚本执行完后，会打印出ffmpeg的配置信息：如图
![](https://img-blog.csdn.net/20131231134044062?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvY2hhbmNlX3lpbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)![](https://img-blog.csdn.net/20131231134137265?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvY2hhbmNlX3lpbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
从encoders 或者decoders中可以看出来，我们的ffmepg已经成功配置上了264
编译完成后，会出现一个警告如下图，这个可以忽略。
![](https://img-blog.csdn.net/20131231134338875?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvY2hhbmNlX3lpbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
（3）配置脚本config-ffmpeg-andorid.sh 执行完后，还需要修改ffmpeg目录下的config.h文件，同样是修改 HAVE_LOGx的宏值
----#define HAVE_LOG2 1
----#define HAVE_LOG2F 1
----#define HAVE_LOG10F 1
+++#define HAVE_LOG2 0
+++#define HAVE_LOG2F 0
+++#define HAVE_LOG10F 0
（4）修改完成后，执行编译脚本 ./make-ffmpeg-android.sh 脚本，脚本内容如下：
**[cpp]**[view
 plain](http://blog.csdn.net/chance_yin/article/details/17711003#)[copy](http://blog.csdn.net/chance_yin/article/details/17711003#)
- #!/bin/bash 
- 
- DEST=`pwd`/build/ffmpeg && rm -rf $DEST  
- SOURCE=`pwd`/ffmpeg  
- 
- TOOLCHAIN=/tmp/vplayer  
- SYSROOT=$TOOLCHAIN/sysroot/  
- 
- export PATH=$TOOLCHAIN/bin:$PATH  
- export CC="arm-linux-androideabi-gcc"
- export LD=arm-linux-androideabi-ld  
- export AR=arm-linux-androideabi-ar  
- 
- CFLAGS="-Os -fPIC -marm"
- 
- VERSION=armv7  
- cd $SOURCE  
- 
- PREFIX="$DEST/$VERSION" && mkdir -p $PREFIX  
- 
- 
- EXTRA_LDFLAGS="-L../android-x264/lib"
- 
- make clean  
- make -j4 || exit 1  
- make install || exit 1  
- 
- $CC -llog -lm -lz -shared --sysroot=$SYSROOT -Wl,--no-undefined -Wl,-z,noexecstack $EXTRA_LDFLAGS libavutil/*.o libavutil/arm/*.o libavcodec/*.o libavcodec/arm/*.o libavformat/*.o libswresample/*.o libswscale/*.o -o $PREFIX/libffmpeg.so  -lx264 -L/home/yinjingyu/workspace/lab-project/ffmpegcpl/FFmpeg-Android/lib/-lx264  
- 
- cp $PREFIX/libffmpeg.so $PREFIX/libffmpeg-debug.so  
- 
- arm-linux-androideabi-strip --strip-unneeded $PREFIX/libffmpeg.so  
- 
- echo "build successfully!"
注意，下载下来的ffmpeg源代码需要修改一些源文件才能顺利编译通过，不过这里为了方便给大家贴出来错误信息提供参考，我直接执行编译脚本，编译过程出现错误如下：
![](https://img-blog.csdn.net/20131231140221593?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvY2hhbmNlX3lpbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
分析错误原因是类型冲突，具体我也没深究，直接想了个简单办法：
首先要知道 strtod 是C库函数，用来实现将字符串转化成double类型的数据。ffmpeg可能自己又重新实现了一套自己的函数叫做：avpriv_strtod，两种实现导致了类型冲突。
我的解决方法就是注释掉ffmpeg自己的实现，直接使用c库的函数。具体步骤如下：
步骤1：进入我们交叉编译链的目录，如果你的配置脚本和我一样，那就应该是 /tmp/vplayer（之前我们专门拷贝了一份编译链到该目录下）。在该目录下有一个sysroot目录（用来保存链接时用到的头文件和库），进入sysroot --> include 下，用vi 打开 stdlib.h 文件，添加如下内容：
+++//add by chance_yin 2013.12.27
+++#undef avpriv_strtod
+++#undef strtod
+++#define strtod strtod
步骤2：注释掉 ffmpeg/compat/strtod.c中的所有代码
步骤3：在如下文件： libavutil/eval.c、libavformat/rtmpproto.c 中的开头位置添加如下代码，
#include <stdlib.h>
#undef avpriv_strtod
#undef strtod
#define strtod strtod
步骤4：上面的文件修改完成后，再次执行编译脚本，又出现了下面的错误：
![](https://img-blog.csdn.net/20131231143323343?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvY2hhbmNlX3lpbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
由于实验室的项目用不到语音，所以对于错误：swri_audio_convert_init_arm，我的处理办法是直接注释掉，
剩下的关于 log2_tab.o multiple definition 的错误，就好解决了，
用vi 打开： libavcodec/Makefile 、libavformat/Makefile 、libswresample/Makefile 文件，将其中的
OBJS-$(CONFIG_SHARED) += log2_tab.o  注释掉即可。
好了，源代码的编译算是告以段落了，贴出来编译成功的图，然后你就可以在FFmpeg-android/build/ffmpeg/armv7目录下找到编译完成的 libffmpeg.so 文件了，：
![](https://img-blog.csdn.net/20131231144428359?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvY2hhbmNlX3lpbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![](https://img-blog.csdn.net/20131231144621796?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvY2hhbmNlX3lpbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
结束：
之前实验室项目一直用的是ffmpeg 0.x的版本，昨天把2.0版本的libffmpeg.so整合到项目中发现原来的代码在  avcodec_open2 报错，在ffmpeg相应源文件中打了log才发现是2.0之后，ffmpeg弃用了很多宏，比如:
项目老版本的程序执行到 avcodec_open2 出错， 在源代码此方法的实现中打log，
发现停在了“specific pixfmt not supported 。。。”，google一下，发现在ffmpeg2.0版本中，ffmpeg不再支持pCodecCtxEnc->pix_fmt = PIX_FMT_YUV420P;只要把该值设置成pCodecCtxEnc->pix_fmt = AV_PIX_FMT_YUV420P; 就可以了。
