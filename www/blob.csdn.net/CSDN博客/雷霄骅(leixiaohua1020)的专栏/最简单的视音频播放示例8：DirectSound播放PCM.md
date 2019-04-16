# 最简单的视音频播放示例8：DirectSound播放PCM - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2014年10月30日 00:08:36[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：26284标签：[DirectSound																[音频																[PCM																[播放](https://so.csdn.net/so/search/s.do?q=播放&t=blog)
个人分类：[我的开源项目																[DirectSound](https://blog.csdn.net/leixiaohua1020/article/category/2619495)](https://blog.csdn.net/leixiaohua1020/article/category/1843731)







=====================================================


最简单的视音频播放示例系列文章列表：

[最简单的视音频播放示例1：总述](http://blog.csdn.net/leixiaohua1020/article/details/40246783)


[最简单的视音频播放示例2：GDI播放YUV, RGB](http://blog.csdn.net/leixiaohua1020/article/details/40266503)


[最简单的视音频播放示例3：Direct3D播放YUV，RGB（通过Surface）](http://blog.csdn.net/leixiaohua1020/article/details/40279297)


[最简单的视音频播放示例4：Direct3D播放RGB（通过Texture）](http://blog.csdn.net/leixiaohua1020/article/details/40301179)


[最简单的视音频播放示例5：OpenGL播放RGB/YUV](http://blog.csdn.net/leixiaohua1020/article/details/40333583)


[最简单的视音频播放示例6：OpenGL播放YUV420P（通过Texture，使用Shader）](http://blog.csdn.net/leixiaohua1020/article/details/40379845)


[最简单的视音频播放示例7：SDL2播放RGB/YUV](http://blog.csdn.net/leixiaohua1020/article/details/40525591)


[最简单的视音频播放示例8：DirectSound播放PCM](http://blog.csdn.net/leixiaohua1020/article/details/40540147)


[最简单的视音频播放示例9：SDL2播放PCM](http://blog.csdn.net/leixiaohua1020/article/details/40544521)


=====================================================


本文记录DirectSound播放音频的技术。DirectSound是Windows下最常见的音频播放技术。目前大部分的音频播放应用都是通过DirectSound来播放的。本文记录一个使用DirectSound播放PCM的例子。
注：一位仁兄已经提醒我DirectSound已经计划被XAudio2取代了。后来考证了一下发现确有此事。因此在下次更新中考虑加入XAudio2播放PCM的例子。本文仍然记录一下DirectSound这位“元老”。

![](https://img-blog.csdn.net/20141028145216149?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)





## DirectSound简介
DirectSound是微软所开发DirectX的组件之一，可以在Windows 操作系统上录音，并且记录波形音效（waveform sound）。目前DirectSound 是一个成熟的API ，提供许多有用的功能，例如能够在较高的分辨率播放多声道声音。
DirectSound3D（DS3D）最早是1993年与 DirectX 3 一起发表的。DirectX 8以后的DirectSound和DirectSound3D的（DS3D）被合称DirectX Audio。


DirectSound有以下几种对象：
|对象|数量|作用|主要接口|
|----|----|----|----|
|设备|每个应用程序只有一个设备对象|用来管理设备，创建辅助缓冲区|IDirectSound8|
|辅助缓冲区|每一个声音对应一个辅助缓冲区|用来管理一个静态的或者动态的声音流，然后在主缓冲区中混音|IDirectSoundBuffer8,IDirectSound3DBuffer8,IDirectSoundNotify8|
|主缓冲区|一个应用程序只有一个主缓冲区|将辅助缓冲区的数据进行混音，并且控制3D参数.|IDirectSoundBuffer,IDirectSound3DListener8|



## DirectSound播放音频的流程

使用DirectSound播放音频一般情况下需要如下步骤：
1.初始化

> 1)创建一个IDirectSound8接口的对象
2)设置协作级
3)创建一个主缓冲对象
4)创建一个副缓冲对象
5)创建通知对象
6)设置通知位置

7)开始播放

2.循环播放声音

> 1)数据填充至副缓冲区

2)等待播放完成

下面结合详细分析一下上文的流程。


**1.初始化1)创建一个IDirectSound8接口的对象**
通过DirectSoundCreate8()方法可以创建一个设备对象。这个对象通常代表缺省的播放设备。DirectSoundCreate8()函数原型如下。

```cpp
HRESULT DirectSoundCreate8(
	 LPCGUID lpcGuidDevice,
	 LPDIRECTSOUND8 * ppDS8,
	 LPUNKNOWN pUnkOuter
)
```

参数的含义如下：
lpcGuidDevice：要创建的设备对象的GUID。可以指定为NULL，代表默认的播放设备。
ppDS8：返回的IDirectSound8对象的地址。
pUnkOuter：必须设为NULL。
例如如下代码即可创建一个IDirectSound8接口的对象

```cpp
IDirectSound8 *m_pDS=NULL;	
DirectSoundCreate8(NULL,&m_pDS,NULL);
```

**2)设置协作级**
Windows 是一个多任务环境，同一时间有多个应用程序去访问设备。通过使用协作级别，DirectSound可以确保应用程序不会在别的设备使用时去访问，每个 DirectSound应用程序都有一个协作级别，这个级别决定着访问硬件的权限。
在创建一个设备对象以后，必须通过用IDirectSound8的SetCooperativeLevel()设置协作权限，否则将听不到声音。SetCooperativeLevel()的原型如下

```cpp
HRESULT SetCooperativeLevel(
 HWND hwnd,
 DWORD dwLevel
)
```

参数的含义如下：
hwnd：应用程序窗口句柄。
dwLevel：支持以下几种级别。
DSSCL_EXCLUSIVE：与DSSCL_PRIORITY具有相同的作用。
DSSCL_NORMAL：正常的协调层级标志，其他程序可共享声卡设备进行播放。
DSSCL_PRIORITY：设置声卡设备为当前程序独占。
DSSCL_WRITEPRIMAR：可写主缓冲区，此时副缓冲区就不能进行播放处理，即不能将次缓冲区的数据送进混声器，再输出到主缓冲区上。这是最完全控制声音播放的方式。


**3)创建一个主缓冲对象**
使用IDirectSound8的CreateSoundBuffer()可以创建一个IDirectSoundBuffer接口的主缓冲区对象。CreateSoundBuffer()的原型如下。

