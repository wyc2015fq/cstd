# 32位Ubuntu 12.04 上搭建android 2.3.4 环境 - V__KING__的专栏 - CSDN博客





2013年10月24日 15:49:29[v__king__](https://me.csdn.net/V__KING__)阅读数：2453标签：[android																[32位																[64位](https://so.csdn.net/so/search/s.do?q=64位&t=blog)
个人分类：[android](https://blog.csdn.net/V__KING__/article/category/1704545)





32位Ubuntu 12.04 安装android 2.3.4 环境搭配

官方网站要求是用64位的系统来编译android 2.3.4，但是我穷啊，买不起64位机子。但是还是可以编译的。尽管官方不推荐。



1. 先安装gcc-4.4 ，Ubuntu 12.04默认的是gcc-4.6版本，不能编译android 2.3.4，所以要降 
 到gcc-4.4版本。[http://blog.csdn.net/nseven/article/details/8288603](http://blog.csdn.net/nseven/article/details/8288603)

2.  安装Python-3.3.2 ，请按照README文件来安装。下载地址为：

[http://www.python.org/ftp/python/3.3.2/Python-3.3.2.tar.bz2](http://www.python.org/ftp/python/3.3.2/Python-3.3.2.tar.bz2)

3. 安装jdk6：方法在：[http://www.linuxidc.com/Linux/2012-08/68296.htm](http://www.linuxidc.com/Linux/2012-08/68296.htm)

   jdk6的下载：[http://download.csdn.net/detail/wxcking/5380215](http://download.csdn.net/detail/wxcking/5380215)



4. 安装依赖包：

 $sudo apt-get update

 $ sudo apt-getinstall git gnupg flex bison gperf build-essential \

  zip curl libc6-dev libncurses5-dev:i386 x11proto-core-dev \

  libx11-dev:i386 libreadline6-dev:i386 libgl1-mesa-glx:i386 \

  libgl1-mesa-dev g++-multilib mingw32 tofrodos \

  python-markdown libxml2-utils xsltproc zlib1g-dev:i386

$ sudo ln -s /usr/lib/i386-linux-gnu/mesa/libGL.so.1/usr/lib/i386-linux-gnu/libGL.so

5.编译android的时候，多数出现类似这样的错误：

/bin/bash: javah: 未找到命令

make:*** [out/host/linux-x86/obj/SHARED_LIBRARIES/libclearsilver-jni_intermediates/org_clearsilver_HDF.h] 错误127

这种错误的解决办法很简单，就是安装javah，这里安装的方法就是前面安装jdk6的方法[http://www.linuxidc.com/Linux/2012-08/68296.htm](http://www.linuxidc.com/Linux/2012-08/68296.htm)](https://so.csdn.net/so/search/s.do?q=32位&t=blog)](https://so.csdn.net/so/search/s.do?q=android&t=blog)




