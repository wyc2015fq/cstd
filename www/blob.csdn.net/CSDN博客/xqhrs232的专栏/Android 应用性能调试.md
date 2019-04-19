# Android 应用性能调试 - xqhrs232的专栏 - CSDN博客
2012年02月29日 16:43:58[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：540标签：[android																[profiling																[工具																[eclipse																[layout																[hierarchy](https://so.csdn.net/so/search/s.do?q=hierarchy&t=blog)](https://so.csdn.net/so/search/s.do?q=layout&t=blog)](https://so.csdn.net/so/search/s.do?q=eclipse&t=blog)](https://so.csdn.net/so/search/s.do?q=工具&t=blog)](https://so.csdn.net/so/search/s.do?q=profiling&t=blog)](https://so.csdn.net/so/search/s.do?q=android&t=blog)
个人分类：[Android/Java](https://blog.csdn.net/xqhrs232/article/category/906925)
原文地址::[http://blog.csdn.net/andy_android/article/details/7019496](http://blog.csdn.net/andy_android/article/details/7019496)
## 概述
创造愉快用户体验的关键是开发响应快捷的应用。借助Android* 软件开发套件（SDK）提供的组件，性能问题调试任务因为简单易用的性能分析工具而变得更简单了。在这篇文章中，我们将认识一些不同的工具，它们可排除故障和调试性能问题或小幅度提升已完成应用的性能。我们不会说得太细，只会概括介绍如何针对您的应用使用这些工具。
我们将在Eclipse 中展示这些工具；您可以根据“[ADT Plugin for Eclipse](http://developer.android.com/sdk/eclipse-adt.html)* （面向 Eclipse 的 ADT 插件）”一文中的说明来安装ADT插件。
## DDMS
DDMS是一款Google* 提供的应用，可作为独立的工具运行，也可通过ADT Eclipse* 插件集成到Eclipse* 中。它提供了强大的特性集合，能帮助您快速了解应用的运行状况。
## 线程更新
DDMS中的线程监控和评测浏览对于管理大量线程的应用很有用。要启用，点击 Update Threads（更新线程）图标即可开始。
![](http://software.intel.com/file/39220)
**图 1**
这使下面的窗口会显示面向选中VM进程的所有线程的名称和其他细节。
![](http://software.intel.com/file/39221)
**图 2**
utime和stime代表了线程在瞬间运行用户代码（utime）和系统代码（STIME）所花的总时间。一瞬间的时间是由系统定义的，但通常为10毫秒。星号表示守护线程；native状态表示线程正执行原生代码。仔细观察上述样本数据，很明显，除了应用主线程花了大量时间外，运行GC也花了挺多的时间。仔细观察应用如何处理对象创建可能有助于提高性能。
## 堆工具
**堆查看**
点击Update Heap（更新堆）按钮，获得有关选定虚拟机中堆分配的信息。
![](http://software.intel.com/file/39222)
**图 3**
![](http://software.intel.com/file/39223)
**图 4**
点击"Cause GC"开始.堆的详细信息被显示出来，并附有针对特定分配类型的分配大小图示。如果您有分配泄漏，这可能是一个很好的检查点，通过观看Heap Size（堆大小）的总体趋势，确保在应用运行期间它不会一直变大。
**Allocation Tracker（分配跟踪器）**
Allocation Tracker（分配跟踪器）视图中显示了有关分配的更深层细节。点击“Start Tracking（开始跟踪）”，在应用中执行某个操作，然后点击“Get Allocations（获得分配）”。
![](http://software.intel.com/file/39224)
**图 5**
所示列表按分配排序，首先显示最新的分配。选中它可看到一个关于分配如何创建的堆栈轨迹（stack trace）。
仔细查看分配细节，下面的代码看起来有改进的空间：
`dataStr += String.format(" Std. Dev.: %.3f, %.3f, %.3f\n", devX, devY, devZ);`
上例可简单重构为下面的代码，节省构造临时char[]的开销。.
`dataStrBuilder.append(String.format(" Std. Dev.: %.3f, %.3f, %.3f\n", devX, devY, devZ));`
## Method Profiling（方法分析）
Method Profiling（方法分析）是DDMS的一款工具，对于快速概览应用中时间的消耗分布非常有用，也可用于时间关键型函数的详细查看。
![](http://software.intel.com/file/39225)
**图 6**
在应用运行并执行某个有趣的任务时，如果您想获得更多有关该任务的性能数据，点击“Start Method Profiling（开始方法分析）”。分析器只收集少量数据（没见过超过2或3秒），所以，几分钟后再次单击该图标以停止收集。从DDMS中激活方法分析器可使工具自动使用内部存储来存储分析结果，当捕捉完成后，将它们发送回主机，作进一步分析。
IDE将自动启动Traceview窗口，帮助您在IDE（图7）中分析结果。
![](http://software.intel.com/file/39226)
**图 7**
解析结果是最有趣的部分。单击底部窗格中的方法调用可创建一个层级结构，为您显示目前的方法——先是调用该方法的母方法，然后是从选定方法中调用的子方法。
在本例被测应用中，我选中了onSensorChanged方法。当您注册接收来自传感器类型的通知时，这种方法便是通过SensorManager API调用的方法。这里的调用方法是handleMessage，它来自操作系统，所以由此开始我的实现方法是一个不错的选择。子方法根据 “总计”所花的时间百分比排序。这里的“总计”表示在该方法及该方法调用的所有子方法内所花费的时间。因此，对于onSensorChanged调用，超过70％的时间花在了calcStandardDeviation和averageSamples上。我期望该调用多花一些时间来计算标准偏差，而不是仅仅将样本平均。所以利用这则新信息，我能更加深入地查看我的实施并发现代码优化点。
想详细了解Traceview，请参考“利用Traceview与dmtracedump进行分析([Profiling with Traceview and dmtracedump](http://developer.android.com/guide/developing/debugging/debugging-tracing.html))”。
## 分析API
为获得有着更高精度的的方法分析细节，调用可在代码中进行以启动和停止分析。您需要在设备中加载一个SD卡来使用此方法。在下面的例子中，我们添加hook（钩子）以更好地了解传感器处理代码：
[- collapse source](http://software.intel.com/zh-cn/articles/performance-debugging-of-android-applications/?cid=sw:prccsdn2072#)[view
 plain](http://software.intel.com/zh-cn/articles/performance-debugging-of-android-applications/?cid=sw:prccsdn2072#)[copy to clipboard](http://software.intel.com/zh-cn/articles/performance-debugging-of-android-applications/?cid=sw:prccsdn2072#)[print](http://software.intel.com/zh-cn/articles/performance-debugging-of-android-applications/?cid=sw:prccsdn2072#)[?](http://software.intel.com/zh-cn/articles/performance-debugging-of-android-applications/?cid=sw:prccsdn2072#)
- privatestatic boolean doOnce = true;  
- 
- @Override  
- publicvoid onSensorChanged(SensorEvent event) {  
- 
- if ( doOnce ) {  
-         android.os.Debug.startMethodTracing();  
-     }  
- 
-     Code under test…  
- 
- if ( doOnce ){  
-         android.os.Debug.stopMethodTracing();  
-         doOnce = false;  
-     }  
- }  
**[cpp]**[view plain](http://blog.csdn.net/andy_android/article/details/7019496#)[copy](http://blog.csdn.net/andy_android/article/details/7019496#)[print](http://blog.csdn.net/andy_android/article/details/7019496#)[?](http://blog.csdn.net/andy_android/article/details/7019496#)
- privatestatic boolean doOnce = true;  
- 
- @Override  
- publicvoid onSensorChanged(SensorEvent event) {  
- 
- if ( doOnce ) {  
-         android.os.Debug.startMethodTracing();  
-     }  
- 
-     Code under test…  
- 
- if ( doOnce ){  
-         android.os.Debug.stopMethodTracing();  
-         doOnce = false;  
-     }  
- }  
	private static boolean doOnce = true;
	
	@Override
	public void onSensorChanged(SensorEvent event) {
		
		if ( doOnce ) {
			android.os.Debug.startMethodTracing();
		}
		
		Code under test…
		if ( doOnce ){
			android.os.Debug.stopMethodTracing();
			doOnce = false;
		}
	}
跟踪文件默认为：/ mnt / sdcard/dmtrace.trace，可通过下面的命令从设备中提取出来：
`adb pull /mnt/sdcard/dmtrace.trace.`
运行独立的Traceview工具：“traceview C:\dmtrace.trace”，会打开一个用户界面，类似于嵌入到Eclipse中的用户界面。
## 布局用户界面工具
**layoutopt（布局选择）**
每当要调用某应用结束时，我就希望在活动的用户界面布局中获得简单的性能增益。 layoutopt这一工具会分析您的布局文件，并指出潜在的性能问题。在这篇博客和参考文件会谈到该问题，让我们快速浏览该如何使用这一工具吧。命令行用法如下：
`layoutopt.bat C:\Projects\workspace\DeviceInformation\res`
注：我把Android* SDK工具目录放在我的路径中。它看起来也像一款工具，只在您详细说明要分析目录的完整路径时才可用。
输出示例：
> C:\Projects\workspace\DeviceInformation\res\drawable\btn_notification_ic_example.xml
C:\Projects\workspace\DeviceInformation\res\drawable\picture_frame.xml
C:\Projects\workspace\DeviceInformation\res\layout\action_bar_custom.xml
23:23 This TextView layout or its LinearLayout parent is useless
C:\Projects\workspace\DeviceInformation\res\layout\content_applicationinfo_main.xml
16:19 This LinearLayout layout or its LinearLayout parent is useless
C:\Projects\workspace\DeviceInformation\res\layout\content_benchmark_main.xml
C:\Projects\workspace\DeviceInformation\res\layout\content_main.xml
C:\Projects\workspace\DeviceInformation\res\layout\content_sensorinfo_main.xml
17:20 This LinearLayout layout or its LinearLayout parent is useless
X：Y是与问题对应的XML标记的开始行和结束行。上面指出的多余布局增加了活动的整体加载时间，可用于方便地提升您的活动加载速度。
## Hierarchy Viewer（层级查看器）
性能问题调试中另一款有用工具就是[Hierarchy Viewer](http://developer.android.com/guide/developing/debugging/debugging-ui.html) （层级查看器）工具。此应用只能连接到Android* 操作系统的开发人员版本中，所以在不用开发设备情况下，使用它的最简单的方法就是利用模拟器。通过命令行运行该工具：
> hierarchyviewer
## 结论
希望我已为您的应用性能提升需要提供了一些新的工具与知识。除了使用这些工具来发现您在哪些方面可以获得增益外，很多性能改进可在代码级别实现。
