# Android的声音编程--使用OpenSL ES Audio - 建建的博客 - CSDN博客
2017年05月31日 18:13:58[纪建](https://me.csdn.net/u013898698)阅读数：2964
OpenSL
 ES? 是无授权费、跨平台、针对[嵌入式系统](http://baike.baidu.com/view/6115.htm)精心优化的硬件音频加速API。Android
 NDK 给出了使用OpenSL ES来实现native-audio的例子。本文介绍了如何使用OpenSL
 ES在Android中进行声音编程。从OpenSL ES的编程编译链接设置到创建声音引擎、创建声音播放器、设置播放缓冲等具体步骤。
# 编译和链接设置
使用OpenSL ES的前提条件是在源代码中包含OpenSL
 ES的头文件如下所示
#include <SLES/OpenSLES.h>
在gcc中的链接选项link option中链接OpenSL
 ES的库文件
-lOpenSLES 
OpenSL ES音频编程步骤
具体的使用步骤如下：
1.创建声音引擎
2.创建声音播放器
3.设置播放缓冲
# OpenSL ES的接口
OpenSL ES 主要操作的是接口（interface），大体来说有如下几个接口：
SLObjectItf ：对象接口，
SLEngineItf ：引擎接口
SLPlayItf：播放接口
SLBufferQueueItf :缓冲队列接口
SLEffectSendItf：音效发送接口
SLMuteSoloItf : 声道关闭 /单声道接口
SLVolumeItf : 声量接口
初始化声音引擎
除了 slCreateEngine 这一条函数是用来创建声音引擎对象接口以外，其它的所有操作都是通过接口的成员函数完成的，现在研究如何初始化声音引擎
初始化声音引擎需要3个接口，要将它们声明为全局变量：
SLObjectItf _aud;/* 声音引擎的对象接口 */
SLEngineItf _aud_eng;/* 声音引擎 */
SLObjectItf _aud_mix;/* 混音器对象接口 */
首先创建声音引擎的对象接口：
slCreateEngine(&_aud, 0, NULL, 0, NULL, NULL);
创建之后的接口并不能立即使用，我们首先要通过接口的子函数实现（Realize）它：
(*_aud)->Realize(_aud, SL_BOOLEAN_FALSE);/* 通过_aud的Realize子函数实现声音引擎的对象接口 */
实现之后，从声音引擎的对象中抓取声音引擎，在这里通过接口的子函数抓取接口（GetInterface），抓取对像是 _aud，抓取类型是引擎，抓取后存放的内存位置是先前声明的引擎接口_aud_eng的指针位置。指令如下：
(*_aud)->GetInterface(_aud, SL_IID_ENGINE, &_aud_eng);
这样声音引擎就初始化了
第三步要做的是从声音引擎中创建 "输出混音器" 对象，这样才可以将声音播放出来。
注意，同声音引擎的对象一样，由于 "输出混音器" 是对象，必须创建后在实现（Realize）它。
由于"输出混音器"创建的同时需要两个音效参数，所以先准备好这两个参数：第一个参数类型是声音接口编号（SLInterfaceID），是一个数组，可以将其看成一个音效配置列表，在其中放置不同的音效编号。第二个参数是一个逻辑数组：是否强制实现声音接口编号中对应音效，如果某个音效的逻辑为真（SL_BOOLEAN_TRUE），则在无法实现该音效时视为"输出混音器"创建失败，否则将忽略该音效。因为安卓的声硬件对音效的支持不同，所以最好不要强制实现，所以在第二个数组中全部填写SL_BOOLEAN_FALSE 。
现在开始创建"输出混音器"，环境回响（SL_IID_ENVIRONMENTALREVERB ）是比较常见的音效，将非强制性的使用该音效。
SLInterfaceID effect[1] = {SL_IID_ENVIRONMENTALREVERB}; /*环境回响音效 */
SLboolean effect_bool[1] = {SL_BOOLEAN_FALSE}; /* 回响音效强制实现逻辑 */
(*_aud_eng)->CreateOutputMix(_aud_eng, &_aud_mix, 1, effect, effect_bool);/* 从声音引擎中创建“输出混音器”
 */
(*_aud_mix)->Realize(_aud_mix, SL_BOOLEAN_FALSE); /* 实现刚创建的“输出混音器”
 */
这样一来，所有的初始化工作就全部完成了。完整代码如下：
SLObjectItf _aud;/* 声音引擎对象 */
SLEngineItf _aud_eng;/* 声音引擎 */
SLObjectItf _aud_mix;/* 输出混音器对象 */
/* audio_init: 初始化OpenSL ES */
int audio_init ()
{
SLInterfaceID effect[1] = {SL_IID_ENVIRONMENTALREVERB}; /* 音效 */
SLboolean effect_bool[1] = {SL_BOOLEAN_FALSE}; /*音效强制实现逻辑 */
slCreateEngine(&_aud, 0, NULL, 0, NULL, NULL);/* 创建声音引擎对象 */
(*_aud)->Realize(_aud, SL_BOOLEAN_FALSE);/* 实现声音引擎对象 */
(*_aud)->GetInterface(_aud, SL_IID_ENGINE, &_aud_eng);/* 从声音引擎对象中抓取声音引擎 */
(*_aud_eng)->CreateOutputMix(_aud_eng, &_aud_mix, 1, effect, effect_bool);/* 通过声音引擎创建输出混音器对象，并且非强制性的开启环境混响效果 */
(*_aud_mix)->Realize(_aud_mix, SL_BOOLEAN_FALSE);/* 实现混音器对象 */
return;
}
# 创建播放器对象
初始化引擎之后，还要创建一个播放器对象，就可以在播放器中输出声音了
播放器对象和输出混音器一样，是对象，创建后需要进行实现：
SLObjectItf _aud_plyobj; /*播放器对象 */
(*_aud_eng)->CreateAudioPlayer(_aud_eng, &_aud_plyobj, &sndsrc, &sndsnk, 3, ids, req);
/* 这是创建声音播放器对象aud_plyobj的函数 */
前两个参数分别把声音引擎和播放器引擎的指针位置填上就可以了
真正需要填写的是后面4个参数：
sndsrc: 声音数据源
sndsnk:声音输出池(data sink)，也就是声音输出设备
3:功能清单数目，我们要让播放器支持3种功能
ids: 功能清单，要让播放器支持的功能
req:功能强制实现逻辑
下面逐一来分析这5个参数
sndsrc是声源，类型是SLDataSource, 是一个有两个成员的结构，两个成员分别是：
pLocator: 数据定位器(DataLocator)的指针位置
PFormat:数据定位器指向数据的格式指针位置，在这里使用pcm 声音格式
那么首先看这两种结构:
1:数据定位器就是定位要播放声音数据的存放位置，分为4种：内存位置，输入/输出设备位置，缓冲区队列位置，和midi缓冲区队列位置。由于我们可以灵活的对缓冲区队列进行安排并设置播放后回传函数，而且我们使用的是pcm声音格式，所以我们采取第三种数据定位器：
SLDataLocator_BufferQueue loc_bufq; /* 缓冲区队列定位器 */
2:数据格式，由于我们采用的是pcm声音格式，所以我们使用：
SLDataFormat_PCM format_pcm; /* pcm 数据格式 */
接下来，我们要填写 "缓冲区队列定位器" 与 "pcm数据格式" 这两种结构的内容
我们要在SLDataLocator_BufferQueue类型变量 loc_bufq中填上的是
SLDataLocator_BufferQueue loc_bufq = { /* 缓冲区队列定位器 */
SL_DATALOCATOR_BUFFERQUEUE, /* 必须填成左边这样,表示数据定位器将定位缓冲区队列 */
2 /* 缓冲队列里的缓冲数目,这里我们填写2个缓冲 */
};
我们要在SLDataFormat_PCM类型变量 format_pcm中填上的是
SLDataFormat_PCM format_pcm = { /* pcm 数据格式 */
SL_DATAFORMAT_PCM, /* 必须填成左边这样,表示pcm 数据格式 */
1, /* 这里是声道数目，多声道数目将交错存放，而不是整体存放，我们使用单声道 */
SL_SAMPLINGRATE_8, /*采样率，我们使用 8kHz的采样率，质量不是很好 */
SL_PCMSAMPLEFORMAT_FIXED_16, /*采样大小，我们使用固定的 16bit采样 */
SL_PCMSAMPLEFORMAT_FIXED_16,/*容器大小，和采样大小保持一致就不会失真，我们采用 16bit容器*/
 SL_SPEAKER_FRONT_CENTER, /* 声道映射掩码，在这里，我们将不用的声道映射到不同的扩音器中，在这里，由于我们的声音是单声道，所以我们将其映射到安卓设备的主扩音器, 也就是前中扩音器：SPEAKER_FRONT_CENTER
 */
SL_BYTEORDER_LITTLEENDIAN /* 尾序：小端模式 */
};
这样，我们这完成了声源参数:
sndsrc
我们还要完成剩下的4个参数，也就是：
sndsnk:声音输出池(data sink)，也就是声音输出设备
3:功能清单数目，我们要让播放器支持3种功能
ids: 功能清单，我们要让播放器支持的功能
req:功能强制实现逻辑
从声音输出池开始，声音输出池和声源的成员差不多：
pLocator: 数据定位器(DataLocator)的指针位置
PFormat:数据定位器指向数据的格式指针位置
但声音输出池需要的是SLDataLocator_OutputMix 输出混音器数据定位器，而且不需要交代数据格式，所以只要填写输出混音器数据定位器就可以了：
SLDataLocator_OutputMix loc_outmix = { /* 输出混音器数据定位器 */
SL_DATALOCATOR_OUTPUTMIX, /* 必须填成左边这样,表示数据定位器将定位输出混音器 */
NULL /* 不需要交代数据格式 */
};
最后只有播放器功能清单要完成了，我们要求播放器具备3种功能，它们的编号如下：
SL_IID_BUFFERQUEUE：缓冲区队列功能(循环播放，随机播放...)
SL_IID_EFFECTSEND：音效功能(环境回响，均衡器...)
SL_IID_VOLUME：音量功能(音量调节，3d声源...)
这样来填写
SLInterfaceID ids[] = { /* 播放器功能列表 */
SL_IID_BUFFERQUEUE, /* 缓冲区队列功能 */
SL_IID_EFFECTSEND,/* 音效功能 */
SL_IID_VOLUME /* 音量功能 */
};
而强制逻辑和之前提到的req完全一样，但这3种功能是常用功能，一般的安卓设备都应具备，所以我们强制要求实现，实现不了则视为错误：
SLboolean req[] = {
SL_BOOLEAN_TRUE, /* 强制实现id中对应的缓冲区队列功能 */
SL_BOOLEAN_TRUE, /* 强制实现id中对应的音效功能 */
SL_BOOLEAN_TRUE /* 强制实现id中对应的音量功能 */
};
整合以上步骤，我们终于得到了一个声音播放器对象 _aud_plyobj，最终指令如下：
(*_aud_eng)->CreateAudioPlayer(_aud_eng, &_aud_plyobj, &sndsrc, &sndsnk, 3, ids, req);
/* 从声音引擎_aud_eng中创建声音播放器对象_aud_plyobj
同其它对象一样，我们来实现声音播放器对象_aud_plyobj:
(*_aud_plyobj)->Realize(_aud_plyobj, SL_BOOLEAN_FALSE); /*实现声音播放器对象 */
这样一来，声音播放器对象就创建好了。
OpenSL ES主要操作的是接口（interface），所以要从播放器中获得一些接口，这样在对接口进行操作时，就能控制声音播放器。在这里，我们从声音播放器中获得4种不同类型的接口，首先进行全局变量声明：
SLPlayItf _aud_ply; /* 播放接口（播放，暂停...） */
SLBufferQueueItf _aud_buf; /*缓冲区接口 */
SLEffectSendItf _aud_bufefx; /*音效接口，在本播放器中将用于缓冲区 */
SLVolumeItf _aud_vol; /*音量接口 */
然后通过GetInterface成员函数从播放器对象中获得接口:
(*_aud_plyobj)->GetInterface(_aud_plyobj, SL_IID_PLAY, &_aud_ply); /*获得播放接口，存至_aud_ply*/
(*_aud_plyobj)->GetInterface(_aud_plyobj, SL_IID_BUFFERQUEUE, &_aud_buf); /*获得缓冲区接口，存至 _aud_buf */
(*_aud_plyobj)->GetInterface(_aud_plyobj, SL_IID_EFFECTSEND, &_aud_bufefx); /*获得音效接口，存至 _aud_bufefx */
(*_aud_plyobj)->GetInterface(_aud_plyobj, SL_IID_VOLUME, &_aud_vol); /*获得音量接口，存至 _aud_vol */
从现在起，所有对上述4个接口的操作都将影响到播放器
为了真正实现缓冲区连续播放的功能，我们要为缓冲区接口_aud_buf注册一个回调函数：
(*_aud_buf)->RegisterCallback(_aud_buf, _newsnd_cb, NULL);
_newsnd_cb函数将在缓冲区播放完毕时自动执行，接下来我们研究这个函数怎么实现
前面创建了引擎和播放器，如果我们命令播放器播放，会不会马上有声音呢？答案是否定的，因为我们根本没有提供数据
提供播放数据
其实自行设计_newsnd_cb回调函数将要实现的功能只有一个，就是为播放器提供将要播放的数据，这样播放器就会自动对其进行播放了
short *snd; /* 16 bit, 8kHz单声道pcm 声音采样 */
unsigned sndlen; /*声音采样数据的大小*/
int num_snd; /* 该采样的剩余播放的次数 */
/* _newsnd_cb: 我们自己创建的缓冲区队列播放回调函数，第一项填写目标缓冲区接口，第二项填NULL即可 */
void _newsnd_cb(SLBufferQueueItf bq, void *context)
{
if (--num_snd > 0 && snd && sndlen > 0) /* 如果采样的剩余播放的次数大于0，声音数据有效，而且声音数据的长度大于0
 */
(*bq)->Enqueue(bq, snd, sndlen); /* 操作缓冲区队列接口，将长度为 sndlen的声音数据snd排进队列 */
}
这样，缓冲区队列没有声音可以播放时，将呼出上述回调函数，而该回调函数将检查剩余播放次数，并在有播放次数的情况下将声音排进队列，从而让缓冲区队列可以播放队列中的第一个声音，也就是刚排进去的声音。当然，由于这个函数是自定义的，灵活度很高，所以大家可以自由发挥，借助它实现随机播放，或是互联网流媒体播放。
现在我们完成了OpenSL
 ES声音引擎和播放器创建，只需要操作_aud_ply 播放接口，即可启动播放器：
(*_aud_ply)->SetPlayState(_aud_ply, SL_PLAYSTATE_PLAYING); /* 将播放状态设为“正在播放”
 */
为了测试我们的播放器是否正常，我们尝试通过函数合成一个时长为一秒种的正弦波采样，将其播放5次:
#include "math.h"
short snd_sin[8000]; /* 时长为1秒的 16bit 单声道正弦波采样 */
void create_sine_wave(void)
{
int i;
for (i = 0; i < 8000; ++i) /* 声音时长1秒，所以创建8000个单声道采样 */
snd_sin[i] = sinf(i*180/M_PI)*32767; /* 通过正弦函数创建单个采样值 */
}
这样就能通过输出声音是否为正弦波来判断播放器是否正常了
最后是将正弦波播放5次的演示程序：
[http://pan.baidu.com/s/1uw672](http://jump.bdimg.com/safecheck/index?url=x+Z5mMbGPAvVIlwZePSt0B3tEqEFWbC4tOatFxkC6cIvigwexUQAdF1yhPrVXPdRrHVW2Ong7n2CBxU8u+UN0fwzHhMDrzo8iRpKf85QGzpe9iCXLH/Zhk1Lv2Njs06vVmYQFUeMaCgwPGbuJnYGNA==)
