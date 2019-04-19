# WinCE中公开源代码的编译方法 - xqhrs232的专栏 - CSDN博客
2013年11月23日 11:39:11[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：912
原文地址::[http://www.cnblogs.com/we-hjb/archive/2009/02/16/1391984.html](http://www.cnblogs.com/we-hjb/archive/2009/02/16/1391984.html)
从WinCE5.0开始，微软就公开了WinCE的相关源代码。一方面，这使我们可以更深入的学习WinCE操作系统，另外一方面，也让定制个性化WinCE操作系统成为可能。这里要介绍的就是如何编译WinCE公开的源代码，以开机输入密码的界面为例。
     WinCE开机输入密码的界面，一定搞晕了不少朋友。先前曾写过一篇文章，尝试着解决这个问题。但毕竟那是没有办法的办法，并且有时候还用不了。而这个问题，对于定制WinCE操作系统的工程师来说，其实很简单，只要添加一个函数调用即可。一个API就能避免N多用户的无奈。
修改文件C:\WINCE500\PUBLIC\COMMON\OAK\DRIVERS\STARTUI\startui.cpp，在该文件的第450行添加SipShowIM(SIPF_ON);
    打开Platform Builder的工程，点击菜单“Build OS”——“Open Release Directory”，浏览到目录
C:\WINCE500\PUBLIC\COMMON\OAK\DRIVERS\STARTUI\下，运行Build，如下图所示，
![](http://images.cnblogs.com/cnblogs_com/we-hjb/build.png)
编译生成的文件在目录C:\WINCE500\PUBLIC\COMMON\OAK\TARGET\ARMV4I\RETAIL下，如下图所示，
![](http://images.cnblogs.com/cnblogs_com/we-hjb/result.png)
最后，重新Sysgen。也许有更快捷的方式，但Sysgen简单保险，机器配置好的话也用不了太长时间。
运行新的操作系统，设置用户密码后的开机界面如下图所示，
![](http://images.cnblogs.com/cnblogs_com/we-hjb/newstartui.png)
软键盘乖乖的出现了，我们也不会再为没有办法输入密码而挠头。
简单总结一下，定制个性化的WinCE操作系统基本分为三步：
第一步，在正确的文件当中修改相应的代码，注意备份其源文件和目标文件；
第二步，正确编译修改好的文件，并确认其目标文件确实被更新；
第三步，重新执行Sysgen。
整个过程，找对源文件最关键。修改和编译相应文件的同时，一定注意备份源文件和目标文件以免酿成大祸，尤其是目标文件也不能忽视，因为源文件和目标文件的版本有时并不一致。
