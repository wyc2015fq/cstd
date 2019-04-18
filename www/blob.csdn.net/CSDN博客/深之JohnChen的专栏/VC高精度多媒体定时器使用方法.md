# VC高精度多媒体定时器使用方法 - 深之JohnChen的专栏 - CSDN博客

2017年10月08日 15:45:48[byxdaz](https://me.csdn.net/byxdaz)阅读数：1131


在VC编程中，用SetTimer可以定义一个定时器，到时间了，就响应OnTimer消息，但这种定时器精度（OnTimer）太低了。如果需要精度更高一些的定时器（精确到1ms），可以使用下面的高精度多媒体定时器进行代码优化，可以达到毫秒级的精度，而且使用方便。

多媒体定时器涉及到的几个重要的函数，具体说明见MSDN！

功能：获得计时器所支持的解析度范围。

1）MMRESULT timeGetDevCaps( LPTIMECAPS ptc, UINT cbtc );

功能：调整计时器的解析度，用在启动计时器之前。

2）MMRESULT timeBeginPeriod( UINT uPeriod );

功能：调整计时器的解析度，用在结束计时器之后。这两个函数通常是成对出现。

3）MMRESULT timeEndPeriod( UINT uPeriod );

功能：设置并启动一个计时器

参数：uDelay：以毫秒指定事件的周期。

   Uresolution：以毫秒指定延时的精度，数值越小定时器事件分辨率越高。缺省值为1ms。

   LpTimeProc：指向一个回调函数。

   DwUser：存放用户提供的回调数据。

   FuEvent：指定定时器事件类型：

   TIME_ONESHOT：uDelay毫秒后只产生一次事件

   TIME_PERIODIC ：每隔uDelay毫秒周期性地产生事件。

4）MMRESULT timeSetEvent( UINT uDelay, UINT uResolution, LPTIMECALLBACK lpTimeProc, DWORD_PTR dwUser, UINT fuEvent );

功能：取消一个定时器，uTimerID标识要取消的事件（由timeSetEvent函数返回的标识符）。如果成功则返回TIMERR_NOERROR，如果定时器时间不存在则返回MMSYSERR_INVALPARAM。

5）MMRESULT timeKillEvent( UINT uTimerID );

6）回调函数

  　　void   CALLBACK   TimeProc(     

  　　UINT   uID,     

  　　UINT   uMsg,     

  　　DWORD   dwUser,     

  　　DWORD   dw1,     

  　　DWORD   dw2);     

  该函数是一个应用程序定义的回调函数，出现定时器事件时该函数被调用。TimeProc是应用程序定义的函数名的占位符。使用该函数   

  时要注意的是，它只能调用以下有限的几组API函数：PostMessage，timeGetSystemTime，   timeGetTime，   timeSetEvent，timeKillEvent  

  ，midiOutShortMsg,   midiOutLongMsg，OutputDebugString。同时也不要使用完成时间很长的API函数，程序尽可能简短。

注意以下几点问题：

1、回调函数的参数不能有误，否则可能引起程序崩掉；

2、事件调用周期uDelay不能小于事件处理时间，否则会引起程序崩溃；

3、timeKillEvent 关掉定时器的函数，一定要一一对应，每次timeSetEvent返回的定时器的ID是不一样的。

4、SetTimer/timeSetEvent不能跨线程使用。

5、不要忘记加上以下头文件和动态链接库。

#include <Mmsystem.h>

#pragma comment(lib,"Winmm.lib")

VC实例

```cpp
// MMTimerTest.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <Mmsystem.h>
#pragma comment(lib,"Winmm.lib")

const int timePeriod = 15;
const int timeRes = 1;

MMRESULT gTimerId = NULL;			//定时器id

void  CALLBACK TimeEvent(UINT uTimerID, UINT uMsg, DWORD_PTR dwUser, DWORD_PTR dw1, DWORD_PTR dw2)
{
	printf("time ID is %d, started,dwUser is %d\n",gTimerId, dwUser);
	return;
}

int main(int argc, char* argv[])
{
	gTimerId = timeSetEvent(timePeriod,timeRes,(LPTIMECALLBACK)TimeEvent,0,TIME_PERIODIC);
	if(gTimerId == NULL)
	{
		printf("timer is not created\n");
		return 0;
	}
	
	int i = 0;
	while(true)
	{
		Sleep(1100);			//模拟操作
		printf("test....\n");
		i++;
		if (i == 10)
		{
			break;
		}
	}
	timeKillEvent(gTimerId); 

	return 0;
}
```

