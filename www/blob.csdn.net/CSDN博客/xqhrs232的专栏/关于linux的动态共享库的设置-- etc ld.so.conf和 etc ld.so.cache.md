# 关于linux的动态共享库的设置--/etc/ld.so.conf和/etc/ld.so.cache - xqhrs232的专栏 - CSDN博客
2016年05月04日 14:34:14[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1275
原文地址::[http://blog.chinaunix.net/uid-20564848-id-74029.html](http://blog.chinaunix.net/uid-20564848-id-74029.html)
相关文章
1、[ldconfig命令以及/etc/ld.so.cache , /etc/ld.so.conf等文件详细说明](http://blog.csdn.net/leo115/article/details/7672144)----[http://blog.csdn.net/leo115/article/details/7672144](http://blog.csdn.net/leo115/article/details/7672144)
2、[linux 动态装载动态库 ldd，ldconfig，ld.so.conf, ld.so.cache](http://blog.csdn.net/jiafu1115/article/details/15808391)----[http://blog.csdn.net/jiafu1115/article/details/15808391](http://blog.csdn.net/jiafu1115/article/details/15808391)
3、/etc/ld.so.conf和/etc/ld.so.cache –linux动态共享库----[http://my.oschina.net/daquan/blog/488335](http://my.oschina.net/daquan/blog/488335)
可执行程序找不到要链接的动态共享库，这是Linux上面编译和运行程序很容易碰到的问题，通过上面的小例子，我们已经大致了解
共享库的一点基本原理，接下来我们要探讨一下怎么设置程序寻找动态共享库的行为。
Linux操作系统上面的动态共享库大致分为三类：
1、操作系统级别的共享库和基础的系统工具库
比方说libc.so, libz.so, libpthread.so等等，这些系统库会被放在/lib和/usr/lib目录下面，如果是64位操作系统，还会
有/lib64和/usr /lib64目录。如果操作系统带有图形界面，那么还会有/usr/X11R6/lib目录，如果是64位操作系统，还
有/usr/X11R6 /lib64目录。此外还可能有其他特定Linux版本的系统库目录。
这些系统库文件的完整和版本的正确，确保了Linux上面各种程序能够正常的运行。
2、应用程序级别的系统共享库
并非操作系统自带，但是可能被很多应用程序所共享的库，一般会被放在/usr/local/lib和/usr/local/lib64这两个目录下面。很多
你自行编译安装的程序都会在编译的时候自动把/usr/local/lib加入gcc的-L参数，而在运行的时候自动到/usr/local/lib下面去寻
找共享库。
以上两类的动态共享库，应用程序会自动寻找到他们，并不需要你额外的设置和担心。这是为什么呢？因为以上这些目录默认就被加
入到动态链接程序的搜索路径里面了。Linux的系统共享库搜索路径定义在/etc/ld.so.conf这个配置文件里面。这个文件的内容格式
大致如下：
1. /usr/X11R6/lib64
2. /usr/X11R6/lib
3. /usr/local/lib
4. /lib64
5. /lib
6. /usr/lib64
7. /usr/lib
8. /usr/local/lib64
9. /usr/local/ImageMagick/lib
假设我们自己编译安装的ImageMagick图形库在/usr/local/ImageMagick目录下面，并且希望其他应用程序都可以使用ImageMagick的
动态共享库，那么我们只需要把/usr/local/ImageMagick/lib目录加入/etc /ld.so.conf文件里面，然后执行：ldconfig 命令即可。
ldcofig将搜索以上所有的目录，为共享库建立一个缓存文件/etc/ld.so.cache。为了确认ldconfig已经搜索到ImageMagick的库，我
们可以用上面介绍的strings命令从ld.so.cache里面抽取文本信息来检查一下：
1. strings /etc/ld.so.cache | grep ImageMagick
输出结果为：
1. /usr/local/ImageMagick/lib/libWand.so.10
2. /usr/local/ImageMagick/lib/libWand.so
3. /usr/local/ImageMagick/lib/libMagick.so.10
4. /usr/local/ImageMagick/lib/libMagick.so
5. /usr/local/ImageMagick/lib/libMagick++.so.10
6. /usr/local/ImageMagick/lib/libMagick++.so
已经成功了！
3、应用程序独享的动态共享库
有很多共享库只被特定的应用程序使用，那么就没有必要加入系统库路径，以免应用程序的共享库之间发生版本冲突。因此Linux还
可以通过设置环境变量 LD_LIBRARY_PATH来临时指定应用程序的共享库搜索路径，就像我们上面举的那个例子一样，我们可以在应用
程序的启动脚本里面预先设置 LD_LIBRARY_PATH，指定本应用程序附加的共享库搜索路径，从而让应用程序找到它。
