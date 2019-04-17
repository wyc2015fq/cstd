# XAudio2播放PCM - DoubleLi - 博客园






XAudio2





是一个跨平台的API，在Xbox 360及Windows中得到支持。在Xbox 360上， XAudio2作为一个静态库编译到游戏可执行文件中。在Windows上，XAudio2提供一个动态链接库（DLL）。以下例子只使用了其中的一部分功能，并不全面。详情请看微软技术页的XAudio2编程相关(英文)。 使用XAudio2来播放未压缩的PCM音频数据的过程并不复杂，主要有以下几个步骤：

1. 建立XAudio2 引擎

使用XAudio2Create函数，该函数的功能是创建一个XAudio2对象(IXAudio2接口)。

示例：XAudio2Create( &pXAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR );

2. 使用第1步建立的引擎建立MasteringVoice

使用IXAudio2成员函数CreateMasteringVoice，该函数功能是创建并设置一个MasteringVoice 原型

示例：pXAudio2->CreateMasteringVoice(&pMasterVoice);

3. 使用第一步建立的引擎建立SourceVoice(或SubmixVoice，以下按SourceVoice举例) 使用IXAudio2成员函数CreateSourceVoice，该函数功能是创建并设置一个SourceVoice 原型

示例：

pXAudio2->CreateSourceVoice(&pSourceVoice,&format,0,XAUDIO2_DEFAULT_FREQ_RATIO,NULL,NULL,NULL);





其中format这样设置：（位数为bits，声道数为channels，采样率为hz）

WAVEFORMATEX format;

format.wFormatTag = WAVE_FORMAT_PCM;//PCM格式

format.wBitsPerSample = bits;//位数

format.nChannels = channels;//声道数

format.nSamplesPerSec = hz;//采样率

format.nBlockAlign = bits*channels/8;//数据块调整

format.nAvgBytesPerSec = format.nBlockAlign*hz;//平均传输速率

format.cbSize = 0;//附加信息

4. 呈交音频数据

使用IXAudio2SourceVoice的成员函数SubmitSourceBuffer，该函数功能是呈交一个XAUDIO2_BUFFER 原型

示例：pSourceVoice->SubmitSourceBuffer(&XAudio2Buffer,NULL);



其中XAudio2Buffer这样设置：

XAUDIO2_BUFFER XAudio2Buffer;

XAudio2Buffer.Flags = 0;//可以设为0或XAUDIO2_END_OF_STREAM，当设为后者时，将使

XAudio2播放完该数据块后自动停止，不再播放下一个数据块

XAudio2Buffer.AudioBytes = BufferSize;// 音频数据的长度，按字节算

XAudio2Buffer.pAudioData = pBuffer;//具体音频数据的地址，unsigned char pBuffer[]

XAudio2Buffer.PlayBegin = 0;//起始播放地址

XAudio2Buffer.PlayLength = 0;//播放长度，0为整数据块

XAudio2Buffer.LoopBegin = 0;//循环起始位置

XAudio2Buffer.LoopLength = 0;//循环长度，按字节算

XAudio2Buffer.LoopCount = 0;//循环次数，0为不循环，255为无限循环

XAudio2Buffer.pContext = NULL;//这里的pContext用来标识该数据块，供回调用，可以是NULL

5. 继续呈交数据和播放数据：

播放呈交的数据使用IXAudio2SourceVoice的成员函数Start，该函数功能是开始播放。

原型：HRESULT Start(

UINT32 Flags,//必须是0

UINT32 OperationSet = XAUDIO2_COMMIT_NOW//使用XAUDIO2_COMMIT_NOW将立即生效，

使用XAUDIO2_COMMIT_ALL将挂起，等待其它的数值的OperationSet的处理完

);

示例：pSourceVoice->Start(0, XAUDIO2_COMMIT_NOW);

第5步做完之后，XAudio2将一块接一块地播放呈交的数据块。我们只需不断重复第四步，就能不断地播放音频数据了。需要注意的是，在XAudio2播放完某个XAudio2Buffer之前，该XAudio2Buffer以及XAudio2Buffer.pAudioData所指向的内存不能被修改或删除，否则将发生错误。但是某个

XAudio2Buffer一旦被播放完，就能被修改了。为此，我们可以创建一个数组XAUDIO2_BUFFER []来循环呈交和更新数据。那怎么知道XAudio2到底播放了几个XAudio2Buffer呢，可以使用

