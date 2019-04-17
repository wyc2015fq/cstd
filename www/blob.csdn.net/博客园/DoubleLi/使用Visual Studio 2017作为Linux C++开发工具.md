# 使用Visual Studio 2017作为Linux C++开发工具 - DoubleLi - 博客园






## Visual Studio 2017

微软的宇宙第一IDE Visual Studio 2017正式版出来了，地址是：https://www.visualstudio.com/vs/whatsnew/

VS2017亮点很多，包括模块化安装、启动速度加快（第一次启动加快50%以上）、github插件、启动页完善、增强代码提示功能，等等，更多参考：https://www.visualstudio.com/en-us/news/releasenotes/vs2017-relnotes。

## Visual Studio 2017 Support for Linux Development

VS2017可以编辑、编译、调试linux上的c++、python、node.js程序（详细）。

甚至可以编译调试.NET程序，支持的Linux平台包括：
- CentOS 7.1 and Oracle Linux 7.1
- Debian 8
- Fedora 23
- Linux Mint 17
- openSUSE 13.2
- Red Hat Enterprise Linux 7.2
- Ubuntu 14.04 and 16.04

博主主要是c++作为开发语言，并且绝大部分代码都有跨平台的需求。博主一般步骤如下：
- 在windows机器上使用visual studio开发大部分算法功能模块，并且调试优化完毕；
- 在涉及到IO或者网络相关的部分，需要port到linux平台，痛苦的用vim编写代码，gcc编译，gdb调试。

重点关注其中Visual Studio在C++跨平台方面的亮点，即Visual C++ for Linux，有了它，就能用最先进的IDE进行跨平台的开发了，好开心！

## Visual C++ for Linux

VC++ 2017支持在编写、调试Linux下的C++程序（[详细(https://blogs.msdn.microsoft.com/vcblog/2016/03/30/visual-c-for-linux-development/)）。

创建跨平台Linux C++程序，如图：

![](https://images2015.cnblogs.com/blog/296875/201703/296875-20170311000412436-1281469527.gif)

实际使用效果如下图，可以看到，能够在VS下面编译Linux代码，甚至包括linux特有的头文件。除了代码补全很爽之外，还有代码着色，可视化调试等各种优点。

![](https://images2015.cnblogs.com/blog/296875/201703/296875-20170311000427107-168345094.png)
![](https://images2015.cnblogs.com/blog/296875/201703/296875-20170311000442576-2036010353.png)

## 使用步骤
- 
目标linux机器上安装好编译环境与调试环境，如在ubuntu下，安装好openssh-server/g++/gdb/gdbserver等。
`sudo apt-get install openssh-server g++ gdb gdbserver`
- 
本机安装好visual studio 2017，需要选择安装Cross Platform Linux C++环境。

- 
打开Visual Studio，新建工程，选择Cross Platform/Linux/Console Application。

- 
在第一次build之前会要求ssh连接到目标linux机器，填上用户名密码连接目标机器。如果需要更换目标机器地址，在Tools/Options/Cross Platform下进行管理。

> 
![](https://images2015.cnblogs.com/blog/296875/201703/296875-20170311000526232-382411590.png)


- 
设置好一系列选项（黑体部分），与普通的Win32程序的设置基本一致。需要注意如下2点：

> - vs不会自动把addtional include中的头文件复制到本地来做代码补全的提示，需要手动将linux下面的/usr/include、/usr/local/include等目录复制到vs的linux header path（如：C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\Common7\IDE\VC\Linux\include\usr）
- 
## 头文件包含目录和库文件包含目录均为linux下的绝对路径。
![](https://images2015.cnblogs.com/blog/296875/201703/296875-20170311000543779-657967104.png)
![](https://images2015.cnblogs.com/blog/296875/201703/296875-20170311000549123-1833977220.png)
![](https://images2015.cnblogs.com/blog/296875/201703/296875-20170311000552982-47754159.png)
![](https://images2015.cnblogs.com/blog/296875/201703/296875-20170311000557389-837001759.png)
![](https://images2015.cnblogs.com/blog/296875/201703/296875-20170311000602748-2010289737.png)


- 
编译时vs会将代码复制到目标机器的~/projects/[project-name]目录下，二进制文件在其目录的bin目录下。如下图，Linux环境下可以看到，已经有代码了。

> 
![](https://images2015.cnblogs.com/blog/296875/201703/296875-20170311000613967-1894961011.png)
![](https://images2015.cnblogs.com/blog/296875/201703/296875-20170311000608592-2115544025.png)



## 实际开发

在初步使用体验中，感觉这个vs for linux还不是特别成熟，有不少问题，包括：复制文件速度过慢、无法多线程编译、编译输出信息丢失（不够友好）、无法操作跳板机等。
不过该工具作为编码的辅助工具，仍然非常具有价值，结合了vs强大的的代码编辑与调试功能和linux机器的天然服务器特性。未来可期！









