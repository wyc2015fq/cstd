# 3个开源TTS



# 一 安装使用

2014年03月31日 09:45:11 [不动声色的蜗牛](https://me.csdn.net/wangshuxuncom) 阅读数：7176



项目在身，要求分析几款开源的TTS引擎（不需要是中文的），然后选择一个为系统朗读英文文本提供接口。先从eGuideDog的余音（Ekho）开始，Ekho（余音）是一个把文字转换成声音的软件。它目前支持粤语、普通话（国语）、诏安客语和韩语（试验中），英文则通过Festival间接实现。先是找到了它的英文实现方式，不过之前看到过这样一篇文章《7个开源的TTS（文本转语音）系统推荐》，因此从中选择了3个。忘了提项目还要求C/C++为主设计的，因此初步筛选为(1)Festival (2)eSpeak (3)Flite 。目前情况是仅在安装有debian 6的虚拟机上安装使用了下3个软件，进一步分析后续再补上。

## 1 Festival

参考文档：https://wiki.archlinux.org/index.php/Festival

Festival提供了一个通用的框架，用于构建语音合成系统，该系统包含了各种模块示例。它提供了完整的文本转语音的API，可以通过shell、C++静态库等多种方式调用，原生支持Mac OS，支持的语言包括英语和西班牙语；而且系统是使用c++编写，底层调用Edinburgh Speech Tools；

首先阅读它的手册得知，下载需要的压缩包（http://festvox.org/packed/festival/2.1/）：

festival-2.1-release.tar.gz Festival语音合成系统源文件

speech_tools-2.1-release.tar.gz Edinburgh语音工具库

festlex_CMU.tar.gz  festlex_POSLEX.tar.gz词典

festvox_kallpc16k.tar.gz 语料库

前两个解压后需要配置编译，后三个解压后自动解压在festival文件夹中。首先编译speech_tools，进入该文件夹，输入

```shell
./configure   # 配置
make  # 编译
```

其它如在Cygwin、vc下的编译在INSTALL文件中有说明。

然后进入festival文件夹，按照INSTALL文件中的说明进行安装，是可以看到详细的安装说明，以及需要文件夹festival speech_tools的。还是一般的./configure make即可，由于仅是在本地使用，没有了Site initialization这一步，在安装完成后运行bin/festival，如无错误提示出现：

Festival Speech Synthesis System 2.1:release November 2010

Copyright (C) University of Edinburgh, 1996-2010. All rights reserved.

clunits: Copyright (C) University of Edinburgh and CMU 1997-2010

clustergen_engine: Copyright (C) CMU 2005-2010

hts_engine: 

The HMM-based speech synthesis system (HTS)

hts_engine API version 1.04 (http://hts-engine.sourceforge.net/)

Copyright (C) 2001-2010  Nagoya Institute of Technology

2001-2008  Tokyo Institute of Technology

All rights reserved.

For details type `(festival_warranty)'

festival>

可以输入help或者（SayText "Hello World!"）测试。如果出现Can't open /dev/dsp的错误，可以在~/.festivalrc文件写入：

​        (Parameter.set 'Audio_Method 'Audio_Command)

​        (Parameter.set 'Audio_Command "aplay -q -c 1 -t raw -f s16 -r $SR $FILE")

## 2 eSpeak

​        eSpeak是c语言写的一个小型的、开放源码的语音合成系统，支持多种语言。eSpeak使用共振峰合成方法，这可以使提供的语言文件非常小，但是缺点是不如采用基于人录音的语料库的声音平滑自然。该系统支持 Windows、Linux同时已被移植到其它平台，如Solaris and Mac OSX，支持命令行、动态链接库，支持Windows平台上的SAPI5，所以能用于屏幕阅读程序和其他支持Windows SAPI5接口的程序。eSpeak可以将文本转换成音素代码，因此它也可以用于另一个语音合成引擎的前端。它的安装比较简单，下载ZIP压缩包（http://espeak.sourceforge.net/download.html），在Windows上直接安装即可。

## 3 Flite

​        Flite是一个小型、快速的TTS系统，是festival的C版本，可用于嵌入式系统，支持WinCE、Palm OS 。下载压缩包（http://www.speech.cs.cmu.edu/flite/download.html）./configure make即可，安装使用参考文件夹下的README即可，如阅读一段文本，并生成.wav文件

\#flite example example.wav在example文件中有英文文本，输出为example.wav。我这里使用有些问题，example.wav文件在debian中无法播放，用Windows Media player也播不了，结果用QQ影音可以。还有就是直接运行flite读文本出现oss_audio: failed to open audio device /dev/dsp的错误，而且即使是生成语音文件也会出现failed to open file "example" for reading的错误，具体情况还不知道。







# 二）eSpeak的简要分析使用



继续开源TTS分析，只能说是给刚起步的人一点帮助了，毕竟不是专业做这一块的。今天主要先简单介绍TTS过程，然后以eSpeak的动态库编译使用，获得wav文件结束。

前文介绍eSpeak是c语言写的一个小型的、开放源码的语音合成系统，支持多种语言，这里包括汉语，甚至是粤语，可以看看他的博客和演讲【1】。在eSpeak的介绍里特别强调了采用“formant synthesis”（共振峰）合成方法，因此简单了解下TTS的一般过程，由于并非专业，难免有问题，请指出和谅解。

TTS（Text to Speech），也称为语音合成技术，一般分为三个步骤，语言学处理，韵律处理和声学处理。语言学处理就是模拟人对自然语言的理解过程，完成文本规范化、分词、语法分析和语义分析，使处理后的输出能够为计算机所理解，并在其中加入所需要的各种发音提示，包括数字、特殊词汇、断句停顿等；韵律处理则是在文本的理解基础上，规划出音段特征，如音高、音长和音强等，利用韵律标记系统语调、节奏和重音这些韵律特征；声学处理则是在前两部分提供的信息基础上，利用语音语料库，统计与规则结合语境参数、声学参数等信息解决韵律的控制语音的合成。简单来说就是模拟人朗读的过程，从一段文本的输入进行分词、理解开始，结合人的朗读规律、发声特点对文本进行标记，作为参数在声音输出时产生模拟人声停顿变化等的过程。eSpeak所说的共振峰合成就是最后的语音产生过程。共振峰合成是最典型的基于声学模型的合成技术，其设计的理论基础是语音产生的源-滤波理论，即来自肺部的气流通过声门后，被看成具有一定谱结构的声源S(f)，也是声道滤波器的激励信号，声道被看成足一个线性的滤波器，其转移凼数T(f)是口和鼻处的体积速度U(f)和声源s(f)之比，由于实际上总是存离口腔有一段距离的地方得到语音p(f)，因此必须考虑到口腔的辐射效应R(f)。（更加详细的资料，自己查找）而我们需要知道共振峰模型受参数提取的影响很大，参数不准确会导致合成音质下降。而波形拼接的合成声音音质最好，但合成新的声音需要事先录制新的说话人的声音，共振峰合成只需在频域对声音作修改。这样的修改包括；振峰偏移、带宽修改、共振峰强度的修改和频带斜坡的修改。Klatt设计了一个串／并联混合型共振峰合成器，可以设置多到八个共振峰．并有单独的滤波器来模拟鼻腔和气管的共振。对声源可做调整或多种选择，以模拟不同的嗓音。（摘自【2】【3】）

从上面的描述过程大致可以知道TTS的一般过程，而共振峰合成技术是一种语音合成技术中的参数合成方法，通过模拟人的发声过程，建立发声模型，利用对人的浊音源、元音源等声源发声进行参数提取、估计，供后期的共振模拟使用。可见像我这种非专业人士只是知道会有大量的参数调整控制，看来想对此软件进行发声控制使用可以，修改部分规则、扩充语言、词典需要大量时间，再进一步想要优化流畅度之类的，估计就非常困难了。那么下面咱们说怎么利用其提供的源码包编译Windows上的动态库，并使用，Linux上的应该类似。

本人的环境是Windows XP，使用VC6.0编译，源码包espeak-1.46.02-source，只为了产生wav格式文件，因此没用到PortAudio库，但是编译命令行版本时是直接可以发声的。编译过程非常简单，在源码包下\platforms\windows\windows_dll文件夹中有!ReadMe.txt文件告知。从源码包根目录下的src文件夹中拷贝源码文件到platforms\windows\windows_dll\src中，但不拷贝speak_lib.h  speech.h  StdAfx.h  stdint.h文件。而该eSpeak产生的windows DLL库提供的接口在speak_lib.h中定义看到。我的编译遇到了'espeak_SetParameter' : redefinition; different linkage的重定义错误，只要在定义出前添加ESPEAK_API即可。

使用代码如下，也可参看【4】，可以把word换成nihao试试。



```cpp
#include <stdio.h>  
#include "speak_lib.h"   
#pragma comment(lib, "espeak_lib.lib")  
  
FILE *f_wave = NULL;  
  
void Write4Bytes(FILE *f, int value)  
{//=================================  
// Write 4 bytes to a file, least significant first  
    int ix;  
  
    for(ix=0; ix<4; ix++)  
    {  
        fputc(value & 0xff,f);  
        value = value >> 8;  
    }  
}  
  
static int OpenWaveFile(const char *path, int rate)//rate 22050  
//=================================================  
{  
    // Set the length of 0x7ffff000 for --stdout  
    // This will be changed to the correct length for -w (write to file)  
    static unsigned char wave_hdr[44] = {  
        'R','I','F','F',0x24,0xf0,0xff,0x7f,'W','A','V','E','f','m','t',' ',  
        0x10,0,0,0,1,0,1,0,  9,0x3d,0,0,0x12,0x7a,0,0,  
        2,0,0x10,0,'d','a','t','a',  0x00,0xf0,0xff,0x7f};  
  
    if(path == NULL)  
        return(2);  
  
    if(strcmp(path,"stdout")==0)  
    {  
#ifdef PLATFORM_WINDOWS  
// prevent Windows adding 0x0d before 0x0a bytes  
        _setmode(_fileno(stdout), _O_BINARY);  
#endif  
        f_wave = stdout;  
    }  
    else  
        f_wave = fopen(path,"wb");  
  
    if(f_wave != NULL)  
    {  
        fwrite(wave_hdr,1,24,f_wave);  
        Write4Bytes(f_wave,rate);  
        Write4Bytes(f_wave,rate * 2);  
        fwrite(&wave_hdr[32],1,12,f_wave);  
        return(0);  
    }  
    return(1);  
}   //  end of OpenWaveFile  
  
  
static void CloseWaveFile()  
//=========================  
{  
   unsigned int pos;  
  
   if((f_wave == NULL) || (f_wave == stdout))  
      return;  
  
   fflush(f_wave);  
   pos = ftell(f_wave);  
  
    fseek(f_wave,4,SEEK_SET);  
    Write4Bytes(f_wave,pos - 8);  
  
    fseek(f_wave,40,SEEK_SET);  
    Write4Bytes(f_wave,pos - 44);  
  
  
   fclose(f_wave);  
   f_wave = NULL;  
  
} // end of CloseWaveFile  
  
int SynthCallback(short *wav, int numsamples, espeak_EVENT *events)  
{  
    // 你可以根据源码程序里编写这部分代码实现生成语音文件功能  
    int wavDataIndex = 0;  
    if(wav == NULL)  
        return 1;  
    else{  
        while(wav[wavDataIndex+numsamples] != NULL)  
   wavDataIndex++;  
        fwrite(wav,sizeof(short),numsamples,f_wave);  
        return(0);  
    }  
}  
  
void main()   
{  
    // TODO: Add your control notification handler code here  
    char word[] = "hello world!";  
    espeak_Initialize(AUDIO_OUTPUT_SYNCHRONOUS, 0, ".", 0);  
    espeak_SetSynthCallback(SynthCallback);   // 设置回调函数  
    espeak_SetVoiceByName("en");  
    OpenWaveFile("test.wav",22050);  
    espeak_Synth(word, 100, 0, POS_CHARACTER, 0, espeakCHARS_UTF8, NULL ,NULL);  
    espeak_Synchronize();  
    espeak_Terminate();  
    CloseWaveFile();  
}  
```



注意拷贝espeak_lib.dll和espeak_lib.lib，还有就是源码包中的espeak-data要拷贝到应用目录下。其中代码都是从eSpeak的命令行源程序中拷贝出来的，因此有些混乱请谅解。而提供的接口自己查看speak_lib.h中的说明就懂意思了。



参看：
【1】开源中国 黄冠能博客http://my.oschina.net/hgneng
【2】语音合成技术的研究与发展-黄南川等
【3】共振峰语音合成算法研究和实现-赵亮
【4】跨平台TTS eSpeak Windows开发 http://www.cnblogs.com/hicjiajia/archive/2011/02/02/1948882.html