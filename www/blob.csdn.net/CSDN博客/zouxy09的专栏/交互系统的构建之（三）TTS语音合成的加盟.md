
# 交互系统的构建之（三）TTS语音合成的加盟 - zouxy09的专栏 - CSDN博客


2012年08月29日 16:48:15[zouxy09](https://me.csdn.net/zouxy09)阅读数：8940


**交互系统的构建之（三）****TTS****语音合成的加盟**
zouxy09@qq.com
http://blog.csdn.net/zouxy09
人机交互系统的构建之（一）http://blog.csdn.net/zouxy09/article/details/7919618中提到我的整个交互系统包含以下部分：TLD系统、TTS语音合成、语音识别、手势和语音控制鼠标和键盘、运行前加入手掌的检测（这样就不用鼠标画目标box了）、拳头的检测等等。
**目前已完成：**
1、TLD系统的介绍与编译：
http://blog.csdn.net/zouxy09/article/details/7893022
2、TLD系统工作过程分析：
http://blog.csdn.net/zouxy09/article/details/7893026
3、重写Makefile编译TLD系统:
http://blog.csdn.net/zouxy09/article/details/7919618
4、Linux下鼠标和键盘的模拟控制：
http://blog.csdn.net/zouxy09/article/details/7920253
5、TTS技术简单介绍和Ekho（余音）TTS的安装与编程：
http://blog.csdn.net/zouxy09/article/details/7909154
**本文将完成：**
TTS语音合成模块，具体是移植Ekho（余音）TTS引擎并整合到TLD系统中。
有关于本系统构建的文章结构都会由三个部分来组织，一是该功能模块的介绍和在Linux下简单应用程序的实现；二是将该功能模块整合到交互系统（先以TLD为地基）中去；三是分析目前存在的问题与未来的解决构思。
**一、****TTS****的介绍和****Ekho****（余音）****TTS****的安装与编程**
该部分已经在之前完成了，具体见：
http://blog.csdn.net/zouxy09/article/details/7909154
**二、整合****Ekho****（余音）****TTS****引擎到****TLD****系统中**
**1****、代码文件整合：**
我的人机交互系统的工程目录是mysystem。
拷贝ekho-4.12源码目录下的libekho.a和lib/*到新建的mysystem/lib下，再将ekho-4.12源码目录下的config.h和include/*，还有sonic/*.h、utfcpp/source/*拷贝到mysystem/include目录下。拷贝结束后，大概的代码结构如下：（红色的部分是TLD本身的，其他的是由ekho加入的，有地方有省略。可能看懂下面这些得先看前面的“**目前已完成**”的内容）
.
├──bounding_boxes.txt
├──include
│├──config.h
│├──FerNNClassifier.h
│├──festival
││├──festival.h
││├──fngram.h
││├──......
│├──LKTracker.h
│├──sonic.h
│├──soundtouch
││├──BPMDetect.h
││├──FIFOSampleBuffer.h
││├──....
│├──speech_tools
││├──EST_audio.h
││├──......
│├──TLD.h
│├──tld_utils.h
│├──utf8
││├──checked.h
││├──core.h
││└──unchecked.h
│├──utf8.h
│└──wave.h
├──lib
│├──libekho.a
│├──libestbase.a
│├──libestools.a
│├──libeststring.a
│├──libFerNNClassifier.a
│├──libFestival.a
│├──libLKTracker.a
│├──libSoundTouch.a
│├──libTLD.a
│└──libtld_utils.a
├──Makefile
├──parameters.yml
├──run_tld
└──src
├──FerNNClassifier.cpp
├──LKTracker.cpp
├──run_tld.cpp
├──TLD.cpp
└──tld_utils.cpp
**2****、往****run_tld.cpp****加入语音提示功能代码：**
2.1、头文件
\#include "config.h" // xiaoyi added here--ekho
\#include "ekho.h"  //xiaoyi added here—ekho
2.2、线程函数
因为语音的合成和播放需要一定的时间，所以为了使得在语音播放的同时，TLD系统正常工作，就需要通过创建新的进程或者线程，将语音合成放在新的线程上面来实现：
在main函数前添加：
//xiaoyi added here
void* voicetip(void *arg)
{
string tiptext = *(string *)arg;  //线程传递参数的转换比较麻烦
Ekho voice("Mandarin");
voice.blockSpeak(tiptext);
}
2.3、添加语音提示：
在main函数中需要添加语音提示的地方（例如：系统初始化时，跟踪目标不见时等）添加创建线程的代码：
//xiaoyi added here
pthread_t tid;
string tiptext = "您好，欢迎进入人机交互系统！";
if (pthread_create(&tid, NULL, voicetip, &tiptext))     //create thread
perror("Fail to create thread");
**3****、修改****Makefile****：**
由于编译TTS需要添加不少的头文件和链接库，所以使得这个Makefile比较凌乱和庞大了。
CXX=g++
AR=ar
GPATH=src lib
VPATH=src lib
CPPFLAGS= -O3 -DNDEBUG `pkg-config --cflags opencv` -Iinclude -Iinclude/sonic -Iinclude/speech_tools -Iinclude/soundtouch -Iinclude/festival -Iinclude/utf8 `pkg-config --cflags libpulse-simple`
LDFLAGS=-Llib
LIBS=`pkg-config --libs opencv` -lpthread -lvorbisenc -lvorbis -lm -logg -lmp3lame -lsndfile -lncurses `pkg-config --libs libpulse-simple`
SLIBS=lib/libTLD.a lib/libFerNNClassifier.a lib/libLKTracker.a lib/libtld_utils.a lib/libekho.a lib/libSoundTouch.a lib/libFestival.a lib/libestools.a lib/libeststring.a lib/libestbase.a
OBJECTS=run_tld.o
TARGET=run_tld
$(TARGET):$(OBJECTS)  $(SLIBS)
$(CXX) $^ $(LDFLAGS) $(SLIBS) $(LIBS) -o $@
lib/libTLD.a:TLD.o
$(AR) rc $@ $<
lib/libFerNNClassifier.a:FerNNClassifier.o
$(AR) rc $@ $<
lib/libLKTracker.a:LKTracker.o
$(AR) rc $@ $<
lib/libtld_utils.a:tld_utils.o
$(AR) rc $@ $<
.cpp.o:
$(CXX) -c $< $(CPPFLAGS) -o $@
.c.o:
$(CC) -c $< $(CPPFLAGS) -o $@
clean:
-rm -rf $(TARGETS) *.o lib/*.a
然后，直接\#make，执行\#./run_tld -p parameters.yml即可听到美妙的提示声音了。
**三、目前存在的问题和解决构思**
1、感觉Ekho（余音）TTS附带的普通话不太顺畅和清晰，不过粤语很有味道。这个问题可能得放到后面移植到嵌入式平台的时候再处理了；
2、得考虑是在TLD运行的时候新开一个进程或者线程在运行语音提示功能还是其他方式，这个需要考虑下。之前了解过进程和线程的差别，现在忘了，哎！
3、线程间的控制问题，例如我忽然想要语音停止播放。解决？学习！Keep learning！

