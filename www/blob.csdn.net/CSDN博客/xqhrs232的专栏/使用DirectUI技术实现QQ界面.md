# 使用DirectUI技术实现QQ界面 - xqhrs232的专栏 - CSDN博客
2012年08月22日 21:50:21[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1535
原文地址::[http://blog.csdn.net/linsky04/article/details/5799968](http://blog.csdn.net/linsky04/article/details/5799968)
**一.介绍**
DirectUI技术说白了就是XML配置文件+图片+JavaScript控制界面。这点与网页css+图片+JavaScript十分相似，就是如开发网页一般开发桌面程序界面，这个开发效率当然大大的提高。将程序员从繁琐的界面绘制工作中解脱出来，专心开发逻辑代码。还能大大减少代码量，因为据统计，传统MFC程序中，界面代码大约占总代码的1/3强！也就是说，这个1/3的代码都可以由xml+javascript替代！
同理，如果网页开发时，每个图片都需要由web程序绘制，你能想象一个网页的代码量有多少吗？
简直不能想象，而传统Window程序就是如此！
DirectUI技术最早被用于window xp资源管理器左边栏，被称为task folder.而DirectUI被广为国人所知，还得感谢腾讯公司。QQ2009的界面就是使用DirectUI技术开发的。类似的有微软的MSN,OCS和百度Hi。最近的Office2007 Ribbon界面，如果使用Spy++查看，可以见到一个名为"NetUI"的窗口，其实这也是微软内部DirectUI的一个变种。不过无论是微软，腾讯，还是百度，都使用了DirectUI技术来开发自己的软件界面，却不肯将其公开。本文将介绍如何使用DirectUI技术开发一个类似QQ的界面演示程序。
**二.背景**
与DirectUI最相似的要数微软最近推出的WPF，其设计思想是相同的。只不过WPF只能运行于托管环境下。window下的C++程序员一直呼吁微软推出native WPF，也就是非托管的WPF。不过微软给出的答案是：NO。所以WPF将定位于为.NET战略服务。使用WPF开发的界面程序，必须带上庞大的.NET运行环境。不过随着Window 7的普及，这个状况会有所改变（window 7继承了.NET环境）。
不过，我想，如果许多公司的产品都是需要支持Window XP（最少QQ现在还是支持Window 2000），所以WPF不是桌面应用程序界面开发的首选。
**三.实现**
废话不多说，先看演示程序的图片，足够以假乱真吧？！
在xml文件中也引入了网页中样式(style)的概念，style控制着网页中一个元素的外观。同理，directui xml文件中的样式(style)控制着一个控件的外观。同样的一个button，使用了不同的style，外观也不一样。如下面的代码及图片所示。
参照网页开发中的JavaScript，DirectUI JavaScript可以控制控件的状态、文本、显示/隐藏、位置等等属性。DirectUI JavaScript是基于事件，比如在button1点击事件中，改变button2的文本。或者button1点击事件之后，button1就要禁用。像这样的需求，在界面开发中是比较常见的。
在c++代码中，剩余的只是界面与程序逻辑的借口。比如，QQ2009中从服务器接收添加用户的通知，然后操作界面将此用户显示出来。如此看来，c++代码中完全没有了绘制界面的代码。
**四.后话**
DirectUI是个好技术，只是微软不愿将其公开，其中最大的原因是与它的.NET战略冲突。而由DirectUI技术演变而来的WPF只适用于托管环境。不过，还是有几个公司或者个人开发了自己的DirectUI界面库。我知道的，有：
DirectUI.com // 公司开发的。居然连试用版也没有，要付了钱才给你用。
Bodsoft DirectUI // 公司开发的。有试用版。设计工具不是太强大，还凑合。
UIEasy DirectUI // 个人开发的，比较简单。没设计工具。