```cpp
HRESULT CreateSoundBuffer(
 LPCDSBUFFERDESC pcDSBufferDesc,
 LPDIRECTSOUNDBUFFER * ppDSBuffer,
 LPUNKNOWN pUnkOuter
)
```

参数的含义如下：
pcDSBufferDesc：描述声音缓冲的DSBUFFERDESC结构体的地址
ppDSBuffer：返回的IDirectSoundBuffer接口的对象的地址。
pUnkOuter：必须设置为NULL。
其中涉及到一个描述声音缓冲的结构体DSBUFFERDESC，该结构体的定义如下：

```cpp
typedef struct _DSBUFFERDESC
{
    DWORD           dwSize;
    DWORD           dwFlags;
    DWORD           dwBufferBytes;
    DWORD           dwReserved;
    LPWAVEFORMATEX  lpwfxFormat;
} DSBUFFERDESC
```

简单解释一下其中的变量的含义：
dwSize：结构体的大小。必须初始化该值。
dwFlags：设置声音缓存的属性。有很多选项，可以组合使用，就不一一列出了。详细的参数可以查看文档。
dwBufferBytes：缓冲的大小。
dwReserved：保留参数，暂时没有用。
lpwfxFormat：指向一个WAVE格式文件头的指针。
设置DSBUFFERDESC完毕后，就可以使用CreateSoundBuffer()创建主缓冲了。示例代码如下：

