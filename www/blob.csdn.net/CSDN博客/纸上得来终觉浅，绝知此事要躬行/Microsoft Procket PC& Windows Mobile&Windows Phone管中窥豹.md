# Microsoft Procket PC& Windows Mobile&Windows Phone管中窥豹 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2014年07月22日 15:06:21[boonya](https://me.csdn.net/boonya)阅读数：1108标签：[windows mobile																[windows phone																[windows ce](https://so.csdn.net/so/search/s.do?q=windows ce&t=blog)
个人分类：[C#](https://blog.csdn.net/boonya/article/category/1776321)





原文标题：[Mircosoft
 正式把Windows Mobile改名为Windows Phone，你会因此而购买Windows Phone吗？](http://www.cnblogs.com/procoder/archive/2009/10/08/Windows-Phone.html)

文章源于原创作者对WIndows Mobile 更名为Windows Phone的一些感触，因为我没有接触过Procket PC和Windows Phone，所以借此机会管中窥豹，感谢作者的分析和分享。




# 事件

今天下班的时候看报纸，有一段新闻关于昨天(2009年10月6日)Mircosoft正式使用Windows Phone这个名字。我去到原先Windows Mobile的主页，已经全部由Windows phone代替了，如下图。

![wp1](http://images.cnblogs.com/cnblogs_com/procoder/WindowsLiveWriter/MSWindowsMobileWindowsPhone_12C56/wp1_thumb.png)

Windows Phone(技术名字叫做Windows Mobile 6.5)其实就是Windows Mobile 6.5新版本开始在设备上发售。但是改个名字那么重要吗？从功能上说Windows Mobile 6.5和Windows Mobile 6.1以及6.0没有革命性的改变，还是使用Windows CE 5.0的内核，那么为什么微软要把名字改掉了，我认为体现了微软对Windows
 Mobile的上策略上是改变。也体现了微软下一步的发展方向。

# 史前Windows Phone (Pocket PC)

Windows Mobile最开始的时候名字叫做Pocket PC，顾名思义，微软开始的想做的就是装在口袋里面的PC，也就是desktop桌面系统。把一个Windows的desktop系统放到口袋里面了，目标宏大，但是我现在认为策略是相对失败的，导致最直接的后果是Windows Mobile功能强大但是易用性很差。由于硬件的限制性，在Desktop上的法宝，在Windows Mobile变成了负累，例如开始菜单，开始菜单是Windows
 Desktop系统的法宝，以至于后来Linux上的GNOME和KDE都模仿Windows的开始菜单，但是在Windows Mobile开始菜单由于屏幕的限制性，只能放7项开始菜单，而且不能放二级菜单，导航性荡然无存，增加了操作的难度。由于做口袋PC的策略，微软直接把Desktop系统上的功能不加以改造就直接拿来到Windows Mobile用，例如GPS功能，尽管在Windows Mobile5以上有GPS Intermediate Driver。但是概念还是Desktop的，Windows Mobile的用户需要使用GPS需要配置串口(Com
 Port)，Com Port的好处是可以屏蔽硬件的差异性，支持Built-in或者蓝牙GPS设备，关于GPS和Com Port的开发可以看我以前写的文章。但是最关键一点，一个手机用户我真的不关心什么Com Port，也不想关心，我要的是我想用GPS，你就帮我立刻启动导航好了，That’s it.我认为这一切一切细节都是由于公司策略决定的，就是Pocket PC这个名字以及策略导致了今天Windows Mobile的发展。

当然Pocket PC的策略也不是一无所处的。最大的好处是Windows Mobile功能强大，定制性强，例如当下很多山寨手机就是基于Windows Mobile的基础上定制出不同的Today Shell来模仿不同的手机，也就是说可以使用Windows Mobile来定制出类iPhone操作界面，类Blackberry和类Symbian操作界面。为什么山寨手机偏爱Windows Mobile呢？首先，Windows
 Mobile功能强大，一切智能手机标配的功能都带上了，Bluetooth,GPS,3G,WIFI,Sensors等等。根据需求裁剪就可以，不需要自己从设备驱动开发。第二，Windows Mobile的定制性强。第三，强大的开发工具支持，Microsoft Desktop的开发者能够使用原先的开发经验来进行Windows Mobile的开发。

# 内忧外患的Windows Mobile

Windows Mobile的发展有得有失，但是Windows Mobile的发展还是内忧外患，微软的CEO史提芬公开说微软把Windows Mobile给搞砸了。可以参考 [Ballmer:
 We 'screwed up with Windows Mobile](http://news.idg.no/cw/art.cfm?id=F2F7C35E-1A64-67EA-E4BC04F120F0B898).这里解释一下，免得变成标题党，这里搞砸的意思是在今天(2009年10月)，微软应该在把Windows Mobile改名字为Windows Phone的时候，也就是展开Windows Phone策略的时候，推出的产品是Windows Mobile7而不是Windows Mobile6.5。我认为Windows Mobile6.5还是不能代表Windows Phone策略的，我还是期待Windows Mobile7。

内忧不少，外患更多，在前几天(2009年9月30日)的一项关于智能手机的调查中，Windows Mobile是众多智能手机中得分最低的一部。参考[Survey:
 Windows Mobile users 'have no idea' they use it](http://blog.seattlepi.com/microsoft/archives/180639.asp). 以100分为满分算，Windows Mobile只是得了66分而其他iPhone(83), Android(77), Palm Pre (77), BlackBerry(73), the Palm Treo (70), 被归为其他类的Symbian和Windows Mobile一样66分。

# 改名Windows Phone意味着什么

改名最大的改变是策略上的改变，从做口袋PC到做真正的电话(Phone)。在Windows Mobile6.5有三大新功能，第一是Windows Mobile Market，模仿Apple Apps Store。第二是My Phone，基于云计算的数据同步。第三是新的Today，同时把开始菜单改成触摸型(finger friendly)。我认为这些都是开始了，我觉得Windows Phone如果想开拓市场，需要从易用性入手，把他做成一个真真正正的电话,简单易用的电话。我想今时今日，在消费市场上（区别于商用市场）使用Windows
 Phone的人很多是喜欢用特定厂商定制的Today，例如HTC的Today，而不是用Windows Mobile操作系统。在上面所说的调查中表示，很多用户根本不知道自己在使用Windows Mobile的操作系统。由于像HTC等厂商的定制简化了使用难度，人们才喜欢使用。这些功能本来应该有微软自己做的，这些是基本功能，操作困难的电话没人愿意买，而且价格也不便宜。我认为Windows Phone未来的发展方向是保留强大功能，同时定制不同的操作界面，不同用户可以使用不同的操作Shell。有简单易用的，有复杂而功能强大的等等多种选择。

# 从开发者的角度对Windows Phone的期望

从开发者的角度，我对微软变策略还是抱有不少期望的。

1.简化UI开发，现在大家都称为眼球经济，界面都需要花哨，当前做Windows Mobile开发都是使用GDI，Alpha belnding, Immaging那些API，还是从API层做起的，我们想开发一个可以换肤的界面实在太麻烦了。

2.增强.NET Compact Framework，CF.net一直认为是完整版.NET的一个子集，但是既然是Phone，那不能仅仅是子集，例如WiFi功能，可能Desktop没有而Phone基本是必须的，但是CF.net本身还没有WiFi功能。同理Bluetooth,GPS,Sensors那些一样，要为Phone特别做。

作者：[Jake
 Lin](http://procoder.cnblogs.com/)（[Jake's
 Blog on 博客园](http://www.cnblogs.com/procoder)） 

出处：[http://procoder.cnblogs.com](http://procoder.cnblogs.com/)](https://so.csdn.net/so/search/s.do?q=windows phone&t=blog)](https://so.csdn.net/so/search/s.do?q=windows mobile&t=blog)




