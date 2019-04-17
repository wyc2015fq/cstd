# C++ 播放音频流(PCM裸流)--改善 - DoubleLi - 博客园






直接上代码，如果有需要可以直接建一个win32控制台程序然后将代码拷过去改个文件名就可以用了（注意将声道和频率与你自己的文件对应）。当然我自己也用VS2008写了个例子上传了，如果有需要下载地址如下：[点击打开链接](http://download.csdn.net/detail/ohantanyanying/7480065)。

    这份代码是打开文件截取一段数据然后播放的，可以轻松的经过加一条线程的方式改成网络传输的形式，但经过本人[测试](http://lib.csdn.net/base/softwaretest)，因为没有缓存机制会有“哒哒”的噪声，也就是说这份代码在网络实时音频上的表现并不太好。为了解决这个问题，可以加上缓存机制，本人因为一开始用的是事件响应方式，所以一直困在这个框架里，不能很好的利用缓存的机制解决上面提到的问题，后来尝试了用回调函数的方式来响应数据播放完成的消息，问题就轻松的解决了。那部分的代码会在稍候放上去。





**[cpp]**[view plain](http://blog.csdn.net/weixinhum/article/details/29943973?utm_source=tuicool&utm_medium=referral#)[copy](http://blog.csdn.net/weixinhum/article/details/29943973?utm_source=tuicool&utm_medium=referral#)



- #include "stdafx.h"  
- #include <stdio.h>  
- #include <Windows.h>  
- #pragma comment(lib, "winmm.lib")  
- 
- char buf[1024 * 1024 * 4];  
- 
- int _tmain(int argc, _TCHAR* argv[]) {  
- FILE*           thbgm;//文件  
- int             cnt;  
-     HWAVEOUT        hwo;  
-     WAVEHDR         wh;  
-     WAVEFORMATEX    wfx;  
- HANDLE          wait;  
- 
-     wfx.wFormatTag = WAVE_FORMAT_PCM;//设置波形声音的格式  
-     wfx.nChannels = 1;//设置音频文件的通道数量  
-     wfx.nSamplesPerSec = 8000;//设置每个声道播放和记录时的样本频率  
-     wfx.nAvgBytesPerSec = 16000;//设置请求的平均数据传输率,单位byte/s。这个值对于创建缓冲大小是很有用的  
-     wfx.nBlockAlign = 2;//以字节为单位设置块对齐  
-     wfx.wBitsPerSample = 16;  
-     wfx.cbSize = 0;//额外信息的大小  
-     wait = CreateEvent(NULL, 0, 0, NULL);  
-     waveOutOpen(&hwo, WAVE_MAPPER, &wfx, (DWORD_PTR)wait, 0L, CALLBACK_EVENT);//打开一个给定的波形音频输出装置来进行回放  
-     fopen_s(&thbgm, "paomo.pcm", "rb");  
-     cnt = fread(buf, sizeof(char), 1024 * 1024 * 4, thbgm);//读取文件4M的数据到内存来进行播放，通过这个部分的修改，增加线程可变成网络音频数据的实时传输。当然如果希望播放完整的音频文件，也是要在这里稍微改一改  
- int dolenght = 0;  
- int playsize = 1024;  
- while (cnt) {//这一部分需要特别注意的是在循环回来之后不能花太长的时间去做读取数据之类的工作，不然在每个循环的间隙会有“哒哒”的噪音  
-         wh.lpData = buf + dolenght;  
-         wh.dwBufferLength = playsize;  
-         wh.dwFlags = 0L;  
-         wh.dwLoops = 1L;  
-         waveOutPrepareHeader(hwo, &wh, sizeof(WAVEHDR));//准备一个波形数据块用于播放  
-         waveOutWrite(hwo, &wh, sizeof(WAVEHDR));//在音频媒体中播放第二个函数wh指定的数据  
-         WaitForSingleObject(wait, INFINITE);//用来检测hHandle事件的信号状态，在某一线程中调用该函数时，线程暂时挂起，如果在挂起的INFINITE毫秒内，线程所等待的对象变为有信号状态，则该函数立即返回  
-         dolenght = dolenght + playsize;  
-         cnt = cnt - playsize;  
-     }  
-     waveOutClose(hwo);  
-     fclose(thbgm);  
- return 0;  
- }  




    离写上面部分已经过了快一年，现在回看之前写的代码感觉略为坑爹，或许是进步了吧。之前说要把双缓存的代码放出来，哪知道后来忙别的项目去了，这部分就丢到一边，去老项目中提取代码感觉好烦一直没弄。在这一年中不少人发私信问我关于这部分代码如何写的事，没想到现在做音频的人还真不少呢。Ok，既然挖了坑就要填，今天乘着周末写了一个双缓存的Demo工程，代码如下：





**[cpp]**[view plain](http://blog.csdn.net/weixinhum/article/details/29943973?utm_source=tuicool&utm_medium=referral#)[copy](http://blog.csdn.net/weixinhum/article/details/29943973?utm_source=tuicool&utm_medium=referral#)



- #include <stdio.h>  
- #include <Windows.h>  
- 
- #pragma comment(lib, "winmm.lib")  
- 
- #define DATASIZE 1024*512 //分次截取数据大小  
- FILE*           pcmfile;  //音频文件  
- HWAVEOUT        hwo;  
- 
- void CALLBACK WaveCallback(HWAVEOUT hWave, UINT uMsg, DWORD dwInstance, DWORD dw1, DWORD dw2)//回调函数  
- {  
- switch (uMsg)  
-     {  
- case WOM_DONE://上次缓存播放完成,触发该事件  
-         {  
-             LPWAVEHDR pWaveHeader = (LPWAVEHDR)dw1;  
-             pWaveHeader->dwBufferLength = fread(pWaveHeader->lpData, 1, DATASIZE, pcmfile);;  
-             waveOutPrepareHeader(hwo, pWaveHeader, sizeof(WAVEHDR));  
-             waveOutWrite(hwo, pWaveHeader, sizeof(WAVEHDR));  
- break;  
-         }  
-     }  
- }  
- 
- void main()   
- {  
- int             cnt;  
-     WAVEHDR         wh1;  
-     WAVEHDR         wh2;  
-     WAVEFORMATEX    wfx;  
- 
-     fopen_s(&pcmfile, "paomo.pcm", "rb");//打开文件  
- 
-     wfx.wFormatTag = WAVE_FORMAT_PCM;//设置波形声音的格式  
-     wfx.nChannels = 1;//设置音频文件的通道数量  
-     wfx.nSamplesPerSec = 8000;//设置每个声道播放和记录时的样本频率  
-     wfx.nAvgBytesPerSec = 16000;//设置请求的平均数据传输率,单位byte/s。这个值对于创建缓冲大小是很有用的  
-     wfx.nBlockAlign = 2;//以字节为单位设置块对齐  
-     wfx.wBitsPerSample = 16;  
-     wfx.cbSize = 0;//额外信息的大小  
- 
-     waveOutOpen(&hwo, WAVE_MAPPER, &wfx, (DWORD)WaveCallback, 0L, CALLBACK_FUNCTION);//打开一个给定的波形音频输出装置来进行声音播放，方式为回调函数方式。如果是对话框程序，可以将第五个参数改为(DWORD)this，操作跟本Demo程序相似  
- 
-     wh1.dwLoops = 0L;//播放区一  
-     wh1.lpData = new char[DATASIZE];  
-     wh1.dwBufferLength = DATASIZE;   
-     fread(wh1.lpData, 1, DATASIZE, pcmfile);  
-     wh1.dwFlags = 0L;  
-     waveOutPrepareHeader(hwo, &wh1, sizeof(WAVEHDR));//准备一个波形数据块用于播放  
-     waveOutWrite(hwo, &wh1, sizeof(WAVEHDR));//在音频媒体中播放第二个参数指定的数据，也相当于开启一个播放区的意思  
- 
-     wh2.dwLoops = 0L;//播放区二，基本同上  
-     wh2.lpData = new char[DATASIZE];  
-     wh2.dwBufferLength = DATASIZE;  
-     fread(wh2.lpData, 1, DATASIZE, pcmfile);  
-     wh2.dwFlags = 0L;  
-     waveOutPrepareHeader(hwo, &wh2, sizeof(WAVEHDR));  
-     waveOutWrite(hwo, &wh2, sizeof(WAVEHDR));  
- 
- while (wh1.dwBufferLength != 0 || wh2.dwBufferLength != 0)//如果文件还在没播放完则等待500ms  
-     {  
-         Sleep(500);  
-     }  
-     waveOutUnprepareHeader(hwo, &wh1, sizeof(WAVEHDR));//清理数据  
-     waveOutUnprepareHeader(hwo, &wh2, sizeof(WAVEHDR));  
- 
- delete []wh1.lpData;  
- delete []wh2.lpData;  
-     fclose(pcmfile);//关闭文件  
- return;  
- }  



    同上面一样，如果想要这个工程的可以到[这个链接](http://download.csdn.net/detail/ohantanyanying/8522073)去下载。不过提醒下，本人已然抛弃了VS2008，直接用VS2013，如果还在用老平台的话要用还是要折腾一会的。



from:http://blog.csdn.net/weixinhum/article/details/29943973?utm_source=tuicool&utm_medium=referral









