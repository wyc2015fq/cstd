# Windows基础-使用XAudio2播放音频（本质是WASAPI） - DoubleLi - 博客园






#### 对于常见的音频播放，使用XAudio2足够了。

## **时间是把杀猪刀，滑稽的是我成了猪**

早在Windows Vista中,M$推出了新的音频[架构](http://lib.csdn.net/base/architecture)UAA，其中的CoreAudio接替了DSound、WaveXxx、MediaFundation，通过Core Audio APIs，Windows的音频性能可以与MacOS X相媲美(手动偷笑)。

## **Universal Audio Architecture (UAA)**

CoreAudio属于UAA，只在用户层进行一系列音频处理，而系统内核只负责递交缓冲数据给音频驱动。 
![这里写图片描述](http://img.blog.csdn.net/20160222104525204)
在UAA出现之前，程序跑起来是这样的： 
![这里写图片描述](http://img.blog.csdn.net/20160222110013813)
现在程序在UAA跑起来是这样的： 
![这里写图片描述](http://img.blog.csdn.net/20160222110255533)

## **WASAPI和XAudio2**

#### Core Audio APIs的明星：WASAPI

**WASAPI可以不进行SRC直接输出，还能提供极低的音频延迟。**
为了降低音频延迟，更像AISO，WASAPI的使用方式分为两种：一种是push，组成缓冲区队列，常用于音频播放。一种是event(必须独占)，由硬件时钟或音频API提供事件来驱动你提交音频数据（好复杂啊真心没用过，缓存欠载怎么办），这样就可以大幅降低音频延迟，好像连DMA都会跳过（未查证），适用于游戏、实时混音等对实时性要求比较苛刻的场合。 
在保真度上，WASAPI被Foobar2000用户吹得神乎其神。而它充其量就是少了个SRC过程，至于两种模式的区别，娱乐一下就行了。什么人声甜美声场宽毛刺少等等故弄玄虚之流，有这功夫还是花点钱吧，你需要更好的器材。 
对用户而言，WASAPI真正厉害的在于APO（Audio Processing Objects），基于此技术的音效理论上兼容所有设备（可能是DSP[算法](http://lib.csdn.net/base/datastructure)的问题，我遇到了采样率上的限制，一旦高于96000Hz就失效了）。

#### 出自DirectX的XAudio2：

**XAudio2在采用UAA的Windows版本中就是对WASAPI的调用。**
关于保真度，你的程序在Xp上跑的是DSound，使用UAA的系统则直接对应WASAPI，同样可以跳过SRC，而且音频低延迟的表现足以满足音乐游戏之类的需求。 
最初的XAudio用于Xbox，XAudio2一开始可用于三红机和Vista及两者以上。现在的大一统环境中（使用Windows原生API），播放音频不是WASAPI就是XAudio2。使用XAudio2最大好处是比WASAPI更易于音频编程，播放音频的时候你只需不断地提交音频缓冲区的数据逐渐组成队列就好，缺点是.。。对于音频播放的常规开发，目前没看到缺点，除了达不到那种连DMA都能跳过的超低延迟（换来的就是更高的硬件资源占用和几乎感受不出的体验提升）。对系统资源的占用还有延迟和WASAPI看不出来有什么差距。仅从音频播放上，个人推荐使用XAudio2而非WASAPI。 
目前在Windows10中它的版本是2.9，并继续在WinRT与UWP技术中供开发者使用。

## **说闲话 IS EASY，这就给你CODE：**

这个是我做语音合成的时候写的一个简单示例，代表了最常用的音频播放场景，全是定式： 
PS：合成出来的SampleRate低于声卡工作的输出频率，必须经过软件SRC（Sample Rate Converter），否则出来的就是快放效果，创建音源时默认参数是开启其内置的SRC。

```
// 需要的头文件和静态库
#include <Windows.h>
#include <XAudio2.h>
#pragma comment(lib,"xaudio2.lib") 

/*
#include"SpeechSynthesis.h"这是SpeechSynthesis的头文件
*/
SpeechSynthesis syth(SC_Mandarin);                              // 语音合成器-简体中文转普通话
WAVEFORMATEX format = syth.wfx;                                 // 波形格式

IXAudio2 * XAudioEngine = NULL;                                 // IXAduio2音频引擎
IXAudio2MasteringVoice * pmaster = NULL;                        // 声音管理器
IXAudio2SourceVoice * pSource = NULL;                           // 音源
XAUDIO2_BUFFER sBuffer = {};                                    // 音源缓冲区
XAUDIO2_VOICE_STATE pState = {};                                // 缓冲区状态

// 单独列出初始化和卸载，可以加入到你的构造-析构函数中
BOOL Init()
{
    // XAudio2初始化
    CoInitializeEx(NULL, COINIT_MULTITHREADED);
    if (FAILED(XAudio2Create(&XAudioEngine)))return FALSE;
    if (FAILED(XAudioEngine->CreateMasteringVoice(&pmaster)))return FALSE;
    if (FAILED(XAudioEngine->CreateSourceVoice(&pSource, &format)))return FALSE;
    return TRUE;
}

void Uninit()
{
    XAudioEngine->Release();
    CoUninitialize();
}
// 语音合成与音频呈现的函数
BOOL Vocalize(wchar_t * srcSentence)
{
    pSource->Start();                                               // 开启音源
    sBuffer.pAudioData = syth.Synthesize(srcSentence);              // 合成音频
    sBuffer.AudioBytes = syth.pcm_data.size();                      // 一次性载入音频缓冲区
    if (FAILED(pSource->SubmitSourceBuffer(&sBuffer)))return 0;     // 装载音频
    // 等待播放完毕或者打断
    for (pSource->GetState(&pState); pState.BuffersQueued; pSource->GetState(&pState))
        Sleep(1);
    pSource->Stop();                                                // 关闭音源
    pSource->FlushSourceBuffers();                                  // 缓冲区清空(可选)
    return TRUE;
}

int main(int argc, char *argv[])
{
    Init();
    Vocalize(L"这是一段测试语音");
    Vocalize(L"节选于《让我在你的心里自由自在》");
    Vocalize(L"她瞬间就摘下了自己平静的面具，阳光般的笑容撒满她的眼睛。她向我投来真实的目光，她的声音让我无所适从地被打动。她时而安静、柔顺得像个邻家女孩，时而一双又黑又大的眼睛闪烁出精灵的光芒。在她的音乐里，她就是一只在草原蔚蓝澄明的天际自由翱翔的苍鹰，释放着自己宽阔的憧憬与广袤的理想。她的内心孤独而迷茫，踯躅而忧郁，但依然顽固地看守着精神的自由，也学会了在现实生活中默默承受。");
    Uninit();
    return 0;
}
```

可见流程非常简单： 
初始化之后可以先pSource->Start()也可以先配置sBuffer和SubmitSourceBuffer，三者顺序很自由，符合常理就行;对于sBuffer，这里只需要让它知道音频缓冲区的指针和缓冲区长度，至于loop等参数，使用起来顾名思义，这里没必要啰嗦。 
然后就是轮询音频缓冲区状态，以防缓冲区欠载。 
停止播放的时候可以清除音源的缓冲区队列，有些情况需要你打断并播放别的声音，需要使用FlushSourceBuffers();否则效果就是队列式地把声音全部播放完。

下次播放直接重复上述操作。

#### ***提示***：

前面已提到，音源初始化的时候默认是开启SRC的，也就是进行自动重采样来保证正确的播放速度。 
创建音源的时候有一个int型Flag叫做：XAUDIO2_VOICE_NOSRC。你可以在初始化的时候修改第三个参数来实现无SRC：
`CreateSourceVoice(&pSource, &format，XAUDIO2_VOICE_NOSRC)；`
顾名思义，当你保证音频采样率和设备输出设定一致的时候就可以这样跳过SRC，你要的bit-perfect并没有因为DX而阉割，而且代码没有WASAPI那么冗杂。









