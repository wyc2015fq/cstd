# Android平台上的Native内存分析 - 逍遥剑客 - CSDN博客
2018年09月28日 09:39:03[xoyojank](https://me.csdn.net/xoyojank)阅读数：1270
### 文章目录
- [背景](#_1)
- [UE4的内存统计](#UE4_5)
- [memreport](#memreport_14)
- [MemoryProfiler2](#MemoryProfiler2_19)
- [LLM](#LLM_23)
- [Android进程内存](#Android_28)
- [DDMSMemoryAnalyzer](#DDMSMemoryAnalyzer_66)
# 背景
UE4游戏在Android上的进程内存占用(PSS)很让人困惑, 没有一个清晰直观的方式可以统计到每一部分的内存占用. 所以在做内存分析的过程中顺手做了一个统计工具, 可以从系统底层统计UE4在Android的所有内存分配(包括Graphics部分).
![在这里插入图片描述](https://img-blog.csdn.net/20180928092342114?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3hveW9qYW5r/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
# UE4的内存统计
UE4本身提供了3种内存分析方式:
- [memreport](https://www.unrealengine.com/zh-CN/blog/debugging-and-optimizing-memory)
- [MemoryProfiler2](https://pzurita.wordpress.com/2015/02/10/limitations-of-memory-tracking-features-in-unreal-engine-4/)
- [LLM](https://www.unrealengine.com/en-US/blog/unreal-engine-4-18-released)
下面分别做一下说明
## memreport
游戏中console command输入”memreport-full”可以保存内存报告在Saved/Profiling/MemReports目录下, 就是一个文本文件, 长这样:
![在这里插入图片描述](https://img-blog.csdn.net/20180928094904487?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3hveW9qYW5r/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
里面有各种类型的内存统计数据, 如Texture, RTT, VB/IB, Particle, Component等, 每一种都有比较详细的信息. 缺点是引擎只会从某个角度做统计, 对于一些统计代码没有覆盖到的内存是没有相关信息的, 而且各个统计类型之间是有重叠, 无法把各个子项加起来就能还原出内存的总体占用分布.
## MemoryProfiler2
引擎代码中打开USE_MALLOC_PROFILER宏, 编译后使用”mprof start/stop/snapshot”命令来dump内存日志, 再使用MemoryProfiler2.exe打开查看:
![在这里插入图片描述](https://img-blog.csdn.net/20180928092704947?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3hveW9qYW5r/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
MemoryProfiler2的统计是基于UE4的内存分配器, 通过记录每次内存操作的调用堆栈, 然后进行累加统计, 使用GUI工具按照树形结构显示出来, 能够非常直观地进行分析. 缺点是会影响内存分配的效率, 而且记录的信息量太大, 中低端Android手机很难保存成功, 抓取到后工具的分析效率也比较慢. 如果一些内存没有通过UE4的内存分配器进行分配的话, 那是统计不到的, 比如一些第三方库或者组件.
## LLM
LLM是4.18才加入的功能, 使用-LLM命令行参数启动才生效, 当时只有PC和主机平台可以用, 游戏中使用”stat LLM/LLMFULL”打开实时查看:
![在这里插入图片描述](https://img-blog.csdn.net/20180928092728115?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3hveW9qYW5r/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
这种统计方式是基于scope的方式, 类似CPU性能分析的原理, 执行某段代码前记录一下, 执行后再统计对比一下, 这样可以做到没有遗漏, 而且也不会影响游戏的执行性能. 缺点是只是按模块或者分类的统计, 没有更细节的信息指导优化.
# Android进程内存
使用dumpsys meminfo命令可以查看Android进程的内存占用信息:
```
adb shell dumpsys meminfo <package_name|pid> [-d]
```
一般[输出的信息](https://unity3d.com/cn/learn/tutorials/topics/best-practices/android-memory-management)像这样:
![在这里插入图片描述](https://img-blog.csdn.net/20180928092817262?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3hveW9qYW5r/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
基中Gfv dev, EGL mtrack, GL mtrack是新版本Android才加入的, 之前版本都被统计在Native Heap中. 对比memreport就会发现, Unknown部分基本上与UE4自身统计到的内存一致, 那是因为UE4底层调用了mmap/munmap来进行内存分配, 所以没有被系统统计到Native Heap中:
```
FMalloc* FAndroidPlatformMemory::BaseAllocator()
{
	const FPlatformMemoryConstants& MemoryConstants = FPlatformMemory::GetConstants();
	// 1 << FMath::CeilLogTwo(MemoryConstants.TotalPhysical) should really be FMath::RoundUpToPowerOfTwo,
	// but that overflows to 0 when MemoryConstants.TotalPhysical is close to 4GB, since CeilLogTwo returns 32
	// this then causes the MemoryLimit to be 0 and crashing the app
	uint64 MemoryLimit = FMath::Min<uint64>( uint64(1) << FMath::CeilLogTwo(MemoryConstants.TotalPhysical), 0x100000000);
#if PLATFORM_ANDROID_ARM64
	// todo: track down why FMallocBinned is failing on ARM64
	return new FMallocAnsi();
#else
	// [RCL] 2017-03-06 FIXME: perhaps BinnedPageSize should be used here, but leaving this change to the Android platform owner.
	return new FMallocBinned(MemoryConstants.PageSize, MemoryLimit);
#endif
}
void* FAndroidPlatformMemory::BinnedAllocFromOS(SIZE_T Size)
{
	void* Ptr = mmap(nullptr, Size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0);
	LLM(FLowLevelMemTracker::Get().OnLowLevelAlloc(ELLMTracker::Platform, Ptr, Size));
	return Ptr;
}
```
如果把FMallocBinned替换成FMallocAnsi, 那么所有的内存都会被统计进Native Heap, 这时就可以使用Android的[Malloc Debug](https://android.googlesource.com/platform/bionic/+/master/libc/malloc_debug/README_marshmallow_and_earlier.md)进行内存分析了:
![在这里插入图片描述](https://img-blog.csdn.net/20180928093223665?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3hveW9qYW5r/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
# DDMSMemoryAnalyzer
```
注: 这个方法只在MiPad(Android4.4)上测试过, 最新版本Android的DDMS已经没有这个功能了.
```
- 首先保证/system/lib/libc_malloc_debug_leak.so存在, 如果没有, 需要自行提取相应版本并拷贝过去
- 然后setprop libc.debug.malloc 1, 开启系统级的Malloc Debug(也可以指定进程)
- C:\Users[yourname].android\ddms.cfg, 加一行””native=true”
- 启动ddms.bat, “Snapshot Current Native Heap Usage”, 可以抓取一个带有调用堆栈地址的txt文件
![在这里插入图片描述](https://img-blog.csdn.net/20180928093324696?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3hveW9qYW5r/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![在这里插入图片描述](https://img-blog.csdn.net/20180928093334996?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3hveW9qYW5r/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
有了堆栈地址, 我们可以通过arm-linux-androideabi-addr2line.exe把地址翻译成函数名. 由于txt中保存的是内存地址, 而addr2line需要的是so的相对地址, 所以我们需要获取libUE.so的内存地址, 做个减法, 才能使用addr2line进行翻译. 使用以下命令可以找到libUE4.so的内存地址:
```
adb shell pmap -x [PID]
```
比如下图, libUE4.so的内存地址是70226000:
![在这里插入图片描述](https://img-blog.csdn.net/20180928093405671?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3hveW9qYW5r/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
有了so的相对地址, 就可以使用addr2line进行翻译了:
```
arm-linux-androideabi-addr2line.exe -C -s -f -e libUE4.so [ADDRESS]
```
但是txt中记录的是每次调用的堆栈, 次数非常多, 每一次的分配大小都比较小, 不具备实际优化的参考价值, 所以把所有的堆栈地址合并进行内存的累加统计, 以树形结构自上而下查看, 内存占用就变得非常直观了, 这就是DDMSMemoryAnalyzer的基本思想:
![在这里插入图片描述](https://img-blog.csdn.net/20180928093441423?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3hveW9qYW5r/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
同样的方法, 也适用于Unity引擎:
![在这里插入图片描述](https://img-blog.csdn.net/20180928093618370?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3hveW9qYW5r/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
