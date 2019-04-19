# WinCE远程桌面的实现----RDP/cerhost.exe - xqhrs232的专栏 - CSDN博客
2013年01月23日 23:21:57[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：3542
原文地址::[http://blog.csdn.net/nanjianhui/article/details/4355597](http://blog.csdn.net/nanjianhui/article/details/4355597)
相关网帖
1、[WinCE下的远程控制](http://blog.csdn.net/xqhrs232/article/details/11178745)[----RDP/cerhost.exe](http://blog.csdn.net/xqhrs232/article/details/11178745)----[http://blog.csdn.net/xqhrs232/article/details/11178745](http://blog.csdn.net/xqhrs232/article/details/11178745)
2、Wince远程控制工具----[http://download.csdn.net/download/flydouble/3510396](http://download.csdn.net/download/flydouble/3510396)
WinCE支持远程桌面功能，可以在PC端访问目标设备的WinCE桌面。这个功能在调试的时候很有用。有些时候，你的目标板不带显示设备，这时你想访问目标板的WinCE系统感觉很不爽，毕竟习惯了桌面操作。通过WinCE远程桌面，你可以在PC上面访问目标板的WinCE桌面并进行操作。
下面来介绍一下具体步骤：
**1. 选择支持WinCE远程桌面的组件：**
在WinCE的工程中选择”Core OS”->”CEBASE”->”Core OS Services”->”Debugging Tools”->”Remote Display Application”，如下图：
![](https://p-blog.csdn.net/images/p_blog_csdn_net/nanjianhui/EntryImages/20090717/Cerdisp_1.JPG)
在这里建议同时添加telnet组件，在”Core OS”->”CEBASE”->”Communication Services and Networking”->”Servers”->”Telnet Server”。由于目标板可能没有任何输入输出设备，所以添加Telnet组件可以在系统启动以后，登陆Telnet来执行命令。
**2. 配置远程桌面主机IP地址：**
打开WinCE工程的注册表文件project.reg，然后添加下面的内容。
**[HKEY_LOCAL_MACHINE/SOFTWARE/CERDISP]**
**"Hostname"="192.168.0.88"**
其中Hostname为PC机的IP地址，也可以是计算机名。
还可以同时添加Telnet注册表配置，如下：
**[HKEY_LOCAL_MACHINE/COMM/TELNETD]**
**"IsEnabled "=dword:1**
**"UseAuthentication"=dword:0**
该配置实现了无需身份验证，直接登录Telnet的功能。
**3. 重新编译，下载运行WinCE：**
由于添加了新的WinCE组件，需要重新编译WinCE工程。编译成功后，可以下载到目标板上面运行。
**4. 启动PC端远程桌面程序：**
WinCE在目标板运行以后，首先在/WINCE600/Public/Common/Oak/Bin/I386目录下找到cerhost.exe，然后运行。运行后选择File，然后选择Connect，在弹出的对话框中输入目标板的IP地址，然后点击OK等待连接。
**5. 启动目标板远程桌面程序：**
首先通过Telnet登录目标板，然后再命令行中输入如下命令：
**Cerdisp –c**
这时，就可以在PC上的cerhost界面中看到目标板WinCE的桌面了，而且可以通过鼠标键盘进行操作，如图：
![](https://p-blog.csdn.net/images/p_blog_csdn_net/nanjianhui/EntryImages/20090717/Cerdisp_2.JPG)
其实很简单，要注意的是首先运行PC端的cerhost，它应该是Server。然后运行目标板上的Cerdisp，这个应该是Client。
//===========================================================================================
备注::
1>WinCE在目标板运行以后，首先在/WINCE600/Public/Common/Oak/Bin/I386目录下找到cerhost.exe，然后运行
2>cerhost.exe++++cerdisp.exe的源代码路径----D:\WINCE600\PUBLIC\COMMON\OAK\DRIVERS\CERDISP
