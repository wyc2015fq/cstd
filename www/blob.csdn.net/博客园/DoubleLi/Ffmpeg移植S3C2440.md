# Ffmpeg移植S3C2440 - DoubleLi - 博客园






**Ffmpeg移植过程：**

     FFmpeg是一个开源免费跨平台的视频和音频流方案，属于自由软件，采用LGPL或GPL许可证。它的移植同样遵循LGPL或GPL移植方法：configure、make、make insatll。



1.下载ffmpeg开源库（ffmpeg-0.5.tar.bz2）放到相应文件夹。

#tar jxvf ffmpeg-0.5.tar.bz2

#cd ffmpeg-0.5



2.配置编译选项

#./configure --cross-prefix=/usr/local/arm/3.4.1/bin/arm-[Linux](http://lib.csdn.net/base/linux)- --enable-cross-compile --target-os=[linux](http://lib.csdn.net/base/linux) --cc=arm-linux-gcc --arch=arm --prefix=/usr/local/ffmpeg --enable-shared --disable-static --enable-gpl --enable-nonfree --enable-ffmpeg --enable-ffplay --enable-ffserver --enable-swscale --disable-vhook --enable-pthreads --disable-network --disable-ipv6 --enable-libamr-nb --enable-libamr-wb --enable-libfaac --enable-libfaad --enable-libmp3lame --disable-armv5te --disable-armv6 --disable-armv6t2 --disable-yasm --disable-stripping --disable-optimizations --extra-cflags=-I/usr/local/ffextra/include --extra-ldflags=-L/usr/local/ffextra/lib

交叉编译器：arm-linux-gcc3.4.1

--cflags：ffmpeg第三方库头文件声明

--extra-ldflags：第三方库库文件路径



3.

#make



4.

#make install

把编译生成的库拷贝到2440开发板，配置环境变量。



**编译过程错误：**

1：libavcodec/arm/dsputil_arm_s.S
     libavcodec/arm/dsputil_arm_s.S: Assembler messages:
     libavcodec/arm/dsputil_arm_s.S:25: Error: unknown pseudo-op: `.eabi_attribute'
     libavcodec/arm/dsputil_arm_s.S:641: Error: bad instruction `push {r4-r10}'
     libavcodec/arm/dsputil_arm_s.S:724: Error: bad instruction `pop {r4-r10}'
     make: *** [libavcodec/arm/dsputil_arm_s.o] 错误 1

解决：在make的时候有一些asm指令找不到 
ffmpeg-0.5/libavcodec/arm/asm.S
24行： .eabi_attribute 24, /val改为@.eabi_attribute 24, /val 
28行： .eabi_attribute 25, /val改为@.eabi_attribute 25, /val

ffmpeg-0.5/libavcodec/arm/dsputil_arm_s.S
641行： push　　　　　　{r4-r10} 改为 stmfd　　　　　 sp!, {r4-r10} @ push　　　　　　{r4-r10} 
724行： pop　　　　　　 {r4-r10} 改为  ldmfd　　　　　 sp!, {r4-r10} @ pop　　　　　　 {r4-r10}

2：/home/liao/ffmpeg-0.5/libavcodec/libavcodec.so: undefined reference to `dlerror'
     /home/liao/ffmpeg-0.5/libavcodec/libavcodec.so: undefined reference to `dlclose'
     /home/liao/ffmpeg-0.5/libavcodec/libavcodec.so: undefined reference to `dlopen'
     /home/liao/ffmpeg-0.5/libavcodec/libavcodec.so: undefined reference to `dlsym'

解决：缺少libdl库文件，在Makefile文件中添加库文件
68行：ffserver_g$(EXESUF): FF_LDFLAGS += $(FFSERVERLDFLAGS)后面添加FF_LDFLAGS +=-ldl

至此，ffmpeg可以顺利编译！！



注意：

ffmpeg中ffplay需要SDL支持，如需编译ffplay，则需下面步骤：

     编译SDL，将其include、lib文件拷贝至第三方库路径（/usr/local/ffextra）

     在configure之前修改configure文件2042行:

     SDL_CONFIG="${cross_prefix}sdl-config"改为SDL_CINFIG="/usr/local/sdl/bin/sdl-config"

ffmpeg针对mp3、wma音频格式解码为浮点型，难以在arm上流畅运行。