```cpp
DSBUFFERDESC dsbd;
	memset(&dsbd,0,sizeof(dsbd));
	dsbd.dwSize=sizeof(dsbd);
	dsbd.dwFlags=DSBCAPS_GLOBALFOCUS | DSBCAPS_CTRLPOSITIONNOTIFY |DSBCAPS_GETCURRENTPOSITION2;
	dsbd.dwBufferBytes=MAX_AUDIO_BUF*BUFFERNOTIFYSIZE; 
	//WAVE Header
	dsbd.lpwfxFormat=(WAVEFORMATEX*)malloc(sizeof(WAVEFORMATEX));
	dsbd.lpwfxFormat->wFormatTag=WAVE_FORMAT_PCM;   
	/* format type */
	(dsbd.lpwfxFormat)->nChannels=channels;          
	/* number of channels (i.e. mono, stereo...) */
	(dsbd.lpwfxFormat)->nSamplesPerSec=sample_rate;     
	/* sample rate */
	(dsbd.lpwfxFormat)->nAvgBytesPerSec=sample_rate*(bits_per_sample/8)*channels; 
	/* for buffer estimation */
	(dsbd.lpwfxFormat)->nBlockAlign=(bits_per_sample/8)*channels;        
	/* block size of data */
	(dsbd.lpwfxFormat)->wBitsPerSample=bits_per_sample;     
	/* number of bits per sample of mono data */
	(dsbd.lpwfxFormat)->cbSize=0;


	//Creates a sound buffer object to manage audio samples. 
	HRESULT hr1;
	if( FAILED(m_pDS->CreateSoundBuffer(&dsbd,&m_pDSBuffer,NULL))){   
		return FALSE;
	}
```


**4)创建一个副缓冲对象**
使用IDirectSoundBuffer的QueryInterface()可以得到一个IDirectSoundBuffer8接口的对象。IDirectSoundBuffer8的GUID为IID_IDirectSoundBuffer8。示例代码如下。

```cpp
IDirectSoundBuffer *m_pDSBuffer=NULL;
IDirectSoundBuffer8 *m_pDSBuffer8=NULL;
...
if( FAILED(m_pDSBuffer->QueryInterface(IID_IDirectSoundBuffer8,(LPVOID*)&m_pDSBuffer8))){
	return FALSE ;
}
```


**5)创建通知对象**
使用IDirectSoundBuffer8的QueryInterface()可以得到一个IDirectSoundNotify8接口的对象。IDirectSoundBuffer8的GUID为IID_IDirectSoundNotify。示例代码如下。

```cpp
IDirectSoundBuffer8 *m_pDSBuffer8=NULL;
IDirectSoundNotify8 *m_pDSNotify=NULL;	
…
if(FAILED(m_pDSBuffer8->QueryInterface(IID_IDirectSoundNotify,(LPVOID*)&m_pDSNotify))){
	return FALSE ;
}
```


一句话概括一下通知对象的作用：当DirectSound缓冲区中的数据播放完毕后，告知系统应该填充新的数据。


**6)设置通知位置**
使用IDirectSoundNotify8的SetNotificationPositions()可以设置通知的位置。SetNotificationPositions()的原型如下。

```cpp
HRESULT SetNotificationPositions(
         DWORD dwPositionNotifies,
         LPCDSBPOSITIONNOTIFY pcPositionNotifies
)
```

参数含义如下。
dwPositionNotifies：DSBPOSITIONNOTIFY结构体的数量。既包含几个通知的位置。
pcPositionNotifies：指向DSBPOSITIONNOTIFY结构体数组的指针。
再这里涉及到一个结构体DSBPOSITIONNOTIFY，它描述了通知的位置。DSBPOSITIONNOTIFY的定义如下。

```cpp
typedef struct DSBPOSITIONNOTIFY {
    DWORD dwOffset;
    HANDLE hEventNotify;
} DSBPOSITIONNOTIFY;
```

它的成员的含义如下。
dwOffset：通知事件触发的位置（距离缓冲开始位置的偏移量）。
hEventNotify：触发的事件的句柄。


**7)开始播放**
使用IDirectSoundBuffer8的SetCurrentPosition ()可以设置播放的位置。SetCurrentPosition ()原型如下

```cpp
HRESULT SetCurrentPosition(
         DWORD dwNewPosition
)
```

其中dwNewPosition是播放点与缓冲区首个字节之间的偏移量。
使用IDirectSoundBuffer8的Play ()可以开始播放音频数据。Play ()原型如下。

```cpp
HRESULT Play(
         DWORD dwReserved1,
         DWORD dwPriority,
         DWORD dwFlags
)
```

参数含义：
dwReserved1：保留参数，必须取0。
dwPriority：优先级，一般情况下取0即可。
dwFlags：标志位。目前常见的是DSBPLAY_LOOPING。当播放至缓冲区结尾的时候，重新从缓冲区开始处开始播放。


