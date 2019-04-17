# Windows PCM音频捕获与播放实现 - DoubleLi - 博客园






在WINDOWS下，音频函数有多种类型，如MCI、多媒体OLE控制、高级音频等，使用方法都比较简单。但如果想编写一个功能较强大的音频处理程序，那就必须使用低级音频函数和多媒体文件I/O来控制音频设备的输入和输出。因为低级音频函数可直接与音频驱动程序交互，通过窗口消息或回调（CALL BACK）函数来管理音频数据块的记录和播放，控制非常灵活。重要的一点是，低级音频函数为我们提供了一个设备无关的接口。

 WINDOWS下音频的采集，播放有三种模式：

（1）通过高级音频函数、媒体控制接口MCI[1、2]设备驱动程序。

（2）低级音频函数MIDI Mapper、低级音频设备驱动（WaveXAPI）。

（3）利用DirectX中的DirectSound。

 使用MCI的方法极其简便，灵活性较差；使用低级音频函数的方法相对来说难一点，但是能够对音频数据进行灵活的操控；而采用DirectSound的方法，控制声音数据灵活，效果比前二者都好，但实现起来是三者中最难的。

DirectSound是DirectXAudio的一个较底层的部件，提供了丰富的接口函数，实现.wav格式的波形声音数据的播放控制。与一般的WindowsAPI提供的声音播放函数不同，DirectSound可实现多个声音的混合播放。DirectSound可充分使用声卡的内存资源，同时也提供了3D声效算法，模拟出真实的3D立体声。

使用 Wave API 进行Windows音频编程可以保持很大的自由度，而且与Linux中的OSS编程模式很像。这里我们主要介绍Wave API。

使用waveOutGetNumDevs和waveOutGetDevCaps来获取波形输出设备的个数和能力。只有在确定设备存在之后，才可以打开设备、使用设备。当waveInOpen/waveOutOpen的第二个参数为WAVE_MAPPER时，函数会自动挑选最适合播放给定的数据格式的设备。当有多种波形输出设备时，建议使用WAVE_MAPPER常数作为设备ID。

录音：

waveInOpen -> waveInPrepareHeader -> waveInAddBuffer -> waveInStart -> waveInStop -> waveInReset ->waveInUnprepareHeader -> waveInClose

播放：

waveOutOpen -> waveOutPrepareHeader -> waveOutWrite -> waveOutReset -> waveOutUnprepareHeader -> waveOutClose

1、查询设备数目和能力
使用waveOutGetNumDevs和waveOutGetDevCaps来获取波形输出设备的个数和能力。只有在确定设备存在之后，才可以打开设备、使用设备。

2、打开波形输出设备

使用waveInOpen/waveOutOpen为进行重放操作打开特定的波形设备。该函数打开与指定的设备ＩＤ相关联的设备，并以给出指定内存句柄的方法返回打开波形设备的句柄。

3、准备音频数据块

在波形捕获/播放之前，要准备好音频数据块，里面包含捕获所需的缓冲区地址和播放所需的数据地址。将数据块传递给设备驱动程序就实现了捕获/播放。使用的函数是waveInPrepareHeader /waveOutPerpareHeader。在用完数据块之后，必须用waveInUnprepareHeader/waveOutUnPrepareHeader函数来清除对波形数据块的准备。

4、发送音频数据块

在成功打开波形输出设备之后，就可以进行波形重放，使用waveOutWrite函数。在调用该函数后，必须等到设备驱动程序使用完音频数据块之后才可以把该数据块释放掉。

5、管理波形播放

在使用低级音频函数播放音频时，应用程序必须不断地向设备驱动程序提供数据块，直到播放结束。WINDOWS提供两种方法管理波形重放：一是使用窗口消息管理，二是使用低级回调函数管理。另外，通过使用waveOutPause、waveOutRestart和waveOutReset来进行暂停、重新启动和停止播放。

