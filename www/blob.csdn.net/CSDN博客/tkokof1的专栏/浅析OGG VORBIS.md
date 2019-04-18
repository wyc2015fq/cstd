# 浅析OGG VORBIS - tkokof1的专栏 - CSDN博客

2009年08月05日 14:25:00[tkokof1](https://me.csdn.net/tkokof1)阅读数：6263




虽说OGG VORBIS的登场并未晚过前辈MP3过多时日，其的出现也正是为了阻拦MP3一路高歌猛进的强劲步伐，可是至少在中国这块破解软件的乐土，MP3虚有其表的版权以及真真切切的封闭却并未驱使人们义无反顾的投身OGG VORBIS的怀抱，尽管他完全的开源，尽管他彻底的免费，虽说在Linux平台上Ogg Vorbis的确流行，但是在中国这片Windows大放异彩的土地上，能够耳闻Ogg Vorbis名号的人估计也不在多数，笔者曾经在网上搜索过Ogg Vorbis相关的文章，大抵都是一些浮光掠影的简介，有些内容的也大多以英文写就，读来确有不便，再加上我在学习游戏编程的过程中对Ogg Vorbis有过一点浅显的认识，所以萌生了写作此文的想法，但文章终究是我的个人看法，期中讹误在所难免，希望大家对于其中不便于理解或者认为错误的地方一一指出，以使我进一步修正改进，总的来说我必须为那句“古语”负责：贻笑大方事小，误人子弟事大：）

  那么闲话少叙,我们开始正题：

之一 Ogg Vorbis概述

Ogg Vorbis 是一种音频压缩格式，类似于MP3等现有的音乐格式, 但有一点不同的是，它是完全免费、开放和没有专利限制的,而对其的维护工作, 则是由Xiph.Org基金会来进行。

(官方网站：[http://www.xiph.org](http://www.xiph.org/vorbis))

  Ogg Vorbis文件的扩展名是.OGG, 如今的主流播放器基本上都可播放，(有兴趣的读者可以自己下一首用千千静音播放来听听: ) )而且，该文件格式还在不断地进行大小和音质的改良 : )。

就音质而言, Ogg Vorbis 虽然使用了与MP3相比完全不同的数学原理，但在当前的聆听测试中，同样位速率编码的Ogg Vorbis 和 MP3 文件基本上具有相同的声音质量 : )。

之二 Ogg Vorbis使用

  Ogg Vorbis 的API分为四个各司其职的类库：Ogg类库，Vorbis类库，Vorbisenc类库以及Vorbisfile类库。

  你可能对于Ogg Vorbis这个名字有些疑惑不解，但其本质意义却与MP3如出一辙。就像MP3本身是RIFF文件的一种编解码器一样，Vorbis也便是Ogg文件下的一种编解码器(尽管只有这么一种...)，所以Ogg的意义便是Xiph组织为压缩的音视频数据所定义的一个框架，而Vorbis便是这个框架下的音频编解码器。再谈到上述Ogg Vorbis的四个类库，其中Ogg类库包含了处理Ogg Vorbis文件的通用例程，而Vorbis类库则包含了一些使用Vorbis进行解码时所必需的基础函数。

  另外关于Vorbisenc类库则用以辅助编码Ogg Vorbis文件，而Vorbisfile类库则是综合使用Ogg类库以及Vorbis类库，并为我们提供了简单的接口以方便的处理Ogg Vorbis文件，功能可谓相当实用。

  至于Ogg Vorbis的安装，可以在Ogg Vorbis的官方英文网站：[http://www.xiph.org](http://www.xiph.org/vorbis)上免费下载，不过这里会有一个问题，虽然在此可以下载到最新的类库(libogg及libvorbis)，但是这些文件下载后都必须在本地进行手工创建之后才能使用，步骤并不简单，所以在此我选择直接使用SDK的方法，但是由于网上此类SDK版本古老的缘故，有些甚至不能完全支持Windows平台下几款主流的C/C++编译器，如MinGW( 我便不幸遇到过这个问题：(  )，不过问题解决起来也并不困难，错误的原因大抵是出在os_types.h这个头文件上，如果你在使用MinGW(其它编译器类似)编译Ogg Vorbis程序时遇到了不能正确识别类型的问题，请在os_types.h文件中的适当位置添加如下代码:(MinGW为例)

#  elif defined(__MinGW32__)

     typedef short ogg_int16_t;                                                                             

     typedef unsigned short ogg_uint16_t;                                                                   

     typedef int ogg_int32_t;                                                                               

     typedef unsigned int ogg_uint32_t;                                                                     

     typedef long long ogg_int64_t;                                                                         

     typedef unsigned long long ogg_uint64_t;  

或者干脆用最新版的os_types.h文件进行替换。但是此番改动之后也并不能百分百保证能够正确编译你的程序，相关文件也许仍需进行细微的变更，所以我整理了一份较新版的SDK，不过仅支持MS Visual编译平台，相应文件请见后链接：)

