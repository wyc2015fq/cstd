# （多核DSP快速入门）0.前言+CCS的安装 - tostq的专栏 - CSDN博客





置顶2016年04月25日 22:40:10[tostq](https://me.csdn.net/tostq)阅读数：9723
所属专栏：[多核DSP快速入门](https://blog.csdn.net/column/details/easymulticoredsp.html)









**原创文章**

转载请注册来源[http://blog.csdn.net/tostq](http://blog.csdn.net/tostq)



**前言**
       最近老师给布置了个作业，让我用多核DSP做一个基于SIFT的RANSAC图像配准，SIFT+RANSAC网上都有许多代码，并不算难，但是多核DSP对于我来说，完全是一片空白，不光单核DSP没接触过，就连CCS是没用过。网上关于DSP的教程虽然很多，但绝大多数都是关于单核的，而多核DSP的教程虽然说也有，但是要么太复杂太专业，如TI的官网资料，要么就是说的不清楚，而且又没有什么完整体系，特别是我买的几本国内教程，完全都是从官网上下个代码，翻译下，甚至有些东西还没人家官网上讲得清楚，更粗暴的是直接从官方资料上截个流程图，然后贴个代码就完事了，想想也是醉了~
       本人出这个教程的目的不是针对于大牛，而是针对于对于DSP和CCS完全不通的小白，就像我一样（以前没怎么弄过硬件），所以自行设计了大概一个月的自学路线，这个系列就是记录个人自学路线，另一方面就是同大家分享经验，共同学习多核DSP~_~，另外文章中涉及到的例程也都放到了我的GitHub上去了：[https://github.com/tostq/EasyMulticoreDSP](https://github.com/tostq/EasyMulticoreDSP)
 欢迎大家逛逛！！
       有人问为什么是软件仿真呢？哈哈，那是因为我现在还没拿到多核DSP的板子=_=||，相信很多导师没钱的同学和我一样很囧Orz
       首先说下我的开发环境：Windows 64位 + CCS 5.2 + MCSDK 2.1
       CCS 5.2

       CCS是DSP的开发平台，也可以有用于DSP+ARM的片上嵌入式平台和达芬奇平台的软件开发，里面提供了两种仿真模式，可以用连接硬件仿真或直接通过软件仿真，本教程主要是软件仿真，所以没有任何硬件也可以测试自己的程序。这里CCS比较经典的版本是3.3，很多同学可能用过这个，当然没用于过也没关系（我也没用过，而且5.2相较于3.3有很大变化），我选择CCS 5.2的原因是因为5.2支持多核DSP开发，另外CCS 5.2完美支持Windows
 64位。




**文章目录：**

[0.前言+CCS的安装](http://blog.csdn.net/tostq/article/details/51245979)

[1.创建简单的多核DSP项目HelloWorld](http://blog.csdn.net/tostq/article/details/51246473)

[2.类OpenCV图像视觉库EMCV的编译与使用](http://blog.csdn.net/tostq/article/details/51246746)

[3.利用EMCV进行图像灰度转换的单核DSP程序](http://blog.csdn.net/tostq/article/details/51246792)

[4.编译测试多核DSP实例Vlfft](http://blog.csdn.net/tostq/article/details/51258667)

[5.SYS/BIOS的使用实例分析](http://blog.csdn.net/tostq/article/details/51264930)

[6.IPC的初级使用+简单实例分析](http://blog.csdn.net/tostq/article/details/51265147)

[7.利用SharedRegion的核间内存共享](http://blog.csdn.net/tostq/article/details/51334961)





... ...

（未完待续）

**CCS 5.2的安装**
       这里首先需要注意下，安装路径不能有中文，另外我的CCS 5.2可以在Windows 64位下完美安装动行，不过你的版本如果不能在当前系统下安装，可以右击当前安装文件，选择属性，更改兼容性设置，选择以兼容模式运行这个程序。

![](https://img-blog.csdn.net/20160425223318063?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
1、双击安装文件ccs_setup_5.x.x.xxxxx.exe

2、同意许可

![](https://img-blog.csdn.net/20160425223351751?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
3、安装路径默认为c:\ti，我选择的是D:\ti，最好让父文件夹名称为ti

![](https://img-blog.csdn.net/20160425223409039?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
4、选择安装类型，这里的Custom是指只安装下面我选择的组件，而Complete Feature Set是完全所有组件，这里选择Custom较好

![](https://img-blog.csdn.net/20160425223434533?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
5、选择支持的处理器，我们这里这需要用于C6000 Multi Core DSPs 和C6000 Single Core DSPs两种系统的DSP，其他的可以根据需要自选。（这个选项只有Custom才有较哦）

![](https://img-blog.csdn.net/20160425223454439?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
6、选择支持的软件开发组件，C6000 Compiler Tools是必须的。（这个选项也只有Custom才有较哦）

![](https://img-blog.csdn.net/20160425223512149?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
7、选择仿真器，这里的JTAG仿真都是硬件方面的仿真器，你也可以考虑到未来开发，选择几个

![](https://img-blog.csdn.net/20160425223529133?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
8、进入安装（这里的图是我为了方便说明整个安装过程而从网上下的图，你的安装没有必要跟它一样，我的已经安装好了，跟这个并不一样）

![](https://img-blog.csdn.net/20160425223550868?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
9、安装完成

![](https://img-blog.csdn.net/20160425223607055?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
10、DSP安装完成后，ti文件夹下目录如下，这里的组件都很重要，特别是IPC、BIOS、XDCtools等，如果少了，看看是安装中哪个步骤出了问题。

![](https://img-blog.csdn.net/20160425223626051?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
11、关于ControlSUITE，TI推荐我安装这个组件，里面集成许多开发例程、网络资源链接、培训 、应用笔记 、Datasheet 芯片数据等很多学习资源，功能非常强大，但是我目前没怎么用过，建议大家安装吧

12、关于破解文件，我把我的破解文件共享到了我的GitHub上了，应该支持5.1和5.2，里面还有一个别人的CCS安装使用的简单教程。另外TI官网上也有一个安装步骤：[http://processors.wiki.ti.com/index.php/GSG:CCSv5_installation](http://processors.wiki.ti.com/index.php/GSG:CCSv5_installation)
 。大家也可以参考一下。

       破解文件下载地址：[https://github.com/tostq/EasyMulticoreDSP/tree/master/0.craker](https://github.com/tostq/EasyMulticoreDSP/tree/master/0.craker)

       转载来源：[http://blog.csdn.net/tostq](http://blog.csdn.net/tostq)



