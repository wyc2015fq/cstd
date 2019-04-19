# DBus编译出错 ----QT中dbus所依赖的一个库 - xqhrs232的专栏 - CSDN博客
2016年06月15日 14:24:08[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：845
原文地址::[http://blog.chinaunix.net/uid-23670869-id-2391671.html](http://blog.chinaunix.net/uid-23670869-id-2391671.html)
相关文章
1、[一篇dbus移植的点点滴滴](http://blog.csdn.net/shanzhizi/article/details/7712729)----[http://blog.csdn.net/shanzhizi/article/details/7712729](http://blog.csdn.net/shanzhizi/article/details/7712729)
2、dbus 1.2.16源代码下载----[https://lists.freedesktop.org/archives/dbus/2009-July/011574.html](https://lists.freedesktop.org/archives/dbus/2009-July/011574.html)
在解压的DBus目录下配置时，运行./configure中出现下面错误：
could not find expat.h
解决方法：
运行 sudo apt-get install libexpat1-dev ，这样在运行configure时就不会再出现这个错误了。
然后编译 make
最好安装 make install
这样在/usr/include/下就会包含一个DBus文件的dbus目录下
/usr/lib/下有一个dbus库目录
接下来就是调用dbus提供的借口函数来编程了，在此之前需要移动下头文件所在的位置，我正常安装完后这些头文件在/usr/include/dbus-1.0/dbus/目录下，我们需要把这些文件移动到/usr/include/dbus/目录下，这是是为什么我在编程时包含头文件时如#include 这样子在前面加上/dbus的原因，当然你也有可以把那些*.h文件全部拷贝在/usr/include/目录下，这要在包含头文件时就不可以直接这要写了#include ,注意我们还有一个dbus-arch-deps.h在/usr/lib/dbus-1.0/include/dbus目录下，（可以使用locate
 dbus-arch-deps.h定位改文件所在位置），同样这个文件也要拷贝到/usr/include/dbus目录下（或/usr/include/目录），这要我们就可以调用这些头文件提供的接口函数了。
[](http://blog.chinaunix.net/uid-23670869-id-2391671.html#)[](http://blog.chinaunix.net/uid-23670869-id-2391671.html#)[](http://blog.chinaunix.net/uid-23670869-id-2391671.html#)[](http://blog.chinaunix.net/uid-23670869-id-2391671.html#)[](http://blog.chinaunix.net/uid-23670869-id-2391671.html#)
