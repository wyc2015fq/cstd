# Linux系统下的C语言开发都需要学些什么 - nosmatch的专栏 - CSDN博客
2011年07月26日 22:59:23[nosmatch](https://me.csdn.net/HDUTigerkin)阅读数：450

**一、工具的使用**
　　1、学会使用vim/emacs，vim/emacs是[**linux**](http://search.yesky.com/search.do?stype=0&wd=Linux)下最常用的源码编辑具，不光要学会用它们编辑源码，还要学会用它们进行查找、定位、替换等。新手的话推荐使用vim，这也是我目前使用的文本编辑器。
　　2、学会makefile文件的编写规则，并结合使用工具aclocal、autoconf和automake生成makefile文件。
　　3、掌握gcc和gdb的基本用法。掌握gcc的用法对于构建一个软件包很有益处，当软件包包含的文件比较多的时候，你还能用gcc把它手动编译出来，你就会对软件包中各个文件间的依赖关系有一个清晰的了解。
　　4、掌握svn/[cvs](http://www.mydown.com/soft/develop/others/276/404776.shtml)的基本用法。这是linux，也是开源社区最常用的版本管理系统。可以去试着参加sourceforge上的一些开源项目。
**二、linux/unix系统调用与标准C库**
　　系统调用应用软件与操作系统的接口，其重要性自然不用说，一定要掌握。推荐学习资料为steven先生的UNIX环境高级编程(简称APUE)。
**三、库的学习**
　　无论是在哪个平台做软件开发，对于库的学习都很重要，linux下的开发库很多，我主要介绍一下我常常用到的一些库。
　　1、glib库
　　glib 库是gtk+和[gnome](http://www.mydown.com/game/dmtp/36/423536.shtml)的基础库，并具是跨平台的，在linux、unix和windows下都可以用。glib库对于linux平台开发的影响就像 MFC对windows平台开发的影响一样，很多开源项目都大量的使用了glib库，包括gimp、gnome、[gaim](http://www.mydown.com/soft/network/comunication/334/405334.shtml)、[evolution](http://www.mydown.com/desktop/77/77385.html)和 linux下的集群软件heartbeat.因为glib库自带有基本的数据结构实现，所以在学习glib库的时候可以顺便学习一下基本的数据结构(包括链表、树、队列和hash表)。
　　2、libxml库
　　libxml是linux平台下解析XML文件的一个基础库，现在很多实用软件都用XML格式的配置文件，所以也有必要学习一下。
　　3、readline库
　　readline 库是bash shell用的库，如果要开发命令行程序，那么使用readline库可以减少很多工作量，比如bash里的命令行自动补全，在readline里就已经有实现，当然你也可以用自己的实现替代库的行为。readline库有很多网站介绍的，只要google一下readline就可以找到一堆了。
　　4、curses库
　　curses 库以前是vi程序的一部分，后来从vi里提取出来成为一个独立的库。curses库对于编写终端相关的程序特别有用，比如要在终端某一行某一列定位输出，改变终端字体的颜色和终端模式。linux下的curses库用的是GNU实现的ncurses(new curses的意思)。
　　5、gtk+和KDE库
　　这两个库是开发GUI应用程序的基础库，现在linux下的大部份GUI程序都是基于这两个库开发的，对于它们 的学习也是很有必要的。
**四、网络的学习**
　　网络这个东西太宽了，推荐学习资料steven先生的UNIX网络编程(简称UNP)和TCP/IP协议详解，更进一步的话可以学习使用libnet编写网络程序。
