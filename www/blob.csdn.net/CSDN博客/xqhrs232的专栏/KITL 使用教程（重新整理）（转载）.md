# KITL 使用教程（重新整理）（转载） - xqhrs232的专栏 - CSDN博客
2009年11月21日 23:20:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：901
使用KITL的详细教程 
//-----------------------------------------------------------------------------------------------------------
// 日期：2009年3月19日     9:58:16
// 作者：[wogoyixikexie@gliet](mailto:wogoyixikexie@gliet)
// 版权：桂林电子科技大学一系科协[wogoyixikexie@gliet](mailto:wogoyixikexie@gliet)
// 平台：2440 5.0 BSP
// 最后修改：2009年3月19日 16:56:28
//-----------------------------------------------------------------------------------------------------------
     在几个以前，我曾经玩过一下子KITL，但是那时候什么都不懂，根本没有了解KITL是什么东西，更别说是工作原理了，这段时间都是在调试一些键盘等本机驱动（系统启动时候就启动的驱动），这个驱动不能使用驱动调试助手来调试，所以非常郁闷，每次都要打包烧写内核，不仅效率非常低，而且烧写过程很容易使电脑重启，哈哈，搞多几次，估计硬盘会严重受损。调试本机驱动，非请KITL这个大神不可！
     现在我们就来一步一步操作吧。
     首先，修改BSP一些配置。
![](http://images.cnblogs.com/cnblogs_com/wogoyixikexie/KITL%E6%95%99%E7%A8%8B/BSP%E8%AE%BE%E7%BD%AE.JPG)
     然后，修改编译设置，这个很关键，否则根本调试不了。之后就可以编译了。当编译出一个KITL内核以后，烧写进flash以后就不用再烧写了，KITL最大方便就是如此。
![](http://images.cnblogs.com/cnblogs_com/wogoyixikexie/KITL%E6%95%99%E7%A8%8B/%E7%BC%96%E8%AF%91%E8%AE%BE%E7%BD%AE.JPG)
     哦，还少了个连接设置，这个也是缺一不可，否则KITL也是无法attach device成功的。
![](http://images.cnblogs.com/cnblogs_com/wogoyixikexie/KITL%E6%95%99%E7%A8%8B/%E8%BF%9E%E6%8E%A5%E8%AE%BE%E7%BD%AE.JPG)
     还有一个重要步骤，就是执行target——》Release Directory Modules
![](http://images.cnblogs.com/cnblogs_com/wogoyixikexie/KITL%E6%95%99%E7%A8%8B/%E5%8A%A0%E5%85%A5DLL.JPG)
     现在，我们来做个实验，看看KITL是否真的起作用了。修改PowerButton驱动，在初始化函数中加入了一个打印信息。看看这样是否真的能打印。Come on！
![](http://images.cnblogs.com/cnblogs_com/wogoyixikexie/KITL%E6%95%99%E7%A8%8B/%E4%BF%AE%E6%94%B9%E4%BB%A3%E7%A0%81.JPG)
     编译PowerButton驱动，然后执行attach device连接，就会停在上图了，并且会在PB打印信息窗口看见真的新增加的信息，看来确实成功了。不过在这里要提醒的是，这个断点必须在编译驱动之前设置，以前我我在attach device之后设置，真是贻笑大方！
![](http://images.cnblogs.com/cnblogs_com/wogoyixikexie/KITL%E6%95%99%E7%A8%8B/%E6%89%93%E5%8D%B0%E4%BF%A1%E6%81%AF.JPG)
     哈哈，寄存器的值都可以显示出来。
![](http://images.cnblogs.com/cnblogs_com/wogoyixikexie/KITL%E6%95%99%E7%A8%8B/%E6%9F%A5%E7%9C%8B%E5%AF%84%E5%AD%98%E5%99%A8%E5%80%BC.jpg)
     OK！KITL调试驱动验证完毕。谢谢CSDN众多网友，尤其是Ricky_hu，sunrain_hjb，gsymichael 等。这些都是雕虫小技，但是很多人都不能拿到三星的操作文档，所以我就来献丑了。在这里我要提醒的是，修改一次驱动就要启动一次设备，否则是不行的，这是KITL的鸡肋，不过像本机驱动这种东西他就是在启动的时候运行的，所以用KITL来调试本机驱动是上上之选。对于流驱动的调试，我觉得万不得已不要用使用。因为KITL之后系统会运行很慢，并且同步软件不能连接，还是使用驱动调试助手控制注册表比较适合。
     最后我要提几个问题，希望了解的朋友能留言告诉我。
     1.编译出KITL内核运行后，触摸屏变得不准了，但是我的BSP之前是准确的，我想是KITL之后系统运行慢很多，导致AD采用速率慢了，所以必须重新校正。——后来发现，是没有得到更新的缘故。我也奇怪，我操作正常，但是却不理想。
     2.还有我编译debug KITL会造成启动过程中，弹出对话框，无法正常启动，不知道是什么原因。现在我只能编译release 的NK，打印信息很少。
     3.我本来可行的ZLG7290 的键盘驱动本来是可行的，但是现在按下毫无反映。莫非也是速度变慢，造成读的键盘的值有误？——后来发现，是没有得到更新的缘故。我也奇怪，我操作正常，但是却不理想。
     请知道的朋友留言指点。谢谢。
     注意事项：sources文件指定生成的DLL名字要和BIB文件汇中的加载名字一致，否则不能产生断点的。例如三星的自带触摸屏驱动就是这样，我刚开始没有修改之前是
 touch.dll      $(_FLATRELEASEDIR)/s3c2440a_touch.dll                 NK  SH
这样是不行的，后来我改成：
 touch.dll       $(_FLATRELEASEDIR)/touch.dll                 NK  SH
就行了。
     支持KITL的 2440 BSP下载地址[http://d.download.csdn.net/down/1115341/gooogleman](http://d.download.csdn.net/down/1115341/gooogleman)
转载请标明:作者[wogoyixikexie@gliet](mailto:wogoyixikexie@gliet).桂林电子科技大学一系科协，原文地址：[http://www.cnblogs.com/wogoyixikexie/](http://www.cnblogs.com/wogoyixikexie/)（或者我在CSDN的博客：[http://blog.csdn.net/gooogleman](http://blog.csdn.net/gooogleman)）——如有错误，希望能够留言指出;如果你有更加好的方法，也请在博客后面留言，我会感激你的批评和分享。
本文来自CSDN博客，转载请标明出处：[http://blog.csdn.net/gooogleman/archive/2009/04/03/4001784.aspx](http://blog.csdn.net/gooogleman/archive/2009/04/03/4001784.aspx)
本文来自CSDN博客，转载请标明出处：[http://blog.csdn.net/gooogleman/archive/2009/04/03/4001784.aspx](http://blog.csdn.net/gooogleman/archive/2009/04/03/4001784.aspx)
本文来自CSDN博客，转载请标明出处：[http://blog.csdn.net/gooogleman/archive/2009/04/03/4001784.aspx](http://blog.csdn.net/gooogleman/archive/2009/04/03/4001784.aspx)
