# vc 中配置gtk - 深之JohnChen的专栏 - CSDN博客

2008年05月05日 22:33:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：1967标签：[gtk																[include																[signal																[object																[dll																[null](https://so.csdn.net/so/search/s.do?q=null&t=blog)](https://so.csdn.net/so/search/s.do?q=dll&t=blog)](https://so.csdn.net/so/search/s.do?q=object&t=blog)](https://so.csdn.net/so/search/s.do?q=signal&t=blog)](https://so.csdn.net/so/search/s.do?q=include&t=blog)](https://so.csdn.net/so/search/s.do?q=gtk&t=blog)
个人分类：[VC](https://blog.csdn.net/byxdaz/article/category/7211574)


在VC++6.0,VC++7.0,VC++8.0下同步配置gtk开发环境,现将配置步骤罗列如下:
　　1.设置系统变量home,任意目录(注:仅为建议)仅为建议)
　　2.下载所需环境: [http://www.pcpm.ucl.ac.be/~gustin/win32_ports/](http://www.pcpm.ucl.ac.be/~gustin/win32_ports/) 下哪个几个,以什么为准呢?我是全部下载的 有点笨 确实没有办法,不过有几个包必须要下:gettext,glib,pkgconfig,gtk+,pango,atk.
　　3.解压缩到某个文件夹 假定d:/gnu 最终形成诸如:bin,dll,etc,include,manshare等若干文件夹 我添加了个package包,放置那些下载的但是还没有使用的包
　　4.配置:Directories（包括include和lib）
　　添加
       include包括
　　D:/gnu/include
　　D:/MinGWStudio/MinGW/include
　　D:/gnu/include/gtk-2.0
　　D:/gnu/include/glib-2.0
　　D:/gnu/lib/glib-2.0/include
　　D:/gnu/lib/gtk-2.0/include
　　D:/gnu/include/pango-1.0
　　D:/gnu/include/atk-1.0
       lib包括
　　D:/gnu/lib
　　D:/gnu/dll
　　D:/gnu/lib/pkgconfig
　　D:/gnu/lib/pango/1.4.0/modules

      5.测试:
　　新建工程hellogtk,创建C++文件mian.cpp（注意包含库文件glib2.0.lib等）
　　#include <gtk/gtk.h>
　　#include <stdio.h>
　　int main(int argc, char *argv[])
　　{ 
　　GtkWidget *win, *label;
　　gtk_set_locale();
　　gtk_init(&argc, &argv);
　　win = gtk_window_new(GTK_WINDOW_TOPLEVEL);
　　gtk_widget_set_usize(GTK_WIDGET(win), 200, 100);
　　gtk_window_set_title(GTK_WINDOW(win), "Test");
　　gtk_signal_connect(GTK_OBJECT(win), "destroy", GTK_SIGNAL_FUNC(gtk_exit), NULL);
　　label=gtk_label_new("Hello, GTK+/n");
　　gtk_container_add(GTK_CONTAINER(win), GTK_WIDGET(label));
　　gtk_widget_show_all(win);
　　gtk_main();
　　return 0;
　　}
　　如果发现有错误提示,一定是您的环境设置不正确


