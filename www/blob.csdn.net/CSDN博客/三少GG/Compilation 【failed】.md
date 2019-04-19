# Compilation 【failed】 - 三少GG - CSDN博客
2013年02月28日 14:24:30[三少GG](https://me.csdn.net/scut1135)阅读数：3209

**设置路径！**
**export PATH=$PATH:/home/XXX/toolchain4/pre/bin**
make sure the cross compiler tool chain can be found in the $PATH 这样才可以查找到arm-apple-darwin9-gcc等
# PjSIP Compilation[](http://code.google.com/p/siphon/wiki/Compilation#PjSIP_Compilation)
## Getting pjsip source code[](http://code.google.com/p/siphon/wiki/Compilation#Getting_pjsip_source_code)
1. Get [PjSIP](http://pjsip.org/) from subversion repository:
svn checkout http://svn.pjsip.org/repos/pjproject/trunk pjproject2. Set your [config_site.h](http://trac.pjsip.org/repos/wiki/Getting-Started/Build-Preparation)  (pjlib/include/pj/config_site.h) to the following: #define PJ_CONFIG_IPHONE 1
#include <pj/config_site_sample.h>
This will activate iPhone specific settings in the config_site_sample.h.
## Media device for ipod/iphone[](http://code.google.com/p/siphon/wiki/Compilation#Media_device_for_ipod/iphone)
### pjMedia[](http://code.google.com/p/siphon/wiki/Compilation#pjMedia)
Download [iphonesound.c](http://siphon.googlecode.com/files/iphonesound-20091226.c), and save in *pjmedia/src/pjmedia* directory. 
Complete *config.h* file in pjmedia/include/pjmedia at line 59 with: 
/** Constant for AudioQueue sound backend. */#define PJMEDIA_SOUND_IPHONE_SOUND          3
### Configuration[](http://code.google.com/p/siphon/wiki/Compilation#Configuration)
Complete *aconfigure.ac* file at line **572**with:(并删除掉自带的，比较区别)
  arm-apple-darwin*)
        LIBS="$LIBS -framework CoreAudio -framework CoreFoundation -framework AudioToolbox"
        ac_pjmedia_snd=iphone
        AC_MSG_RESULT([Checking sound device backend...AudioQueue]);;
Complete *os-auto.mak.in* in *pjmedia/build* at line **150 **with following lines:
## iPod/iPhone#
ifeq ($(AC_PJMEDIA_SND),iphone)export SOUND_OBJS = iphonesound.o
INCLUDE_IPHONE=/path/of/include/in/iphone/sdk
export CFLAGS +=-DPJMEDIA_SOUND_IMPLEMENTATION=PJMEDIA_SOUND_IPHONE_SOUND \-D__COREAUDIO_USE_FLAT_INCLUDES__ \-I$(INCLUDE_IPHONE)/AudioToolbox\-I$(INCLUDE_IPHONE)/CoreAudio\-I$(INCLUDE_IPHONE)/CarbonCore\-I$(INCLUDE_IPHONE)/CoreFoundation
endif
其中 /path/of/include/in/iphone/sdk
 类似于/home/XXX/toolchain4/sys/usr/include
Generate configuration script 
autoconf aconfigure.ac > aconfigure
## Compilation[](http://code.google.com/p/siphon/wiki/Compilation#Compilation)
1. Configure pjsip without codec except G711 and GSM: 
./aconfigure **--host=arm-apple-darwin9 \**--disable-speex-aec --disable-speex-codec \--disable-l16-codec --disable-g722-codec  --disable-ilbc-codec
可通过如下指令查看供选的编译选项： The list of customizable features can be viewed by running "./configure --help" command:
 Below is an example of specifying CFLAGS in configure:
$ ./configure  CFLAGS="-O3 -DNDEBUG -msoft-float -fno-builtin"
...
./aconfigure CC="clang" CFLAGS="-ccc-host-triple arm-apple-darwin9" --host=arm-apple-darwin9   --disable-speex-aec --disable-speex-codec --disable-l16-codec --disable-g722-codec --disable-ilbc-codec
有如下错误：
checking whether the C compiler works... no
aconfigure: error: in `/home/grant/pjproject':
aconfigure: error: C compiler cannot create executables
See `config.log' for more details
config.log里面显示：aconfigure:2823: clang -ccc-host-triple arm-apple-darwin9   conftest.c  >&5
**ld: library not found for -lcrt1.10.5.o**
解决办法：
~~正解在这里：http://stackoverflow.com/questions/1365211/error-in-xcode-project-ld-library-not-found-for-lcrt1-10-6-o~~
~~照做，Edit Project Settings－>Build，将MACOSX_DEPLOYMENT_TARGET 由Compiler default修改为10.6。OK。~~

2. make dep
 “
This was the output when you run  :      
checking for arm-elf-linux-gcc... no      
checking for gcc... gcc     
 checking for C compiler default output file name... a.out     
 checking whether the C compiler works... yes      
checking whether we are cross compiling... no
”
3. make
  1).  类似上篇文章，需要增补绝对路径
  2).  报错：
/toolchain4/sys/usr/include/Foundation/NSString.h:282: error: format string argument follows the args to be formatted
其实是上篇文章toolchain搭建遗留的问题，参考：[http://code.google.com/p/iphonedevonlinux/wiki/Installation](http://code.google.com/p/iphonedevonlinux/wiki/Installation)
解决办法： ~~ 原来是sdk4的头文件有问题，对比一下sdk3的，把出问题的语句行的多余部分去掉就可以了。。。。Foundation和UIkit下面的几个头文件也有问题，把出错的语句行的^字符改为*就可以~~
 ” I did a little reading and the '^' token has to do with Apple's block syntax that they have added to the language:
[http://developer.apple.com/mac/articles/cocoa/introblocksgcd.html](http://developer.apple.com/mac/articles/cocoa/introblocksgcd.html)
These extensions must have been added for iOS 4 and the compiler built by this script doesn't support them. “
"format string argument follows the args to be formatted" **Only clang can compile the new Apple headers containing new 'Blocks' feature. gcc can't.**
【必看参考文献】：
1. [http://trac.pjsip.org/repos/wiki/Getting-Started/Autoconf](http://trac.pjsip.org/repos/wiki/Getting-Started/Autoconf)
2.  [http://code.google.com/p/ios-toolchain-based-on-clang-for-linux/wiki/HowTo_en](http://code.google.com/p/ios-toolchain-based-on-clang-for-linux/wiki/HowTo_en)  第二种方法！！！暂未试
小技巧：
若make dep 出现如下error： .pjmedia-arm-apple-darwin9.depend:1:***
 missing separator.Stop.
make[1]:***[pjmedia]Error2
则手动将所提示build目录下的 .**.depend **文件统统删除。重试即可。
