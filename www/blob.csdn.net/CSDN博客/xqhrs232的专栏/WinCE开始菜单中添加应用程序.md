# WinCE开始菜单中添加应用程序 - xqhrs232的专栏 - CSDN博客
2013年01月24日 20:45:27[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：495
个人分类：[WINCE](https://blog.csdn.net/xqhrs232/article/category/906924)
原文地址::[http://blog.csdn.net/nanjianhui/article/details/4313044](http://blog.csdn.net/nanjianhui/article/details/4313044)
作者：ARM-WinCE
添加应用程序到WinCE的开始菜单中应该是一个比较常用的功能，在产品定制的时候，添加本产品的应用到StartMenu中，让用户可以方便启动程序，下面就来介绍一下：
先说一下原理，其实在WinCE启动以后，点击Start，然后点击Programs，里面所能看到的应用其实也都是以文件的形式存放的，它们存放在/Windows/Programs目录下面，都是一些快捷方式文件，这些快捷方式文件连接了真正的应用程序，就这么简单。
理解了这个原理，也就清楚该怎么做了，步骤如下：
1. 更改project.bib文件，添加应用程序，这里以Hello, world为例如下：
hello.exe     d:/WINCE600/OSDesigns/Emulator/Emulator/RelDir/DeviceEmulator_ARMV4I_Release/hello.exe           NK S
2. 为该应用程序创建一个快捷方式文件，可以新建一个*.txt文件，然后在里面添加：**18#/windows/hello.exe**，其中18表示后面的字符数，然后关闭该txt文件，重命名为hello.lnk，并将该文件拷贝到WinCE工程的release目录下面。
3. 更改project.bib文件，包含hello.lnk文件，如下：
hello.lnk     d:/WINCE600/OSDesigns/Emulator/Emulator/RelDir/DeviceEmulator_ARMV4I_Release/hello.lnk           NK SH
4. 打开project.dat文件，将hello.lnk重新定位到/Windows/Programs目录下面，如下：
Directory("/Windows/Programs"):-File("HELLO.lnk", "/Windows/hello.lnk")
5. 将project.dat和project.bib拷贝到工程的release目录下面，然后Makimge就可以了。
最后运行WinCE的时候，应该可以看到应用程序已经被添加到StartMenu的Programs中了，如图：
![](https://p-blog.csdn.net/images/p_blog_csdn_net/nanjianhui/EntryImages/20090701/StartMenu_1.JPG)
通过这个例子可以看出，BIB中包含的文件在Makeimg以后，都会放在WinCE系统的/Windows目录下面，而我们可以通过更改DAT文件来将他们重定向到另一个文件夹中。通过这种方式可以添加程序到Programs中，但是如果想改StartMenu本身，就需要更改源代码了，在/WINCE600/PUBLIC/SHELL/OAK/HPC/EXPLORER/TASKBAR/stmenu.cpp下面。
