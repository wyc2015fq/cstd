# TPService 编译过程 - 三少GG - CSDN博客
2015年04月26日 00:08:27[三少GG](https://me.csdn.net/scut1135)阅读数：995
个人分类：[重构C/C++																[云计算/大数据](https://blog.csdn.net/scut1135/article/category/1449467)](https://blog.csdn.net/scut1135/article/category/621651)
1.
ubuntu  cannot find -lz
I had the exact same error, and like you, installing zlib1g-dev did not fix it. Installing** lib32z1-dev** got me past it. I have a 64 bit system and it seems like it wanted the 32 bit library.
2.
# HowTo Compile a 32-bit Application Using gcc On the 64-bit Linux Version
To fix this problem on a Debian
 or Ubuntu Linux type the following [apt-get
 command](http://www.cyberciti.biz/tips/linux-debian-package-management-cheat-sheet.html):
```
$sudo
 apt-get install g++-multilib libc6-dev-i386
```
3. 
undefined reference to "shm_open"
**-lrt 放在编译选项最后**
Move your sourcefiles to the begin of the command and put -lrt after the
 source files.
**G++ can be a little picky about the order**
 (i know why, but i don't feel like explaining it)
**并非如下：**
FYI: since these are 64-bit ubuntu bots, you need to install
32bit libs for nacl-gcc to run (e.g., to build the IRT). To
do this on the bots, use "install-build-deps.sh --lib32"
which will apt-get ubuntu's "ia32-libs" along with a bunch
of other libraries (see the script).
R=bradnelson@chromium.org
in Ubuntu 13.10 the ia32-libs package is no more present.
```
**sudo apt-get install lib32z1 lib32ncurses5 lib32bz2-1.0sudo apt-get install libgl1-mesa-dri:i386**
```
the second, shall install all its dependencies and resolve the compiling problems. The dependencies are:
```
gcc-4.8-base:i386 libc6:i386 libdrm-intel1:i386 libdrm-nouveau2:i386 
libdrm-radeon1:i386 libdrm2:i386 libelf1:i386 libexpat1:i386 libffi6:i386
libgcc1:i386 libglapi-mesa:i386 libllvm3.3:i386 libpciaccess0:i386 
libstdc++6:i386 libtxc-dxtn-s2tc0:i386 zlib1g:i386
```
Bye,
4. zconf.h: No
 such file or directory
Suggested workaround from bugzilla.mozilla org:
**sudo ln -s /usr/include/x86_64-linux-gnu/zconf.h /usr/include**
原文：https://bugs.launchpad.net/ubuntu/+source/zlib/+bug/1155307
