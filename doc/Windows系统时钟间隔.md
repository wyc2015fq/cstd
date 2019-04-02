# Windows系统时钟间隔



系统时钟间隔是个很少被关心到的系统标量，它反映了系统产生时钟中断的频率，间隔越小频率越高，反之亦然。每当时钟中断产生，系统相关的中断函数将会处理这个中断。时钟中断处理函数会更新系统时间，检查内核调试信息等。

# 1. 时钟间隔

系统时钟间隔和另一个极重要的系统标量关联在一起：线程调度时限。按照Windows系统的设计，线程调度时限被分为普通程序和后台服务两种类型。前者运行2个时钟间隔，后者运行12个时钟间隔，用户可以在系统属性中手动更改时限类型。

不同硬件平台定义有不同的最小、最大时钟间隔值。对于X86平台而言，最小时钟间隔是0.5ms，最大值大概是15.6001ms。在内部，时钟间隔以100ns为单位进行表述，所以0.5ms被表示为5000个100ns单位，15.6ms被表示为156001个100ns单位。

使用ClockInterval工具可以查看/设置系统时钟频率（类似的工具是Sysinternal的ClockRes工具，可以查看当前值）。下图是运行ClockInterval后的界面截图：

[![clock-interval](http://www.yiiyee.cn/Blog/wp-content/uploads/2013/09/clock-interval-300x203.png)](http://www.yiiyee.cn/Blog/wp-content/uploads/2013/09/clock-interval.png)

图1 获取时钟频率

线程调度时限是一个重要的系统标量，对不同功能的应用而言，它的取值具有矛盾性。如果调度时限太小，系统就会频繁切换线程（因为时间片很快用完），从而导致性能降低。而如果调度时限太长，某些对于实时性要求很高的任务则不能接受。

系统把一个时钟间隔分成三份，以其中的一份（1/3时钟间隔）作为线程调度的基本时间片段，称为一个时限单元。如果一个线程在运行过程中放弃剩余的时间片，则它用掉和放弃的，都是时限单元的倍数。

哪些任务需要很强的实时性呢？包括：音视频软件、实时监控软件等。MediaPlayer是一个强实时要求的音频软件，笔者在测试机上（Win7 64）做实验时发现，每当运行MediaPlayer程序，它都会把系统时钟频率调低到10ms；而当退出MediaPlayer，时钟频率又会恢复至原值。类似的软件还有WinAMP和鲁大师等，读者有空可以实测一下，这两款软件会把系统时间间隔设置成1ms（如果读者使用这些软件时，未测试出相同的时钟间隔，可能是由于版本差异引起，笔者对此不做特别保证）。

# 2. 如何设置

用户程序如要控制系统时钟间隔，最简单的办法是调用Windows MMLib库的接口函数timeBeginPeriod和timeEndPeriod。前者用来设置一个新的最小时钟间隔，后者用来结束当前设置。这两个函数原型定义如下：

```
MMRESULT timeBeginPeriod(UINT uPeriod );
MMRESULT timeEndPeriod(UINT uPeriod );
```

程序执行期间这两个函数应当成对调用。据笔者观察，退出程序时如果忘记调用timeEndPeriod函数，系统时钟间隔也会自动恢复。这两个函数较简单，下面是一段演示代码：

```
UINT newInverval = 1;
MMRESULT r = timeBeginPeriod( newInverval ); // 设置当前系统时钟间隔为1ms

//do something here...

if(r == TIMER_NOERROR)
{
  timeEndPeriod(newInterval);// 最后，再恢复到原频率
}
```

如要获取当前的时钟间隔值，可使用Win32 API：GetSystemTimeAdjustment。其函数声明如下：

```
BOOL WINAPI GetSystemTimeAdjustment(
  __out         PDWORD lpTimeAdjustment,
  __out         PDWORD lpTimeIncrement,
  __out         PBOOL lpTimeAdjustmentDisabled
);
```

这个函数没有一个参数是用来输入的，全部是输出参数。第二个参数lpTimeIncrement返回的就是当前的时钟频率（100ns单位）。下面的实例代码是工具ClockRes V1.0的源码，使用了这个函数：

```
void main( int argc, char *argv[] )
{
    DWORD adjustment,clockInterval;
    BOOL  adjustmentDisabled;
    GetSystemTimeAdjustment(&adjustment,
                            &clockInterval,
                             &adjustmentDisabled );
    printf( "The systemclock interval is %d ms\n", clockInterval / 10000 );
}
```

除了上述公开的API接口外，另外有两个未文档函数：NtQueryTimerResolution和NtSetTimerResolution，也可用来获取和设置时钟频率。在此不做详细分析。

# 3. 关于恢复

前文说过应成对地使用timeBeginPeriod和timeEndPeriod函数，忘记调用timeEndPeriod会导致系统无法将时钟间隔恢复到较大值。当一个进程设置了系统时钟间隔后，系统中的其他进程只能继续将时钟间隔设置成更低值，而无法恢复为较大值。可以用ClockInterval做下面的实验：

1. 开启ClockInterval，假设你当前得到的时钟间隔是16ms，将它设置成一个较小的值10ms。
2. 开启另一个ClockInterval实例，此时将看到当前的时钟间隔是10ms。尝试将当前时钟间隔设置成最大值15.6ms，会发现设置失败。
3. 关闭第一个ClockInterval实例后，在第二个ClockInterval实例中，再次将当前时钟间隔设置成最大值15.6ms，可以成功！

打开ClockInterval程序后，界面上显示的当前时钟间隔值是你所能重设的时钟间隔的上限。点击ClockInterval界面的“测试”按钮，会提示最大可设置的时钟间隔值。

 ![img](http://hi.csdn.net/attachment/201108/10/0_1312963485c8Kx.gif)

图2 Test测试

图中信息所示：在系统中的其它进程/线程结束其所设置的1ms时钟间隔前，当前进程无法设置>1ms的时钟间隔。