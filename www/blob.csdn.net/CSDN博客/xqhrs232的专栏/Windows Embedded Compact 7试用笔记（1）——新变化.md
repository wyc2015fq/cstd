# Windows Embedded Compact 7试用笔记（1）——新变化 - xqhrs232的专栏 - CSDN博客
2010年06月30日 09:53:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：762标签：[windows																[wince																[silverlight																[程序开发																[android																[windows phone](https://so.csdn.net/so/search/s.do?q=windows phone&t=blog)](https://so.csdn.net/so/search/s.do?q=android&t=blog)](https://so.csdn.net/so/search/s.do?q=程序开发&t=blog)](https://so.csdn.net/so/search/s.do?q=silverlight&t=blog)](https://so.csdn.net/so/search/s.do?q=wince&t=blog)](https://so.csdn.net/so/search/s.do?q=windows&t=blog)
个人分类：[WINCE](https://blog.csdn.net/xqhrs232/article/category/906924)
原文地址::[http://www.cnblogs.com/yashi88/archive/2010/06/23/1763029.html](http://www.cnblogs.com/yashi88/archive/2010/06/23/1763029.html)
其实在几个月前就开始试用Windows Embedded Compact 7的社区预览版了，但当时因为NDA的关系，不能公开相关信息。在与微软开发人员的交流中，能明显感觉到他们承受的极大压力，甚至有MVP直接质问：WinCE 是不是已经走向死亡了？国内做WinCE/Mobile开发的人，应该对此也有很深切的感受，Android不仅仅是Windows mobile/windows phone的强大对手，同时也是WinCE的强大对手。我知道今年以来，国内很多公司陆续放弃了WinCE/mobile平台，转向Android，其中最知名的就是魅族了；很多朋友也不得不转变技术方向，去学习Android的开发；连我所在的公司，也大大削减了WinCE开发人员数量，而大规模招聘Andriod开发人员。在这样的形式下，将于年底推出的Windows
 CE新的版本--改名后的Windows Embedded Compact 7到底表现如何，牵动着很多开发人员的心。
    说起我这几个月的使用感受，那真是百味杂陈。总的说，WinEC7确实做了很大的改进，但这些改进是否能让人耳目一新，是否能让它打败竞争对手，那还真的很难说。
    好了，闲话少说，下面我们就看看，到底新的版本有了些什么变化：
    1.系统开发和应用程序开发都需要在Visual studio 2008下进行，PB7做为其下面的一个插件。这个应该在预料之中，但是有一条，可能很多人都接受不了：VS2010既不能用于WinEC7的系统开发，也不能用于应用程序开发。据说之所以这样，是VS的插件结构，每个版本都会做很大的改动，而因为Windows Phone开发优先调用了相关人力资源，所以WinEC7团队找不到人来做这件事，所以不得不做出妥协。
    2.Silverlight功能的加强，在WinCE6 r3中，微软第一次把Silverlight引入到嵌入式平台，在新版本中，该功能得到了很大的加强，不过依然是只支持C/C++的native code开发，不能使用manage code
    3.无线网络的支持得到很大的加强，WIFI，蓝牙，CELLCORE部分的支持都比以前的版本强大很多，估计用它做手机系统，应该会比原来容易很多。
    4.MFC应该是彻底抛弃了，和WinCE6一样的是，在CATALOG里面你找不到MFC；和WinCE6不一样的是，在OTHER目录下也没有了MFC的踪影。
    5.Device Emulator没有了，Vitual PC取代了这部分功能。
    6.新的IE7增强了网络浏览的体验。
    7.支持的CPU类型ARMV5,ARMV6,ARMV7,MIPSII,X86，原来的SH4，ARMV4I已经不支持了。
    8.很多驱动都进行了改写，同时新增了很多新的驱动。
    9.支持多点触摸，支持手势输入。
    以上，是WinEC7的一些主要变化，接下来，我们一起来动手体验一下实际的开发过程，首先是定制一个系统。