包含了正确的头文件，链接上正确的DLL，下面便仅剩得使用Ogg Vorbis的API了，个中函数在Ogg Vorbis所提供的帮助文档中都有很好的论述，有兴趣的读者可以进一步的学习，而在此篇粕文中，我会在第三部分辅以一个简单的实例来进行一番浅显的介绍，有兴趣的读者可以进一步参考相应的帮助文档。

之三 Ogg Vorbis实践

对于一个Ogg Vorbis文件，我们能想到的第一个处理便是播放，这也是它最重要也是最基本的功能，以下的一个实例也便是围绕此点展开，其中的代码我尽量进行注释与评说，但是限于篇幅，有些内容仍需读者参看相关文档，不方便之处请各位读者谅解:)

  第一，自然是选择语言及相应编译器，在此我选择了C++(当然C也不错，但我本人对C++较为熟悉，所以...),编译器方面为了简洁，我本想采用MinGW，但是由于未能找到对应的Ogg Vorbis类库的原因，我还是选择了当今主流编译器——微软的Visual系列(严格来讲其实是个IDE:))，不过使用它也有一些缺陷：一是由于其对C++标准的支持有限，实在有些我行我素；二是对于示例程序来说也并不需要如此庞大的IDE，用来颇有些杀鸡用牛刀的感觉。不过他的编译效率和质量自然有目共睹，Win32平台其他的C/C++编译平台在此方面可谓无出其右。

  第二，便是对于程序做一个简单的设计，由于当今声卡底层主要支持的声音格式不过两种：Wave和Midi，而Ogg Vorbis同MP3一样，都是为了弥补Wave格式臃肿的体型而出现的，所以我们可以写一个类，用于将Ogg Vorbis格式解码成Wave格式的文件，然后便可以轻松的调用Win32平台的API进行播放了:)

  那么，事不宜迟，让我们马上开始:

//DealOggVorbis.h

#ifndef DEALOGGVORBIS_H

#define DEALOGGVORBIS_H

#include <vector>//为了使用STL中的vector

#include <list>//为了使用STL中的list

#include <vorbis/codec.h>//包含相应的库文件

#include <vorbis/vorbisfile.h>

class DealOggVorbis

{

public:

    ~DealOggVorbis();//析构函数

    const char* OggToWave( const char* oggFilename );//转换Ogg Vorbis至Wave

    //其他的扩展方法

private:

    int samples;//样本数

    int channels;//声道数

    int samplerate;//采样率

    std::list<char*> res_handler;//资源管理

    static const int MAX_BUFFER_SIZE;//缓冲区的最大范围，用以辅助解码

    const char* setWave( const std::vector<char>& pcmdata );//将数据包装成Wave

};

const int DealOggVorbis::MAX_BUFFER_SIZE = 4096;

#endif

//DealOggVorbis.cpp

#ifndef DEALOGGVORBIS_CPP

#define DEALOGGVORBIS_CPP

#include "DealOggVorbis.h"

DealOggVorbis::~DealOggVorbis()

{

std::list<char*>::iterator it = res_handler.begin();

for( ; it != res_handler.end(); ++it )//逐个删除先前new分配的内存

{

    if( *it )

        {

        delete[] *it;

            *it = NULL;

        }

}

}

const char* DealOggVorbis::OggToWave( const char* oggFilename )

