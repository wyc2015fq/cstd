# 如何精确的控制FPS - 逍遥剑客 - CSDN博客
2007年05月18日 02:53:00[xoyojank](https://me.csdn.net/xoyojank)阅读数：5925
作者：LOVEHINA-AVC
类型：游戏制作
对应级别：初级
声明：没有什么特别的声明，转载时注明作者及出处就可以了
一些类型的游戏对速度稳定性要求很高，如STG、FTG、ACT等等。如何控制FPS是每个初级的游戏设计者都可能面临的问题。在接下来的文章当中，笔者将从Windows平台游戏设计者的角度出发，与大家稍微探讨并总结一下精确控制FPS的方法。
基础篇：如何将FPS固定在60
如果你借助DirectX或OpenGL来编写游戏的图形引擎部分，让FPS固定在60将会是一件非常容易办到的事情（前提当然是配置要够），因为这些SDK提供了让程序等待显示器垂直同步的功能。在一个全屏的游戏环境中，显示器每秒钟刷新60次，负责绘图的图形接口将在显示器完成一次刷新后返回到你的调用代码当中，如此便可以精确的以每秒60帧的速度运行游戏。这里有一个小问题：60Hz的刷新率在传统的CRT显示器中会让玩家产生一定的视觉疲劳——它的画面存在明显的闪烁现象。如果你希望以更高的刷新率运行游戏，或者让游戏运行于窗口模式之下，那么垂直同步可能并不是一个很好的选择。必须指出的是，玩家能够借助显卡驱动中的某些选项关闭垂直同步，即使你在游戏程序中明确指出要使用它。
怎样打开垂直同步？以D3D9为例，让我们先来看看IDirect3D9.CreateDevice的原型：
HRESULT CreateDevice(          UINT Adapter,
    D3DDEVTYPE DeviceType,
    HWND hFocusWindow,
    DWORD BehaviorFlags,
    D3DPRESENT_PARAMETERS *pPresentationParameters,
    IDirect3DDevice9 **ppReturnedDeviceInterface
);
这里需要传递一个D3DPRESENT_PARAMETERS类型的参数，再看看D3DPRESENT_PARAMETERS的构造：
typedef struct _D3DPRESENT_PARAMETERS_ {
    UINT BackBufferWidth, BackBufferHeight;
    D3DFORMAT BackBufferFormat;
    UINT BackBufferCount;
    D3DMULTISAMPLE_TYPE MultiSampleType;
    DWORD MultiSampleQuality;
    D3DSWAPEFFECT SwapEffect;
    HWND hDeviceWindow;
    BOOL Windowed;
    BOOL EnableAutoDepthStencil;
    D3DFORMAT AutoDepthStencilFormat;
    DWORD Flags;
    UINT FullScreen_RefreshRateInHz;
    UINT PresentationInterval;
} D3DPRESENT_PARAMETERS;
将FullScreen_RefreshRateInHz与PresentationInterval成员分别设置为60和D3DPRESENT_INTERVAL_ONE即可。
以下是D3DPRESENT_INTERVAL_ONE标志的说明：
The driver will wait for the vertical retrace period (the runtime will "beam follow" to prevent tearing). IDirect3DDevice9::Present operations will not be affected more frequently than the screen refresh; the runtime will complete at most one IDirect3DDevice9::Present operation per adapter refresh period. This is equivalent to using D3DSWAPEFFECT_COPYVSYNC in Microsoft DirectX 8.1. This option is always available for both windowed and full-screen swap chains. See Remarks. 
进阶篇：如何将FPS固定在任意值
要控制FPS就少不了测量时间的步骤，我们先来认识几个在Windows中得到时间数据的函数：
GetSystemTimeAsFileTime     —— 等价于GetSystemTime() + SystemTimeToFileTime()，好像没有人用过XD
GetSystemTime和GetLocalTime —— 一个是带时区修正的，另外一个没有
GetTickCount                —— 返回当前Windows运行的时长，由内核负责更新到共享存储区，取出来的值还要乘以一个倍率
以上几个函数的返回值在刷新频度方面不能满足游戏的需求。我们再来看另外两个：
timeGetTime             —— 作用与GetTickCount相同，32位精度，精确到1毫秒
QueryPerformanceCounter —— 同上，64位精度，精确到微秒
需要注意的是，timeGetTime在某些版本的操作系统上会有精度不足的问题，WinMM库提供另外一个叫做timeBeginPeriod的函数，根据MSDN中的解释，你可以使用timeBeginPeriod来提升timeGetTime的计时精度。QueryPerformanceCounter则要求硬件支持，不过你完全不必担心这个函数在玩家的机器上是否能跑，至少我还没有见过不支持它的PC。
以下是MSDN原文：
Windows NT/2000: The default precision of the timeGetTime function can be five milliseconds or more, depending on the machine. You can use the timeBeginPeriod and timeEndPeriod functions to increase the precision of timeGetTime. If you do so, the minimum difference between successive values returned by timeGetTime can be as large as the minimum period value set using timeBeginPeriod and timeEndPeriod. Use the QueryPerformanceCounter and QueryPerformanceFrequency functions to measure short time intervals at a high resolution, 
Windows 95: The default precision of the timeGetTime function is 1 millisecond. In other words, the timeGetTime function can return successive values that differ by just 1 millisecond. This is true no matter what calls have been made to the timeBeginPeriod and timeEndPeriod functions. 
QueryPerformanceFrequency与QueryPerformanceCounter的函数原型是一样的：
BOOL QueryPerformanceFrequency(
    LARGE_INTEGER *lpFrequency  // address of current frequency
   );
QueryPerformanceFrequency返回一秒间的计时频率数，将这个值除以60（或其它任意预期的FPS），我们就可以得到渲染一帧所应该花费并等待的时长。下面这段简短的代码演示了在游戏中保持速度为60 FPS的做法：
_int64 freq, cnt, oldcnt;
QueryPerformanceFrequency((PLARGE_INTEGER)&freq); //QueryPerformanceFrequency返回的值不会在系统运行当中改变，因此只需调用一次
QueryPerformanceCounter((PLARGE_INTEGER)&oldcnt);
freq /= 60;
while (TRUE)
{
 Render();
 do
 {
  QueryPerformanceCounter((PLARGE_INTEGER)&cnt);
 } while (cnt - oldcnt < freq);
 oldcnt = cnt;
}
关于QueryPerformanceCounter，我注意到MSDN的Remark中有这么几句：
On a multiprocessor computer, it should not matter which processor is called. However, you can get different results on different processors due to bugs in the basic input/output system (BIOS) or the hardware abstraction layer (HAL). To specify processor affinity for a thread, use the SetThreadAffinityMask function. 
这可能是一个不太严重的问题，因为QueryPerformanceCounter函数在我的双核系统上运行得很好。但是为了稳妥起见，我们仍然应该在主线程中调用SetThreadAffinityMask。这个函数执行线程到CPU的绑定操作，以下是其原型：
DWORD SetThreadAffinityMask (
    HANDLE hThread, // handle to the thread of interest
    DWORD dwThreadAffinityMask // a thread affinity mask
   );
在Win9x/ME中，dwThreadAffinityMask的值必须为1，也就是说，调用SetThreadAffinityMask没有意义，因为它们不支持多处理器。要使用这个函数，只需在游戏初始化的时候调用一次SetThreadAffinityMask(GetCurrentThread(), 1)。这样，我们的主线程就绑定到第一颗处理器上面了。
除了QueryPerformanceCounter函数，我们还可以使用精度较低的timeGetTime来获得稳定的FPS，这需要一点小技巧，因为1000并不能被30或60整除。事实上，唯一比较接近60并且能整除于1000的数是50，但我们通常不希望得到这样的FPS。
为了达成目的，我们首先要寻找预期FPS与1000的最大公约数。在此我仍然以60 FPS为例，计算可知60与1000的最大公约数是20，1000除20得50，60除20得3，即每50ms渲染3帧。由于我们不可能一次渲染3帧然后等50ms再描绘一次，因此这里还必须维护一个变量作为延迟循环的基础时长。我给这个变量赋以初始值16，这样最初的3帧将以每帧16ms的时长渲染。下面是实现代码：
unsigned int cnt, time, frame;
time = 16;
frame = 0;
cnt = timeGetTime();
while (TRUE)
{
 Render();
 frame ++;
 if (timeGetTime() - cnt >= 50)
 {
  if (frame < 3)
   time --;
  if (frame > 3)
   time ++;
  frame = 0;
  cnt = timeGetTime();
 }
 Sleep(time);
}
在这段代码中，程序每隔50ms就对time的值作一次校正。当渲染帧的总数达到了某个临界点的时候，渲染多个帧花费的时间总和将与预期有所出入，这将导致某个时刻在if块中的frame值不等于3。我们以time的最小精度——1ms来修正下一次延迟循环的时长，这样便能够保持平滑的渲染速度，同时最小化FPS上的误差。
你或许会注意到，我在这段代码中使用了Sleep——一个使线程休眠一段时间的函数。这并不是一个推荐的做法，因为Sleep将强迫调用线程放弃剩余的时间片，然后等待不知何时才会再次来临的线程调度。在基于NT内核的操作系统中，Sleep与WaitForSingle/MultipleObject(s)的精度并不会太差，但是这取决于CPU的繁忙状况。如果你打算在游戏中使用它们，你可能首先需要提升一下游戏程序的线程优先级别。不推荐通过调用SetPriorityClass的方式将线程优先类别提升到REALTIME_PRIORITY_CLASS，因为这通常需要运行游戏程序的帐户具备Administrator权限。对于一个游戏而言，在NORMAL_PRIORITY_CLASS的基础上将线程优先级别提升到TIME_CRITICAL就已经足够了。这两个函数的说明原文比较长，而你只需简单的调用一次SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_TIME_CRITICAL)，其余细节大可不必理会。有额外需求的朋友可自行查阅MSDN或《Win32 Programmer's Reference》，请教船夫和狗狗大神亦是一个不错的选择。
附一个示例2的改进代码：
int time;
unsigned int cnt, cnt2, frame, frame2, fps, temp;
time = 16;
frame = frame2 = 0;
cnt = cnt2 = timeGetTime();
while (TRUE)
{
 Render();
 frame2 ++;
 temp = timeGetTime();
 if (temp - cnt2 >= 50)
 {
  frame += 3;
  if (frame2 < frame)
  {
   time --;
   if (time < 0)
    time = 0;
  }
  if (frame2 > frame)
   time ++;
  cnt2 = temp;
 }
 if (temp - cnt >= 1000)
 {
  fps = frame2;
  frame = frame2 = 0;
  cnt = temp;
 }
 Sleep(time);
}
