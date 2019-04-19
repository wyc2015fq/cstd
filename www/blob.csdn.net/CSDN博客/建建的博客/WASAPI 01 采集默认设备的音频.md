# WASAPI 01 采集默认设备的音频 - 建建的博客 - CSDN博客
2018年05月09日 11:25:31[纪建](https://me.csdn.net/u013898698)阅读数：769

音频采集或播放时，需要用**MMDevice API**来枚举和获取指定设备，用**WASAPI**来处理应用程序和音频设备之间的音频流数据，如采集或播放。
#### 1. MMDevice API
> 
The Windows Multimedia Device (MMDevice) API **enables audio clients to discover audio endpoint devices, determine their capabilities, and create driver instances for those devices**.
Header file Mmdeviceapi.h defines the interfaces in the MMDevice API.
音频client利用MMDevice API来**发现audio endpoint devices, 为devices创建驱动实例**等。
所需头文件:
```cpp
```cpp
#include <MMDeviceAPI.h>
```
```
##### 1.1 创建 IMMDeviceEnumerator interface
```
const CLSID CLSID_MMDeviceEnumerator = __uuidof(MMDeviceEnumerator);
  const IID IID_IMMDeviceEnumerator = __uuidof(IMMDeviceEnumerator);
  hr = CoCreateInstance(
         CLSID_MMDeviceEnumerator, NULL,
         CLSCTX_ALL, IID_IMMDeviceEnumerator,
         (void**)&pEnumerator);
```
> 
**A device enumerator is an object with an [**IMMDeviceEnumerator**](https://link.jianshu.com?t=https%3A%2F%2Fmsdn.microsoft.com%2Fen-us%2Flibrary%2Fwindows%2Fdesktop%2Fdd371399%28v%3Dvs.85%29.aspx) interface**.
variable pEnumerator is a pointer to the IMMDeviceEnumerator interface of a device-enumerator object.
**IMMDeviceEnumerator provides methods for enumerating audio endpoint devices**.
##### 1.2 获取默认音频设备GetDefaultAudioEndpoint
> 
The GetDefaultAudioEndpoint method retrieves the default audio endpoint for the specified data-flow direction and role.
[https://msdn.microsoft.com/en-us/library/windows/desktop/dd371401(v=vs.85).aspx](https://link.jianshu.com?t=https%3A%2F%2Fmsdn.microsoft.com%2Fen-us%2Flibrary%2Fwindows%2Fdesktop%2Fdd371401%28v%3Dvs.85%29.aspx)
```
HRESULT GetDefaultAudioEndpoint(
  [in]  EDataFlow dataFlow,
  [in]  ERole     role,
  [out] IMMDevice **ppDevice
);
```
![](https://upload-images.jianshu.io/upload_images/1720840-f606b10f12f4d3d3.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/700)
参数说明
- 获取麦克风
```php
```php
IMMDevice           *pDevice        = NULL;
...
hr = pEnumerator->GetDefaultAudioEndpoint(eCapture, eConsole, &pDevice);
```
```
- 获取render
```php
```php
hr = pEnumerator->GetDefaultAudioEndpoint(eRender, eConsole, &pDevice);
```
```
#### 2. WASAPI
> 
The Windows Audio Session API (WASAPI) **enables client applications to manage the flow of audio data between the application and an [audio endpoint device](https://link.jianshu.com?t=https%3A%2F%2Fmsdn.microsoft.com%2Fen-us%2Flibrary%2Fwindows%2Fdesktop%2Fdd370793%28v%3Dvs.85%29.aspx)**.
Header files Audioclient.h and Audiopolicy.h define the WASAPI interfaces.
```cpp
```cpp
#include <AudioClient.h>
```
```
> 
An audio session is **a group of related audio streams that a WASAPI client can manage collectively**. Clients can control the volume level and muting state of each individual session. The system applies client-specified volume and mute settings uniformly to all of the streams in the session.
**Every audio stream is a member of an [audio session](https://link.jianshu.com?t=https%3A%2F%2Fmsdn.microsoft.com%2Fen-us%2Flibrary%2Fwindows%2Fdesktop%2Fdd370796%28v%3Dvs.85%29.aspx)**. Through the session abstraction, a WASAPI client can identify an audio stream as a member of a group of related audio streams. The system can manage all of the streams in the session as a single unit.
audio session是相关音频流的group，WASAPI client可以session中的流进行集体处理。
> 
The audio engine is the [user-mode audio component](https://link.jianshu.com?t=https%3A%2F%2Fmsdn.microsoft.com%2Fen-us%2Flibrary%2Fwindows%2Fdesktop%2Fdd316780%28v%3Dvs.85%29.aspx) through which applications share access to an audio endpoint device.
The audio engine transports audio data between an endpoint buffer and an endpoint device.
To play an audio stream through a rendering endpoint device, an application periodically writes audio data to a rendering endpoint buffer. The audio engine mixes the streams from the various applications.
To record an audio stream from a capture endpoint device, an application periodically reads audio data from a capture endpoint buffer.
**程序可通过audio engine，以共享模式访问audio endpoint device(比如麦克风  或Speakers)**。
audio engine在endpoint buffer和endpoint device之间传输数据。
当播放音频数据时，程序向rendering endpoint buffer周期性写入数据。
当采集音频数据时，程序从capture endpoint buffer周期性读取数据。
> 
WASAPI consists of several interfaces.
The first of these is the [**IAudioClient**](https://link.jianshu.com?t=https%3A%2F%2Fmsdn.microsoft.com%2Fen-us%2Flibrary%2Fwindows%2Fdesktop%2Fdd370865%28v%3Dvs.85%29.aspx) interface.
To access the WASAPI interfaces,
a client first obtains a reference to the **IAudioClient** interface of an audio endpoint device by calling the [**IMMDevice::Activate**](https://link.jianshu.com?t=https%3A%2F%2Fmsdn.microsoft.com%2Fen-us%2Flibrary%2Fwindows%2Fdesktop%2Fdd371405%28v%3Dvs.85%29.aspx) method with parameter *iid* set to **REFIID** IID_IAudioClient.
The client calls the [**IAudioClient::Initialize**](https://link.jianshu.com?t=https%3A%2F%2Fmsdn.microsoft.com%2Fen-us%2Flibrary%2Fwindows%2Fdesktop%2Fdd370875%28v%3Dvs.85%29.aspx) method to initialize a stream on an endpoint device.
After initializing a stream, the client can obtain references to the other WASAPI interfaces by calling the [**IAudioClient::GetService**](https://link.jianshu.com?t=https%3A%2F%2Fmsdn.microsoft.com%2Fen-us%2Flibrary%2Fwindows%2Fdesktop%2Fdd370873%28v%3Dvs.85%29.aspx) method.
使用WASAPI的几个重要函数:
- **IMMDevice::ActivateIMMDevice::Activate来获取an audio endpoint device的IAudioClient interface引用**。
> 
In Windows Vista, which supports endpoint devices, the process of connecting to the same endpoint device is much simpler:
- **Select a microphone from a collection of endpoint devices**.
- **Activate an audio-capture interface on that microphone**.
The operating system does all the work necessary to identify and enable the endpoint device. For example, if the data path from the microphone includes a multiplexer, the system automatically selects the microphone input to the multiplexer.
Windows Vista，以音频采集为例，连接到endpoint device就两步:
(1) 从设备集合中选择一个麦克风。
(2) 用Activate激活该麦克风的音频采集接口。
```
IAudioClient        *pAudioClient   = NULL;
...
hr = pDevice->Activate(IID_IAudioClient, CLSCTX_ALL, NULL, (void**)&pAudioClient);
```
- **IAudioClient::InitializeIAudioClient::Initialize用来在endpoint device初始化流**。
通用格式:
```php
```php
hr = pAudioClient->GetMixFormat(&pwfx);
    EXIT_ON_ERROR(hr)
    hr = pAudioClient->Initialize(
                         AUDCLNT_SHAREMODE_SHARED,
                         0,
                         hnsRequestedDuration,
                         0,
                         pwfx,
                         NULL);
    EXIT_ON_ERROR(hr)
```
```
官方demo中，
- 采集时
(1) 采集麦克风时
```cpp
```cpp
HRESULT hr = _AudioClient->Initialize(AUDCLNT_SHAREMODE_SHARED, 
        AUDCLNT_STREAMFLAGS_EVENTCALLBACK | AUDCLNT_STREAMFLAGS_NOPERSIST, 
        _EngineLatencyInMS*10000, 0, _MixFormat, NULL);
    if (FAILED(hr))
    {
        printf("Unable to initialize audio client: %x.\n", hr);
        return false;
    }
```
```
AUDCLNT_STREAMFLAGS_EVENTCALLBACK 表示当audio buffer数据就绪时，会给系统发个信号，也就是事件触发。
(2) 采集立体声混音
希望同时采集本机声卡上的默认麦克风和默认render的数据, 用**AUDCLNT_STREAMFLAGS_LOOPBACK参数**来设置。
```php
```php
hr = pAudioClient->Initialize(
        AUDCLNT_SHAREMODE_SHARED,
        AUDCLNT_STREAMFLAGS_LOOPBACK, 
        hnsRequestedDuration,
        0,
        pwfx,
        NULL);
```
```
```cpp
```cpp
/*
    The AUDCLNT_STREAMFLAGS_LOOPBACK flag enables loopback recording. 
    In loopback recording, the audio engine copies the audio stream 
    that is being played by a rendering endpoint device into an audio endpoint buffer 
    so that a WASAPI client can capture the stream. 
    If this flag is set, the IAudioClient::Initialize method attempts to open a capture buffer on the rendering device. 
    This flag is valid only for a rendering device 
    and only if the Initialize call sets the ShareMode parameter to AUDCLNT_SHAREMODE_SHARED. 
    Otherwise the Initialize call will fail. 
    If the call succeeds, 
    the client can call the IAudioClient::GetService method 
    to obtain an IAudioCaptureClient interface on the rendering device. 
    For more information, see Loopback Recording.
*/
```
```
AUDCLNT_STREAMFLAGS_LOOPBACK模式下，**音频engine会将rending设备正在播放的音频流， 拷贝一份到音频的endpoint buffer**。
这样的话，WASAPI client可以采集到the stream。
**如果AUDCLNT_STREAMFLAGS_LOOPBACK被设置， IAudioClient::Initialize会尝试在rending设备开辟一块capture buffer**。
AUDCLNT_STREAMFLAGS_LOOPBACK只对rending设备有效，
**Initialize仅在AUDCLNT_SHAREMODE_SHARED时才可以使用, 否则Initialize会失败**。
Initialize成功后，可以用IAudioClient::GetService可获取该rending设备的IAudioCaptureClient接口。
- render时
```cpp
```cpp
int _EngineLatencyInMS = 50;
...
HRESULT hr = _AudioClient->Initialize(AUDCLNT_SHAREMODE_SHARED, 
        AUDCLNT_STREAMFLAGS_NOPERSIST, 
        _EngineLatencyInMS*10000, 
        0, 
        _MixFormat, 
        NULL);
    if (FAILED(hr))
    {
        printf("Unable to initialize audio client: %x.\n", hr);
        return false;
    }
```
```
- **IAudioClient::GetService初始化流之后，可调用IAudioClient::GetService来获取其它 WASAPI interfaces的引用**。
- 采集时
```cpp
```cpp
IAudioCaptureClient *pCaptureClient = NULL;
...
hr = pAudioClient->GetService(
                         IID_IAudioCaptureClient,
                         (void**)&pCaptureClient);
    EXIT_ON_ERROR(hr)
```
```
- render时
```cpp
```cpp
IAudioRenderClient *pRenderClient = NULL;
...
 hr = pAudioClient->GetService(
                         IID_IAudioRenderClient,
                         (void**)&pRenderClient);
    EXIT_ON_ERROR(hr)
```
```
- **IAudioClient::Start**
> 
**Start** is a control method that the client calls to start the audio stream.
**Starting the stream causes the [**IAudioClient**](https://link.jianshu.com?t=https%3A%2F%2Fmsdn.microsoft.com%2Fen-us%2Flibrary%2Fwindows%2Fdesktop%2Fdd370865%28v%3Dvs.85%29.aspx) object to begin streaming data between the endpoint buffer and the audio engine**.
It also causes the stream's audio clock to resume counting from its current position.
- **IAudioCaptureClient::GetNextPacketSize**
> 
The GetNextPacketSize method retrieves **the number of frames in the next data packet in the capture endpoint buffer**.
这里有两个注意的。
(1) 单位为**audio frame**。
(2) 注意是采集buffer(capture endpoint buffer)
> 
Use this method only with shared-mode streams. It does not work with exclusive-mode streams.
仅在共享模式下生效，独占模式下无效。
> 
**Before calling the [**IAudioCaptureClient::GetBuffer**](https://link.jianshu.com?t=https%3A%2F%2Fmsdn.microsoft.com%2Fen-us%2Flibrary%2Fwindows%2Fdesktop%2Fdd370859%28v%3Dvs.85%29.aspx) method to retrieve the next data packet, the client can call **GetNextPacketSize** to retrieve the number of audio frames in the next packet**.
The count reported by **GetNextPacketSize** matches the count retrieved in the **GetBuffer** call (through the *pNumFramesToRead* output parameter) that follows the **GetNextPacketSize** call.
在调用**GetBuffer**之前，可调用**GetNextPacketSize**来获取下一个数据包的音频帧个数。
> 
A packet always consists of an integral number of audio frames.
**GetNextPacketSize** must be called in the same thread as the [**GetBuffer**](https://link.jianshu.com?t=https%3A%2F%2Fmsdn.microsoft.com%2Fen-us%2Flibrary%2Fwindows%2Fdesktop%2Fdd370859%28v%3Dvs.85%29.aspx) and [**IAudioCaptureClient::ReleaseBuffer**](https://link.jianshu.com?t=https%3A%2F%2Fmsdn.microsoft.com%2Fen-us%2Flibrary%2Fwindows%2Fdesktop%2Fdd370861%28v%3Dvs.85%29.aspx) method calls that get and release the packets in the capture endpoint buffer.
**GetNextPacketSize**必须和**GetBuffer**及**IAudioCaptureClient::ReleaseBuffer**在同一线程中调用。
- **IAudioCaptureClient::GetBuffer**
> 
Retrieves a pointer to the next available packet of data in the capture endpoint buffer.
最重要的函数。用于获取capture endpoint buffer中下一个数据包的指针。
```
HRESULT GetBuffer(
  [out] BYTE   **ppData,
  [out] UINT32 *pNumFramesToRead,
  [out] DWORD  *pdwFlags,
  [out] UINT64 *pu64DevicePosition,
  [out] UINT64 *pu64QPCPosition
);
```
[https://msdn.microsoft.com/en-us/library/windows/desktop/dd370859(v=vs.85).aspx](https://link.jianshu.com?t=https%3A%2F%2Fmsdn.microsoft.com%2Fen-us%2Flibrary%2Fwindows%2Fdesktop%2Fdd370859%28v%3Dvs.85%29.aspx)
> 
![](https://upload-images.jianshu.io/upload_images/1720840-0013287e61d702df.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/700)
参数
ppData: 可读的下个数据包的起始地址。
pNumFramesToRead : 下个数据包的长度(单位为audio frames)。客户端或者全部读取或者一个也不读。
pdwFlags:
```php
```php
The method writes either 0 or the bitwise-OR combination of one or more of the following [**_AUDCLNT_BUFFERFLAGS**](https://msdn.microsoft.com/en-us/library/windows/desktop/dd371458(v=vs.85).aspx) enumeration values:
AUDCLNT_BUFFERFLAGS_SILENT
AUDCLNT_BUFFERFLAGS_DATA_DISCONTINUITY
AUDCLNT_BUFFERFLAGS_TIMESTAMP_ERROR
```
```
可利用flag来判断是否为静音。
```java
```java
enum _AUDCLNT_BUFFERFLAGS
    {   AUDCLNT_BUFFERFLAGS_DATA_DISCONTINUITY  = 0x1,
    AUDCLNT_BUFFERFLAGS_SILENT  = 0x2,
    AUDCLNT_BUFFERFLAGS_TIMESTAMP_ERROR = 0x4
    } ;
其中
AUDCLNT_BUFFERFLAGS_SILENT
Treat all of the data in the packet as silence and ignore the actual data values.
```
```
代码如下:
```cpp
```cpp
//
//  The flags on capture tell us information about the data.
//
//  We only really care about the silent flag 
//  since we want to put frames of silence into the buffer
//  when we receive silence.  
//  We rely on the fact that a logical bit 0 is silence for both float and int formats.
//
if (flags & AUDCLNT_BUFFERFLAGS_SILENT)
{
    //
    //  Fill 0s from the capture buffer to the output buffer.
    //
    ZeroMemory(&_CaptureBuffer[_CurrentCaptureIndex], framesToCopy*_FrameSize);
}
else
{
    //
    //  Copy data from the audio engine buffer to the output buffer.
    //
    CopyMemory(&_CaptureBuffer[_CurrentCaptureIndex], pData, framesToCopy*_FrameSize);
}
```
```
#### 核心流程
![](https://upload-images.jianshu.io/upload_images/1720840-fec9827d8b886ee8.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/700)
采集默认设备的音频 流程
#### 代码
```cpp
```cpp
// WindowsAudioSession.cpp
// 基本的利用WAS采集音频的demo
/*
References:
https://msdn.microsoft.com/en-us/library/dd370800(v=vs.85).aspx
http://blog.csdn.net/leave_rainbow/article/details/50917043
http://blog.csdn.net/lwsas1/article/details/46862195?locationNum=1
WindowsSDK7-Samples-master\multimedia\audio\CaptureSharedEventDriven
*/
#include "stdafx.h"
#include <MMDeviceAPI.h>
#include <AudioClient.h>
#include <iostream>
using namespace std;
// ns(nanosecond) : 纳秒，时间单位。一秒的十亿分之一
// 1秒=1000毫秒; 1毫秒=1000微秒; 1微秒=1000纳秒
// The REFERENCE_TIME data type defines the units for reference times in DirectShow. 
// Each unit of reference time is 100 nanoseconds.(100纳秒为一个REFERENCE_TIME时间单位)
// REFERENCE_TIME time units per second and per millisecond
#define REFTIMES_PER_SEC       (10000000)
#define REFTIMES_PER_MILLISEC  (10000)
#define EXIT_ON_ERROR(hres)  \
    if (FAILED(hres)) { goto Exit; }
#define SAFE_RELEASE(punk)  \
    if ((punk) != NULL)  \
                { (punk)->Release(); (punk) = NULL; }
const CLSID CLSID_MMDeviceEnumerator = __uuidof(MMDeviceEnumerator);
const IID   IID_IMMDeviceEnumerator  = __uuidof(IMMDeviceEnumerator);
const IID   IID_IAudioClient         = __uuidof(IAudioClient);
const IID   IID_IAudioCaptureClient  = __uuidof(IAudioCaptureClient);
#define MoveMemory RtlMoveMemory
#define CopyMemory RtlCopyMemory
#define FillMemory RtlFillMemory
#define ZeroMemory RtlZeroMemory
#define min(a,b)            (((a) < (b)) ? (a) : (b))
//
//  WAV file writer.
//
//  This is a VERY simple .WAV file writer.
//
//
//  A wave file consists of:
//
//  RIFF header:    8 bytes consisting of the signature "RIFF" followed by a 4 byte file length.
//  WAVE header:    4 bytes consisting of the signature "WAVE".
//  fmt header:     4 bytes consisting of the signature "fmt " followed by a WAVEFORMATEX 
//  WAVEFORMAT:     <n> bytes containing a waveformat structure.
//  DATA header:    8 bytes consisting of the signature "data" followed by a 4 byte file length.
//  wave data:      <m> bytes containing wave data.
//
//
//  Header for a WAV file - we define a structure describing the first few fields in the header for convenience.
//
struct WAVEHEADER
{
    DWORD   dwRiff;                     // "RIFF"
    DWORD   dwSize;                     // Size
    DWORD   dwWave;                     // "WAVE"
    DWORD   dwFmt;                      // "fmt "
    DWORD   dwFmtSize;                  // Wave Format Size
};
//  Static RIFF header, we'll append the format to it.
const BYTE WaveHeader[] = 
{
    'R',   'I',   'F',   'F',  0x00,  0x00,  0x00,  0x00, 'W',   'A',   'V',   'E',   'f',   'm',   't',   ' ', 0x00, 0x00, 0x00, 0x00
};
//  Static wave DATA tag.
const BYTE WaveData[] = { 'd', 'a', 't', 'a'};
//
//  Write the contents of a WAV file.  We take as input the data to write and the format of that data.
//
bool WriteWaveFile(HANDLE FileHandle, const BYTE *Buffer, const size_t BufferSize, const WAVEFORMATEX *WaveFormat)
{
    DWORD waveFileSize = sizeof(WAVEHEADER) + sizeof(WAVEFORMATEX) + WaveFormat->cbSize + sizeof(WaveData) + sizeof(DWORD) + static_cast<DWORD>(BufferSize);
    BYTE *waveFileData = new (std::nothrow) BYTE[waveFileSize];
    BYTE *waveFilePointer = waveFileData;
    WAVEHEADER *waveHeader = reinterpret_cast<WAVEHEADER *>(waveFileData);
    if (waveFileData == NULL)
    {
        printf("Unable to allocate %d bytes to hold output wave data\n", waveFileSize);
        return false;
    }
    //
    //  Copy in the wave header - we'll fix up the lengths later.
    //
    CopyMemory(waveFilePointer, WaveHeader, sizeof(WaveHeader));
    waveFilePointer += sizeof(WaveHeader);
    //
    //  Update the sizes in the header.
    //
    waveHeader->dwSize = waveFileSize - (2 * sizeof(DWORD));
    waveHeader->dwFmtSize = sizeof(WAVEFORMATEX) + WaveFormat->cbSize;
    //
    //  Next copy in the WaveFormatex structure.
    //
    CopyMemory(waveFilePointer, WaveFormat, sizeof(WAVEFORMATEX) + WaveFormat->cbSize);
    waveFilePointer += sizeof(WAVEFORMATEX) + WaveFormat->cbSize;
    //
    //  Then the data header.
    //
    CopyMemory(waveFilePointer, WaveData, sizeof(WaveData));
    waveFilePointer += sizeof(WaveData);
    *(reinterpret_cast<DWORD *>(waveFilePointer)) = static_cast<DWORD>(BufferSize);
    waveFilePointer += sizeof(DWORD);
    //
    //  And finally copy in the audio data.
    //
    CopyMemory(waveFilePointer, Buffer, BufferSize);
    //
    //  Last but not least, write the data to the file.
    //
    DWORD bytesWritten;
    if (!WriteFile(FileHandle, waveFileData, waveFileSize, &bytesWritten, NULL))
    {
        printf("Unable to write wave file: %d\n", GetLastError());
        delete []waveFileData;
        return false;
    }
    if (bytesWritten != waveFileSize)
    {
        printf("Failed to write entire wave file\n");
        delete []waveFileData;
        return false;
    }
    delete []waveFileData;
    return true;
}
//
//  Write the captured wave data to an output file so that it can be examined later.
//
void SaveWaveData(BYTE *CaptureBuffer, size_t BufferSize, const WAVEFORMATEX *WaveFormat)
{
    HRESULT hr = NOERROR;
    SYSTEMTIME st;
    GetLocalTime(&st);
    char waveFileName[_MAX_PATH] = {0};
    sprintf(waveFileName, ".\\WAS_%04d-%02d-%02d_%02d_%02d_%02d_%02d.wav", 
        st.wYear, st.wMonth, st.wDay, 
        st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
    HANDLE waveHandle = CreateFile(waveFileName, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, 
        FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, 
        NULL);
    if (waveHandle != INVALID_HANDLE_VALUE)
    {
        if (WriteWaveFile(waveHandle, CaptureBuffer, BufferSize, WaveFormat))
        {
            printf("Successfully wrote WAVE data to %s\n", waveFileName);
        }
        else
        {
            printf("Unable to write wave file\n");
        }
        CloseHandle(waveHandle);
    }
    else
    {
        printf("Unable to open output WAV file %s: %d\n", waveFileName, GetLastError());
    }
                    
}
#define DEF_CAPTURE_MIC
/*
注1: 静音时 填充0
注2: 测试时 应该将录音设备中的麦克风设为默认设备
注3: 定义DEF_CAPTURE_MIC时仅测试采集麦克风 否则测试采集声卡。
注4:
     测试采集声卡:
     Initialize时需要设置AUDCLNT_STREAMFLAGS_LOOPBACK
     这种模式下，音频engine会将rending设备正在播放的音频流， 拷贝一份到音频的endpoint buffer
     这样的话，WASAPI client可以采集到the stream.
     此时仅采集到Speaker的声音
*/
int _tmain(int argc, _TCHAR* argv[])
{
    HRESULT hr;
    IMMDeviceEnumerator *pEnumerator    = NULL;
    IMMDevice           *pDevice        = NULL;
    IAudioClient        *pAudioClient   = NULL;
    IAudioCaptureClient *pCaptureClient = NULL;
    WAVEFORMATEX        *pwfx           = NULL;
    REFERENCE_TIME hnsRequestedDuration = REFTIMES_PER_SEC;
    UINT32         bufferFrameCount;
    UINT32         numFramesAvailable;
    BYTE           *pData;
    UINT32         packetLength = 0;
    DWORD          flags;
    hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
    if (FAILED(hr))
    {
        printf("Unable to initialize COM in thread: %x\n", hr);
        return hr;
    }
    // 首先枚举你的音频设备
    // 你可以在这个时候获取到你机器上所有可用的设备，并指定你需要用到的那个设置
    hr = CoCreateInstance(CLSID_MMDeviceEnumerator, 
                          NULL,
                          CLSCTX_ALL, 
                          IID_IMMDeviceEnumerator,
                          (void**)&pEnumerator);
    EXIT_ON_ERROR(hr)
#ifdef DEF_CAPTURE_MIC
    hr = pEnumerator->GetDefaultAudioEndpoint(eCapture, eConsole, &pDevice); // 采集麦克风
    //hr = pEnumerator->GetDefaultAudioEndpoint(eCapture,  eMultimedia, &pDevice);
#else 
    hr = pEnumerator->GetDefaultAudioEndpoint(eRender, eConsole, &pDevice);  // 采集声卡
#endif  
    EXIT_ON_ERROR(hr)
    // 创建一个管理对象，通过它可以获取到你需要的一切数据
    hr = pDevice->Activate(IID_IAudioClient, CLSCTX_ALL, NULL, (void**)&pAudioClient);
    EXIT_ON_ERROR(hr)
    hr = pAudioClient->GetMixFormat(&pwfx);
    EXIT_ON_ERROR(hr)
    /*
    typedef struct tWAVEFORMATEX
    {
        WORD        wFormatTag;         // format type 
        WORD        nChannels;          // number of channels (i.e. mono, stereo...) 
        DWORD       nSamplesPerSec;     // sample rate 
        DWORD       nAvgBytesPerSec;    // for buffer estimation 
        WORD        nBlockAlign;        // block size of data
        WORD        wBitsPerSample;     // number of bits per sample of mono data
        WORD        cbSize;             // the count in bytes of the size of  extra information (after cbSize) 
    } WAVEFORMATEX;
    */
    printf("\nGetMixFormat...\n");
    cout<<"wFormatTag      : "<<pwfx->wFormatTag<<endl
        <<"nChannels       : "<<pwfx->nChannels<<endl
        <<"nSamplesPerSec  : "<<pwfx->nSamplesPerSec<<endl
        <<"nAvgBytesPerSec : "<<pwfx->nAvgBytesPerSec<<endl
        <<"nBlockAlign     : "<<pwfx->nBlockAlign<<endl
        <<"wBitsPerSample  : "<<pwfx->wBitsPerSample<<endl
        <<"cbSize          : "<<pwfx->cbSize<<endl<<endl;
    // test for IsFormatSupported
    //////////////////////////////////////////////////////////////////////////
    /*WAVEFORMATEX *wf;
    hr = pAudioClient->IsFormatSupported(AUDCLNT_SHAREMODE_SHARED, pwfx, &wf);
    if (FAILED(hr)) 
    {
        printf("IsFormatSupported fail.\n");
    }
    printf("\IsFormatSupported...\n");
    cout<<"wFormatTag      : "<<wf->wFormatTag<<endl
        <<"nChannels       : "<<wf->nChannels<<endl
        <<"nSamplesPerSec  : "<<wf->nSamplesPerSec<<endl
        <<"nAvgBytesPerSec : "<<wf->nAvgBytesPerSec<<endl
        <<"nBlockAlign     : "<<wf->nBlockAlign<<endl
        <<"wBitsPerSample  : "<<wf->wBitsPerSample<<endl
        <<"cbSize          : "<<wf->cbSize<<endl<<endl;*/
    //////////////////////////////////////////////////////////////////////////
    int nFrameSize = (pwfx->wBitsPerSample / 8) * pwfx->nChannels;
    cout<<"nFrameSize           : "<<nFrameSize<<" Bytes"<<endl
        <<"hnsRequestedDuration : "<<hnsRequestedDuration
        <<" REFERENCE_TIME time units. 即("<<hnsRequestedDuration/10000<<"ms)"<<endl;
    // 初始化管理对象，在这里，你可以指定它的最大缓冲区长度，这个很重要，
    // 应用程序控制数据块的大小以及延时长短都靠这里的初始化，具体参数大家看看文档解释
    // https://msdn.microsoft.com/en-us/library/dd370875(v=vs.85).aspx
#ifdef DEF_CAPTURE_MIC
    hr = pAudioClient->Initialize(
        AUDCLNT_SHAREMODE_SHARED,
        AUDCLNT_STREAMFLAGS_EVENTCALLBACK | AUDCLNT_STREAMFLAGS_NOPERSIST,
        hnsRequestedDuration,
        0,
        pwfx,
        NULL);
#else
    /*
        The AUDCLNT_STREAMFLAGS_LOOPBACK flag enables loopback recording. 
        In loopback recording, the audio engine copies the audio stream 
        that is being played by a rendering endpoint device into an audio endpoint buffer 
        so that a WASAPI client can capture the stream. 
        If this flag is set, the IAudioClient::Initialize method attempts to open a capture buffer on the rendering device. 
        This flag is valid only for a rendering device 
        and only if the Initialize call sets the ShareMode parameter to AUDCLNT_SHAREMODE_SHARED. 
        Otherwise the Initialize call will fail. 
        If the call succeeds, 
        the client can call the IAudioClient::GetService method 
        to obtain an IAudioCaptureClient interface on the rendering device. 
        For more information, see Loopback Recording.
    */
    hr = pAudioClient->Initialize(
        AUDCLNT_SHAREMODE_SHARED,
        AUDCLNT_STREAMFLAGS_LOOPBACK, // 这种模式下，音频engine会将rending设备正在播放的音频流， 拷贝一份到音频的endpoint buffer
                                      // 这样的话，WASAPI client可以采集到the stream.
                                      // 如果AUDCLNT_STREAMFLAGS_LOOPBACK被设置，IAudioClient::Initialize会尝试
                                      // 在rending设备开辟一块capture buffer。
                                      // AUDCLNT_STREAMFLAGS_LOOPBACK只对rending设备有效，
                                      // Initialize仅在AUDCLNT_SHAREMODE_SHARED时才可以使用, 否则Initialize会失败。
                                      // Initialize成功后，可以用IAudioClient::GetService可获取该rending设备的IAudioCaptureClient接口。
        hnsRequestedDuration,
        0,
        pwfx,
        NULL);
#endif
    EXIT_ON_ERROR(hr)
    /*
        https://msdn.microsoft.com/en-us/library/windows/desktop/dd370874(v=vs.85).aspx
        The GetStreamLatency method retrieves the maximum latency for the current stream 
        and can be called any time after the stream has been initialized.
        This method requires prior initialization of the IAudioClient interface. 
        All calls to this method will fail with the error AUDCLNT_E_NOT_INITIALIZED until 
        the client initializes the audio stream by successfully calling the IAudioClient::Initialize method.
        This method retrieves the maximum latency for the current stream. 
        The value will not change for the lifetime of the IAudioClient object.
        Rendering clients can use this latency value to compute the minimum amount of data 
        that they can write during any single processing pass. 
        To write less than this minimum is to risk introducing glitches into the audio stream. 
        For more information, see IAudioRenderClient::GetBuffer.
    
        1. 该函数返回当前流的最大延时 在IAudioClient对象的生命周期内 不会发生变化
        2. Rendering客户端可以用这个延时值，来计算每次处理pass可以写的最小数据量。
        注: 使用前须先调用IAudioClient::Initialize
    */
    REFERENCE_TIME hnsStreamLatency;
    hr = pAudioClient->GetStreamLatency(&hnsStreamLatency);
    EXIT_ON_ERROR(hr)
    
    cout<<"GetStreamLatency     : "<<hnsStreamLatency
    <<" REFERENCE_TIME time units. 即("<<hnsStreamLatency/10000<<"ms)"<<endl;
    /*
        phnsDefaultDevicePeriod [out]
        Pointer to a REFERENCE_TIME variable into which the method writes a time value 
        specifying the default interval between periodic processing passes by the audio engine. 
        The time is expressed in 100-nanosecond units. 
        
        phnsMinimumDevicePeriod [out]
        Pointer to a REFERENCE_TIME variable into which the method writes a time value 
        specifying the minimum interval between periodic processing passes by the audio endpoint device. 
        The time is expressed in 100-nanosecond units.
    */
    REFERENCE_TIME hnsDefaultDevicePeriod;
    REFERENCE_TIME hnsMinimumDevicePeriod;
    hr = pAudioClient->GetDevicePeriod(&hnsDefaultDevicePeriod, &hnsMinimumDevicePeriod);
    EXIT_ON_ERROR(hr)
    cout<<"GetDevicePeriod  ...\n"
        <<"hnsDefaultDevicePeriod : "<<hnsDefaultDevicePeriod
        <<" REFERENCE_TIME time units. 即("<<hnsDefaultDevicePeriod/10000<<"ms)"<<endl
        <<"hnsMinimumDevicePeriod : "<<hnsMinimumDevicePeriod
        <<" REFERENCE_TIME time units. 即("<<hnsMinimumDevicePeriod/10000<<"ms)"<<endl;
    // Get the size of the allocated buffer.
    // 这个buffersize，指的是缓冲区最多可以存放多少帧的数据量
    /*
        https://msdn.microsoft.com/en-us/library/dd370866(v=vs.85).aspx
        HRESULT GetBufferSize(
        [out] UINT32 *pNumBufferFrames
        );
    
        pNumBufferFrames [out]
        Pointer to a UINT32 variable into which the method writes the number of audio frames 
        that the buffer can hold.
        The IAudioClient::Initialize method allocates the buffer. 
        The client specifies the buffer length in the hnsBufferDuration parameter value 
        that it passes to the Initialize method. For rendering clients, 
        the buffer length determines the maximum amount of rendering data 
        that the application can write to the endpoint buffer during a single processing pass. 
        For capture clients, the buffer length determines the maximum amount of capture data 
        that the audio engine can read from the endpoint buffer during a single processing pass. 
    
        The client should always call GetBufferSize after calling Initialize 
        to determine the actual size of the allocated buffer, 
        which might differ from the requested size.
        Rendering clients can use this value to calculate the largest rendering buffer size 
        that can be requested from IAudioRenderClient::GetBuffer during each processing pass.
    */
    hr = pAudioClient->GetBufferSize(&bufferFrameCount);
    EXIT_ON_ERROR(hr)
    cout<<endl<<"GetBufferSize        : "<<bufferFrameCount<<endl;
    // SetEventHandle
    //////////////////////////////////////////////////////////////////////////
    HANDLE hAudioSamplesReadyEvent = CreateEventEx(NULL, NULL, 0, EVENT_MODIFY_STATE | SYNCHRONIZE);
    if (hAudioSamplesReadyEvent == NULL)
    {
        printf("Unable to create samples ready event: %d.\n", GetLastError());
        goto Exit;
    }
    /*
        The SetEventHandle method sets the event handle that the system signals 
        when an audio buffer is ready to be processed by the client.
        当音频的buffer就绪 可被client处理时， 会发出系统信号
        SetEventHandle用于设置处理该信号的event的handle
        During stream initialization, the client can, as an option, enable event-driven buffering. 
        To do so, the client calls the IAudioClient::Initialize method 
        with the AUDCLNT_STREAMFLAGS_EVENTCALLBACK flag set. 
        After enabling event-driven buffering, 
        and before calling the IAudioClient::Start method to start the stream, 
        the client must call SetEventHandle to register the event handle 
        that the system will signal each time a buffer becomes ready to be processed by the client.
        使用SetEventHandle， 需要在IAudioClient::Initialize设置AUDCLNT_STREAMFLAGS_EVENTCALLBACK。
        SetEventHandle应该在调用IAudioClient::Start之前调用。
    */
    hr = pAudioClient->SetEventHandle(hAudioSamplesReadyEvent);
    if (FAILED(hr))
    {
        printf("Unable to set ready event: %x.\n", hr);
        return false;
    }
    //////////////////////////////////////////////////////////////////////////
    // 创建采集管理接口
    hr = pAudioClient->GetService(IID_IAudioCaptureClient, (void**)&pCaptureClient);
    EXIT_ON_ERROR(hr)
    hr = pAudioClient->Start();  // Start recording.
    EXIT_ON_ERROR(hr)
    printf("\nAudio Capture begin...\n\n");
    int  nCnt  = 0;
    size_t nCaptureBufferSize   = 8*1024*1024;
    size_t nCurrentCaptureIndex = 0;
    BYTE *pbyCaptureBuffer = new (std::nothrow) BYTE[nCaptureBufferSize];
    HANDLE waitArray[3];
    waitArray[0]= hAudioSamplesReadyEvent;
    bool stillPlaying = true;
    // Each loop fills about half of the shared buffer.
    while (stillPlaying)
    {
        DWORD waitResult = WaitForMultipleObjects(1, waitArray, FALSE, INFINITE);
        switch (waitResult)
        {
        case WAIT_OBJECT_0 + 0:     // _AudioSamplesReadyEvent
            hr = pCaptureClient->GetNextPacketSize(&packetLength);
            EXIT_ON_ERROR(hr)
            printf("%06d # _AudioSamplesReadyEvent packetLength:%06u \n", nCnt, packetLength);
            while (packetLength != 0)
            {
                // Get the available data in the shared buffer.
                // 锁定缓冲区，获取数据
                hr = pCaptureClient->GetBuffer(&pData,
                                               &numFramesAvailable,
                                               &flags, NULL, NULL);
                EXIT_ON_ERROR(hr)
                nCnt++;
    
                // test flags
                //////////////////////////////////////////////////////////////////////////
                if (flags & AUDCLNT_BUFFERFLAGS_SILENT)
                {
                    printf("AUDCLNT_BUFFERFLAGS_SILENT \n");
                }
                if (flags & AUDCLNT_BUFFERFLAGS_DATA_DISCONTINUITY)
                {
                    printf("%06d # AUDCLNT_BUFFERFLAGS_DATA_DISCONTINUITY \n", nCnt);
                }
                //////////////////////////////////////////////////////////////////////////
                UINT32 framesToCopy = min(numFramesAvailable, static_cast<UINT32>((nCaptureBufferSize - nCurrentCaptureIndex) / nFrameSize));
                if (framesToCopy != 0)
                {
                    //
                    //  The flags on capture tell us information about the data.
                    //
                    //  We only really care about the silent flag since we want to put frames of silence into the buffer
                    //  when we receive silence.  We rely on the fact that a logical bit 0 is silence for both float and int formats.
                    //
                    if (flags & AUDCLNT_BUFFERFLAGS_SILENT)
                    {
                        //
                        //  Fill 0s from the capture buffer to the output buffer.
                        //
                        ZeroMemory(&pbyCaptureBuffer[nCurrentCaptureIndex], framesToCopy*nFrameSize);
                    }
                    else
                    {
                        //
                        //  Copy data from the audio engine buffer to the output buffer.
                        //
                        CopyMemory(&pbyCaptureBuffer[nCurrentCaptureIndex], pData, framesToCopy*nFrameSize);
                    }
                    //
                    //  Bump the capture buffer pointer.
                    //
                    nCurrentCaptureIndex += framesToCopy*nFrameSize;
                }
                hr = pCaptureClient->ReleaseBuffer(numFramesAvailable);
                EXIT_ON_ERROR(hr)
                hr = pCaptureClient->GetNextPacketSize(&packetLength);
                EXIT_ON_ERROR(hr)
                // test GetCurrentPadding
                //////////////////////////////////////////////////////////////////////////
                /*
                    This method retrieves a padding value that indicates the amount of 
                    valid, unread data that the endpoint buffer currently contains. 
                    返回buffer中合法的未读取的数据大小。
                    The padding value is expressed as a number of audio frames. 
                    The size in bytes of an audio frame equals 
                    the number of channels in the stream multiplied by the sample size per channel. 
                    For example, the frame size is four bytes for a stereo (2-channel) stream with 16-bit samples.
                    The padding value的单位是audio frame。
                    一个audio frame的大小等于 通道数 * 每个通道的sample大小。
                    For a shared-mode capture stream, the padding value reported by GetCurrentPadding 
                    specifies the number of frames of capture data 
                    that are available in the next packet in the endpoint buffer. 
                */
                UINT32 ui32NumPaddingFrames;
                hr = pAudioClient->GetCurrentPadding(&ui32NumPaddingFrames);
                EXIT_ON_ERROR(hr)
                if (0 != ui32NumPaddingFrames)
                {
                    printf("GetCurrentPadding : %6u\n", ui32NumPaddingFrames);
                }
                //////////////////////////////////////////////////////////////////////////
                // 采集一定数目个buffer后退出
                if (nCnt == 1000)
                {
                    stillPlaying = false;
                    break;
                }
            } // end of 'while (packetLength != 0)'
            break;
        } // end of 'switch (waitResult)'
    } // end of 'while (stillPlaying)'
    //
    //  We've now captured our wave data.  Now write it out in a wave file.
    //
    SaveWaveData(pbyCaptureBuffer, nCurrentCaptureIndex, pwfx);
    printf("\nAudio Capture Done.\n");
    hr = pAudioClient->Stop();  // Stop recording.
    EXIT_ON_ERROR(hr)
Exit:
    CoTaskMemFree(pwfx);
    SAFE_RELEASE(pEnumerator)
    SAFE_RELEASE(pDevice)
    SAFE_RELEASE(pAudioClient)
    SAFE_RELEASE(pCaptureClient)
    CoUninitialize();
    if (pbyCaptureBuffer)
    {
        delete [] pbyCaptureBuffer;
        pbyCaptureBuffer = NULL;
    }
    if (hAudioSamplesReadyEvent)
    {
        CloseHandle(hAudioSamplesReadyEvent);
        hAudioSamplesReadyEvent = NULL;
    }
    getchar();
    return 0;
}
```
```
github: [https://github.com/ZhaoliangGuo/MediaExamples/blob/master/AudioCapture/WindowsAudioSession/WindowsAudioSession.cpp](https://link.jianshu.com?t=https%3A%2F%2Fgithub.com%2FZhaoliangGuo%2FMediaExamples%2Fblob%2Fmaster%2FAudioCapture%2FWindowsAudioSession%2FWindowsAudioSession.cpp)
作者：FlyingPenguin
链接：https://www.jianshu.com/p/968f684ecd83
來源：简书
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。
