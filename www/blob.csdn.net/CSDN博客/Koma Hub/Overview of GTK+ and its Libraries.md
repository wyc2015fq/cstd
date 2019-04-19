# Overview of GTK+ and its Libraries - Koma Hub - CSDN博客
2019年01月14日 21:25:49[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：37
个人分类：[GTK																[Glib](https://blog.csdn.net/Rong_Toa/article/category/8611689)](https://blog.csdn.net/Rong_Toa/article/category/7156203)
![](https://img-blog.csdnimg.cn/20190114212423386.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1JvbmdfVG9h,size_16,color_FFFFFF,t_70)
**目录**
[Architecture](#Architecture)
[Creation](#Creation)
[License](#License)
[Languages](#Languages)
[Community](#Community)
## Architecture
Over time GTK+ has been built up to be based on four libraries, also developed by the GTK+ team:
- [GLib](https://developer.gnome.org/glib/), a low-level core library that forms the basis of GTK+. It provides data structure handling for C, portability wrappers and interfaces for such run-time functionality as an event loop, threads, dynamic loading and an object system.
- [Pango](http://www.pango.org/), a library for layout and rendering of text with an emphasis on internationalization. It forms the core of text and font handling for GTK+.
- [Cairo](http://www.cairographics.org/), a library for 2D graphics with support for multiple output devices (including the X Window System, Win32) while producing a consistent output on all media while taking advantage of display hardware acceleration when available.
- [ATK](https://developer.gnome.org/atk/), a library for a set of interfaces providing accessibility. By supporting the ATK interfaces, an application or toolkit can be used with tools such as screen readers, magnifiers, and alternative input devices.
## Creation
GTK+ was initially developed for and used by the GIMP, the GNU Image Manipulation Program. It is called the "The GIMP ToolKit" so that the origins of the project are remembered. Today it is more commonly known as GTK+ for short and is used by a large number of applications including the GNU project's GNOME desktop.
Peter Mattis started it all, with help from Spencer Kimball and Josh Macdonald. Marius Vollmer taught us how to make GTK+ work for language bindings. Lars Hamann and Stefan Jeske added all sorts of features to the core widgets. Carsten Haitzler designed and did the first implementation of theme support. Shawn Amundson did release management for GTK+ 1.0 and GTK+ 1.2.
## License
GTK+ is free software and part of the GNU Project. However, the licensing terms for GTK+, the GNU LGPL 2.1, allow it to be used by all developers, including those developing proprietary software, without any license fees or royalties.
## Languages
GTK+ has been designed from the ground up to support a range of [languages](https://www.gtk.org/language-bindings.php), not only C/C++. Using GTK+ from languages such as Perl and Python (especially in combination with the Glade GUI builder) provides an effective method of rapid application development.
## Community
GTK+ has a large development community, as can be seen by the lists of contributors in GTK+ release announcements. While there are people that can be identified as responsible for one piece of code or the other, there are also [core developers](https://www.gtk.org/development.php#Team) who are always working on the toolkit.
# 什么是GDK? 
GDK是标准Xlib函数调用的一个基本封装(wrapper),如果你对Xlib很熟悉,就不需要来重新熟悉绝大多数的GDK函数.所有的函数都是为了提供一个方便直观的风格来访问Xlib函数库.另外,自从GDK使用GLib,在多平台上使用时,GDK变得更加的方便和安全. 
# 什么是GLib? 
GLib库提供了一系列函数(functions)和定义(definitions),在设计GDK和GTK程序时很有用.它提供了一些标准c函数库(standard libc)的替代函数,比如malloc,但在其他的系统上使用时有很多问题. 
# 什么是GTK+? 
GTK+是一个小型而高效的控件库,具有Motif的外观和风格.实际上,它比Motif看起来好多了,它包含有基本的控件和一些很复杂的的控件:例如文件选择、控件和颜色选择控件. GTK+提供了一些独特的特性,(至少,我知道其他的控件库不提供他们),例如,按钮不提供标签,它包含了一个子控件,在很多的时候是一个标签,但是,这个子控件也可以是一个映射,图像或者任何其他的程序员想要的集合.在整个的库中,你随处可见这种伸缩性.
        Gnome的开发基础结构是围绕一组函数库的，所有的内容都是用可移植的ANSI C语言写成的，可以用于所有类UNIX系统，与图形相关的函数库依赖于X Window系统。 Gnome函数库是最高层的。GTK＋由两部分组成，GTK和GDK。 GTK层为C语言提供了一个对象模型，并为UI工具包提供了最基本的widget，它是上层GUI派生的基础。 GTK是依赖于GDK的，GDK是Xlib的一个底层包装，Xlib库直接与X服务器对话。 除了Xlib以外的任何部分都是基于GLib的，它是一个有用的C函数库，提供了很多实用程序和可移植的功能以及一组容易被C语言使用的容器。 
       一个Gnome程序使用多个库组成的层次结构： 
       Gnome库在最高层，包括帮助例程，类和特殊的widget，并为应用程序提供一个基础架构。
       第二层是GTK，它是GTK＋库的一部分。这个库提供了基本的工具包和widget来创建GUI应用程序。大多数GUI是直接用GTK编写的。GTK同时为Gnome库提供了一个功能强大的对象系统。 
       再下一层是GDK，它简单的对X函数库进行了包装，只有在我们进行特殊绘画或设置窗口的特殊属性的时候才会用到它.  
       最下面一层是Glib，它是C的一个实用程序库，包括可移植性和一些实用程序功能，以及一些容器类，例如：链表，可变数组，变长字符串，HASH（貌似是哈西表），缓存，一个事件循环和其他有用的结构
--------------------- 
作者：wtz1985 
来源：CSDN 
原文：https://blog.csdn.net/wtz1985/article/details/2915017 
版权声明：本文为博主原创文章，转载请附上博文链接！
