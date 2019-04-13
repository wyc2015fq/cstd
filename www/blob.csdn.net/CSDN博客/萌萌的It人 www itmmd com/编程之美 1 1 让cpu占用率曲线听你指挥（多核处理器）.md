
# 编程之美 1.1 让cpu占用率曲线听你指挥（多核处理器） - 萌萌的It人 www.itmmd.com - CSDN博客


2013年08月07日 18:38:52[Jlins](https://me.csdn.net/dyllove98)阅读数：2288


**【目录】**
不考虑其他进程，cpu画正弦曲线
获取总体cpu利用率
获取多核处理器单个cpu利用率
考虑其他进程，cpu画正弦曲线
---

下面的程序针对多核处理器，可以设置让任何一个cpu显示相应的曲线（本文以正弦曲线为例）
代码编译环境：windows 7 64位 酷睿 i5 处理器，vs2010.
可以修改CpuSin函数的计算 busySpan 和 idleSpan的部分以显示不同的曲线。
下面的代码没有考虑cpu中其他进程的占用情况，这种情况详见第二部分
1\#include <windows.h>2\#include <stdio.h>3\#include <math.h>45//把一条正弦曲线0~2pi 之间的弧度等分200份抽样，计算每个点的振幅6//然后每隔300ms设置下一个抽样点,并让cpu工作对应振幅时间7constintsamplingCount =200;//抽样点数目8constdoublepi =3.1415926;9constinttotalAmplitude =300;//每个抽样点对应时间片10constdoubledelta =2.0/samplingCount;//抽样弧度的增量1112intbusySpan[samplingCount];//每个抽样点对应的busy时间13intidleSpan[samplingCount];//每个抽样点对应的idle时间1415//一个线程调用MakeUsageSin，并把该线程绑定到一个cpu，那么该cpu呈现正弦曲线16DWORD WINAPI MakeUsageSin(LPVOID lpParameter)17{18DWORD startTime =0;19for(intj =0; ; j = (j +1) %samplingCount)20{21startTime =GetTickCount();22while((GetTickCount() - startTime) <busySpan[j]);23Sleep(idleSpan[j]);24}25}2627//如果cpuindex < 0 则所有cpu都显示正弦曲线28//否则只有第 cpuindex个cpu显示正弦曲线29//cpuindex 从 0 开始计数30voidCpuSin(intcpuIndex)31{32//计算 busySpan 和 idleSpan33doubleradian =0;34intamplitude = totalAmplitude /2;35for(inti =0; i < samplingCount; i++)36{37busySpan[i] = (DWORD)(amplitude + sin(pi*radian)*amplitude);38idleSpan[i] = totalAmplitude -busySpan[i];39radian +=delta;40}4142//获取系统cup数量43SYSTEM_INFO SysInfo;44GetSystemInfo(&SysInfo);45intnum_processors =SysInfo.dwNumberOfProcessors;46if(cpuIndex +1>num_processors)47{48printf("error: the index of cpu is out of boundary\n");49printf("cpu number: %d\n", num_processors);50printf("your index: %d\n", cpuIndex);51printf("** tip: the index of cpu start from 0 **\n");52return;53}5455if(cpuIndex <0)56{57HANDLE* threads =newHANDLE[num_processors];58for(inti =0;i < num_processors;i++)59{60DWORD mask =1<<i;61threads[i] = CreateThread(NULL,0, MakeUsageSin, &mask,0, NULL);62SetThreadAffinityMask(threads[i],1<<i);//线程指定在某个cpu运行63}64WaitForMultipleObjects(num_processors, threads, TRUE, INFINITE);65}66else67{68HANDLE thread;69DWORD mask =1;70thread = CreateThread(NULL,0, MakeUsageSin, &mask,0, NULL);71SetThreadAffinityMask(thread,1<<cpuIndex);72WaitForSingleObject(thread,INFINITE);73}7475}76intmain()77{7879CpuSin(0);80return0;81}
运行结果：
![](https://img-blog.csdn.net/20130807183844531?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZHlsbG92ZTk4/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
---
下面我们考虑其他进程对cpu的影响，即需要检测出某个cpu当前的使用率。
首先对于单核处理器获取cpu利用率，或者多核处理器获取总的cpu利用率，可以通过windows api “GetSystemTimes” 来实现
该函数声明如下：BOOL GetSystemTimes（LPFILETIME  IdleTime，LPFILETIME   KernelTime，LPFILETIME   UserTime），具体可以参考[msdn接口介绍](http://msdn.microsoft.com/en-us/library/windows/desktop/ms724400(v=vs.85).aspx)
cpu利用率计算公式为：**CPURate=100.0-（NowIdleTime-LastIdleTime）/（NowKernelTime-LastKernelTime+NowUserTime-LastUserTime）*100.0**
计算总的cpu利用率或者单核处理器cpu利用率的类实现如下：
1classCCPUUseRate2{3public:4BOOL Initialize()5{6FILETIME ftIdle, ftKernel, ftUser;7BOOL flag =FALSE;8if(flag = GetSystemTimes(&ftIdle, &ftKernel, &ftUser))9{10m_fOldCPUIdleTime =FileTimeToDouble(ftIdle);11m_fOldCPUKernelTime =FileTimeToDouble(ftKernel);12m_fOldCPUUserTime =FileTimeToDouble(ftUser);1314}15returnflag;16}17//调用Initialize后要等待1左右秒再调用此函数18intGetCPUUseRate()19{20intnCPUUseRate = -1;21FILETIME ftIdle, ftKernel, ftUser;22if(GetSystemTimes(&ftIdle, &ftKernel, &ftUser))23{24doublefCPUIdleTime =FileTimeToDouble(ftIdle);25doublefCPUKernelTime =FileTimeToDouble(ftKernel);26doublefCPUUserTime =FileTimeToDouble(ftUser);27nCPUUseRate= (int)(100.0- (fCPUIdleTime -m_fOldCPUIdleTime)28/ (fCPUKernelTime - m_fOldCPUKernelTime + fCPUUserTime -m_fOldCPUUserTime)29*100.0);30m_fOldCPUIdleTime =fCPUIdleTime;31m_fOldCPUKernelTime =fCPUKernelTime;32m_fOldCPUUserTime =fCPUUserTime;33}34returnnCPUUseRate;35}36private:37doubleFileTimeToDouble(FILETIME &filetime)38{39return(double)(filetime.dwHighDateTime *4.294967296E9) + (double)filetime.dwLowDateTime;40}41private:42doublem_fOldCPUIdleTime;43doublem_fOldCPUKernelTime;44doublem_fOldCPUUserTime;45};
注意：前后两次调用GetSystemTimes之间要间隔一定时间，使用方法如下：
1intmain()2{3CCPUUseRate cpuUseRate;4if(!cpuUseRate.Initialize())5{6printf("Error! %d\n", GetLastError());7getch();8return-1;9}10else11{12while(true)13{14Sleep(1000);15printf("\r当前CPU使用率为：%4d%%", cpuUseRate.GetCPUUseRate());16}17}18return0;19}
对于计算多核处理器中单个cpu的使用率，可以使用pdh.h头文件中的接口，该头文件是visual studio自带的，包含该头文件时，还需要引入相关的lib库：
1\#include <TCHAR.h>2\#include <windows.h>3\#include <pdh.h>4\#include <cstdio>5\#include <cmath>6\#pragmacomment(lib, "pdh.lib")78//---------------------------------------------------comput the cpu usage rate9staticPDH_HQUERY cpuQuery;10staticPDH_HCOUNTER cpuTotal;1112//cpuindex 为指定的cpu id ，从0开始13voidinit(intcpuIndex)14{15PDH_STATUS Status = PdhOpenQuery(NULL, NULL, &cpuQuery);16if(Status !=ERROR_SUCCESS)17{18printf("\nPdhOpenQuery failed with status 0x%x.", Status);19exit(-1);20}21charbuf[50];22sprintf(buf,"\\Processor(%d)\\%% Processor Time", cpuIndex);23PdhAddCounter(cpuQuery, LPCSTR(buf), NULL, &cpuTotal);24PdhCollectQueryData(cpuQuery);25}262728doublegetCpuUsageRate()29{30PDH_FMT_COUNTERVALUE counterVal;31PdhCollectQueryData(cpuQuery);32PdhGetFormattedCounterValue(cpuTotal, PDH_FMT_DOUBLE, NULL, &counterVal);33returncounterVal.doubleValue;34}
注：该方法也可以计算总的cpu利用率，只要把PdhAddCounter的第二个字符串参数改为"\\Processor(_Total)\\%% Processor Time"
前后两次调用PdhCollectQueryData之间也需要间隔一定时间
使用方法如下：
1intmain()2{3init(0);4while(1)5{6Sleep(800);7printf("\n%f\n", getCpuUsageRate());8}9}
---
利用上述方法获取cpu当前利用率后，再画正弦曲线，只需要改变进程的busy时间和idle时间，如果当前点曲线需要的cpu利用率是a%，cpu实际利用率是b%
若a>b, 那么进程的busy时间为该点时间片的(a-b)%
若a<=b，那么进程busy时间为0（实际情况中由于cpu使用率采集的不精确以及使用率的不断变化，busy时间设置为0效果不一定最好，本文中是设置为原来时间的3/4）
实际上除了当前进程外，如果cpu一直占用某个使用率，会影响曲线的形状，特别是曲线的下部分.
代码如下：
1\#include <TCHAR.h>2\#include <windows.h>3\#include <pdh.h>4\#include <cstdio>5\#include <cmath>6\#pragmacomment(lib, "pdh.lib")78//---------------------------------------------------comput the cpu usage rate9staticPDH_HQUERY cpuQuery;10staticPDH_HCOUNTER cpuTotal;1112//cpuindex 为指定的cpu id ，从0开始13voidinit(intcpuIndex)14{15PDH_STATUS Status = PdhOpenQuery(NULL, NULL, &cpuQuery);16if(Status !=ERROR_SUCCESS)17{18printf("\nPdhOpenQuery failed with status 0x%x.", Status);19exit(-1);20}21charbuf[50];22sprintf(buf,"\\Processor(%d)\\%% Processor Time",cpuIndex);23PdhAddCounter(cpuQuery, LPCSTR(buf), NULL, &cpuTotal);24PdhCollectQueryData(cpuQuery);25}262728doublegetCpuUsageRate()29{30PDH_FMT_COUNTERVALUE counterVal;31PdhCollectQueryData(cpuQuery);32PdhGetFormattedCounterValue(cpuTotal, PDH_FMT_DOUBLE, NULL, &counterVal);33returncounterVal.doubleValue;34}3536//--------------------------------------------------------------------cpu sin37//把一条正弦曲线0~2pi 之间的弧度等分200份抽样，计算每个点的振幅38//然后每隔300ms设置下一个抽样点,并让cpu工作对应振幅时间39constintsamplingCount =200;//抽样点数目40constdoublepi =3.1415926;41constinttotalAmplitude =300;//每个抽样点对应时间片42constdoubledelta =2.0/samplingCount;//抽样弧度的增量4344DWORD busySpan[samplingCount];//每个抽样点对应的busy时间45intidleSpan[samplingCount];//每个抽样点对应的idle时间4647//一个线程调用MakeUsageSin，并把该线程绑定到一个cpu，那么该cpu呈现正弦曲线48DWORD WINAPI MakeUsageSin(LPVOID lpParameter)49{50DWORD startTime =0;51for(intj =0; ; j = (j +1) %samplingCount)52{53startTime =GetTickCount();54DWORD realBusy =busySpan[j];5556doublecurrentCpuUsageRate =getCpuUsageRate();57if(currentCpuUsageRate < busySpan[j]*1.0/totalAmplitude)58realBusy = (busySpan[j]*1.0/totalAmplitude - currentCpuUsageRate)*totalAmplitude;59else60realBusy *=0.75;6162while((GetTickCount() - startTime) <realBusy);63Sleep(idleSpan[j]);64}65}6667//如果cpuindex < 0 则所有cpu都显示正弦曲线68//否则只有第 cpuindex个cpu显示正弦曲线69//cpuindex 从 0 开始计数70voidCpuSin(intcpuIndex)71{72//计算 busySpan 和 idleSpan73doubleradian =0;74intamplitude = totalAmplitude /2;75for(inti =0; i < samplingCount; i++)76{77busySpan[i] = (DWORD)(amplitude + sin(pi*radian)*amplitude);78idleSpan[i] = totalAmplitude -busySpan[i];79radian +=delta;80}8182//获取系统cup数量83SYSTEM_INFO SysInfo;84GetSystemInfo(&SysInfo);85intnum_processors =SysInfo.dwNumberOfProcessors;86if(cpuIndex +1>num_processors)87{88printf("error: the index of cpu is out of boundary\n");89printf("cpu number: %d\n", num_processors);90printf("your index: %d\n", cpuIndex);91printf("** tip: the index of cpu start from 0 **\n");92return;93}9495if(cpuIndex <0)96{97HANDLE* threads =newHANDLE[num_processors];98for(inti =0;i < num_processors;i++)99{100DWORD mask =1<<i;101threads[i] = CreateThread(NULL,0, MakeUsageSin, &mask,0, NULL);102SetThreadAffinityMask(threads[i],1<<i);//线程指定在某个cpu运行103}104WaitForMultipleObjects(num_processors, threads, TRUE, INFINITE);105}106else107{108init(cpuIndex);109HANDLE thread;110DWORD mask =1;111thread = CreateThread(NULL,0, MakeUsageSin, &mask,0, NULL);112SetThreadAffinityMask(thread,1<<cpuIndex);113WaitForSingleObject(thread,INFINITE);114}115116}117//-------------------------------------118119intmain()120{121CpuSin(0);122}
主要改动在MakeUsageSin函数，初始化在上面代码108行
结果如下：
![](https://img-blog.csdn.net/20130807183845562?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZHlsbG92ZTk4/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
**【版权声明】转载请注明出处****[http://www.cnblogs.com/TenosDoIt/p/3242910.html](http://www.cnblogs.com/TenosDoIt/p/3242910.html)**


