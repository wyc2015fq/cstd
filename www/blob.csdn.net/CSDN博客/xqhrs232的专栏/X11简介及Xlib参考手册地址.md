# X11简介及Xlib参考手册地址 - xqhrs232的专栏 - CSDN博客
2017年11月03日 21:59:28[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：380
个人分类：[X11/Qt/Qt quick/Qxml界面技术](https://blog.csdn.net/xqhrs232/article/category/6455019)
原文地址::[http://blog.csdn.net/yygydjkthh/article/details/41487493](http://blog.csdn.net/yygydjkthh/article/details/41487493)
相关文章
1、linux图形界面编程基本知识(X11, xorg, gdm/gnome)----[http://blog.csdn.net/junmuzi/article/details/50134605](http://blog.csdn.net/junmuzi/article/details/50134605)
2、**[linux图形界面基本知识（X、X11、Xfree86、Xorg、GNOME、KDE之间的关系）](http://blog.csdn.net/qq_38880380/article/details/78274272)----[http://blog.csdn.net/qq_38880380/article/details/78274272](http://blog.csdn.net/qq_38880380/article/details/78274272)**
3、如何开发一个Linux/X11平台的输入法----[http://blog.csdn.net/kartorz/article/details/53995850](http://blog.csdn.net/kartorz/article/details/53995850)
4、使用SSH的X11特性远程运行X界面程序----[http://blog.csdn.net/gothicane/article/details/1669978](http://blog.csdn.net/gothicane/article/details/1669978)
/*********************************************************************
 * Author  : Samson
 * Date    : 11/25/2014
 * Test platform:
 *              3.13.0-24-generic
 *              GNU bash, 4.3.11(1)-release 
 * *******************************************************************/
一,GNU Linux本身没有图形界面,linux现在的图形界面的实现只是linux下的应用程序实现的.
图形界面并不是GNU Linux的一部分,GNU Linux只是一个基于命令行的操作系统,GNU Linux和Xfree的关系就相当于当年的DOS和WINDOWS3.0一样,windows3.0不是独立的操作系统,它只是DOS的扩充,是DOS下的应用程序级别的系统,不是独立的操作系统,同样XFree只是GNU
 Linux下的一个应用程序而已.不是系统的一部分,但是X的存在可以方便用户使用电脑.WINDOWS95及以后的版本就不一样了,他们的图形界面是操作系统的一部分,图形界面在系统内核中就实现了,没有了图形界面windows就不成为windows了,但GNU Linux却不一样,没有图形界面GNU Linux还是GNU
 Linux,很多装GNU Linux的WEB服务器就根本不装X服务器.这也WINDOWS和GNU Linux的重要区别之一.
二,X是协议,不是具体的某个软件:
X是协议,就像HTTP协议,IP协议一样.因为输入设备和显示设备不是同一个设备，而且他们需要相互配合，进行画面显示，所以需要一个交互协议，建立他们直接的沟通桥梁。
X协议主要有X应用程序和X服务器组成，如：我需要划个圆，X应用程序告诉X服务器在屏幕的什么地方用什么颜色画一个多大的圆,而具体的"画"的动作,比如这个圆如何生成,用什么显卡的驱动程序去指挥显卡完成等等工作是由X服务器来完成的.
X服务器还负责捕捉键盘和鼠标的动作,假设X服务器捕捉到鼠标的左键被按下了,他就告诉X应用程序:亲爱的应用程序先生,我发现鼠标被按下了,您有什么指示吗?如果X应用程序被设计成当按下鼠标左健后再在屏幕上画一个正方形的话,X应用程序就对X服务器说:请再画一个正方形,当然他会告诉服务器在什么地方用什么颜色画多大的正方形,但不关心具体怎么画--那是服务器的事情.
那么协议是需要具体的软件来实现的,这就是下面我要讲的:
三,X和XFree86的关系.
有了协议就需要具体的软件来实现这个协议.就好比我们有了交通法规就需要交警去根据法规维护交通秩序一样.Xfree86就是这样一个去根据法规实现协议的"交警".他按照X协议的规定来完成X应用程序提交的在屏幕上显示的任务.当然不仅仅是某个特定的交警才可以去维护和实现这个法规,比如还可以由交通协管员来实现交通法规,必要的时候警察也可以介入,当然前提是他们都要懂得交通法规,也就是要懂得协议.所以实现X协议的软件也并不只有XFree86,XFree86只是实现X协议的一个免费X服务器软件.商业上常用MOTIF,现在还有XORG,还有很多很小的由爱好者写的小的X服务器软件.甚至可以在WINDOWS上有X服务器运行,这样你可以在linux系统上运行一个X应用程序然后在另一台windows系统上显示.多么神奇.你可以用google找到这样的X服务器软件.只不过在LINUX上最常用的是XFree86.(现在的linux发行版都用Xorg了)顺便说一句,苹果电脑的图形界面用的也是X协议,而且被认为是做的最好的X协议图形界面,并且他对X协议的实施是做在系统内核里的,所以性能明显好很多,这就是为什么很多大型三维图形设计软件都是在苹果平台上的原因.
协议是X
实现这个协议的常用服务器有Xfree86,Xorg,Xnest等，目前用到的更多的是Xorg的实现。
请求这些服务器来完成显示任务的客户:所有的X应用程序.
X协议的Client端有时谁实现的呢？
XFree86是X协议的服务器软件(注以软件的形式实现)，但Client端更多的是方便程序员更方便的使用X协议与服务器端的XFree86交互，故就产生了一个Xlib库，封装了X协议内容，如果你还觉得直接使用Xlib不方便，于是就产生了qt和gtk，对Xlib进一步封装，提供了很多控件，可以可视化的使用X协议更方便程序员的开发。
而GTK及QT都是在Xlib基础之上的再次封装。
关于Xlib的具体接口的说明参照以下地址：
The Xlib Manual
[http://tronche.com/gui/x/xlib/](http://tronche.com/gui/x/xlib/)
Xorg的文档[](http://tronche.com/gui/x/xlib/)
[http://xorg.freedesktop.org/wiki/Documentation/](http://xorg.freedesktop.org/wiki/Documentation/)
REF：
[http://blog.csdn.net/lanmayi003/article/details/6584521](http://blog.csdn.net/lanmayi003/article/details/6584521)