**2.循环播放声音1)数据填充至副缓冲区**
数据填充至副缓冲区之前，需要先使用Lock()锁定缓冲区。然后就可以使用fread()，memcpy()等方法将PCM音频采样数据填充至缓冲区。数据填充完毕后，使用Unlock()取消对缓冲区的锁定。
Lock()函数的原型如下。

```cpp
HRESULT Lock(
         DWORD dwOffset,
         DWORD dwBytes,
         LPVOID * ppvAudioPtr1,
         LPDWORD  pdwAudioBytes1,
         LPVOID * ppvAudioPtr2,
         LPDWORD pdwAudioBytes2,
         DWORD dwFlags
)
```

参数的含义如下。
dwOffset：锁定的内存与缓冲区首地址之间的偏移量。
dwBytes：锁定的缓存的大小。
ppvAudioPtr1：获取到的指向缓存数据的指针。
pdwAudioBytes1：获取到的缓存数据的大小。
ppvAudioPtr2：没有用到，设置为NULL。
pdwAudioBytes2：没有用到，设置为0。
dwFlags：暂时没有研究。


UnLock()函数的原型如下。

```cpp
HRESULT Unlock(
         LPVOID pvAudioPtr1,
         DWORD dwAudioBytes1,
         LPVOID pvAudioPtr2,
         DWORD dwAudioBytes2
)
```

参数含义如下。
pvAudioPtr1：通过Lock()获取到的指向缓存数据的指针。
dwAudioBytes1：写入的数据量。
pvAudioPtr2：没有用到。

dwAudioBytes2：没有用到。

**2)等待播放完成**
根据此前设置的通知机制，使用WaitForMultipleObjects()等待缓冲区中的数据播放完毕，然后进入下一个循环。



## 播放音频流程总结

DirectSound播放PCM音频数据的流程如下图所示。

