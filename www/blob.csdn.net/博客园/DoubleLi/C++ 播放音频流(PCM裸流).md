# C++ 播放音频流(PCM裸流) - DoubleLi - 博客园






直接上代码，如果有需要可以直接建一个win32控制台程序然后将代码拷过去改个文件名就可以用了（注意将声道和频率与你自己的文件对应哦）。当然我自己也用VS2008（VS2013好用太多，强烈推荐还是用VS2013，只是它的工程太大，上传起来相当费劲，所以还是用VS2008重写了。。）写了个例子上传了，如果有需要下载地址如下（自己没什么下载分了，所以弄了3分，见谅哈哈）：点击打开链接

这份代码是打开文件截取一段数据然后播放的，可以轻松的经过加一条线程的方式改成网络传输的形式。 
#include "stdafx.h"
#include <stdio.h>
#include <Windows.h>
#pragma comment(lib, "winmm.lib")

char buf[1024 * 1024 * 4];

int _tmain(int argc, _TCHAR* argv[]) {
	FILE*           thbgm;//文件
	int             cnt;
	HWAVEOUT        hwo;
	WAVEHDR         wh;
	WAVEFORMATEX    wfx;
	HANDLE          wait;

	wfx.wFormatTag = WAVE_FORMAT_PCM;//设置波形声音的格式
	wfx.nChannels = 1;//设置音频文件的通道数量
	wfx.nSamplesPerSec = 8000;//设置每个声道播放和记录时的样本频率
	wfx.nAvgBytesPerSec = 16000;//设置请求的平均数据传输率,单位byte/s。这个值对于创建缓冲大小是很有用的
	wfx.nBlockAlign = 2;//以字节为单位设置块对齐
	wfx.wBitsPerSample = 16;
	wfx.cbSize = 0;//额外信息的大小
	wait = CreateEvent(NULL, 0, 0, NULL);
	waveOutOpen(&hwo, WAVE_MAPPER, &wfx, (DWORD_PTR)wait, 0L, CALLBACK_EVENT);//打开一个给定的波形音频输出装置来进行回放
	fopen_s(&thbgm, "paomo.pcm", "rb");
	cnt = fread(buf, sizeof(char), 1024 * 1024 * 4, thbgm);//读取文件4M的数据到内存来进行播放，通过这个部分的修改，增加线程可变成网络音频数据的实时传输。当然如果希望播放完整的音频文件，也是要在这里稍微改一改
	int dolenght = 0;
	int playsize = 1024;
	while (cnt) {//这一部分需要特别注意的是在循环回来之后不能花太长的时间去做读取数据之类的工作，不然在每个循环的间隙会有“哒哒”的噪音
		wh.lpData = buf + dolenght;
		wh.dwBufferLength = playsize;
		wh.dwFlags = 0L;
		wh.dwLoops = 1L;
		waveOutPrepareHeader(hwo, &wh, sizeof(WAVEHDR));//准备一个波形数据块用于播放
		waveOutWrite(hwo, &wh, sizeof(WAVEHDR));//在音频媒体中播放第二个函数wh指定的数据
		WaitForSingleObject(wait, INFINITE);//用来检测hHandle事件的信号状态，在某一线程中调用该函数时，线程暂时挂起，如果在挂起的INFINITE毫秒内，线程所等待的对象变为有信号状态，则该函数立即返回
		dolenght = dolenght + playsize;
		cnt = cnt - playsize;
	}
	waveOutClose(hwo);
	fclose(thbgm);
	return 0;
}








