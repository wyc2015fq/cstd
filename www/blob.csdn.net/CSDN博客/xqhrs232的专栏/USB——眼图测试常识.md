# USB——眼图测试常识 - xqhrs232的专栏 - CSDN博客
2017年02月19日 12:33:53[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：7224
原文地址::[http://blog.csdn.net/vvcumt/article/details/38852347](http://blog.csdn.net/vvcumt/article/details/38852347)
相关文章
1、眼图测量----[http://baike.baidu.com/link?url=FIzR0swQ8ShGVQBV7wzMsYzv0HNEdRbQoTRO4vRwruhRdNsfmmlw0BvtRD9Dl0bWxpJJfmXrEdHN6FMjVaPwB5Cbg_ZINv7S26ms1_AKC6wjzbiEVUnXhdFYpX1HB0q8](http://baike.baidu.com/link?url=FIzR0swQ8ShGVQBV7wzMsYzv0HNEdRbQoTRO4vRwruhRdNsfmmlw0BvtRD9Dl0bWxpJJfmXrEdHN6FMjVaPwB5Cbg_ZINv7S26ms1_AKC6wjzbiEVUnXhdFYpX1HB0q8)
2、工程师必须懂得眼图分析方法----[http://wenku.baidu.com/link?url=5LPJRBX9YGGenmqngpUk-fkRrSGQYTRD3T2Qr2nkut3VsuIBIlgWAxpXuVxPKNUX5N9yptrKvRhg8HUWXng2RTnX_ISHKDYL1zMc9uK7spK](http://wenku.baidu.com/link?url=5LPJRBX9YGGenmqngpUk-fkRrSGQYTRD3T2Qr2nkut3VsuIBIlgWAxpXuVxPKNUX5N9yptrKvRhg8HUWXng2RTnX_ISHKDYL1zMc9uK7spK)
3、[[讨论]](http://bbs.52rd.com/forum.php?mod=forumdisplay&fid=81&filter=typeid&typeid=70)**USB 眼图如何调整---[http://bbs.52rd.com/Thread-298203-1-1.html](http://bbs.52rd.com/Thread-298203-1-1.html)**
4、**usb眼图不过----[http://bbs.21ic.com/icview-114269-1-1.html](http://bbs.21ic.com/icview-114269-1-1.html)**

最近的项目外设的USB总是传输数据异常，供应商建议我们做USB眼图[测试](http://lib.csdn.net/base/softwaretest)，对于纯软件出身的我对USB本身都知之甚少，更别提眼图了，赶紧补课：
如果将被测信号输入示波器，并且当示波器的触发时钟和被测信号同步时，在示波器上显示的图形很象人的眼睛，因此被称为眼图。
眼图是由各段码元波形叠加而成的，眼图中央的垂直线表示最佳抽样时刻，位于两峰值中间的水平线是判决门限电平。
在无码间串扰和噪声的理想情况下，波形无失真“眼”开启得最大。当有码间串扰时，波形失真，引起“眼”部分闭合。由此可知，眼图能直观地表明码间串扰和噪声的影响，可评价一个基带传输系统性能的优劣。
眼图有以下特征：
（1）眼图张开的宽度决定了接收波形可以不受串扰影响而抽样再生的时间间隔。显然，最佳抽样时刻应选在眼睛张开最大的时刻。
（2）眼图斜边的斜率表示系统对定时抖动（或误差）的灵敏度，斜边越陡，系统对定时抖动越敏感。
（3）眼图左（右）角阴影部分的水平宽度表示信号零点的变化范围，称为零点失真量，在许多接收设备中，定时信息是由信号零点位置来提取的，对于这种设备零点失真量很重要。
（4）在抽样时刻，阴影区的垂直宽度表示最大信号失真量。
（5）在抽样时刻，上、下两阴影区间隔的一半是最小噪声容限，噪声瞬时值超过它就有可能发生错误判决。
（6）水平方向上虚线位置对应的电压为判决门限电平。
[百度百科-眼图](http://baike.baidu.com/view/1227430.htm?fr=aladdin)
[豆丁网-眼图基础知识](http://www.docin.com/p-677890412.html)


