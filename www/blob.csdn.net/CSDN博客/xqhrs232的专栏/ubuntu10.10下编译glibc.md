# ubuntu10.10下编译glibc - xqhrs232的专栏 - CSDN博客
2017年02月21日 16:17:17[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：666
原文地址::[http://blog.lchx.me/index.php/ubuntu10-10%E4%B8%8B%E7%BC%96%E8%AF%91glibc/](http://blog.lchx.me/index.php/ubuntu10-10%E4%B8%8B%E7%BC%96%E8%AF%91glibc/)
目的很单纯，就是下一个glibc看看源码，学习学习。学习之前想编译一下，编译不过当然不爽。
分享一下，希望对同样目的单纯的人有所帮助。
glibc源码下载地址[ftp://ftp.gnu.org/gnu/glibc/](ftp://ftp.gnu.org/gnu/glibc/)
一开始下载了glibc-2.9.tar.bz2
configure: error:
*** These critical programs are missing or too old: as ld
*** Check the INSTALL file for required versions.
出现这个错误，估计是和内核版本不匹配。
查看下系统使用的libc的版本号
~$ ls -l /lib/libc.so.6
lrwxrwxrwx 1 root root 14 2012-11-03 23:16 /lib/libc.so.6 -> libc-2.12.1.so
因为只是学习，版本不重要，就下了glibc-2.12.2。
然后解压到glibc-2.12.2目录。
~/glibc-2.12.2$ ./configure 
checking build system type... i686-pc-linux-gnu
checking host system type... i686-pc-linux-gnu
checking for gcc... gcc
checking for suffix of object files... o
checking whether we are using the GNU C compiler... yes
checking whether gcc accepts -g... yes
checking for gcc option to accept ISO C89... none needed
checking how to run the C preprocessor... gcc -E
checking for g++... g++
checking whether we are using the GNU C++ compiler... yes
checking whether g++ accepts -g... yes
configure: error: you must configure in a separate build directory
glibc能直接在源码根目录下编译。
~$ mkdir glibc~$ cd glibc
~/glibc$ ~/glibc-2.12.2/configure
*** On GNU/Linux systems the GNU C Library should not be installed into
*** /usr/local since this might make your system totally unusable.
*** We strongly advise to use a different prefix.  For details read the FAQ.
*** If you really mean to do this, run configure again using the extra
*** parameter `--disable-sanity-checks'.
不推荐安装到/usr/local,自己添加设在路径，这里需要使用绝对路径才行
~/glibc$ ~/glibc-2.12.2/configure --prefix=/home/lu_chengxiang/glibc
configure: creating ./config.status
config.status: creating config.make
config.status: creating Makefile
config.status: creating config.h
config.status: executing default commands
~/glibc$ make
make[1]: *** 没有规则可以创建“/home/lu_chengxiang/glibc/abi-versions.h”需要的目标
“/home/lu_chengxiang/glibc/Versions.all”。 停止。
又有 了个新问题，这是应为ubuntu默认装了mawk，只需安装gawk就可一解决
~/glibc$ sudo apt-get install gawk
~/glibc$ make
../sysdeps/i386/fpu/s_frexp.S: Assembler messages:
../sysdeps/i386/fpu/s_frexp.S:66: Error: invalid identifier for ".ifdef"
../sysdeps/i386/fpu/s_frexp.S:66: Error: junk at end of line, first unrecognized
 character is `1'
../sysdeps/i386/fpu/s_frexp.S:66: Error: junk at end of line, first unrecognized
 character is `1'
../sysdeps/i386/fpu/s_frexp.S:66: Error: Missing symbol name in directive
../sysdeps/i386/fpu/s_frexp.S:66: Error: junk at end of line, first unrecognized
 character is `1'
../sysdeps/i386/fpu/s_frexp.S:66: Error: Missing symbol name in directive
../sysdeps/i386/fpu/s_frexp.S:66: Error: junk at end of line, first unrecognized
 character is `.'
../sysdeps/i386/fpu/s_frexp.S:66: Error: junk at end of line, first unrecognized
 character is `1'
../sysdeps/i386/fpu/s_frexp.S:66: Error: expected comma after name `' in .size
 directive
../sysdeps/i386/fpu/s_frexp.S:66: Error: ".endif" without ".if"
../sysdeps/i386/fpu/s_frexp.S:66: Error: junk `.get_pc_thunk.dx' after expression
这个错误不解释，解释不了。修改两个文件就行了
加号表示添加该行代码
~/glibc$ vim ../glibc-2.12.2/sysdeps/unix/sysv/linux/i386/sysdep.h
/* There is some commonality.  */
#include <sysdeps/unix/i386/sysdep.h>
#include 
#include 
/* Defines RTLD_PRIVATE_ERRNO and USE_DL_SYSINFO.  */
#include 
#include 
+#if defined __i686 && defined __ASSEMBLER__
+#undef __i686
+#define __i686 __i686
+#endif
~/glibc$ vim ../glibc-2.12.2/nptl/sysdeps/pthread/pt-initfini.c
/* Embed an #include to pull in the alignment and .end directives. */
asm ("\n#include \"defs.h\"");
+asm ("\n#if defined __i686 && defined __ASSEMBLER__");
+asm ("\n#undef __i686");
+asm ("\n#define __i686 __i686");
+asm ("\n#endif");
/* The initial common code ends here. */
asm ("\n/*@HEADER_ENDS*/");
~/glibc$ make
In file included from ../sysdeps/unix/sysv/linux/syslog.c:10:
../misc/syslog.c: In function ‘__vsyslog_chk’:
../misc/syslog.c:123: sorry, unimplemented: inlining failed in call to ‘syslog’
: function body not available
../misc/syslog.c:155: sorry, unimplemented: called from here
现实是残酷的，又出错了。
解决方法从网上查的，试了好多，终于找到可行的。
```
export CFLAGS="-O2 -U_FORTIFY_SOURCE
 -march=i686 -mtune=native -fno-stack-protector"
```
~/glibc$ ~/glibc-2.12.2/configure --prefix=/home/lu_chengxiang/glibc~/glibc$ make
终于可以安心学习的。
还没出错的同志门可以直接执行紫色部分的命令，没装g++的先把g++装上，不然好像装不了的，我就不验证了。
