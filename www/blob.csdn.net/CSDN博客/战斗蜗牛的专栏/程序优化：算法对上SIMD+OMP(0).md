# 程序优化：算法对上SIMD+OMP(0) - 战斗蜗牛的专栏 - CSDN博客





2014年04月05日 03:20:57[vbskj](https://me.csdn.net/vbskj)阅读数：1306
个人分类：[SSE并行](https://blog.csdn.net/vbskj/article/category/2169499)









程序优化是个永恒的话题。随着MMX，SSE，SSE2硬件的普及，越来越多的编译器开始支持SIMD指令编程，程序员编写程序时可以很方便的利用先进的硬件为程序带来更大的性能提升，所为“花一样钱补两样”，真是惠而不费，程序员的福音。

能够使用高性能的硬件固然好，不过，如果亲身进行实例分析，想当然的认为只要利用硬件的先进功能就能带来程序效率上提升，可能反而会编制出能够利用高级硬件功能的低效程序来。


究竟是算法和高级硬件谁能够取得更高的性能？传统的算法与先进硬件谁能够为程序带来更大的效能提升？还是让我们写个程序实例，让数据来说话。



假设要写这样一个程序：对一幅尺寸为1024X1024，每像素为32位的图像进行某种处理，处理方法如下：

1）如果各颜色分量与0xFF的差值之和小于24，则转到2步进行处理，否则处理下一像素

2）对像素进行如下处理：

过滤比值：颜色分量/255

新的颜色分量＝原颜色分量*(1-过滤比值) + 指定颜色的分量 * 过滤比值



         伪码如下：

if ( (255 * 3 - (R + G + B)) < 3 * 8 )

 {

      delta_R = R / 255.0

      delta_G = G / 255.0

      delta_B = B / 255.0



      R = R * (1 - delta_R) + newColor_R * delta_R

      G = G * (1 - delta_G) + newColor_G * delta_G

      B = B * (1 - delta_B) + newColor_B * delta_B

 }



该处理方法可以用于颜色过滤。

     拟使用以下方法来进行处理：

1）传统方法

2）传统方法＋OMP

3）使用Intrinsics，进行SIMD处理

4）使用Intrinsics，进行SIMD处理＋OMP



为了提高度量精度，使用QueryPerfromanceFrequency记录时间。为了方便使用，定义了以下宏：



//申明时间度量变量

#define DECL_PERF()     __int64 m_CounterFreq, m_CounterStart, m_CounterEnd;

//初始化时间变量

#define INIT_PERF()     QueryPerformanceFrequency((LARGE_INTEGER *)&m_CounterFreq);

//记录操作开始时间

#define BEGIN_PERF()    QueryPerformanceCounter((LARGE_INTEGER *)&m_CounterStart);

//记录结束时间

#define END_PERF()      QueryPerformanceCounter((LARGE_INTEGER *)&m_CounterEnd);

//计算操作耗费的时间

#define GET_PERF()      ((m_CounterEnd - m_CounterStart) / (FLOAT)m_CounterFreq)



每种处理方法都在进行处理循环的之前，先调用BEGIN_PERF()，并在结束之后，立即调用END_PERF()，然后以GET_PERF()作为返回值。同时，使用具有双核CPU的计算机来运行程序，以真正发挥OMP的作用。

测试机器(我的爱机)：

CPU：AMD X64 3600+

内存：万紫千红1G单条

主板：七彩虹NF4

硬盘：西捷250G SATA

显卡：七彩虹1650XG CF3



