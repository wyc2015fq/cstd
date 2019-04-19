# Linux下编译libc库 - xqhrs232的专栏 - CSDN博客
2018年04月08日 14:44:27[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：401
原文地址::[http://blog.sina.com.cn/s/blog_602f877001010tpt.html](http://blog.sina.com.cn/s/blog_602f877001010tpt.html)
相关文章
1、[configure: error: you must configure in a separate build directory](https://blog.csdn.net/testcs_dn/article/details/45437481)----[https://blog.csdn.net/testcs_dn/article/details/45437481](https://blog.csdn.net/testcs_dn/article/details/45437481)
2、Glibc-2.4.tar.gz的安装问题一个一个解决----[https://blog.csdn.net/Giegie/article/details/6625155](https://blog.csdn.net/Giegie/article/details/6625155)
方法一：
得到Linux下命令源码的方法：
Sam常需要在嵌入式Linux上实现一些busybox没有提供的,与Linux命令类似的功能。可以很方便的找到源码：
例如，需要找到df的源码。
#rpm -qf `which df` 
得到安装包为：coreutils-6.9-9.fc8
于是下载coreutils源码。
通常都可以在www.gun.org下找到。
方法二：
编译glibc.
因为新版本Fedora下不带libc.a.所以想自己编译一个。原来问题挺多的。
[ftp://ftp.gnu.org/gnu/glibc/](ftp://ftp.gnu.org/gnu/glibc/)
下载了对应版本。
#cd glibc-2.14
#./configure
configure: error: you must configure in a separate build directory
不明白怎么回事。google之后才知道要创建一个build目录。
#mkdir ../glibc_build/
#cd ../glibc_build
#../glibc-2.14/configure --exec-prefix=/home/sam/temp --prefix=/home/sam/temp --with-tls --with-arch=i686
在编译glibc的过程中可能出现错误：“../sysdeps/i386/fpu/s_frexp.S:66: Error: invalid identifier for ".ifdef"”，解决方法是：
1)、在glibc源码目录下找到文件：nptl/sysdeps/pthread/pt-initfini.c，找到第46行附近：asm ("\n#include \"defs.h\"");在其后添加代码：
asm ("\n#if defined __i686 && defined __ASSEMBLER__");
asm ("\n#undef __i686");
asm ("\n#define __i686 __i686");
asm ("\n#endif");
2)、在glibc源码目录下找到文件：sysdeps/unix/sysv/linux/i386/sysdep.h,找到第30行附近：#include <tls.h>，在其后添加代码：
#if defined __i686 && defined __ASSEMBLER__
#undef __i686
#define __i686 __i686
#endif
重新make
//==========================================================================================
备注
1>新建个目录，用于编译
   mkdir  /root/rpmbuild/SOURCES/glibc-2.13/glibc_build
   cd glibc_build
2>配置及编译
/root/rpmbuild/SOURCES/glibc-2.13/configure  --prefix=/root/rpmbuild/SOURCES/glibc-2.13/
make
