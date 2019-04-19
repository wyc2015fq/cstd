# WinCE5.0开发环境的建立 - xqhrs232的专栏 - CSDN博客
2014年02月21日 22:58:02[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：876
原文地址::[http://blog.csdn.net/hejianbang/article/details/2043744](http://blog.csdn.net/hejianbang/article/details/2043744)
# WinCE5.0开发环境的建立
本文主要介绍WinCE5.0开发环境的建立过程及注意事项。
目前WinCE5.0的开发工具主要有以下几种:Platform Builder5.0、EVC4.0+SP4、Visual
 Studio2005。其中Platform Builder主要用于定制WinCE操作系统，它也可以用来编写驱动程序和应用程序。EVC顾名思义就是嵌入式版的VC，它主要用来开发应用程序，当然也可用来编写驱动程序。Visual
 Studio2005是微软的集大成开发工具。它能用来定制WinCE6.0的操作系统，也可以用来开发应用和驱动程序。
##      Platform Builder5.0的安装过程及注意事项
1．Platform Builder5.0的安装程序是基于.NET Framework 1.1的，所以在安装Platform Builder之前需要安装.NET
 Framework 1.1，否则安装程序不能运行。
2．Platform Builder5.0的安装文件有2.55G左右，我推荐将所有安装包拷贝到硬盘的某一目录下面再进行安装。一则比用光盘直接安装速度更快，二则用光盘安装时会出现让你再次插入光盘的问题，哪怕你用的是DVD光盘，并且所有的安装包都在同一目录下。
3．准备工作做完以后便可以开始正式安装Platform Builder了。
在这里补充一点，Platform Builder对开发主机的配置要求较高。内存至少512MB，当然多多益善，硬盘总的剩余空间至少10G，其中C盘的剩余空间至少6G。因为Platform
 Builder的安装过程中的配置最好都默认，即Platform Builder最好安装在C盘。实在没有办法的情况下安装在D盘或别的盘里面也可以使用(经测试基本上也没问题)。但如果以后使用过程中出现问题就麻烦了。
另外，不推荐在Windows Vista上安装Platform Builder。曾经有人装过，结果安装快结束的时候又Roll Back了，再试一次，依然如此。半天时间就那么浪费了。即使能够安装成功，我想在编译的时候也够费劲的。一般推荐装纯净版的Windows
 XP Professional，然后再安装PB等软件，并且在安装PB时关掉所有的安全软件，尤其是杀毒软件的实时监控。因为在安装PB的过程中会创建很多文件，开着监控会导致安装速度变慢。
4．跟一般的安装程序一样，运行setup.exe程序，开始安装，选择Install。
![](https://p-blog.csdn.net/images/p_blog_csdn_net/hejianbang/image001.png)
一路默认，一直到下图所示对话框。此处需要选择自己所用的CPU的类型。一般选择如下两图所示。
![](https://p-blog.csdn.net/images/p_blog_csdn_net/hejianbang/image003.png)
![](https://p-blog.csdn.net/images/p_blog_csdn_net/hejianbang/image005.png)
至此一路Next，直至安装成功。
我们知道微软的软件都是要打补丁的，PB也不例外。PB的补丁可以从官网([http://msdn2.microsoft.com/zh-cn/embedded/aa731256(en-us).aspx](http://msdn2.microsoft.com/zh-cn/embedded/aa731256(en-us).aspx))上下载。个人经验，补丁不求最新，能用就行。因为曾经有一次打了最新补丁后，编译原有工程竟然出错，后来恢复回去才行。目前一般推荐安装06年的全年补丁即可。补丁的安装比较简单，不再赘述。
这样Platform Builder集成开发环境安装完成，有关其使用的方法将在后续文档再进行说明。
## EVC4.0的安装过程及注意事项
1．EVC的安装最省事的方法就是完全安装所有组件。
2．如果硬盘空间不足，那么可以在下图所示的选项中，只选择自己所需的组件。
![](https://p-blog.csdn.net/images/p_blog_csdn_net/hejianbang/image007.png)
![](https://p-blog.csdn.net/images/p_blog_csdn_net/hejianbang/image009.png)
在安装完EVC的IDE后，会自动安装SDK。硬盘空间不足时，也可以只安装自己所需的组件。如下图所示。
![](https://p-blog.csdn.net/images/p_blog_csdn_net/hejianbang/image011.png)
![](https://p-blog.csdn.net/images/p_blog_csdn_net/hejianbang/image013.png)
一路Next，直至安装完成。
最后安装EVC SP4的升级包，安装过程比较简单，也不再赘述。
3．EVC安装完成以后可能会遇到一些问题。如模拟器启动不起来。每次开机时出现如下图所示的提示对话框。
![](https://p-blog.csdn.net/images/p_blog_csdn_net/hejianbang/image015.png)
   并且在设备管理器中出现如下图所示情况
![](https://p-blog.csdn.net/images/p_blog_csdn_net/hejianbang/image017.png)
出现这种情况的主要原因就是Virtual PC/Windows CE Emulator与物理地址扩展(PAE)有冲突。PAE主要是使系统能使用4GB以上的内存，一般来说基本上没有用，所以直接禁用PAE即可。确认PAE是否工作的方式如下图所示，如果显示“物理地址扩展”，就说明PAE启动了。
![](https://p-blog.csdn.net/images/p_blog_csdn_net/hejianbang/image019.png)
禁用PAE的方法即修改boot.ini文件中的内容(注意红色部分)
原来Boot.ini文件的内容
[boot loader]
timeout=2
default=multi(0)disk(0)rdisk(0)partition(1)/WINDOWS
[operating systems]
multi(0)disk(0)rdisk(0)partition(1)/WINDOWS="Microsoft Windows XP Professional" /noexecute=optin /fastdetect
修改后的Boot.ini文件的内容
[boot loader]
timeout=2
default=multi(0)disk(0)rdisk(0)partition(1)/WINDOWS
[operating systems]
multi(0)disk(0)rdisk(0)partition(1)/WINDOWS="Microsoft Windows XP Professional" /execute=optin /fastdetect
修改完成以后，保存，并重启机器就不会出现异常了，模拟器也能正常启动。至此，有关EVC的安装过程及注意事项介绍完毕。
## VS2005的安装及注意事项
1．VS2005的安装最省事的方法就是完全安装。但这要求有足够的硬盘空间。如果硬盘空间有限，那就选择自己所需的组件即可。安装过程简单但很慢，就不截图示例了。
2．在使用VS2005的时候，经常会碰到无法使用WinCE模拟器的问题。关于这一点推荐到微软官网上寻找答案。[http://www.microsoft.com/downloads/details.aspx?FamilyID=474B03A6-E87D-455D-BC16-B8CF18EF39B4&displaylang=en](http://www.microsoft.com/downloads/details.aspx?FamilyID=474B03A6-E87D-455D-BC16-B8CF18EF39B4&displaylang=en))
简单说明一下，VS2005的模拟器与EVC中的模拟器不一样，EVC的模拟器是基于X86的，而VS2005的WinCE模拟器是模拟了一个ARMV4I的平台。所以需要从微软的网站上下载专门的BSP定制操作系统，然后导出其SDK。具体过程有很多注意事项，请参考微软的相关文档。
WinCE的开发环境至此基本搭建完成，主要包括Platform Builder、EVC(SP4)和VS2005。在实际做开发的过程中，还经常用到同步工具MICROSOFT
 ACTIVESYNC 4.5,很多网站都提供下载，安装也很简单，不再赘述。
