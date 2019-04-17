# OpenCV学习笔记（二十三）——OpenCV的GUI之凤凰涅槃Qt - 迭代的是人，递归的是神 - CSDN博客





2011年11月09日 20:08:38[yang_xian521](https://me.csdn.net/yang_xian521)阅读数：12053
所属专栏：[OpenCV学习笔记](https://blog.csdn.net/column/details/opencv-manual.html)










OpenCV的发展趋势真是越来越向Linux方向发展，对Windows平台的支持越来越少，2.2版本之后把CvvImage也给删除了，这让人如何开发MFC程序啊，网上搜了好久，基本的办法都是调用之前版本的代码，继续使用CvvImage，这样会导致一个很严重的问题，就是程序里显示的所有的图片都必须是CvvImage类，不能用MFC的图片显示方法，这就需要对代码进行大量的修改，不符合开发的要求。感觉大家的办法都是治标不治本，而且MFC也是快要被日新月异的新技术拍在沙滩上的东西。


我总不能永远在控制台下面开发OpenCV应用程序吧。看了一下OpenCV的用户手册，发现它支持跨平台的Qt这个UI开发框架。Qt是诺基亚开发的一个跨平台的C++图形用户界面应用程序框架，我索性也学习一下。Qt的安装我参考的是这篇文章[http://blog.sina.com.cn/s/blog_661159d50100ib38.html](http://blog.sina.com.cn/s/blog_661159d50100ib38.html)，文章里介绍的方案也是我的开发环境VS2008，想必对于配置OpenCV相当熟练的大牛们，配置Qt肯定也难不倒各位。安装好后，就是进行OpenCV的GUI开发咯

![](http://hi.csdn.net/attachment/201111/9/0_1320839988clyO.gif)



从上图中我们可以看出Qt对OpenCV的支持还是很好的，需要用到的开发工具为Qt*GUI，这个GUI可以提供状态栏、工具栏和控制面板。控制面板上面可以制作进度条河按钮等控件（使用trackbar，要确保窗口的参数为NULL；使用buttonbar之前，要先创建一个按钮）。相关的函数有setWindowProperty、getWindowProperty、fontQt、addText、displayOverlay、displayStatusBar、createOpenGLCallback、saveWindowParameters、loadWindowParameters、createButton。由于Qt我也是刚接触，对这些函数的理解和新收获，我会陆续补充在这篇文章里。



