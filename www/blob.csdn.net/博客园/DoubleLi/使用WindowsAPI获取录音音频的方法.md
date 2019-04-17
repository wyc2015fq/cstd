# 使用WindowsAPI获取录音音频的方法 - DoubleLi - 博客园







这篇文章主要介绍了使用WindowsAPI获取录音音频的方法,非常实用的功能,需要的朋友可以参考下







本文实例介绍了使用winmm.h进行音频流的获取的方法，具体步骤如下：

**一、首先需要包含以下引用对象**




[?](http://www.jb51.net/article/53858.htm#)

```
```cpp
#include <Windows.h>
```



```cpp
#include "mmsystem.h"
```



```cpp
#pragma comment(lib, "winmm.lib")
```
```




**二、音频的获取需要调用7个函数**

1. waveInGetNumDevs：返回系统中就绪的波形声音输入设备的数量




[?](http://www.jb51.net/article/53858.htm#)

```
```cpp
UINT
```

```cpp
waveInGetNumDevs(
```

```cpp
VOID
```

```cpp
);
```
```




2. waveInGetDevCaps：检查指定波形输入设备的特性




[?](http://www.jb51.net/article/53858.htm#)

```
```cpp
MMRESULT waveInGetDevCaps(
```



```cpp

```

```cpp
UINT_PTR
```

```cpp
uDeviceID,
```



```cpp

```

```cpp
LPWAVEINCAPS pwic,
```



```cpp

```

```cpp
UINT
```

```cpp
cbwic
```



```cpp
);
```



```cpp
//uDeviceID 音频输入设备标识,也可以为一个打开的音频输入设备的句柄.
```



```cpp
//  个人认为如果上一步获得了多个设备，可以用索引标识每一个设备.
```



```cpp
//
```



```cpp
//pwic 对WAVEINCAPS结构体的一个指针,包含设备的音频特性.
```



```cpp
//
```



```cpp
//cbwic WAVEINCAPS结构体的大小,使用sizeof即可.
```



```cpp
//
```



```cpp
//MMRESULT 函数执行的结果
```



```cpp
//  MMSYSERR_NOERROR 表示执行成功
```



```cpp
//  MMSYSERR_BADDEVICEID 索引越界
```



```cpp
//  MMSYSERR_NODRIVER 没有就绪的设备
```



```cpp
//  MMSYSERR_NOMEM 不能分配或者锁定内存
```
```




介绍WAVEINCAPS结构体的含义：




[?](http://www.jb51.net/article/53858.htm#)

```
```cpp
typedef
```

```cpp
struct
```

```cpp
{
```



```cpp

```

```cpp
WORD
```

```cpp
wMid;
```

```cpp
//音频设备制造商定义的驱动程序标识
```



```cpp

```

```cpp
WORD
```

```cpp
wPid;
```

```cpp
//音频输入设备的产品标识
```



```cpp

```

```cpp
MMVERSION vDriverVersion;
```

```cpp
//驱动程序版本号
```



```cpp

```

```cpp
TCHAR
```

```cpp
szPname[MAXPNAMELEN];
```

```cpp
//制造商名称
```



```cpp

```

```cpp
DWORD
```

```cpp
dwFormats;
```

```cpp
//支持的格式,参见MSDN
```



```cpp

```

```cpp
WORD
```

```cpp
wChannels;
```

```cpp
//支持的声道数
```



```cpp

```

```cpp
WORD
```

```cpp
wReserved1;
```

```cpp
//保留参数
```



```cpp
} WAVEINCAPS;
```
```




3. waveInOpen：打开指定的音频输入设备，进行录音




[?](http://www.jb51.net/article/53858.htm#)

```
```cpp
MMRESULT waveInOpen(
```



```cpp

```

```cpp
LPHWAVEIN    phwi,
```

```cpp
//接收打开的音频输入设备标识的HWAVEIN结构的指针
```



```cpp

```

```cpp
UINT_PTR
```

```cpp
uDeviceID,
```

```cpp
//指定一个需要打开的设备标识.可以使用WAVE_MAPPER选择一个按指定录音格式录音的设备
```



```cpp

```

```cpp
LPWAVEFORMATEX pwfx,
```

```cpp
//一个所需的格式进行录音的WAVEFORMATEX结构的指针
```



```cpp

```

```cpp
DWORD_PTR
```

```cpp
dwCallback,
```

```cpp
//指向一个回调函数、事件句柄、窗口句柄、线程标识,对录音事件进行处理.
```



```cpp

```

```cpp
DWORD_PTR
```

```cpp
dwCallbackInstance,
```

```cpp
//传给回调机制的参数
```



```cpp

```

```cpp
DWORD
```

```cpp
fdwOpen
```

```cpp
//打开设备的方法标识,指定回调的类型.参见CSDN
```



```cpp
);
```
```




介绍WAVEFORMATEX结构体的含义：




[?](http://www.jb51.net/article/53858.htm#)

```
```cpp
typedef
```

```cpp
struct
```

```cpp
{
```



```cpp

```

```cpp
WORD
```

```cpp
wFormatTag;
```

```cpp
//波形声音的格式,单声道双声道使用WAVE_FORMAT_PCM.当包含在WAVEFORMATEXTENSIBLE结构中时,使用WAVE_FORMAT_EXTENSIBLE.
```



```cpp

```

```cpp
WORD
```

```cpp
nChannels;
```

```cpp
//声道数量
```



```cpp

```

```cpp
DWORD
```

```cpp
nSamplesPerSec;
```

```cpp
//采样率.wFormatTag为WAVE_FORMAT_PCM时,有8.0kHz,11.025kHz,22.05kHz,和44.1kHz.
```



```cpp

```

```cpp
DWORD
```

```cpp
nAvgBytesPerSec;
```

```cpp
//每秒的采样字节数.通过nSamplesPerSec * nChannels * wBitsPerSample / 8计算
```



```cpp

```

```cpp
WORD
```

```cpp
nBlockAlign;
```

```cpp
//每次采样的字节数.通过nChannels * wBitsPerSample / 8计算
```



```cpp

```

```cpp
WORD
```

```cpp
wBitsPerSample;
```

```cpp
//采样位数.wFormatTag为WAVE_FORMAT_PCM时,为8或者16
```



```cpp

```

```cpp
WORD
```

```cpp
cbSize;
```

```cpp
//wFormatTag为WAVE_FORMAT_PCM时,忽略此参数
```



```cpp
} WAVEFORMATEX;
```
```




介绍dwCallback回调函数格式：




[?](http://www.jb51.net/article/53858.htm#)

```
```cpp
void
```

```cpp
CALLBACK waveInProc(
```



```cpp

```

```cpp
HWAVEIN hwi,
```

```cpp
//回调此函数的设备句柄
```



```cpp

```

```cpp
UINT
```

```cpp
uMsg,
```

```cpp
//波形声音输入信息,标识关闭(WIM_CLOSE)、缓冲区满(WIM_DATA)、打开(WIM_OPEN).
```



```cpp

```

```cpp
DWORD_PTR
```

```cpp
dwInstance,
```

```cpp
//用户在waveInOpen指定的数据
```



```cpp

```

```cpp
DWORD_PTR
```

```cpp
dwParam1,
```

```cpp
//(LPWAVEHDR)dwParam1,用户指定的缓冲区
```



```cpp

```

```cpp
DWORD_PTR
```

```cpp
dwParam2
```



```cpp
);
```
```




4. waveInPrepareHeader：为音频输入设备准备一个缓冲区




[?](http://www.jb51.net/article/53858.htm#)

```
```cpp
MMRESULT waveInPrepareHeader(
```



```cpp

```

```cpp
HWAVEIN hwi,
```

```cpp
//音频输入设备句柄
```



```cpp

```

```cpp
LPWAVEHDR pwh,
```

```cpp
//指向WAVEHDR结构的指针,标识准备的缓冲区
```



```cpp

```

```cpp
UINT
```

```cpp
cbwh
```

```cpp
//WAVEHDR结构的大小,使用sizeof即可
```



```cpp
);
```
```




介绍WAVEHDR结构：




[?](http://www.jb51.net/article/53858.htm#)

```
```cpp
typedef
```

```cpp
struct
```

```cpp
wavehdr_tag {
```



```cpp

```

```cpp
LPSTR
```

```cpp
lpData;
```

```cpp
//指向波形格式的缓冲区
```



```cpp

```

```cpp
DWORD
```

```cpp
dwBufferLength;
```

```cpp
//缓冲区的大小
```



```cpp

```

```cpp
DWORD
```

```cpp
dwBytesRecorded;
```

```cpp
//当前存储了多少数据
```



```cpp

```

```cpp
DWORD_PTR
```

```cpp
dwUser;
```

```cpp
//用户数据
```



```cpp

```

```cpp
DWORD
```

```cpp
dwFlags;
```

```cpp
//为缓冲区提供的信息,在waveInPrepareHeader函数中使用WHDR_PREPARED
```



```cpp

```

```cpp
DWORD
```

```cpp
dwLoops;
```

```cpp
//输出时使用,标识播放次数
```



```cpp

```

```cpp
struct
```

```cpp
wavehdr_tag * lpNext;
```

```cpp
//reserved
```



```cpp

```

```cpp
DWORD_PTR
```

```cpp
reserved;
```

```cpp
//reserved
```



```cpp
} WAVEHDR, *LPWAVEHDR;
```
```




5. waveInAddBuffer：将缓冲区发送给设备，若缓冲区填满，则不起作用。（参数同上）




[?](http://www.jb51.net/article/53858.htm#)

```
```cpp
MMRESULT waveInAddBuffer(
```



```cpp

```

```cpp
HWAVEIN hwi,
```



```cpp

```

```cpp
LPWAVEHDR pwh,
```



```cpp

```

```cpp
UINT
```

```cpp
cbwh
```



```cpp
);
```
```




6. waveInStart：开始进行录制




[?](http://www.jb51.net/article/53858.htm#)

```
```cpp
MMRESULT waveInStart(
```



```cpp

```

```cpp
HWAVEIN hwi
```

```cpp
//设备句柄
```



```cpp
);
```
```




7. waveInClose：关闭设备




[?](http://www.jb51.net/article/53858.htm#)

```
```cpp
MRESULT waveInClose(
```



```cpp

```

```cpp
HWAVEIN hwi
```

```cpp
//设备句柄
```



```cpp
);
```
```




**三、完整实例代码如下：**




[?](http://www.jb51.net/article/53858.htm#)

```
```cpp
//Run.c文件
```



```cpp
#include <Windows.h>
```



```cpp
#include <stdio.h>
```



```cpp
#include "mmsystem.h"
```



```cpp
#pragma comment(lib, "winmm.lib")
```



```cpp
void
```

```cpp
PlayMusi();
```



```cpp
void
```

```cpp
WaveInitFormat(LPWAVEFORMATEX m_WaveFormat,
```

```cpp
WORD
```

```cpp
nCh,
```

```cpp
DWORD
```

```cpp
nSampleRate,
```

```cpp
WORD
```

```cpp
BitsPerSample);
```



```cpp
DWORD
```

```cpp
CALLBACK MicCallback(HWAVEIN hwavein,
```

```cpp
UINT
```

```cpp
uMsg,
```

```cpp
DWORD
```

```cpp
dwInstance,
```

```cpp
DWORD
```

```cpp
dwParam1,
```

```cpp
DWORD
```

```cpp
dwParam2);
```



```cpp
void
```

```cpp
RecordWave();
```



```cpp

```



```cpp
void
```

```cpp
main()
```



```cpp
{
```



```cpp

```

```cpp
//PlayMusi();
```



```cpp

```

```cpp
RecordWave();
```



```cpp

```

```cpp
while
```

```cpp
(1);
```



```cpp
}
```



```cpp
void
```

```cpp
RecordWave()
```



```cpp
{
```



```cpp

```

```cpp
int
```

```cpp
count = waveInGetNumDevs();
```

```cpp
//1
```



```cpp

```

```cpp
printf
```

```cpp
(
```

```cpp
"\n音频输入数量：%d\n"
```

```cpp
,count);
```



```cpp

```



```cpp

```

```cpp
WAVEINCAPS waveIncaps;
```



```cpp

```

```cpp
MMRESULT mmResult = waveInGetDevCaps(0,&waveIncaps,
```

```cpp
sizeof
```

```cpp
(WAVEINCAPS));
```

```cpp
//2
```



```cpp

```

```cpp
printf
```

```cpp
(
```

```cpp
"\n音频输入设备：%s\n"
```

```cpp
,waveIncaps.szPname);
```



```cpp

```



```cpp

```

```cpp
if
```

```cpp
(MMSYSERR_NOERROR==mmResult)
```



```cpp

```

```cpp
{
```



```cpp

```

```cpp
HWAVEIN phwi;
```



```cpp

```

```cpp
WAVEFORMATEX pwfx;
```



```cpp

```

```cpp
WaveInitFormat(&pwfx,1,8000,8);
```



```cpp

```

```cpp
printf
```

```cpp
(
```

```cpp
"\n请求打开音频输入设备"
```

```cpp
);
```



```cpp

```

```cpp
printf
```

```cpp
(
```

```cpp
"\n采样参数：单声道 8kHz 8bit\n"
```

```cpp
);
```



```cpp

```

```cpp
mmResult=waveInOpen(&phwi,WAVE_MAPPER,&pwfx,(
```

```cpp
DWORD
```

```cpp
)(MicCallback),NULL,CALLBACK_FUNCTION);
```

```cpp
//3
```



```cpp

```



```cpp

```

```cpp
if
```

```cpp
(MMSYSERR_NOERROR==mmResult)
```



```cpp

```

```cpp
{
```



```cpp

```

```cpp
WAVEHDR pwh1;
```



```cpp

```

```cpp
char
```

```cpp
buffer1[10240];
```



```cpp

```

```cpp
pwh1.lpData=buffer1;
```



```cpp

```

```cpp
pwh1.dwBufferLength=10240;
```



```cpp

```

```cpp
pwh1.dwUser=1;
```



```cpp

```

```cpp
pwh1.dwFlags=0;
```



```cpp

```

```cpp
mmResult=waveInPrepareHeader(phwi,&pwh1,
```

```cpp
sizeof
```

```cpp
(WAVEHDR));
```

```cpp
//4
```



```cpp

```

```cpp
printf
```

```cpp
(
```

```cpp
"\n准备缓冲区1"
```

```cpp
);
```



```cpp

```



```cpp

```

```cpp
WAVEHDR pwh2;
```



```cpp

```

```cpp
char
```

```cpp
buffer2[10240];
```



```cpp

```

```cpp
pwh2.lpData=buffer2;
```



```cpp

```

```cpp
pwh2.dwBufferLength=10240;
```



```cpp

```

```cpp
pwh2.dwUser=2;
```



```cpp

```

```cpp
pwh2.dwFlags=0;
```



```cpp

```

```cpp
mmResult=waveInPrepareHeader(phwi,&pwh2,
```

```cpp
sizeof
```

```cpp
(WAVEHDR));
```

```cpp
//4
```



```cpp

```

```cpp
printf
```

```cpp
(
```

```cpp
"\n准备缓冲区2\n"
```

```cpp
);
```



```cpp

```



```cpp

```

```cpp
if
```

```cpp
(MMSYSERR_NOERROR==mmResult)
```



```cpp

```

```cpp
{
```



```cpp

```

```cpp
mmResult=waveInAddBuffer(phwi,&pwh1,
```

```cpp
sizeof
```

```cpp
(WAVEHDR));
```

```cpp
//5
```



```cpp

```

```cpp
printf
```

```cpp
(
```

```cpp
"\n将缓冲区1加入音频输入设备"
```

```cpp
);
```



```cpp

```

```cpp
mmResult=waveInAddBuffer(phwi,&pwh2,
```

```cpp
sizeof
```

```cpp
(WAVEHDR));
```

```cpp
//5
```



```cpp

```

```cpp
printf
```

```cpp
(
```

```cpp
"\n将缓冲区2加入音频输入设备\n"
```

```cpp
);
```



```cpp

```



```cpp

```

```cpp
if
```

```cpp
(MMSYSERR_NOERROR==mmResult)
```



```cpp

```

```cpp
{
```



```cpp

```

```cpp
mmResult=waveInStart(phwi);
```

```cpp
//6
```



```cpp

```

```cpp
printf
```

```cpp
(
```

```cpp
"\n请求开始录音\n"
```

```cpp
);
```



```cpp

```

```cpp
}
```



```cpp

```

```cpp
}
```



```cpp

```

```cpp
}
```



```cpp

```



```cpp

```

```cpp
}
```



```cpp
}
```



```cpp
DWORD
```

```cpp
CALLBACK MicCallback(HWAVEIN hwavein,
```

```cpp
UINT
```

```cpp
uMsg,
```

```cpp
DWORD
```

```cpp
dwInstance,
```

```cpp
DWORD
```

```cpp
dwParam1,
```

```cpp
DWORD
```

```cpp
dwParam2)
```



```cpp
{
```



```cpp

```

```cpp
switch
```

```cpp
(uMsg)
```



```cpp

```

```cpp
{
```



```cpp

```

```cpp
case
```

```cpp
WIM_OPEN:
```



```cpp

```

```cpp
printf
```

```cpp
(
```

```cpp
"\n设备已经打开...\n"
```

```cpp
);
```



```cpp

```

```cpp
break
```

```cpp
;
```



```cpp

```



```cpp

```

```cpp
case
```

```cpp
WIM_DATA:
```



```cpp

```

```cpp
printf
```

```cpp
(
```

```cpp
"\n缓冲区%d存满...\n"
```

```cpp
,((LPWAVEHDR)dwParam1)->dwUser);
```



```cpp

```

```cpp
waveInAddBuffer (hwavein, (LPWAVEHDR)dwParam1,
```

```cpp
sizeof
```

```cpp
(WAVEHDR)) ;
```



```cpp

```

```cpp
break
```

```cpp
;
```



```cpp

```



```cpp

```

```cpp
case
```

```cpp
WIM_CLOSE:
```



```cpp

```

```cpp
printf
```

```cpp
(
```

```cpp
"\n设备已经关闭...\n"
```

```cpp
);
```



```cpp

```

```cpp
break
```

```cpp
;
```



```cpp

```

```cpp
default
```

```cpp
:
```



```cpp

```

```cpp
break
```

```cpp
;
```



```cpp

```

```cpp
}
```



```cpp

```

```cpp
return
```

```cpp
0;
```



```cpp
}
```



```cpp
void
```

```cpp
WaveInitFormat(LPWAVEFORMATEX m_WaveFormat,
```

```cpp
WORD
```

```cpp
nCh,
```

```cpp
DWORD
```

```cpp
nSampleRate,
```

```cpp
WORD
```

```cpp
BitsPerSample)
```



```cpp
{
```



```cpp

```

```cpp
m_WaveFormat->wFormatTag = WAVE_FORMAT_PCM;
```



```cpp

```

```cpp
m_WaveFormat->nChannels = nCh;
```



```cpp

```

```cpp
m_WaveFormat->nSamplesPerSec = nSampleRate;
```



```cpp

```

```cpp
m_WaveFormat->nAvgBytesPerSec = nSampleRate * nCh * BitsPerSample/8;
```



```cpp

```

```cpp
m_WaveFormat->nBlockAlign = m_WaveFormat->nChannels * BitsPerSample/8;
```



```cpp

```

```cpp
m_WaveFormat->wBitsPerSample = BitsPerSample;
```



```cpp

```

```cpp
m_WaveFormat->cbSize = 0;
```



```cpp
}
```



```cpp
void
```

```cpp
PlayMusi()
```



```cpp
{
```



```cpp

```

```cpp
int
```

```cpp
error = mciSendString(
```

```cpp
"open C:\\Users\\Angel\\Desktop\\有多少爱可以重来.mp3 alias myDivece"
```

```cpp
, NULL, 0, NULL);
```



```cpp

```

```cpp
if
```

```cpp
(error == 0)
```



```cpp

```

```cpp
{
```



```cpp

```

```cpp
mciSendString(
```

```cpp
"play myDivece"
```

```cpp
, NULL, 0, NULL);
```

```cpp
//播放
```



```cpp

```

```cpp
}
```



```cpp
}
```
```