![](https://img-blog.csdn.net/20141028150152468?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


其中涉及到的几个结构体之间的关系如下图所示。


![](https://img-blog.csdn.net/20141028150131853?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)





## 代码

贴上源代码。

```cpp
/**
 * 最简单的DirectSound播放音频的例子（DirectSound播放PCM）
 * Simplest Audio Play DirectSound (DirectSound play PCM) 
 *
 * 雷霄骅 Lei Xiaohua
 * leixiaohua1020@126.com
 * 中国传媒大学/数字电视技术
 * Communication University of China / Digital TV Technology
 * http://blog.csdn.net/leixiaohua1020
 *
 * 本程序使用DirectSound播放PCM音频采样数据。
 * 是最简单的DirectSound播放音频的教程。
 *
 * 函数调用步骤如下: 
 *
 * [初始化]
 * DirectSoundCreate8(): 创建一个DirectSound对象。
 * SetCooperativeLevel(): 设置协作权限，不然没有声音。
 * IDirectSound8->CreateSoundBuffer(): 创建一个主缓冲区对象。
 * IDirectSoundBuffer->QueryInterface(IID_IDirectSoundBuffer8..): 
 *			创建一个副缓冲区对象，用来存储要播放的声音数据文件。
 * IDirectSoundBuffer8->QueryInterface(IID_IDirectSoundNotify..): 
 *			创建通知对象，通知应用程序指定播放位置已经达到。
 * IDirectSoundNotify8->SetNotificationPositions(): 设置通知位置。
 * IDirectSoundBuffer8->SetCurrentPosition(): 设置播放的起始点。
 * IDirectSoundBuffer8->Play(): 开始播放。
 *
 * [循环播放数据]
 * IDirectSoundBuffer8->Lock(): 锁定副缓冲区，准备写入数据。
 * fread(): 读取数据。
 * IDirectSoundBuffer8->Unlock(): 解锁副缓冲区。
 * WaitForMultipleObjects(): 等待“播放位置已经达到”的通知。
 *
 * This software plays PCM raw audio data using DirectSound.
 * It's the simplest tutorial about DirectSound.
 *
 * The process is shown as follows:
 *
 * [Init]
 * DirectSoundCreate8(): Init DirectSound object.
 * SetCooperativeLevel(): Must set, or we won't hear sound.
 * IDirectSound8->CreateSoundBuffer(): Create primary sound buffer.
 * IDirectSoundBuffer->QueryInterface(IID_IDirectSoundBuffer8..): 
 *			Create secondary sound buffer.
 * IDirectSoundBuffer8->QueryInterface(IID_IDirectSoundNotify..): 
 *			Create Notification object.
 * IDirectSoundNotify8->SetNotificationPositions():
 *			Set Notification Positions.
 * IDirectSoundBuffer8->SetCurrentPosition(): Set position to start.
 * IDirectSoundBuffer8->Play(): Begin to play.
 *
 * [Loop to play data]
 * IDirectSoundBuffer8->Lock(): Lock secondary buffer.
 * fread(): get PCM data.
 * IDirectSoundBuffer8->Unlock(): UnLock secondary buffer.
 * WaitForMultipleObjects(): Wait for Notifications.
 */
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <dsound.h>


#define MAX_AUDIO_BUF 4 
#define BUFFERNOTIFYSIZE 192000 


int sample_rate=44100;	//PCM sample rate
int channels=2;			//PCM channel number
int bits_per_sample=16;	//bits per sample

BOOL main(int argc,char * argv[])
{
	int i;
	FILE * fp;
	if((fp=fopen("../NocturneNo2inEflat_44.1k_s16le.pcm","rb"))==NULL){
		printf("cannot open this file\n");
		return -1;
	}

	IDirectSound8 *m_pDS=NULL;					
	IDirectSoundBuffer8 *m_pDSBuffer8=NULL;	//used to manage sound buffers.
	IDirectSoundBuffer *m_pDSBuffer=NULL;	
	IDirectSoundNotify8 *m_pDSNotify=NULL;		
	DSBPOSITIONNOTIFY m_pDSPosNotify[MAX_AUDIO_BUF];
	HANDLE m_event[MAX_AUDIO_BUF];

	SetConsoleTitle(TEXT("Simplest Audio Play DirectSound"));//Console Title
	//Init DirectSound
	if(FAILED(DirectSoundCreate8(NULL,&m_pDS,NULL)))
		return FALSE;
	if(FAILED(m_pDS->SetCooperativeLevel(FindWindow(NULL,TEXT("Simplest Audio Play DirectSound")),DSSCL_NORMAL)))
		return FALSE;


	DSBUFFERDESC dsbd;
	memset(&dsbd,0,sizeof(dsbd));
	dsbd.dwSize=sizeof(dsbd);
	dsbd.dwFlags=DSBCAPS_GLOBALFOCUS | DSBCAPS_CTRLPOSITIONNOTIFY |DSBCAPS_GETCURRENTPOSITION2;
	dsbd.dwBufferBytes=MAX_AUDIO_BUF*BUFFERNOTIFYSIZE; 
	//WAVE Header
	dsbd.lpwfxFormat=(WAVEFORMATEX*)malloc(sizeof(WAVEFORMATEX));
	dsbd.lpwfxFormat->wFormatTag=WAVE_FORMAT_PCM;   
	/* format type */
	(dsbd.lpwfxFormat)->nChannels=channels;          
	/* number of channels (i.e. mono, stereo...) */
	(dsbd.lpwfxFormat)->nSamplesPerSec=sample_rate;     
	/* sample rate */
	(dsbd.lpwfxFormat)->nAvgBytesPerSec=sample_rate*(bits_per_sample/8)*channels; 
	/* for buffer estimation */
	(dsbd.lpwfxFormat)->nBlockAlign=(bits_per_sample/8)*channels;        
	/* block size of data */
	(dsbd.lpwfxFormat)->wBitsPerSample=bits_per_sample;     
	/* number of bits per sample of mono data */
	(dsbd.lpwfxFormat)->cbSize=0;

	//Creates a sound buffer object to manage audio samples. 
	HRESULT hr1;
	if( FAILED(m_pDS->CreateSoundBuffer(&dsbd,&m_pDSBuffer,NULL))){   
		return FALSE;
	}
	if( FAILED(m_pDSBuffer->QueryInterface(IID_IDirectSoundBuffer8,(LPVOID*)&m_pDSBuffer8))){
		return FALSE ;
	}
	//Get IDirectSoundNotify8
	if(FAILED(m_pDSBuffer8->QueryInterface(IID_IDirectSoundNotify,(LPVOID*)&m_pDSNotify))){
		return FALSE ;
	}
	for(i =0;i<MAX_AUDIO_BUF;i++){
		m_pDSPosNotify[i].dwOffset =i*BUFFERNOTIFYSIZE;
		m_event[i]=::CreateEvent(NULL,false,false,NULL); 
		m_pDSPosNotify[i].hEventNotify=m_event[i];
	}
	m_pDSNotify->SetNotificationPositions(MAX_AUDIO_BUF,m_pDSPosNotify);
	m_pDSNotify->Release();

	//Start Playing
	BOOL isPlaying =TRUE;
	LPVOID buf=NULL;
	DWORD  buf_len=0;
	DWORD res=WAIT_OBJECT_0;
	DWORD offset=BUFFERNOTIFYSIZE;

	m_pDSBuffer8->SetCurrentPosition(0);
	m_pDSBuffer8->Play(0,0,DSBPLAY_LOOPING);
	//Loop
	while(isPlaying){
		if((res >=WAIT_OBJECT_0)&&(res <=WAIT_OBJECT_0+3)){
			m_pDSBuffer8->Lock(offset,BUFFERNOTIFYSIZE,&buf,&buf_len,NULL,NULL,0);
			if(fread(buf,1,buf_len,fp)!=buf_len){
				//File End
				//Loop:
				fseek(fp, 0, SEEK_SET);
				fread(buf,1,buf_len,fp);
				//Close:
				//isPlaying=0;
			}
			m_pDSBuffer8->Unlock(buf,buf_len,NULL,0);
			offset+=buf_len;
			offset %= (BUFFERNOTIFYSIZE * MAX_AUDIO_BUF);
			printf("this is %7d of buffer\n",offset);
		}
		res = WaitForMultipleObjects (MAX_AUDIO_BUF, m_event, FALSE, INFINITE);
	}

	return 0;
}
```



## 运行结果

代码运行之后，会弹出一个“控制台”对话框如下图所示。同时音频设备里面可以听到播放的声音。

![](https://img-blog.csdn.net/20141028150252937?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)




## 下载

代码位于“Simplest Media Play”中


SourceForge项目地址：[https://sourceforge.net/projects/simplestmediaplay/](https://sourceforge.net/projects/simplestmediaplay/)

CSDN下载地址：[http://download.csdn.net/detail/leixiaohua1020/8054395](http://download.csdn.net/detail/leixiaohua1020/8054395)






注：

该项目会不定时的更新并修复一些小问题，最新的版本请参考该系列文章的总述页面：

[《最简单的视音频播放示例1：总述》](http://blog.csdn.net/leixiaohua1020/article/details/40246783)

上述工程包含了使用各种API（Direct3D，OpenGL，GDI，DirectSound，SDL2）播放多媒体例子。其中音频输入为PCM采样数据。输出至系统的声卡播放出来。视频输入为YUV/RGB像素数据。输出至显示器上的一个窗口播放出来。
通过本工程的代码初学者可以快速学习使用这几个API播放视频和音频的技术。
一共包括了如下几个子工程：
simplest_audio_play_directsound: 使用DirectSound播放PCM音频采样数据。
simplest_audio_play_sdl2: 使用SDL2播放PCM音频采样数据。
simplest_video_play_direct3d: 使用Direct3D的Surface播放RGB/YUV视频像素数据。
simplest_video_play_direct3d_texture:使用Direct3D的Texture播放RGB视频像素数据。
simplest_video_play_gdi: 使用GDI播放RGB/YUV视频像素数据。
simplest_video_play_opengl: 使用OpenGL播放RGB/YUV视频像素数据。
simplest_video_play_opengl_texture:使用OpenGL的Texture播放YUV视频像素数据。
simplest_video_play_sdl2: 使用SDL2播放RGB/YUV视频像素数据。](https://so.csdn.net/so/search/s.do?q=PCM&t=blog)](https://so.csdn.net/so/search/s.do?q=音频&t=blog)](https://so.csdn.net/so/search/s.do?q=DirectSound&t=blog)




