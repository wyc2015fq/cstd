# C++精进篇（四）之―结构体和头文件 - fanyun的博客 - CSDN博客
2016年03月25日 22:21:58[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：2751
所属专栏：[C/C++/C#开发实战365](https://blog.csdn.net/column/details/c-plus-plus-01.html)
**一、结构体是类的一种、一般来说，结构体会定义在头文件中。**
由于暂时不涉及到类的方法，只涉及到类的数据，因此可以先从结构体开始。
比如下面这个结构体：
   struct Sales_data {
       string bookNo;
       unsigned units_sold;
       double revenue;
    };
新的C++语言标准规定，可以在结构体的声明里面进行初始化，这可能需要对编译器进行设置。
**二、一般来说，头文件的名称来源于结构体的名称。**
使用预定义来使得头文件仅包含一次：
#ifndef SALES_ITEM_H_
#define SALES_ITEM_H_
/* ...... */
#endif /* SALES_ITEM_H_ */
1.       #include“headfile.h”
搜索顺序为：
①先搜索当前目录
②然后搜索-I指定的目录
③再搜索gcc的环境变量CPLUS_INCLUDE_PATH（C程序使用的是C_INCLUDE_PATH）
④最后搜索gcc的内定目录
/usr/include
/usr/local/include
/usr/lib/gcc/x86_64-redhat-linux/4.1.1/include
各目录存在相同文件时，先找到哪个使用哪个。
2.       #include<headfile.h>
①先搜索-I指定的目录
②然后搜索gcc的环境变量CPLUS_INCLUDE_PATH
③最后搜索gcc的内定目录
/usr/include
/usr/local/include
/usr/lib/gcc/x86_64-redhat-linux/4.1.1/include
 与上面的相同，各目录存在相同文件时，先找到哪个使用哪个。这里要注意，#include<>方式不会搜索当前目录！
这里要说下include的内定目录，它不是由$PATH环境变量指定的，而是由g++的配置prefix指定的(知道它在安装g++时可以指定，不知安装后如何修改的，可能是修改配置文件，需要时再研究下)：
-bash-3.2$ g++ -v
Using built-in specs.
Target: x86_64-redhat-linux
Configured with: ../configure --prefix=/usr --mandir=/usr/share/man--infodir=/usr/share/info --enable-shared --enable-threads=posix--enable-checking=release --with-system-zlib --enable-__cxa_atexit --disable-libunwind-exceptions--enable-libgcj-multifile--enable-languages=c,c++,objc,obj-c++,java,fortran,ada
 --enable-java-awt=gtk--disable-dssi --enable-plugin--with-java-home=/usr/lib/jvm/java-1.4.2-gcj-1.4.2.0/jre --with-cpu=generic--host=x86_64-redhat-linux
Thread model: posix
gcc version 4.1.2 20080704 (Red Hat4.1.2-46)
在安装g++时，指定了prefix，那么内定搜索目录就是：
Prefix/include
Prefix/local/include
Prefix/lib/gcc/--host/--version/include
编译时可以通过-nostdinc++选项屏蔽对内定目录搜索头文件。
 库文件：
编译的时候：
①gcc会去找-L
②再找gcc的环境变量LIBRARY_PATH
③再找内定目录 /lib /usr/lib /usr/local/lib 这是当初compilegcc时写在程序内的（不可配置的？）
运行时动态库的搜索路径：
动态库的搜索路径搜索的先后顺序是：
①编译目标代码时指定的动态库搜索路径（这是通过gcc 的参数"-Wl,-rpath,"指定。当指定多个动态库搜索路径时，路径之间用冒号"："分隔）
②环境变量LD_LIBRARY_PATH指定的动态库搜索路径（当通过该环境变量指定多个动态库搜索路径时，路径之间用冒号"："分隔）
③配置文件/etc/ld.so.conf中指定的动态库搜索路径；
④默认的动态库搜索路径/lib；
⑤默认的动态库搜索路径/usr/lib。
（应注意动态库搜寻路径并不包括当前文件夹，所以当即使可执行文件和其所需的so文件在同一文件夹，也会出现找不到so的问题，类同#include <header_file>不搜索当前目录）
 参考资料: http://my.oschina.net/alphajay/blog/4953?from=rss