6、开始音频捕获

在成功打开波形输入设备之后，使用waveInStart函数。在调用该函数后，开始捕获输入设备的音频数据拷贝至音频数据块中的缓冲区地址中。

7、停止音频捕获

调用waveInStop函数停止在指定的波形输入设备上的输入

8、关闭波形设备

用完设备之后，必须调用waveInClose/waveOutClose函数关闭波形设备，以便其他程序可以使用设备。

更多函数见附录。

下面给出实例代码：

PS：工程在链接中使用了winmm.lib


#include "stdafx.h"

#include

#include



#define MUTE_LENGTH  128



HWAVEIN hWaveIn;//输入设备

HWAVEOUT hWaveOut;//输出设备

WAVEFORMATEX waveform;//采集音频的格式，结构体

BYTE *pBuffer;//采集音频时的数据缓存

WAVEHDR wHdr;//采集音频时包含数据缓存的结构体

FILE *pf;

char mute[MUTE_LENGTH];//静音符号串



int _tmain(int argc, _TCHAR* argv[])

{

HANDLE wait;

waveform.wFormatTag = WAVE_FORMAT_PCM;//声音格式为PCM

waveform.nSamplesPerSec = 8000;//采样率，8000次/秒

waveform.wBitsPerSample = 16;//采样比特，16bits/次

waveform.nChannels = 1;//采样声道数，单声道

waveform.nAvgBytesPerSec = 16000;//每秒的数据率，就是每秒能采集多少字节的数据

waveform.nBlockAlign = 2;//一个块的大小，采样bit的字节数乘以声道数/8

waveform.cbSize = 0;



wait = CreateEvent(NULL,0,0,NULL);



FillMemory(mute,MUTE_LENGTH,(BYTE)0xFE);

//用静音符号填充.

//pcm表示的是时域信号，有震动才有声音，所以为静音并不一定表示都为0，如果都为1同样也是静音，所以只要是每个样本值没有变化都是静音 



DWORD bufsize = 1024*1024;//每次开辟10k的缓存存储录音数据

int i = 20;//录音时间

#if 0

    // Device   

    int nReturn = waveInGetNumDevs();  

    printf("输入设备数目：%d\n", nReturn);  

    for (int i=0; i

    {  

        WAVEINCAPS wic;  

        waveInGetDevCaps(i, &wic, sizeof(WAVEINCAPS));  

        printf("%d\t设备名：%s\n", i, wic.szPname);  

    }  

//使用waveInOpen函数开启音频采集

waveInOpen(&hWaveIn,WAVE_MAPPER,&waveform,(DWORD_PTR)wait,0L,CALLBACK_EVENT);

WAVEINCAPS wic;  

    waveInGetDevCaps((UINT_PTR)hWaveIn, &wic, sizeof(WAVEINCAPS));  

    printf("打开的输入设备：%s\n", wic.szPname); 

fopen_s(&pf,"录音测试.pcm","wb");

pBuffer = new BYTE[bufsize];

wHdr.lpData = (LPSTR)pBuffer;

wHdr.dwBufferLength = bufsize;

wHdr.dwBytesRecorded = 0;

wHdr.dwUser = 0;

wHdr.dwFlags = 0;

wHdr.dwLoops = 1;

waveInPrepareHeader(hWaveIn, &wHdr, sizeof(WAVEHDR));

//准备一个波形数据块头用于录音

waveInAddBuffer(hWaveIn,&wHdr,sizeof(WAVEHDR));

//指定波形数据块为录音输入缓存

waveInStart(hWaveIn);//开始录音

while(--i)

{

Sleep(1000);

printf("%ds\n",i);

}

waveInStop(hWaveIn);  

    waveInReset(hWaveIn); //停止录音

waveInClose(hWaveIn);

fwrite(pBuffer,1,wHdr.dwBytesRecorded,pf);

delete pBuffer;

fclose(pf);

#else

    // Device   

    int nReturn = waveOutGetNumDevs();  

    printf("\n输出设备数目：%d\n", nReturn);  

    for (int i=0; i

    {  

        WAVEOUTCAPS woc;  

        waveOutGetDevCaps(i, &woc, sizeof(WAVEOUTCAPS));  

        printf("#%d\t设备名：%s\n", i, woc.szPname);  

    }  

pBuffer = new BYTE[bufsize];

wHdr.lpData = mute;

wHdr.dwBufferLength = MUTE_LENGTH;

wHdr.dwBytesRecorded = 0;

wHdr.dwUser = 0;

wHdr.dwFlags = 0;

wHdr.dwLoops = 1;

waveOutOpen (&hWaveOut, WAVE_MAPPER, &waveform, (DWORD)wait, 0, CALLBACK_EVENT);

waveOutPrepareHeader (hWaveOut, &wHdr, sizeof(WAVEHDR));

while(1)

{

waveOutWrite (hWaveOut, &wHdr, sizeof (WAVEHDR)) ;

printf(".");

//Sleep(1000);

WaitForSingleObject(wait,INFINITE);

}

    waveOutReset(hWaveOut);  

    waveOutUnprepareHeader(hWaveOut, &wHdr, sizeof(WAVEHDR));  

    waveOutClose(hWaveOut);  

#endif

return 0;

}


附录：

|waveInGetNumDevs|返回系统中存在的波形输入设备的数量|
|----|----|
|waveInAddBuffer|向波形输入设备添加一个输入缓冲区|
|waveInGetDevCaps|查询指定的波形输入设备以确定其性能|
|waveInGetErrorText|检取由指定的错误代码标识的文本说明|
|waveInGetID|获取指定的波形输入设备的标识符|
|waveInGetPosition|检取指定波形输入设备的当前位置|
|waveInMessage|发送一条消息给波形输入设备的驱动器|
|waveInOpen|为录音而打开一个波形输入设备|
|waveInPrepareHeader|为波形输入准备一个输入缓冲区|
|waveInStart|启动在指定的波形输入设备的输入|
|waveInReset|停止给定的波形输入设备的输入，且将当前位置清零|
|waveInStop|停止在指定的波形输入设备上的输入|
|waveInUnprepareHeader|清除由waveInPrepareHeader函数实现的准备|
|WaveInClose|关闭指定的波形输入设置|
|waveOutBreakLoop|中断给定的波形输出设备上一个循环，并允许播放驱动取列表中的下一个块|
|waveOutClose|关闭指定的波形输出设备|
|waveOutGetDevCaps|查询一个指定的波形输出设备以确定其性能|
|waveOutGetErrorText|检取由指定的错误代码标识的文本说明|
|waveOutGetID|检取指定的波形输出设备的标识符|
|waveOutGetNumDevs|检取系统中存在的波形输出设备的数量|
|waveOutGetPitch|查询一个波形输出设备的当前音调设置|
|waveOutGetPlaybackRate|查询一个波形输出设备当前播放的速度|
|waveOutGetPosition|检取指定波形输出设备的当前播放位置|
|waveOutGetVolume|查询指定波形输出设备的当前音量设置|
|waveOutMessage|发送一条消息给一个波形输出设备的驱动器|
|waveOutOpen|为播放打开一个波形输出设备|
|waveOutPause|暂停指定波形输出设备上的播放|
|waveOutPrepareHeader|为播放准备一个波形缓冲区|
|waveOutRestart|重新启动一个被暂停的波形输出设备|
|waveOutSetPitch|设置一个波形输出设备的音调|
|waveOutSetPlaybackRate|设置指定波形输出设备的速度|
|waveOutSetVolume|设置指定的波形输出设备的音量|
|waveOutUnprepareHeader|清除由waveOutPrepareHeader函数实现的准备|
|waveOutWrite|向指定的波形输出设备发送一个数据块|