IXAudio2SourceVoice的成员函数

原型：GetState(

XAUDIO2_VOICE_STATE *pVoiceState,// 这里返回结构体指针

[optional] UINT32 Flags//获取方式,可选,默认0.设为XAUDIO2_VOICE_NOSAMPLESPLAYED将

只获取挂起(包括正在播放)的XAudio2Buffer数量，速度较快。注意：DirectX SDK版本没有此参数 );

XAUDIO2_VOICE_STATE包含三个成员：

void * pCurrentBufferContext//对应XAUDIO2_BUFFER中的pContext

UINT32 BuffersQueued//挂起(包括正在播放)的XAudio2Buffer数量

UINT64 SamplesPlayed//已播放的样本数

示例： pSourceVoice->GetState(&state);

6. 暂停和停止播放

暂停播放使用IXAudio2SourceVoice的成员函数：Stop

原型：HRESULT Stop(

UINT32 Flags,// 设为0或XAUDIO2_PLAY_TAILS，后者代表等待音效放完

UINT32 OperationSet = XAUDIO2_COMMIT_NOW// XAUDIO2_COMMIT_NOW立即生效

);

如果设定XAUDIO2_PLAY_TAILS，应在音效输出完成后设定0,再Stop一次。

暂停后再次调用Start将在暂停的位置开始播放。



如果要完全停止，还需要使用IXAudio2SourceVoice的成员函数FlushSourceBuffers，该函数功能是清除挂起的XAudio2Buffer队列。

原型：HRESULT FlushSourceBuffers();

说明：该函数使用后要到XAudio2播放完一个XAudio2Buffer才生效，建议在回调中使用。使用该函数后，XAudio2Buffer队列计数将置0

7. IXAudio2SourceVoice的其他功能：设置声调使用SetFrequencyRatio函数

原型：HRESULT SetFrequencyRatio(

float Ratio,//1.0为正常声调，>1.0为高声调快放，<1.0为低声调慢放

UINT32 OperationSet = XAUDIO2_COMMIT_NOW

);



IXAudio2SourceVoice继承自，所以还有许多IXAudio2Voice的功能，比如设置音量用SetVolume等。



注意：以上IXAudio2SourceVoice的成员函数中， Stop、GetState、 FlushSourceBuffers可以在回调中使用 释放相关实例的顺序与创建他们的顺序相反。需要包含头文件Xaudio2.h和Objbase.h以及链接ole32.lib(而不是Microsoft网站上的Xaudio2.lib)









=============================================================================================================

以上摘抄

程序说明：

1、参考DirectX SDK 例程XAudio2BasicSound编写。

2、实现功能：验证XAduio2播放语音流程，实现播放二进制存储的PCM格式音频数据文件。





