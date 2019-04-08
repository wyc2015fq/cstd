# 开源TTS（Text To Speah）的选择和使用



> TTS是Text To Speech的缩写，即“从文本到语音”，是人机对话的一部分，让机器能够说话。
>
> TTS是语音合成应用的一种，它将文件内容或应用上的文字等，如应用菜单或者网页，转换成自然语音输出。
>
> TTS不仅能帮助有视觉障碍的人阅读计算机上的信息，更能增加文本文档的可读性。

### 一、比较流行的开源TTS项目

以下信息来自：[TTS open source project](http://www.findbestopensource.com/tagged/text-to-speech?start=0)

| MARY -- Text-to-Speech System                                | MARY is an open-source, multilingual Text-to-Speech Synthesis platform written in Java. It supports German, British and American English, Telugu, Turkish, and Russian. |
| ------------------------------------------------------------ | ------------------------------------------------------------ |
| SpeakRight Framework -- Helps to build Speech Recognition Applications | SpeakRight is an Java framework for writing speech recognition applications in VoiceXML. Dynamic generation of VoiceXML is done using the popular StringTemplate templating framework. Although VoiceXML uses a similar web architecture as HTML, the needs of a speech app are very different. SpeakRight lives in application code layer, typically in a servlet. The SpeakRight runtime dynamically generates VoiceXML pages, one per HTTP request. |
| Festival -- Speech Synthesis System                          | Festival offers a general framework for building speech synthesis systems as well as including examples of various modules. It offers full text to speech through a APIs via shell and though a Scheme command interpreter. It has native support for Apple OS. It supports English and Spanish languages. |
| FreeTTS -- Speech Synthesizer in Java                        | FreeTTS is a speech synthesis system written entirely in the Java. It is based upon Flite, a small run-time speech synthesis engine developed at Carnegie Mellon University. Flite is derived from the Festival Speech Synthesis System from the University of Edinburgh and the FestVox project from Carnegie Mellon University. FreeTTS supports a subset of the JSAPI 1.0 java speech synthesis specification. |
| Festvox -- Builds New Synthetic Voices                       | The Festvox project aims to make the building of new synthetic voices more systemic and better documented, making it possible for anyone to build a new voice. Festvox is the base for most of the Speech Synthesis libraries. |
| Kaldi -- Speech Recognition Toolkit                          | Kaldi is a Speech recognition research toolkit. It is similar in aims and scope to HTK. The goal is to have modern and flexible code, written in C++, that is easy to modify and extend. |
| eSpeak -- Text to Speech                                     | eSpeak is a compact open source software speech synthesizer for English and other languages. eSpeak uses a formant synthesis method. This allows many languages to be provided in a small size. It supports SAPI5 version for Windows, so it can be used with screen-readers and other programs that support the Windows SAPI5 interface. It can translate text into phoneme codes, so it could be adapted as a front end for another speech synthesis engine. |
| Flite -- Fast Run time Synthesis Engine                      | Flite (festival-lite) is a small, fast run-time synthesis engine developed at CMU and primarily designed for small embedded machines and/or large servers. Flite is designed as an alternative synthesis engine to Festival for voices built using the FestVox suite of voice building tools. |

 

### 二、开源项目的选择

基于需求，选择C/C++的开源项目，主要有以下三个：

（1）Festvial

> 它提供一个通用框架来建立语音合成系统，而且包含了多种模块的示例。它提供了完整的从文本到语音的API。它原生支持Apple OS，支持英语和西班牙语。

（2）eSpeak

> 它是一个开源语音合成软件，支持英语和其他多种语言。使用共振峰合成的方法。这就使得提供的很多语言文件很小。它位windows支持SAPI5版本，所以也能用于那些支持Windows SAPI5接口的屏幕阅读和其他程序。它可以翻译文本为音速代码，所以能用于另一种语音合成引擎的前端。

（3）Flite

> Festival-lite版，是一种小型，反应快速的合成引擎，由CMU开发，主要设计用于小的嵌入式机器或大服务器。它是一种可代替Festival的语音合成引擎，使用FestVix语音建立工具套件来建立语音库。

下面将对这三个项目的使用分别进行介绍。环境：NMware Workstation + Lubuntu-16.04.2 32位

### 三、开源TTS项目的使用（一） Festival

1、下载

2、编译

3、使用

4、问题与解决

### 四、开源TTS项目的使用（二） eSpeak

1、下载

[espeak.sourceforge.net](http://espeak.sourceforge.net/)

espeak依赖portaudio进行播放，因此还要下载

<http://www.portaudio.com/download.html>

2、编译

eSpeak编译：

```
cd src
make
make install
```

protaudio编译：

<http://portaudio.com/docs/v19-doxydocs/compile_linux.html>

编译后生成在：lib/.libs/ 目录下，为其制作软链接

```
ln -s lib/.libs/libportaudio.so.2.0.0 /usr/lib/libportaudio.so
```

3、使用

```
espeak "hello world" -w hello.wav
```

4、问题与解决

（1）编译问题

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
g++  -o speak speak.o compiledict.o dictionary.o intonation.o readclause.o setlengths.o numbers.o synth_mbrola.o synthdata.o synthesize.o translate.o mbrowrap.o tr_languages.o voices.o wavegen.o phonemelist.o klatt.o sonic.o -lstdc++ -lportaudio -lpthread 
wavegen.o：在函数‘WavegenOpenSound() [clone .part.2]’中：
wavegen.cpp:(.text+0x23a)：对‘Pa_StreamActive’未定义的引用
wavegen.o：在函数‘WavegenCloseSound()’中：
wavegen.cpp:(.text+0x552)：对‘Pa_StreamActive’未定义的引用
collect2: error: ld returned 1 exit status
Makefile:105: recipe for target 'speak' failed
make: *** [speak] Error 1
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

（1）解决方法

```
cp portaudio19.h portaudio.h
make clean
make
```

5、应用举例

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
#include "./speak_lib.h"  // espeak头文件
#include <string.h>
#include <unistd.h>

int main(int argc, char **argv)
{
    char word[] = "吃葡萄不吐葡萄皮";
    espeak_Initialize(AUDIO_OUTPUT_PLAYBACK, 0, NULL, 0);
    espeak_SetVoiceByName("zh+f2");
    espeak_Synth(word, strlen(word) + 1, 0, POS_CHARACTER, 0,
                    espeakCHARS_UTF8, NULL, NULL);
    sleep(3);
    espeak_Terminate();
}
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

如果需要将文字转的wav语音文件保存下来，需要实现callback。如需具体代码示例，可发私信。

 

### 五 、开源TTS项目的使用（三） Flite

1、下载

<http://www.speech.cs.cmu.edu/flite/index.html>

2、编译

```
sodu su
./configure
make
make install
```

3、使用

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
flite -t hello 
    语音读出“Hello world”
flite "hello world."
    语音读出“Hello world”
flite hello
    语音读出文件“hello”的内容
flite -f "hello world"
    语音读出文件“hello world”的内容
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

4、问题

（1）问题

```
root@lubuntu:# flite "hello world"
oss_audio: failed to open audio device /dev/dsp
```

（1）解决

ls /dev/dsp  发现该目录并不存在，搜索了解到flite使用oss框架进行语音播放。

```
root@lubuntu:# cat /proc/asound/version 
Advanced Linux Sound Architecture Driver Version k4.8.0-36-generic.
```

说明当前系统使用ALSA音频驱动框架。尝试：

方法一：
        安装程序padsp，可以把对OSS的请求派发到ALSA
        apt install pulseaudio-utils
        padsp flite
        失败!!!

方法二：
        sudo apt-get install pulseaudio
        sudo apt-get install libpulse-dev
        sudo apt-get install osspd
        成功!!!   能看到/dev/dsp目录了，但是依然提示failed to open!!!

最后发现将vmware的声卡设备连接上，就不报错，能正常出声了！( *¯ㅿ¯*)







[好文要顶](javascript:void(0);) [关注我](javascript:void(0);) [收藏该文](javascript:void(0);) [![img](https://common.cnblogs.com/images/icon_weibo_24.png)](javascript:void(0);) [![img](https://common.cnblogs.com/images/wechat.png)](javascript:void(0);)