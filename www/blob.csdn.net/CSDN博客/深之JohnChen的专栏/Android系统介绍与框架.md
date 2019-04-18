# Android系统介绍与框架 - 深之JohnChen的专栏 - CSDN博客

2013年07月24日 23:30:24[byxdaz](https://me.csdn.net/byxdaz)阅读数：52918
个人分类：[Android](https://blog.csdn.net/byxdaz/article/category/715665)



## 一、Andriod是什么？

Android系统是Google开发的一款开源移动OS，Android中文名被国内用户俗称“安卓”。Android操作系统基于Linux内核设计，使用了Google公司自己开发的Dalvik
 Java虚拟机。Android操作系统已经成为全球最大的智能手机操作系统。

1）、开放性

Android完全开源，且该平台从底层操作系统到上层的用户界面和应用程序都不存在任何阻碍产业创新的专有权障碍。同时开源的最大好处是，使得Android平台会拥有越来越壮大的开发者队伍，并且随着用户与应用的日益丰富，必然会使得Android这个崭新的平台走向成熟与稳定。

2）、厂商支持

Android阵营

HTC、三星、摩托罗拉、LG、索爱、华硕、宏碁、华为、中兴、夏普、联想、魅族等数百家巨头推出了基于Android操作系统的智能手机或平板电脑。

Symbian阵营

目前仅有诺基亚一家支持，主要有N系列，E系列和X系列的手机。

iOS阵营

仅有苹果公司一家支持，主要有iPhone和iPad

WebOS阵营

仅有RIM黑莓公司一家支持

类似Android阵营的还有Windows Phone 7，但由于WP7并不是开源系统很多厂商没有机会参与，仅有几个手机制造商支持，同时没有相对应的平板电脑操作系统，Android123作为多年资深移动开发者，通过SDK对比，Windows
 Phone 7 和 Android有很大的差距，从目前微软的战略和封闭的API，可以说很难和Android抗衡。

3）、Dalvik虚拟机

Dalvik虚拟机是Google开发的一款Java
 VM，相对于Sun VM来说对文件作了优化，比如将多个class文件合并为一个dex文件，Android应用程序主要由Java语言开发，但Google提供了系统级别的Java
 API，这和非智能手机支持的Sun J2ME Java VM有很大的不同，我们可以看到Android软件可以支持来电防火墙显示归属地这样的底层应用，这些是Windows
 Phone 7 和 iPhone所不具备的。

4）、多元化

目前Android系统除了应用在智能手机外，还有平板电脑以及智能电视，目前摩托罗拉、三星、LG、HTC、宏碁、华硕等公司均推出了平板电脑，同时国内的创维、TCL等厂商已经推出了Android智能电视，最终将会有更多的智能家电、机顶盒、车载电子设备的出现。

5）、应用程序间的无界限

Android打破了应用程序间的界限，开发人员可以将在自己开发的程序与本地的联系人、日历、位置信息等很好的联系起来。此外，应用程序可以自申明其功能可以被其他应用程序所使用。

6）、紧密结合Google应用

全球最大的在线搜索服务商Google在过去的10年中，已经逐渐的渗透进人们的日常生活中了。人们不再仅仅满足于使用电脑终端来享受诸如Gamil、谷歌地图、在线翻译等在线服务，PC到移动终端的延伸成了一种必然的趋势。而Android与Google服务的无缝集成，则可以十足的满足人们的愿望。

![](https://img-blog.csdn.net/20130724232620828?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYnl4ZGF6/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

## 二、Android框架

![](https://img-blog.csdn.net/20130724232645984?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYnl4ZGF6/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

从上图中可以看出，Android系统架构为四层结构，从上层到下层分别是应用程序层、应用程序框架层、系统运行库层以及Linux内核层，分别介绍如下：

1）**应用程序层**

Android平台不仅仅是操作系统，也包含了许多应用程序，诸如SMS短信客户端程序、电话拨号程序、图片浏览器、Web浏览器等应用程序。这些应用程序都是 用Java语言编写的，并且这些应用程序都是可以被开发人员开发的其他应用程序所替换，这点不同于其他手机操作系统固化在系统内部的系统软件，更加灵活和个
 性化。

2）**应用程序框架层**

应用程序框架层是我们从事Android开发的基础，很多核心应用程序也是通过这一层来实现其核心功能的，该层简化了组件的重用，开发人员可以直接使用其提供的组件来进行快速的应用程序开发，也可以通过继承而实现个性化的拓展。

a）Activity Manager（活动管理器）

管理各个应用程序生命周期以及通常的导航回退功能

b）Window Manager（窗口管理器）

管理所有的窗口程序

c）Content Provider（内容提供器）

使得不同应用程序之间存取或者分享数据

d）View System（视图系统）

构建应用程序的基本组件

e）NotificationManager(通告管理器)

使得应用程序可以在状态栏中显示自定义的提示信息

f）Package Manager（包管理器）

Android系统内的程序管理

g）TelephonyManager(电话管理器)

管理所有的移动设备功能

h）Resource Manager（资源管理器）

提供应用程序使用的各种非代码资源，如本地化字符串、图片、布局文件、颜色文件等

i）LocationManager(位置管理器)

提供位置服务

j）XMPP Service（XMPP服务）

提供GoogleTalk服务

3）**系统运行库层**

从图中可以看出，系统运行库层可以分成两部分，分别是系统库和Android运行时，分别介绍如下：

a）系统库

系统库是应用程序框架的支撑，是连接应用程序框架层与Linux内核层的重要纽带。其主要分为如下几个：

Surface Manager：

执行多个应用程序时候，负责管理显示与存取操作间的互动，另外也负责2D绘图与3D绘图进行显示合成。

Media Framework：

多媒体库，基于PacketVideo OpenCore;支持多种常用的音频、视频格式录制和回放，编码格式包括MPEG4、MP3、H.264、AAC、ARM。

SQLite:

小型的关系型数据库引擎

OpenGL|ES：

根据OpenGLES 1.0API标准实现的3D绘图函数库

FreeType：

提供点阵字与向量字的描绘与显示

WebKit：

一套网页浏览器的软件引擎

SGL：

底层的2D图形渲染引擎

SSL：

在Andorid上通信过程中实现握手

Libc：

从BSD继承来的标准C系统函数库，专门为基于embedded linux的设备定制

b）Android运行时

Android应用程序时采用Java语言编写，程序在Android运行时中执行，其运行时分为核心库和Dalvik虚拟机两部分。

核心库

核心库提供了Java语言API中的大多数功能，同时也包含了Android的一些核心API,如android.os、android.net、android.media等等。

Dalvik虚拟机

Android程序不同于J2me程序，每个Android应用程序都有一个专有的进程，并且不是多个程序运行在一个虚拟机中，而是每个Android程序都有一个Dalivik虚拟机的实例，并在该实例中执行。Dalvik虚拟机是一种基于寄存器的Java虚拟机，而不是传统的基于栈的虚拟机，并进行了内存资源使用的优化
 以及支持多个虚拟机的特点。需要注意的是，不同于J2me,Android程序在虚拟机中执行的并非编译后的字节码，而是通过转换工具dx将Java字节码转成dex格 式的中间码。

4）**Linux内核层**

Android是基于Linux2.6内核，其核心系统服务如安全性、内存管理、进程管理、网路协议以及驱动模型都依赖于Linux内核。

## 5）特别的Apps

在图中有些基于Dalvik虚拟机的Apps看起来像是Android的一部分，其实是由Google提供，这些应用包括Dialer、Contact、Calendar、Gmail和Chat等。它们中的绝大部分是开源并可复用的。只有少部分例外，比如Google Maps和Android Market.

