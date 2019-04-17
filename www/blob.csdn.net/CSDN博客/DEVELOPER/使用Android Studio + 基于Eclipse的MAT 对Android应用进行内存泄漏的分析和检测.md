# 使用Android Studio + 基于Eclipse的MAT 对Android应用进行内存泄漏的分析和检测 - DEVELOPER - CSDN博客





2017年05月17日 11:00:38[学术袁](https://me.csdn.net/u012827205)阅读数：527








刚来到北京进行面试的第一家公司，HR 问了一个问题：关于安卓开发过程中对性能分析和内存泄漏有没有接触过，使用过什么工具？其实，一句话问着我了。因为我之前上班的公司都是创业型公司，开发效率最重要！因为boss需要利用成品去拉投资，所以对于程序的内存泄漏和性能的分析还真的是没有接触过。但是同一个坑总不能跌进去两次，因此我就恶补了三个钟头接触了一下并借助之前的项目进行了一下检测和分析并进行了调整和优化，效果真的挺好；

本篇博客先什么也不多说，就是分析一下怎么使用！

![](https://img-blog.csdn.net/20170516225620244?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


首先你要使用Android Studio 安装你的项目到手机上，然后按照上图点击（1）Android Monitor 。这时会显示性能图形的展示界面，我们这时候能看到 Memory 内存耗得是很严重的（这是经过我三个页面快速的打开关闭进行测试的）；然后点击一下（2）强制进行GC（每隔几秒钟gc一下），我们可以看到内存释放了了好多，说明有问题存在！然后点击（3）生成快照 .hprof 文件。如下图所示：

![](https://img-blog.csdn.net/20170516230150221)


图中（1）就是快照文件；这时需要进行分析一下，查找到底是哪里出现了内存的泄露，点击（2）展开，如下图：

![](https://img-blog.csdn.net/20170516230332904?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


然后再点击绿色按钮，就能看到分析的结果，如下图：

![](https://img-blog.csdn.net/20170516230639071?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


其中，（1）就是分析的结果。展开Leaked Activities 我们可以看到都是那些类出现了内存泄漏，这样就很好的为我们指明了方向。接下来我们点击（2）可以查看我们生成的Java Heap 文件列表 快照Fiels：Heap Snapshot，如下图：

![](https://img-blog.csdn.net/20170517102158767?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


然后我们要右击选择"Export to standard .hprof" 生成  .hprof 文件使用基于Eclipse的 [MAT(Memory Analyzer Tool)](http://www.eclipse.org/mat/downloads.php) 打开，进行更详细的分析。他的使用方式和Eclipse相似，我们一起看一下：

![](https://img-blog.csdn.net/20170517103806698?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


上面就是分析的图表，如果我们想要更精准的定位分析，这时我们可点击下图中 snapeshot.hprof 下工具栏中的柱状图图标 按钮然后就能切换到下图页面：![](https://img-blog.csdn.net/20170517104009777?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

这时，我们根据上面的图四所分析的内存泄漏 Leaked Activitys 列表下的Activity-WaitPermissActivity ,在（1）位置输入这个Activity的名字然后回车 就能搜索到。然后右击选择（2）、（3）就能精确进入到该Activity的具体分析展示。

![](https://img-blog.csdn.net/20170517104722648?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


最后，我们可以看到在这个WaitPermissActivity类中出现的内存泄漏问题原因了吧？！他的问题就是这个类依赖的一个广播导致的内存泄漏。然后结合代码发现这个广播是全局的并且是在配置文件 Manefests 中进行静态注册的，广播功能是检测当前的应用网络状态，对某些页面若随时发生网络断开、连接的状态与用户进行交互反馈。因此发现问题有两个，一就是NetBroadcastReceiver这个广播没有反注册；而是广播接收其中的容器的容量一直在增加，没有及时的释放；然后我就根据这两点，把之前的静态注册改换为动态的注册方便控制，并在使用到的Activity和Fragment中进行注册和反注册及释放容器；然后进行修改并再次进行内存的泄露检测，问题都已成功解决！






