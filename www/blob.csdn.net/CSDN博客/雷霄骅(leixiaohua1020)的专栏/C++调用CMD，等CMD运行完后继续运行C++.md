# C++调用CMD，等CMD运行完后继续运行C++ - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2013年09月23日 21:24:00[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：52









在这里记录一个小技巧，可以在程序运行的时候调用CMD命令。然后等待CMD命令执行完的时候，继续运行程序。

这在做视频编码实验的时候非常好用，因为像HEVC的tappencoder这种编码速度超级慢的程序，比较适合提前设置好参数，然后用这种方法让C++自动调用CMD编码一个一个的序列。我们就可以回去睡大觉了~~



```cpp
CString paramstr;
STARTUPINFO si={sizeof(si)};
PROCESS_INFORMATION pi;
```





```cpp
//要执行的完整CMD命令，一般是一个字符串
paramstr.Format("tappencoder -c encoder_lowdelay_P_main.cfg -i %s_%dx%d.yuv -wdt %d -hgt %d -b %s_%dx%d_%.3f.hm10 -o %s_%dx%d_%.3f_hevc.yuv",clipname,reso.width,reso.height,reso.width,reso.height,clipname,reso.width,reso.height,bitpp,clipname,reso.width,reso.height,bitpp);
//创建线程
BOOL fRet1 = CreateProcess(NULL,paramstr.GetBuffer(),NULL,NULL,FALSE,NORMAL_PRIORITY_CLASS,NULL,NULL,&si,&pi);
if (fRet1 == TRUE)
{
	WaitForSingleObject(pi.hThread,INFINITE);
	CloseHandle(pi.hThread);
	CloseHandle(pi.hProcess);
}
```


















