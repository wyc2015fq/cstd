# 分离PS/TS流的播放库，提供测试程序 - 爱写代码的自由人 - CSDN博客





2018年02月23日 17:38:55[zhoubotong2012](https://me.csdn.net/zhoubotong2012)阅读数：107








在监控和广播电视领域经常会用到PS和TS格式的文件。从PS，TS流分离出视频和音频可以用FFmpeg，FFmpeg对PS，TS流支持是很好的，支持从文件或内存流中读数据。怎么从内存流中读PS和TS流可以参考我的一篇博文：http://blog.csdn.net/zhoubotong2012/article/details/79349359。在这里我向大家介绍另外一种方法（不依赖FFmpeg），我写了一个解析PS和TS流的动态库，封装了几个主要的操作接口，支持文件和内存流，可以分离出视频包和音频包。

下面看看接口的声明：

```cpp
//初始化SDK
PLAYPT_API BOOL  PT_InitSDK();

//注销SDK
PLAYPT_API BOOL  PT_UnitSDK();

//获取SDK版本号
PLAYPT_API LONG  PT_GetSDKVersion();

//打开PS/TS格式的流，支持从文件或内存读取流
//参数：
// handle -- 返回的这个句柄来调用其他函数; 
// nStreamType --流类型（1--PS, 2--TS）
// dwError -- 失败时返回的错误码; 
//
PLAYPT_API BOOL    PT_OpenFile(SOURCE_TYPE srcType, LPCTSTR lpszFilePath, int nStreamType, int & handle, DWORD & dwError);

 //关闭文件或内存流
PLAYPT_API BOOL    PT_CloseFile(int handle);

PLAYPT_API BOOL    PT_Pause(int handle);  //暂停播放

PLAYPT_API BOOL    PT_Play(int handle);  //开始播放

PLAYPT_API BOOL    PT_IsRunning(int handle); //是否正在播放

PLAYPT_API BOOL    PT_AddStreamData(int handle, BYTE * pData, int nBytes ); //支持PS/TS流输入

PLAYPT_API BOOL    PT_SetFrameCallback(int handle, EsFrameCallback  lpFrameCB); 

PLAYPT_API BOOL    PT_SetStreamParams(int handle, BOOL bHasPMT); //bHasPMT--TS流中是否有PMT表

PLAYPT_API BOOL    PT_GetVideoInfo(int handle, VideoEncodeFormat & videoformat, int& nWidth, int& nHeight, int & nTrackNum); //获取视频流的信息

PLAYPT_API BOOL    PT_GetAudioInfo(int handle, AudioEncodeFormat & nType, int & nTrackNum);//获取音频流的信息

//探测格式，输入的是视频ES流，调用后返回视频的宽高。
// 这个函数要传入一块内存数据，并且要传入一个VideoEncodeFormat类型的参数，即用户要预知视频的编码格式。解析成功后获得视频的宽高信息。目前只支持MPEG2、MPEG4、H264。
// 该接口不需要SDK句柄。
PLAYPT_API BOOL    PT_ProbeVideoInfo(BYTE * pData, int nBytes, VideoEncodeFormat  videoformat, int& nWidth, int& nHeight); //探测格式，获取视频流的信息.

PLAYPT_API BOOL    PT_GetFileDuration(int handle, __int64 & llDuration); //获取文件播放时长（只对文件有效）

PLAYPT_API BOOL    PT_Seek(int handle, __int64 llPos); //跳到某个播放时间点（只对文件有效）

PLAYPT_API BOOL    PT_GetPlayPos(int handle, __int64 & llPos); //获取当前播放时间点
```

这个库的用法很简单，使用步骤：

  1.  调用PT_InitSDK()接口初始化SDK。

  2.  调用 PT_OpenFile打开一个文件或打开内存流。 PT_OpenFile传入的参数包括：SOURCE_TYPE srcType（流的类型，文件或内存流）, LPCTSTR lpszFilePath（文件路径）, int nStreamType（1--PS流，2--TS流），然后接口返回一个句柄，我们要把这个句柄保存起来，后面调用其他函数要用到这个句柄。如果打开成功，返回True，如果出错，则会返回false，参数dwError携带了出错的错误码。

 3. 获取流的属性。打开一个输入文件或内存流之后，可以调用PT_GetVideoInfo和PT_GetAudioInfo来获取视频和音频轨道的信息，还有可以通过接口PT_GetFileDuration获取文件播放时长。

 4. 设置回调函数。 调用PT_SetFrameCallback接口设置分离后数据的回调接收函数，这个回调函数的原型如下：

typedef int  (WINAPI * EsFrameCallback)(int handle, BYTE * pBuf, int nBufSize, int nTrackNum,  __int64 llPts, int nFrameType);

回调函数的说明和各个参数的意义：

//函数作用：回调视频帧，音频帧数据，其中年nFrameType=0的为音频帧

//参数意义：

// handle -- SDK的播放句柄；

// pBuf -- ES帧的数据起始地址；

// nBufSize -- 数据的大小；

// nTrackNum--流ID号，区别不同流的唯一标识符；

// llPts -- 时间戳，以10000000L为单位；

// nFrameType -- 帧的类型。nFrameType = 0, 音频帧； nFrameType = 1, I帧；nFrameType = 2， P帧； nFrameType = 3， B帧

  5. 调用PT_Play接口运行“播放”任务，实际上SDK没有做解码和播放，只是做了Demux，即从TS/PS分离出视频和音频，如果用户设置了回调函数，则SDK会把分离出来的数据通过回调函数传递给应用层。

 6. 如果输入源是内存流，则需要不断地调用PT_AddStreamData接口把内存中的数据传给SDK，PT_AddStreamData需要传递的参数包括：SDK实例句柄，数据Buf的地址，Buff大小。如果输入源是文件，则在任务运行的过程中可以调用PT_GetPlayPos接口查询当前的播放进度。

7.  “播放”完成后调用 PT_CloseFile关闭输入源。

**注意：该SDK只做分离操作，解码和播放视音频的操作是由应用层来完成的。这个库是不开源的，只提供了头文件，LIB和编译好的DLL。**

我写了一个Demo来演示怎么使用这个SDK，SDK和Demo的下载地址：[点击打开链接](http://download.csdn.net/download/zhoubotong2012/10255728)