{

    std::vector<char> pcmdata;//用以存储解码后的数据

    char pcmout[MAX_BUFFER_SIZE];//解码时的缓冲区

    OggVorbis_File vf;

    int eof = 0;

    int current_section = 0;

    FILE* f = fopen( oggFilename, "rb" );

    if( f == NULL )

    {

        return NULL;//打开文件出错

    }

    if( ov_open_callbacks( f, &vf, NULL, 0, OV_CALLBACKS_DEFAULT ) < 0 )      //在此有个问题...见后注1

    {

        return NULL;//打开并初始化OggVorbis_File结构出错

    }

    vorbis_info *vi = ov_info( &vf, -1 );//得到文件信息

    this->samples = ov_pcm_total( &vf, -1 );//样本数

    this->channels = vi->channels;//声道数

    this->samplerate = vi->rate;//采样率

    if( samples > 0 )

        pcmdata.reserve( channels*2*samples );//扩展pcmdata的容量至指定大小   

    while( !eof )

       {

        long ret = ov_read( &vf, pcmout, sizeof( pcmout ), 0, 2, 1,      ¤t_section );//进行解码

        if( ret == 0 )

        {

            eof = 1;//文件结束        }

        else

        {

            if( ret < 0 )

            {

                return NULL;//文件流中发生不明错误

            }

            else

            {

                //在此并未处理采样率发生变化的情况...

                for( int i=0; i!=ret; ++i )

                    pcmdata.push_back( pcmout[i] );//讲数据转存入pcmdata

            }

        }

    }

    ov_clear(&vf);//该函数中已调用了fclose函数

    return setWave( pcmdata );

}

注1：一般此处使用ov_open函数，但是在Windows平台下往往会导致保护错误，原因是vorbisfile库和应用程序常常会链接到不同版本的CRT所引起的，所以在Windows上我们应该使用ov_fopen函数或者ov_open_callbacks函数，进一步信息请参看Ogg Vorbis帮助文档。

const char* DealOggVorbis::setWave( const std::vector<char>& pcmdata )

{

    char* waveFile = new char[44+pcmdata.size()];//Wave文件有44字节的文件头

    waveFile[0] = 'R';

    waveFile[1] = 'I';

    waveFile[2] = 'F';

    waveFile[3] = 'F';//设置"RIFF"标志

    (*(int*)(&waveFile[4])) = 36+pcmdata.size();//设置块大小

    waveFile[8] = 'W';

    waveFile[9] = 'A';

    waveFile[10] = 'V';

    waveFile[11] = 'E';//设置"WAVE"标志

    waveFile[12] = 'f';

    waveFile[13] = 'm';

    waveFile[14] = 't';

    waveFile[15] = ' ';//设置"fmt "标志

    (*(int*)(&waveFile[16])) = 16;//设置子块大小

    *(short*)(&waveFile[20]) = 1;// 1 代表 "PCM" 音频格式

    *(short*)(&waveFile[22]) = channels;//设置声道

    *(int*)(&waveFile[24]) = samplerate;//设置采样率

    *(int*)(&waveFile[28]) = channels*samplerate*2;//设置字节率

    *(short*)(&waveFile[32]) = channels*2;//设置块阵列

    *(short*)(&waveFile[34]) = 16;//设置样本位数

    waveFile[36] = 'd';

    waveFile[37] = 'a';

    waveFile[38] = 't';

    waveFile[39] = 'a';//设置"data"标志(子块2 ID)

    *(int*)(&waveFile[40]) = pcmdata.size();//设置子块2大小

    for( int i = 44;i!=44+pcmdata.size();++i )

        waveFile[i] = pcmdata[i-44];//填充数据

    res_handler.push_back( waveFile );//登记资源

    return waveFile;

} 

#endif

至此，一个简单的Ogg Vorbis格式转换至Wave格式的类已经可以工作了，也许你认为程序还比较粗糙，仍可以进一步雕琢：例如引入流技术，加上单例设计模式，或者更好的资源管理等等，但是对于一个示例程序来说，其也算是比较明朗清晰了，所以暂不管它的尚待改进之处，让我们先写一个示例程序来感受一番Ogg Vorbis格式音乐的动人之处：），以后的扩展工作就留给你尽情想象发挥吧：）

首先写上一段最简单的Win32应用程序，然后添上下面一段简洁的代码：

    DealOggVorbis dov;

    const char* data = dov.OggToWave( "River.ogg" );

    PlaySound( data, NULL, SND_MEMORY|SND_ASYNC );

就这么简单，一首天籁随即萦绕我们的耳旁了：)

至此，相关Ogg Vorbis的知识也浮光掠影的讲述完毕了，希望有兴趣的读者可以进一步的学习研究，继续支持Ogg Vorbis，继续发扬我们伟大的开源精神：）

（ 相关参考：Ogg Vorbis帮助文档；《游戏音效编程》；开发ogg播放程序的一点心得(网页);Ogg Vorbis - 自由的声音(网页) ）

 链接(ogg vorbis sdk):http://download.csdn.net/source/1548448

