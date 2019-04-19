# WinCE下3G模块的调试----解释了假连接状况 - xqhrs232的专栏 - CSDN博客
2013年01月27日 20:35:05[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：980
个人分类：[3G/RIL/TAPI/网络技术](https://blog.csdn.net/xqhrs232/article/category/906928)
原文地址::[http://www.cnblogs.com/we-hjb/archive/2009/10/22/1587788.html](http://www.cnblogs.com/we-hjb/archive/2009/10/22/1587788.html)
这两天在WinCE6.0上调试了下3G模块，华为的EM730，它支持GPRS和WCDMA。虽然没有涉及到驱动的移植（没有源代码），但调试过程还是几经周折，记下来以便后用。
　　EM730是MINI PCI接口的，也支持USB。于是先请硬件工程师帮忙飞了一堆线，在PC上测试了一下，工作完全正常，证明模块本身没有问题。
　　在WinCE6.0上，安装了HW提供的一个驱动，并将模块插入，驱动似乎也是正常工作了，没有提示需要再安装驱动。打开串口调试助手，竟然发现多了三个可用的串口，确定驱动是工作起来了。用串口调试助手打开这三个串口逐一发送AT命令，也有正确返回。到这里，我以为离成功就不远了，但最终是行百里者半九十，恰恰在这里停顿了很久，费了很多时间。因为HW没有提供更多的有关模块在CE上的使用说明，只有一个CAB文件。根据先前调试GPRS的经验，试了N次，始终无法完成拨号。甚至由深圳那边的王工远程调试，也没什么进展。最后得出结论，可能是HW提供的驱动有一些问题。为了证明这一点，我和王工几乎花了一天的时间，很冤枉。后来，王工尽力帮忙找HW要了最新的驱动和拨号后台程序，在我们的设备上测试了一下，可以拨号，浏览网页。但事情还没有完，用它的拨号后台程序可以连接，但自己手动建立的拨号连接却不可以。难道是拨号后台程序对模块做了初始化的工作，发送了一些特殊的指令。为了证实这个问题，又动了番脑筋，想着以前曾用过的串口分离软件，想借助该软件捕获后台拨号程序到底发送了哪些初始化的指令。不过这个方法最终没有成功。在调试的过程中逐一排查，最后基本确定应该是注册表配置的问题，拨号后台程序并没有发送特殊的初始化指令，它只是通过其中一个串口获取模块的状态而已。利用驱动调试助手的注册表监视和导出功能，找出区别并修改后，就可以在控制面板中直接用新建的拨号连接拨号了。
　　以下是调试过程中一些关键的地方。在调试的过程中有一个中间状态，拨号很快完成，快得很假，显示已经连接，但实际上这是个虚假的状态。截图如下，
[](http://images.cnblogs.com/cnblogs_com/we-hjb/WindowsLiveWriter/WinCE3G_8E0/clip_image002_2.jpg)
![clip_image002](http://images.cnblogs.com/cnblogs_com/we-hjb/WindowsLiveWriter/WinCE3G_8E0/clip_image002_thumb.jpg)
　　可以看到此处显示设备已连接，但实际上是虚假的，这种状态维持不了一会儿，便会自动断开。
　　下面是拨号连接真正成功时的截图，
[](http://images.cnblogs.com/cnblogs_com/we-hjb/WindowsLiveWriter/WinCE3G_8E0/clip_image004_2.jpg)
![clip_image004](http://images.cnblogs.com/cnblogs_com/we-hjb/WindowsLiveWriter/WinCE3G_8E0/clip_image004_thumb.jpg)
　　可以看到连接上以后，在网络连接中多了一个“COM1：”的连接。此时打开IE就可以浏览网页了，也可以用ipconfig工具查看网络连接的状态，截图如下，
[](http://images.cnblogs.com/cnblogs_com/we-hjb/WindowsLiveWriter/WinCE3G_8E0/clip_image006_2.jpg)
![clip_image006](http://images.cnblogs.com/cnblogs_com/we-hjb/WindowsLiveWriter/WinCE3G_8E0/clip_image006_thumb.jpg)
　　从能虚假连接上到实际拨号成功，主要修改了注册表中的APN设置，安装驱动时默认用的是CMWAP，而用WCDMA模块时需设置成3gnet。另外，该模块是USB模拟成串口的，串口号因平台不同而不同，需要确定用于拨号连接的是哪一个串口，然后在拨号属性中设置一下。虽然是模拟成串口，但波特率等属性是无需关心的，不必做任何修改。
　　调试3G模块时又发现了驱动调试助手的一个BUG，不能正常导入MULTI_SZ类型的键，导出MULTI_SZ键时用十六进制的方式显示，不方便查看和修改，花了点时间修改了一下，现在可以很好的支持MULTI_SZ了，比原来更方便、更直观。有需要的同学可在这里下载，[http://download.csdn.net/source/1759970](http://download.csdn.net/source/1759970)
　　EM730驱动程序的下载地址:[http://files.cnblogs.com/we-hjb/HWEM730ARMWinCE6.0Driver.zip](http://files.cnblogs.com/we-hjb/HWEM730ARMWinCE6.0Driver.zip)
//=============================================================
备注::
1、成功连接会多出一个----在网络连接中多了一个“COM1：”的连接----在我自己的平台只看到一次，然后就消失了，然后也马上报下面的错误。觉得不像是不能拨号成功，而是连接了不稳定什么的。
2、虚假的连接过不了多久就自动断开并报----“已与您拔叫的远程计算机断开。请验证“波特率”设置并重试连接”；并且连接的过程中不显示“真正验证用户名和密码”++++“正在网络上注册你的计算机”这两串字符串！！！
3、从能虚假连接上到实际拨号成功，主要修改了注册表中的APN设置，安装驱动时默认用的是CMWAP，而用WCDMA模块时需设置成3gnet----真的是这样的吗？？？
4、今天调通ME800
 EVDO 3G模块的一点经验
 
 //1111----拨号成功会出现一个新的连接，要注意这个新连接的IP地址获取方式----DHCP动态分配
  //2222----如果有网卡最好把网卡那个连接禁止
  //3333----INTERNET选项里面的连接的LAN选择为自己建立的那个连接
  //4444----3G的驱动有没加载成功，最好先通过串口工具测试一下
  //5555----EVDO的网络的拨号的号码为#777，并且要求输入用户名跟密码，他们都是card!

