# Hadoop基于Linux-CentOS7安装-初识Linux - DEVELOPER - CSDN博客





2017年10月08日 11:47:27[学术袁](https://me.csdn.net/u012827205)阅读数：221
个人分类：[Hadoop](https://blog.csdn.net/u012827205/article/category/7211245)

所属专栏：[Hadoop-学习](https://blog.csdn.net/column/details/20399.html)









首先介绍一下，我呢？一周前基本上没有接触过Linux更不要说Hadoop。还记得14年实习的时候好奇装了一个Linux系统Ubantu，但是安装后不到三天我就炸了，怎么玩？？于是果断卸载！

但是，现在我想要去学习一下Hadoop，我大学室友在南京银行上班做的Hadoop，给他要了一些学习的视频。刚看不到一个小时，就遇到了一个问题——需要基础的Linux基础。OMG！！！肿么办呢？于是就硬着头皮去学唄。当然在下定决心去学的时候，我是不相信凭借现在网络上的资料，我还能安装不上Hadoop，配不了它的环境？后来遇到惹人“疼”的Liunx之后，我怂了。Linux操作我压根不懂。

然后就开始了我漫长的学习过程！先-Lunix-然后Hadoop。从Linux的背景知识到教学安装到指令学习，在Linux上简单的软件安装，最后尝试JDK安装、配置环境和Hadoop安装、环境配置。到今天终于配置好了，所以呢，就在这里做一个简单的记录描述。就从我的学习过程开始：

[Linux达人养成计划I](http://www.imooc.com/learn/175)

[简介：本课程以通俗易懂的语言、风趣幽默的实例、清晰严谨的逻辑介绍了Linux的基础内容。课程以CentOS操作系统为例，为你带来Linux的简介、系统安装和常用命令等内容。让您在轻松的氛围中感受到Linux之美。](http://www.imooc.com/learn/175)


[Linux达人养成计划II](http://www.imooc.com/learn/111)

[简介：本课程介绍Linux系统下操作VI编辑器、创建文本文件、VI的三种操作模式、磁盘分区与格式化、用户及用户组权限的相关操作与管理等，让童鞋们对Linux系统有进一步的理解，对Linux服务器的维护操作更加得心应手。](http://www.imooc.com/learn/111)



[Linux软件安装管理](http://www.imooc.com/learn/447)

## [简介：软件安装对于操作系统来说是常用的操作，那如何在Linux中使用命令进行软件安装呢？本课程就带你来了解Linux中的软件如何进行安装、卸载、升级等管理。课程内容包括rpm命令管理、yum在线管理、源码包管理和脚本安装包。](http://www.imooc.com/learn/447)



经历了这三个课程，就到网上找了一个安装教程，随着教程学着安装。但是中间也是遇到很多问题！这种感受，“过程异常痛苦”

介绍一下我的工具：VMware Workstation-8.0.4、hadoop-0.20.2.tar.gz、CentOS-7-x86_64-DVD-1708.iso、jdk-1.8

jdk的环境，我是直接使用的本机的jdk-1.8版本，

接下来，先了解一下Linux的安装，我直接截图我的安装过程，请欣赏：

![](https://img-blog.csdn.net/20171008175652872?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


![](https://img-blog.csdn.net/20171008175723046?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


![](https://img-blog.csdn.net/20171008175734902?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


![](https://img-blog.csdn.net/20171008175749602?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


![](https://img-blog.csdn.net/20171008175803697?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


哦！发现问题了，好像步骤标记串了~

![](https://img-blog.csdn.net/20171008175925747?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


![](https://img-blog.csdn.net/20171008175957584?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


![](https://img-blog.csdn.net/20171008180008373?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


![](https://img-blog.csdn.net/20171008180020720?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


![](https://img-blog.csdn.net/20171008180403134?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


![](https://img-blog.csdn.net/20171008180539741?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


![](https://img-blog.csdn.net/20171008180919919?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


![](https://img-blog.csdn.net/20171008182437005?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


![](https://img-blog.csdn.net/20171008182519540?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


![](https://img-blog.csdn.net/20171008182532891?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


![](https://img-blog.csdn.net/20171008182548321?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


![](https://img-blog.csdn.net/20171008182558986?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


![](https://img-blog.csdn.net/20171008182614473?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


这一步的设置是灰常重要的，不哟啊忘记哦~

![](https://img-blog.csdn.net/20171008184818605?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


![](https://img-blog.csdn.net/20171008182832543?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


![](https://img-blog.csdn.net/20171008182719423?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


![](https://img-blog.csdn.net/20171008182737633?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


![](https://img-blog.csdn.net/20171008182639186?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


![](https://img-blog.csdn.net/20171008183151513?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


好了！系统的安装就这么多了。然后，提醒一下值得注意的地方。比如我的问题，现在我也灭有搞懂。

我在VMware8.0.2上先安装了一个系统A，通过指令，ifconfig -a 能查看到安装的Linux的IP地址；但是我如果再安装一个系统B，使用通过的指令但是却看不到了。所以我提醒一下，在安装系统的时候，IP地址和主机名要记下哦~，看图：

看得到的IP

![](https://img-blog.csdn.net/20171008185227650?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


看不到的IP

![](https://img-blog.csdn.net/20171008190909671?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


八嘎！我只是想要看看IP，这是什么玩意儿？ether 00:0c:29:cc:13:56  txqueuelen 1000 (Ethernet)

那。。。。那。。。到底是为什么出现了这种情况呢？其实经过我百战的实践经验，已经得出了结果。看图

![](https://img-blog.csdn.net/20171008190510895?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)







