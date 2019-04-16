# Windows平台下NS2网络仿真环境的搭建 - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2013年10月02日 11:36:00[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：47
个人分类：[网络](https://blog.csdn.net/leixiaohua1020/article/category/8433201)












NS2（Network Simulator 2） 是一种针对网络技术的源代码公开的、免费的软件模拟平台，研究人员使用它可以很容易的进行网络技术的开发，而且发展到今天，它所包含的模块几乎涉及到了网络技术的所有方面。所以，NS成了目前学术界广泛使用的一种网络模拟软件。此外，NS也可作为一种辅助教学的工具，已被广泛应用在了网络技术的教学方面。因此，目前在学术界和教育界，有大量的人正在使用或试图使用NS。本文介绍Windows平台下NS2网络仿真环境的搭建（使用Cygwin）。

**（一）准备工作**

NS2（Network Simulator 2）必须在UNIX/Linux平台下运行，因此一般需要安装UNIX/Linux 操作系统。若要在Windows下使用NS可以采用Windows+虚拟机（VMware、Virtual PC）+NS组合的方式，或者是Windows+Cygwin（一个模拟的Linux平台）+NS组合的方式。目前Windows的使用较为普遍，因此本文主要介绍Windows+Cygwin（一个模拟的Linux平台）+NS的方式。

### 1.Cygwin

Cygwin是一个在Windows平台上运行的UNIX模拟环境，是Cygnus solutions公司开发的自由软件。它对于学习UNIX/Linux操作环境，从UNIX到Windows的应用程序移植，以及进行某些特殊的开发工作，尤其是使用gnu工具集在Windows上进行嵌入式系统开发，非常有用。随着嵌入式系统开发在国内日渐流行，越来越多的开发者对Cygwin产生了兴趣。

Cygwin当初首先把gcc，gdb，gas等开发工具进行了改进，使他们能够生成并解释Win32的目标文件。然后，他们要把这些工具移植到Windows平台上去。一种方案是基于Win32 API对这些工具的源代码进行大幅修改，这样做显然需要大量工作。因此，他们采取了一种不同的方法——他们写了一个共享库(就是Cygwin dll)，把Win32 API中没有的UNIX风格的调用（如fork,spawn,signals,select,sockets等）封装在里面，也就是说，他们基于Win32 API写了一个UNIX系统库的模拟层。这样，只要把这些工具的源代码和这个共享库连接到一起，就可以使用UNIX主机上的交叉编译器来生成可以在Windows平台上运行的工具集。以这些移植到Windows平台上的开发工具为基础，Cygnus又逐步把其他的工具（几乎不需要对源代码进行修改，只需要修改他们的配置脚本）软件移植到Windows上来。这样，在Windows平台上运行bash和开发工具、用户工具，感觉好像在Linux上工作。

目前国内的网站上有"在线安装"和"本地安装"两种。两者在本质上并没有多少不同，只是Cygwin必须安装特定的软件包。默认的软件包不够用。在线安装更够更新Cygwin的的软件包，可以减少安装过程中的一些不必要的错误。因此，本文采用在线安装，下面介绍一下安装Cygwin的步骤：

1.首先关闭杀毒软件，否则可能会误报病毒导致安装失败。

2.从[http://www.Cygwin.com/](http://www.cygwin.com/) 下载setup.exe在线安装包。

3.点击setup.exe开始安装，选择”install from Internet”从网络上进行安装，如图

![](https://img-blog.csdn.net/20131002111942625?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



4. 对软件包进行筛选，不需要的软件包点击“skip”，则会跳过该安装包的安装。如图所示

![](https://img-blog.csdn.net/20131002112051093?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



必要软件包是Cygwin用来生成并解释Win32的目标文件，缺一不可。安装过程也可以不点击skip，则默认将所有软件包都安装，如果是系统允许，可建议这么做，避免有些必要的安装包没有选择到，而出现错误。Cygwin必要的安装包如下：



```
gcc
gcc-g++
gnuplot
make
patch
perl
tar
X-startup-scripts
xorg-x11-base
xorg-x11-bin
xorg-x11-devel
xorg-x11-bin-dlls
xorg-x11-bin-lndir
xorg-x11-etc
xorg-x11-fenc
xorg-x11-fnts
xorg-x11-libs-data
```





软件包安装成功后，点下一步，则安装成功。

### 2.下载NS2源代码

### NS2存在很多版本，不同的版本存在不同的兼容性问题。例如：Ns2.28以后不再提供编译好的可执行文件，因此需要自己编译源码，时间比较长；Ns2.29,2.30等较早版本存在兼容性问题，安装后无法使用，需要打补丁或改源码，操作难度较大，因此要选择最新版本2.35，避免打补丁等操作。打开网址 http://www.isi.edu/下载ns-allinone-2.35 安装包。下载完后，解压到c:\Cygwin\home\Administrator\ns-allinone-2.35。路径要严格选择对，否则使用过程中会出现问题。



**（二）NS2编译安装**



打开Cygwin terminal，切换到解压的目录cd/home/Administrator/ns-allinone-2.35

输入./install安装。此安装过程很缓慢，因为是从源代码进行编译。

（注意：一定要关闭杀毒软件！我发现金山毒霸会误报病毒而导致编译失败）

安装完成后，用 ultraedit 打开 c:\Cygwin\home\Administrator\.bashrc配置环境变量。

在文件的尾部添加如下代码：



```
export
NS_HOME=/home/Administrator/ns-allinone-2.35
export
PATH=$NS_HOME/nam-1.15:$NS_HOME/tcl8.5.10/UNIX:$NS_HOME/tk8.5.10/UNIX:$NS_HOME/bin:$PATH
export
LD_LIBRARY_PATH=$NS_HOME/tcl8.5.10/UNIX:$NS_HOME/tk8.5.10/UNIX:$NS_HOME/otcl-1.14:$NS_HOME/lib:$LD_LIBRARY_PATH
export
TCL_LIBRARY=$NS_HOME/tcl8.5.10/library
```





使用UtraEdit而不要使用记事本，因为是UNIX文本，删除不必要的空格和回车；环境变量之间要用冒号分隔，而不是分号。

2、必须在Cygwin 下运行 source .bashrc使环境变量生效。

3、环境变量生效后，输入startxwin启动Cygwin的xwindow界面，只有xwindow界面才可以运行NS仿真，否则会出错。如图所示：

![](https://img-blog.csdn.net/20131002112631062?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

4、切换到需要使用的TCL文件目录。输入ns [file-name].tcl启动模拟。可以使用自带的example2.tcl进行测试，如果配置成功，会出现如图界面：

![](https://img-blog.csdn.net/20131002112816562?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)












