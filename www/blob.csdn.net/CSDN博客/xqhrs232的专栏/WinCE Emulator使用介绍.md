# WinCE Emulator使用介绍 - xqhrs232的专栏 - CSDN博客
2013年02月01日 16:00:08[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1008
原文地址::[http://blog.csdn.net/nanjianhui/article/details/3481965](http://blog.csdn.net/nanjianhui/article/details/3481965)
相关网帖
1.2010-10-02 : Wince 6.0模拟器的使用----[http://taoqimhw.blog.163.com/blog/static/1155316720109225422491/](http://taoqimhw.blog.163.com/blog/static/1155316720109225422491/)
作者：ARM-WinCE
最近用了一下WinCE的模拟器，我做WinCE有几年了，从来没有用过WinCE的模拟器，第一次接触WinCE的时候就是一块S3C2410的板子和PB4.2的开发平台，然后就开始折腾，后来各种开发板都用过，WinCE也从4.2版本用到了6.0 R2版本，就是没用过模拟器。前段时间要验证点东西，手头又没板子，所以这次也用了一下。我看到网上很多人写过关于WinCE Emulator的文章，那我要写的应该和其他的没有什么区别，只是给自己留一个记忆。
WinCE模拟器是一个不依赖于硬件可以运行WinCE的环境，它提供了一个虚拟的硬件平台，这样可以在上面测试一些应用程序。在WinCE6.0中提供了针对于ARM的模拟器的支持，一般和底层硬件相关的程序是不能在模拟器上面调试的，但是有些界面程序和简单的应用程序，在WinCE模拟器上面调试还是很方便的。
**1   WinCE模拟器配置**
首先要创建一个基于模拟器的工程，按照WinCE的工程向导，在选择BSP的时候，要选择Device Emulator: ARMV4I，如图：
![](https://p-blog.csdn.net/images/p_blog_csdn_net/nanjianhui/EntryImages/20081209/emulator_1.JPG)
当工程创建完成后，需要编译WinCE，编译完成后，就可以将WinCE下载到模拟器上面运行。在下载之前，需要配置连接属性，如图：
![](https://p-blog.csdn.net/images/p_blog_csdn_net/nanjianhui/EntryImages/20081209/emulator_2.JPG)
如图所示，Download和Transport都要选择Device Emulator(DMA)，Debugger选项要选择KdStub。然后点击Download右侧的Settings按钮，会弹出模拟器设置的对话框，在对话框中选择display，如图：
![](https://p-blog.csdn.net/images/p_blog_csdn_net/nanjianhui/EntryImages/20081209/emulator_3.JPG)
可以看到，在Display对话框中可以设置最终模拟器显示的宽和高，以及显示深度，和翻转角度等。用户可以根据自己的需要来设置。然后选择Network，如图：
![](https://p-blog.csdn.net/images/p_blog_csdn_net/nanjianhui/EntryImages/20081209/emulator_4.JPG)
如果要想使在模拟器上运行的WinCE具有网络连接，可以在这里选择绑定某个网卡，也可以选择绑定当前PC的网卡。Peripherals用于配置外设，一般用于配置串口，如图：
![](https://p-blog.csdn.net/images/p_blog_csdn_net/nanjianhui/EntryImages/20081209/emulator_5.JPG)
可以在这里选择在模拟器上运行时所支持的串口，如果想获得打印信息，还可以选择Create text console window for serial por 1来实现。最后模拟器配置完成后，点OK就可以了。然后在菜单中选择Target，然后选择Attach Device，WinCE的image就会被下载，然后在模拟器中运行，如图：
![](https://p-blog.csdn.net/images/p_blog_csdn_net/nanjianhui/EntryImages/20081209/emulator_6.JPG)
WinCE在模拟器上面成功运行以后，就会看到这个界面。可以移动鼠标在模拟器里面进行操作，也可以运行应用程序。一般来说，调试一些显示界面的程序，使用模拟器还是很方便的。
//============================================================
备注：：
1、会很好地使用模拟器真会方便很多，很多设计或者思路立马就可以得到验证，而不需要借助于实际的硬件，你要借助实际的硬件又有很多不受你控制的因素存在，硬件有没问题，BSP有没问题，编译有没问题，下载有没问题，能不能同步或者连接，这样你就很累了。
2、WINCE模拟器几乎可以模拟一般所有的实现，除了一些非得借助硬件实现的东西，比如3D，OPENGL等这些高级特性，模拟器也有串口可以访问，也有网络可以访问，也可以打印调试消息，也能跟PC同步，可以在线调试APP等等，WINCE下的模拟器确实很完美！
3、很多东西/技术，做到了能够收发自如才能事半功倍，所以说很多东西真的靠时间积累与长期浸淫，要不然永远都只是半瓶子水，在那里晃荡的空空响，永远只是停留在肤浅的表面。希望自己能够真正做到厚积薄发！
