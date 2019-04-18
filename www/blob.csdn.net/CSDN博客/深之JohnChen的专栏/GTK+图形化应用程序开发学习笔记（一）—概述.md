# GTK+图形化应用程序开发学习笔记（一）—概述 - 深之JohnChen的专栏 - CSDN博客

2008年12月17日 23:34:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：1905


**GTK+图形化应用程序开发学习笔记（一）—概述**

一、什么是GNOME、

GNOME的意思是“GNU Network Object Model Environment”(GNU网络对象模型环境），是Linux世界令人激动的软件开发成果。虽然它的名字比较长，但这个软件项目的目的却很简单，那就是实现一个永恒友好的、功能强大的用户和开发桌面环境，而这一环境又完全是以免费的开源代码软件为基础的。

正像它名字的那样，GNOME既是一个桌面，也是一个完整的程序设计环境。GNOME是底层X窗口系统和高层管理器软件中间的一个程序设计界面，它向GUI程序员提供了丰富的功能和强大的开发工具，这些工具是传统意义上的Linux所缺乏的。

二、什么是GIMP、GTK+

GNOME中的基本工具包是GTK+（即GIMP工具包），它最初是为了简化GIMP的开发工作而编写出来的。

GIMP是UNIX类操作系统上的影像处理程序。GIMP是通用影像处理程序或者GNU影像处理程序的缩写。它的功能类似Windows的Photoshop。

GTK+是一个性能优异的、面向对象的、跨平台的、不依赖于具体语言的开发工具包，在GNOME出现之前就已经大量地用来开发各种软件程序了。GTK+能够支持许多程序设计语言，其中包括C、C++、TOM、PERL、Python、GUILE、ADA等。

三、GTK+使用的库

GTK+使用GLIB库和GDK（GIMP Drawing Kit，GIMP绘图工具包）系列的开发库，GLIB定义了数据类型，提供了错误处理和内存管理方面的函数；而GDK则是本地图形化API和GTK+ 中间的一个过渡层，它需要依赖具体的计算机平台。因此，向其他计算机平台上移植GTK+ 只需要重新编写GDK。

四、GTK+的安装

一般来说最新的Redhat完全安装就带GTK+工具库了。大家直接可以在上面开发程序。如果当前系统没有GTK+工具库，可以到[www.gtk.org](http://www.gtk.org/).下载源码安装。具体安装方法可以。参见CU上的GTK+ 2.0安装的帖子，这里不再叙述。

检验系统GTK+ 2.0的方法是

$ pkg-config --modversion gtk+-2.0

五、使用GTK+工具集编写程序

下面就是一个简单的使用GTK+库编写的程序，目的是让大家体验到GTK+编程的乐趣。

/*base.c*/

#include <gtk/gtk.h>

int main(int argc, char* argv[])

{

GtkWidget *window;

gtk_init(&argc,&argv);

window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

gtk_widget_show(window);

gtk_main();

return FALSE;

}

下面我们编译它：

$ gcc -o base base.c 'pkg-config --cflags --libs gtk+-2.0'

要注意的是pkg-config --cflags --libs gtk+-2.0旁边两个像单引号的东西其实不是单引号，它是数字键1左边的符号。大家一定要注意。如果用单引号编译器会提示你pkg-config --cflags --libs gtk+-2.0没有那个文件或目录。

下面我们来运行：

$./base

程序运行后会出现一个名字为base窗体。

![](https://p-blog.csdn.net/images/p_blog_csdn_net/byxdaz/EntryImages/20081217/22935_070523223010.png)

大家可以感觉到GTK+ 程序和普通的C语言程序没什么差别，它只不过是调用了GTK+工具库。


