
# TTS技术简单介绍和Ekho（余音）TTS的安装与编程 - zouxy09的专栏 - CSDN博客


2012年08月26日 15:27:59[zouxy09](https://me.csdn.net/zouxy09)阅读数：22811


**TTS****技术简单介绍和****Ekho****（余音）****TTS****的安装与编程**
zouxy09@qq.com
http://blog.csdn.net/zouxy09
**一、****TTS****技术简单介绍：**
TTS技术，TTS是Text To Speech的缩写，即“从文本到语音”。它将计算机自己产生的、或外部输入的文字信息转变为可以听得懂的、流利的汉语口语（或者其他语言语音）输出的技术，隶属于语音合成（SpeechSynthesis）。
**TTS****的基本组成：**
**（****1****）文本分析**
对输入文本进行语言学分析（主要模拟人对自然语言的理解过程），逐句进行词汇的、语法的和语义的分析，以确定句子的低层结构和每个字的音素的组成，包括文本的断句、字词切分、多音字的处理、数字的处理、缩略语的处理等。使计算机对输入的文本能完全理解，并给出后两部分所需要的各种发音提示。
**（****2****）韵律建模**
为合成语音规划出音段特征，如音高、音长和音强等，使合成语音能正确表达语意，听起来更加自然。
**（****3****）语音合成（核心模块）**
根据韵律建模的结果，把处理好的文本所对应的单字或短语的语音基元从语音合成库中提取，利用特定的语音合成技术对语音基元进行韵律特性的调整和修改，最终合成出符合要求的语音。
**TTS****的工作原理如下：**
例如：你输入了：“今天吃饭了没有啊？”，想让电脑读出来。那么我们会怎么做呢？
**（1）最简单的TTS：**
就是电脑把每一个字分开，对于“我”字，就通过查询把“我”字对应的语音基元读出来，这样同样方式处理剩余的字，那么这句话就读出来了。但是很明显，这样读出来的语音肯定不够自然。
另外，每个字对应的语音基元哪里来呢？人工苦力活，就是请人把每个汉字读出来，然后录下来。你会问，岂不要录制6千多个汉字的语音？幸运的是，汉语的音节很少，很多同音字。所以我们最多只是需要录制： 声母数×韵母数×4，（其实不是每个读音都有4声），这样算来，最多只需要录制几百个语音就可以了。当然，也有非人工的方法，如eSpeak使用共振峰合成方法，这可以使提供的语言文件非常小，但是缺点是不如采用基于人录音的语料库的声音平滑自然。另外，在合成的时候需要一张汉字对应拼音的对照表（实际上就是字典），汉字拼音输入法也依赖这张表。
上面单字朗读做出来的TTS效果也还可以，特别是朗读一些没有特别含义的如姓名，家庭住址，股票代码等汉语句子，听起来足够清晰。这要归功于我们伟大的母语通常都是单音节，从古代的时候开始，每个汉字就有一个词，表达一个意思。而且汉字不同于英语，英语里面很多连读，音调节奏变化很大，汉字就简单多了。
当然，你仍然要处理一些细节，比如多音字，把“银行”读成“yin xing”就不对了；再比如，标点符号的处理，数字、字母的处理。
**（2）稍微复杂点的TTS：**
如果要把TTS的效果弄好一点，再来点力气活，把基本的词录制成语音，如常见的两字词，四字成语等，再做个词库和语音库的对照表，每次需要合成时到词库里面找。这样以词为单位，比以字为单位，效果自然是好多了。当然，这就涉及到分词的技术，要把复杂的句子断成合理的词序列。另外，为了追求更好的效果，还有进化到以常用句子为单位来录音了。当然，这就得需要更大的工作量了，因为你需要读单字、词、成语、句子等等。而且语音数据也会庞大起来。
再来说说文本的标识，对于复杂文本，某些内容程序没有办法处理，需要标识出来。比如，单纯的数字“128”，是应该念成“一百二十八”还是“一二八”？解决办法通常是加入XML标注，如微软的TTS："<context ID = "number_cardinal">128</context>"念成“一百二十八”，"<context ID = "number_digit">128</context>"将念成“一二八”。TTS引擎可以去解释这些标注。遗憾的是，语音XML标注并没有形成大家都完全认可的标准，基本上是各自一套。
**二、****Ekho****（余音）****TTS****的安装与编程：**
目前，TTS还是比较多的，有商业的，也有开源的，当然，前者的性能肯定要好一些，但对于业余非专业应用来说，开源的就足够了。下面就介绍下如何在ubuntu 12.04下安装和编程Ekho这个TTS。
Ekho（余音）是一个把文字转换成声音的软件。它目前支持粤语、普通话（国语）和韩语（试验中），英文则通过Festival间接实现。网址如下（提供下载）：
http://www.eguidedog.net/ekho.php
Festival提供了一个通用的框架，用于构建语音合成系统，该系统包含了各种模块示例。它提供了完整的文本转语音的API，可以通过shell、C++静态库等多种方式调用，原生支持Mac OS，支持的语言包括英语和西班牙语；而且系统是使用c++编写，底层调用Edinburgh Speech Tools（Edinburgh语音工具库）；
Ekho支持Linux和Windows平台，使用时需要同时下载源文件和声音数据，解压到"jyutping"或"pinyin"。（以jyutping开头的声音数据是粤语，而以pinyin开头的声音数据是国语。文件名中的44100和16000表示的是采样率。采样率高意味着声音质量高。）
**1****、下载****ekho-4.12.tar.bz2****解压：**
http://www.eguidedog.net/ekho.php该网址下载即可，开源码包已经包含了声音数据。
解压后看到，ekho源码涉及几个部分：
（1）include/soundtouch: SoundTouch是一个开源的音频处理库，主要实现包含变速、变调、变速同时变调等三个功能模块，能够对媒体流实时操作，也能对音频文件操作；
（1）include/festival:通过Festival间接实现英文阅读；
（2）Include/speech_tools：Edinburgh Speech Tools（Edinburgh语音工具库）；
（3）speechd-api：speech-dispatcher API（平台独立的语音合成API）；
（4）sonic：一个语音变速的算法实现；
（5）ekho-data：ekho的字典文件和声音数据，上面说到了，TTS是通过文本分析，再查找字典，找到对应的声音数据，然后通过语音合成出来的；
（6）Utfcpp：处理Unicode编码字符串的库。
（7）sr-convert：改变.WAV声音文件采样率sample rate的库。
然后就是src（ekho的主要实现）和lib（上面说的这几部分的静态链接库*.a）了，就没什么特别的了。
**2****、安装依赖库：**
\#apt-get install libsndfile1-dev libpulse-dev libncurses5-dev libgtk2.0-dev libvorbis-dev libmp3lame-dev
**3****、配置、编译及安装：**
如果需要阅读英语，则需要配置编译Festival，在configure选项中加上--enable-festival，如果需要编译speech-dispatcher API和支持屏幕阅读的话，就加上--enable-speechd。（speech-dispatcher是一个平台独立的语音合成API，其框架是基于client/server模型的，两者通信是通过TCP连接使用Speech Synthesis Independent Protocol (SSIP)协议通信或者接口库通信的。）
$ ./configure
$ make
$ sudo make install
安装后，ekho这个可执行文件和声音数据ekho-data都会复制到系统目录下。但是好像头文件（除ekho.h外）和库等就没有复制进去，所以编程链接时需要注意指定位置。
**4****、安装成功后，运行测试：**
\# ekho '123'
成功的话，系统会把“一百二十三”读出来。
**5****、编程：**
编程需在源代码目录下，也就是ekho-4.12下，因为依赖的头文件都在该文件夹里，并没有安装到/usr/include等目录下；
Echo支持的API，具体见ekho.h。
编程代码：ekhotest.cpp
/* ekhotest.cpp */
\#include "config.h" // this file is generated when building Ekho
\#include "ekho.h"
using namespace ekho;
int main(int argc, char **argv)
{
/*ekho支持粤语Cantonese、普通话Mandarin国语和韩语Korean，在这指定*/
Ekho wong("Mandarin");
wong.blockSpeak("你好，欢迎！");
return 0;
}
编译的话，因为依赖的库比较多，所以最好在该目录下写个sheel脚本complize.sh，如下：
g++ ekhotest.cpp libekho.a lib/libSoundTouch.a lib/libFestival.a lib/libestools.a lib/libeststring.a lib/libestbase.a -I. -Iutfcpp/source -Isonic -lpthread -lvorbisenc -lvorbis -lm -logg -lmp3lame -lsndfile -lncurses `pkg-config --libs libpulse-simple` `pkg-config --cflags libpulse-simple`
更改运行属性：
\#chmod 777 complize.sh
然后：
\#./complize.sh
就生成运行./a.out即可
**6****、写****Makefile****管理**
个人感觉上面的编译过程有点乱，为了让ekho可以更好的嵌入其他应用程序编程（得清晰明白它的编译需要哪些头文件和库），我就简单的将其进行统一，将编译ekho所需的头文件和库分别放在一个目录中，并通过Makefile来编译自己写的ekho程序。
先新建一个文件夹program_tts，再在这里文件夹中新建lib和include两个文件夹。然后拷贝ekho-4.12源码目录下的libekho.a和lib/*到新建的lib下，再将源码目录下的config.h和include/*，还有sonic/*.h、utfcpp/source/*拷贝到新建的include目录下。
自己的编程文件ekhotest.cpp和Makefile在program_tts目录下。
Makefile文件如下：
CXX=g++
CFLAGS=-Iinclude -Iinclude/sonic -Iinclude/speech_tools -Iinclude/soundtouch -Iinclude/festival -Iinclude/utf8 `pkg-config --cflags libpulse-simple`
LIBS=-lpthread -lvorbisenc -lvorbis -lm -logg -lmp3lame -lsndfile -lncurses `pkg-config --libs libpulse-simple`
SLIBS=lib/libekho.a lib/libSoundTouch.a lib/libFestival.a lib/libestools.a lib/libeststring.a lib/libestbase.a
OBJECTS=ekhotest.o
TARGETS=testekho
$(TARGETS):$(OBJECTS)
$(CXX) $^ $(SLIBS) $(LIBS) -o $@
.cpp.o:
$(CXX) -c $< $(CFLAGS) -o $@
.c.o:
$(CC) -c $< $(CFLAGS) -o $@
clean:
-rm -rf $(TARGETS) $(OBJECTS)
这样，修改程序，直接make即可。

