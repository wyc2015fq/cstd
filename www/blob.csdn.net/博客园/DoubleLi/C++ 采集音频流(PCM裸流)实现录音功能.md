# C++ 采集音频流(PCM裸流)实现录音功能 - DoubleLi - 博客园






与上一篇的“C++ 播放音频流(PCM裸流)” [点击打开链接](http://blog.csdn.net/weixinhum/article/details/29943973) 相对应，本篇是关于用C++实现录音功能的。同样是直接建一个win32控制台程序然后将代码拷过去改个文件名就可以用，也可以下载本人上传的相关工程，这个工程是用VS2013写的，如果是较低的版本就呵呵了。[点击打开链接](http://download.csdn.net/detail/ohantanyanying/7503473)

代码部分如下：





**[cpp]**[view plain](http://blog.csdn.net/weixinhum/article/details/31155985#)[copy](http://blog.csdn.net/weixinhum/article/details/31155985#)



- #include "stdafx.h"  
- #include <stdio.h>    
- #include <Windows.h>    
- #pragma comment(lib, "winmm.lib")    
- 
- HWAVEIN hWaveIn;  //输入设备  
- WAVEFORMATEX waveform; //采集音频的格式，结构体  
- BYTE *pBuffer1;//采集音频时的数据缓存  
- WAVEHDR wHdr1; //采集音频时包含数据缓存的结构体  
- FILE *pf;  
- int _tmain(int argc, _TCHAR* argv[])  
- {  
- HANDLE          wait;  
-     waveform.wFormatTag = WAVE_FORMAT_PCM;//声音格式为PCM  
-     waveform.nSamplesPerSec = 8000;//采样率，16000次/秒  
-     waveform.wBitsPerSample = 16;//采样比特，16bits/次  
-     waveform.nChannels = 1;//采样声道数，2声道  
-     waveform.nAvgBytesPerSec = 16000;//每秒的数据率，就是每秒能采集多少字节的数据  
-     waveform.nBlockAlign = 2;//一个块的大小，采样bit的字节数乘以声道数  
-     waveform.cbSize = 0;//一般为0  
- 
-     wait = CreateEvent(NULL, 0, 0, NULL);  
- //使用waveInOpen函数开启音频采集  
-     waveInOpen(&hWaveIn, WAVE_MAPPER, &waveform,(DWORD_PTR)wait, 0L, CALLBACK_EVENT);  
- 
- //建立两个数组（这里可以建立多个数组）用来缓冲音频数据  
- DWORD bufsize = 1024*100;//每次开辟10k的缓存存储录音数据  
- int i = 20;  
-     fopen_s(&pf, "录音测试.pcm", "wb");  
- while (i--)//录制20左右秒声音，结合音频解码和网络传输可以修改为实时录音播放的机制以实现对讲功能  
-     {  
-         pBuffer1 = new BYTE[bufsize];  
-         wHdr1.lpData = (LPSTR)pBuffer1;  
-         wHdr1.dwBufferLength = bufsize;  
-         wHdr1.dwBytesRecorded = 0;  
-         wHdr1.dwUser = 0;  
-         wHdr1.dwFlags = 0;  
-         wHdr1.dwLoops = 1;  
-         waveInPrepareHeader(hWaveIn, &wHdr1, sizeof(WAVEHDR));//准备一个波形数据块头用于录音  
-         waveInAddBuffer(hWaveIn, &wHdr1, sizeof (WAVEHDR));//指定波形数据块为录音输入缓存  
-         waveInStart(hWaveIn);//开始录音  
-         Sleep(1000);//等待声音录制1s  
-         waveInReset(hWaveIn);//停止录音  
-         fwrite(pBuffer1, 1, wHdr1.dwBytesRecorded, pf);  
- delete pBuffer1;      
-         printf("%ds  ", i);  
-     }  
-     fclose(pf);  
- 
-     waveInClose(hWaveIn);  
- return 0;  
- }  



from:http://blog.csdn.net/weixinhum/article/details/31155985