**[cpp]**[view plain](http://blog.csdn.net/hongxy0635/article/details/54617947#)[copy](http://blog.csdn.net/hongxy0635/article/details/54617947#)



- 




**[cpp]**[view plain](http://blog.csdn.net/hongxy0635/article/details/54617947#)[copy](http://blog.csdn.net/hongxy0635/article/details/54617947#)



- // XAudio2_test.cpp : 定义控制台应用程序的入口点。  
- //  
- 
- #include "stdafx.h"  
- #include <Windows.h>  
- #include "XAudio2.h"  
- 
- //--------------------------------------------------------------------------------------  
- // Helper macros  
- //--------------------------------------------------------------------------------------  
- #ifndef SAFE_DELETE_ARRAY  
- #define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p);   (p)=NULL; } }  
- #endif  
- #ifndef SAFE_RELEASE  
- #define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=NULL; } }  
- #endif  
- int main()  
- {  
-     IXAudio2 *pXAudio2;//这里返回XAudio2对象的指针  
-     CoInitializeEx(NULL, COINIT_MULTITHREADED);  
- HRESULT hr;  
- if (FAILED(hr = XAudio2Create(&pXAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR)))  
-     {  
-         wprintf(L"Failed to init XAudio2 engine: %#X\n", hr);  
-         CoUninitialize();  
- return 0;  
-     }  
- //  
- // Create a mastering voice  
- //  
-     IXAudio2MasteringVoice* pMasteringVoice = NULL;  
- if (FAILED(hr = pXAudio2->CreateMasteringVoice(&pMasteringVoice)))  
-     {  
-         wprintf(L"Failed creating mastering voice: %#X\n", hr);  
-         SAFE_RELEASE(pXAudio2);  
-         CoUninitialize();  
- return 0;  
-     }  
-     WAVEFORMATEX pwfx;  
-     pwfx.wFormatTag         = WAVE_FORMAT_PCM;  //PCM格式  
-     pwfx.wBitsPerSample     = 16;               //位数  
-     pwfx.nChannels          = 1;                //声道数  
-     pwfx.nSamplesPerSec     = 8000;             //采样率  
-     pwfx.nBlockAlign        = 16*1 / 8;         //数据块调整  
-     pwfx.nAvgBytesPerSec    = pwfx.nBlockAlign * 8000;      //平均传输速率  
-     pwfx.cbSize             = 0;                //附加信息  
- //  
- // Play the wave using a XAudio2SourceVoice  
- //  
- // Create the source voice  
-     IXAudio2SourceVoice* pSourceVoice;  
- if (FAILED(hr = pXAudio2->CreateSourceVoice(&pSourceVoice, &pwfx)))  
-     {  
-         wprintf(L"Error %#X creating source voice\n", hr);  
- return hr;  
-     }  
- 
- FILE *fp;  
- //SPEECHDECODE文件说明： 该文件为保存的二进制音频数据，格式:16位，采样率8K.  
- int re = fopen_s(&fp, "SPEECHDECODE", "rb");  
-     XAUDIO2_BUFFER XBuffer[4];  
- BYTE *pRBuffer[4];  
- int i = 0;  
- for (i = 0; i < 4; i++)  
-     {  
-         pRBuffer[i] = (BYTE *)malloc(640);  
-         fread(pRBuffer[i], 640, 1, fp);  
-         XBuffer[i].AudioBytes = 640;  
-         XBuffer[i].Flags = 0;  
-         XBuffer[i].LoopBegin = 0;  
-         XBuffer[i].LoopCount = 0;  
-         XBuffer[i].LoopLength = 0;  
-         XBuffer[i].pAudioData = pRBuffer[i];  
-         XBuffer[i].pContext = (void*)new int[1];  
-         XBuffer[i].PlayBegin = 0;  
-         XBuffer[i].PlayLength = 0;  
- if (FAILED(hr = pSourceVoice->SubmitSourceBuffer(&XBuffer[i])))  
-         {  
-             wprintf(L"Error %#X submitting source buffer\n", hr);  
-             pSourceVoice->DestroyVoice();  
- return hr;  
-         }  
-     }  
- 
-     hr = pSourceVoice->Start(0);  
- BOOL isRunning = TRUE;  
-     i = 0;  
- while (SUCCEEDED(hr) && isRunning)  
-     {  
-         XAUDIO2_VOICE_STATE state;  
- //检查播放缓存区缓冲块数量，当缓存块数量不足时，则新加入  
-         pSourceVoice->GetState(&state);        
-         isRunning = (state.BuffersQueued > 0) != 0;  
- // Wait till the escape key is pressed  
- if (GetAsyncKeyState(VK_ESCAPE))    //Esc键退出  
- break;  
-         printf("state.BuffersQueued = %d\n", state.BuffersQueued);  
- if (state.BuffersQueued <= 2)  
-         {  
- if (fread(pRBuffer[i], 640, 1, fp) <= 0) break;  
-             XBuffer[i].pAudioData = pRBuffer[i];  
- if (FAILED(hr = pSourceVoice->SubmitSourceBuffer(&XBuffer[i])))  
-             {  
-                 wprintf(L"Error %#X submitting source buffer\n", hr);  
-                 pSourceVoice->DestroyVoice();  
- return hr;  
-             }  
-             i = (i + 1) % 4;  
-         }  
-         Sleep(10);  
-     }  
- END:  
-     hr = pSourceVoice->Stop(0);  
-     pSourceVoice->DestroyVoice();  
- for (i = 0; i < 4; i++)  
-     {  
- if (pRBuffer[i]) free(pRBuffer[i]);  
-     }  
- if (pXAudio2) {  
-         pXAudio2->Release();   
-         pXAudio2 = NULL;  
-     }  
-     CoUninitialize();  
- return 0;  
- }  

![](http://static.blog.csdn.net/images/save_snippets.png)












