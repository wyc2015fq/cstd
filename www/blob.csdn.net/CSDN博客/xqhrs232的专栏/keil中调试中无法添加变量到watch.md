# keil中调试中无法添加变量到watch - xqhrs232的专栏 - CSDN博客
2016年07月17日 00:18:38[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：3196
原文地址::[http://blog.sina.com.cn/s/blog_0414b7360101qk5x.html](http://blog.sina.com.cn/s/blog_0414b7360101qk5x.html)
相关文章
1、KEIL调试时的WATCH窗口是查看的全局变量值，不能看内部变量值 
 ----[http://blog.163.com/ssou_1985/blog/static/295320362011229113049584/](http://blog.163.com/ssou_1985/blog/static/295320362011229113049584/)
2、**Keil中调试时Watch窗口变量不实时显示为何故？----[http://bbs.21ic.com/icview-311996-1-1.html](http://bbs.21ic.com/icview-311996-1-1.html)**
3、Keil调试局部变量显示"not
 in scope"的问题解决----[http://j1o1y.blog.sohu.com/302807722.html](http://j1o1y.blog.sohu.com/302807722.html)
在keil中通过JLink来在线调试STM32F103的项目，发现无论如何都不能将我要观察的变量添加到wahch窗口。真是郁闷非常。
后来发现，当运行完变量所在的子函数时，该变量的存储空间就会被释放，无法观察。所以，采用设置断点的方式，让程序停留在本子函数内部，以此来将要观察的变量添加到watch窗口之内，这样就解决了无法添加变量到watch窗口的问题。
并且，只能够添加变量来观察，如果是端口的话，则不能添加到watch的#1和#2中。但是可以通过外围设备 （Peripherals）下面的general purpose IO来查看，还可以通过工具来随意的更改IO的输出。（最好对自己的程序心中有数，知道更改哪个输出口的状态，乱改的话也可能出现问题）
如下图的界面，我选的是GPIOA
![keil中调试中无法添加变量到watch](http://s13.sinaimg.cn/mw690/0004Dhgagy6E8tYNIm09c&690)
最上面显示了 输出口的设置（输入还是输出，有无上拉等）
下面的GPIOA_ODR是输出寄存器，我们可以更改8号口的状态，8好扣输出到led，选中下面的方框，灯就亮了。所以我们可以很容易的来调试硬件问题。
//===========================================================================
备注::
1》
调试状态下，勾上View->Periodic Window Update
2》降低Keil的编译优化选项，如下图所示，优化等级改为0（默认为3）
