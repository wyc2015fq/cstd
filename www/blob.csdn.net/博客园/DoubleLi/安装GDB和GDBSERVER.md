# 安装GDB和GDBSERVER - DoubleLi - 博客园






安装GDB和GDBSERVER

转自[http://www.360doc.com/content/10/0407/17/155970_21971613.shtml](http://www.360doc.com/content/10/0407/17/155970_21971613.shtml)

把GDBSERVER装入文件系统

转自[http://www.arm9home.net/read.php?tid-1396.html](http://www.arm9home.net/read.php?tid-1396.html)


在mini2440上使用GDB远程调试 
哈哈，一直都没有发过有建设性的帖子，今次发个《在mini2440上使用GDB远程调试》，下面的东西虽算不上绝对原创， 
但是经过我试验和总结的，用NFS作根文件系统配合GDB为我们的开发和学习提供了很大的便利，希望大家会喜欢！

**arm-linux-gdb 移植——PC端**1. 到[http://www.gnu.org/software/gdb](http://www.gnu.org/software/gdb)下载gdb-6.6.tar.gz到/tmp目录 
2. 解压到/opt目录下 
#cd /opt 
#tar xzvf /tmp/gdb-6.6.tar.gz 
3. 建立配置文件、编译 
gdb允许把编译配置和编译结果放到任意的目录，因此可以在gdb目录之外建立一个专门存放编译结果的目录。 
#cd /opt 
#mkdir -p arm-gdb/build 
#cd arm-gdb/build 
#/opt/gdb-6.6/configure --target=arm-linux --prefix=/opt/arm-gdb 
#make 
#make install 
prefix指定了编译结果的存放位置，也就是安装目录。 
如果在编译arm-linux-low.c这个文件时提示找不到“sys/reg.h”，则修改arm-linux-low.c，注释掉#include "sys/reg.h"。 
编译完后可以在/opt/arm-gdb/bin目录下找到可执行的arm-linux -gdb, arm-linux -gdbtui, arm-linux-run。 
拷贝arm-linux-gdb 到/usr/bin目录 
#cd /opt/arm-gdb/bin/ 
#cp arm-linux-gdb /usr/bin/



错误：

我使用的GCC版本是4.3.3，在编译过程中报错：

/bin/sh ./libtool --mode=compile gcc -DHAVE_CONFIG_H -I. -I.././opcodes -I. -D_GNU_SOURCE -I. -I.././opcodes -I../bfd -I.././opcodes/../include -I.././opcodes/../bfd -I.././opcodes/../intl -I../intl   -W -Wall -Wstrict-prototypes -Wmissing-prototypes -Werror -g -O2 -c -o arm-dis.lo arm-dis.c
gcc -DHAVE_CONFIG_H -I. -I.././opcodes -I. -D_GNU_SOURCE -I. -I.././opcodes -I../bfd -I.././opcodes/../include -I.././opcodes/../bfd -I.././opcodes/../intl -I../intl -W -Wall -Wstrict-prototypes -Wmissing-prototypes -Werror -g -O2 -c arm-dis.c -o arm-dis.o
cc1: warnings being treated as errors
arm-dis.c: In function 'print_insn_thumb16':
arm-dis.c:2950: error: format not a string literal and no format arguments
arm-dis.c:2957: error: format not a string literal and no format arguments
arm-dis.c: In function 'print_insn_thumb32':
arm-dis.c:3491: error: format not a string literal and no format arguments
arm-dis.c:3499: error: format not a string literal and no format arguments
make[4]: *** [arm-dis.lo] Error 1
make[4]: Leaving directory `/opt/gdb-6.5/opcodes'
make[3]: *** [all-recursive] Error 1
make[3]: Leaving directory `/opt/gdb-6.5/opcodes'
make[2]: *** [all] Error 2
make[2]: Leaving directory `/opt/gdb-6.5/opcodes'
make[1]: *** [all-opcodes] Error 2
make[1]: Leaving directory `/opt/gdb-6.5'
make: *** [all] Error 2

这是源码中有些函数定义不规范，用对函数格式非常严格的GCC 4.3编译就会出错，而GCC 4.1 不会报错。

自然不乐意重新安装GCC退回到4.1的版本来解决这个问题，我是这样做的。

$cd /opt/gdb-6.5/opcodes

进入编译出错的目录；

$/bin/sh ./libtool --mode=compile gcc -DHAVE_CONFIG_H -I. -I.././opcodes -I. -D_GNU_SOURCE -I. -I.././opcodes -I../bfd -I.././opcodes/../include -I.././opcodes/../bfd -I.././opcodes/../intl -I../intl   -W -Wall -Wstrict-prototypes -Wmissing-prototypes -g -O2 -c -o arm-dis.lo arm-dis.c

将编译的命令拷贝，去掉“-Werror”，重新编译，只有警告信息，没有报错误。

$cd ..

推出子目录

$make

继续make

4，make install


** gdbserver的移植——开发板端**
gdbserver要用到gdb源码目录下的一些头文件，因此无法在gdb源码目录之外编译文件。 
#cd /opt/gdb-6.6/gdb/gdbserver 
#./configure --host=arm-linux --target=arm-linux --prefix=/opt/arm-gdb/gdbserver 
#make 
#make install 
注意：此处target参数和host参数都要设置为arm-linux，因为gdbserver是放在目标板上运行的。 
编译后，可以在/opt/arm-gdb/gdbserver/bin目录下找到arm-linux-gdbserver，下载该文件到目标板的/usr/bin目录并重命名为gdbserver。 
同时要下载gdbserver需要的库文件libthread_db-x.x.so（在交叉编译器的安装目录的lib子目录下可以找到）到目录板的/usr/lib目录， 
再建立两个符号链接libthread_db.so和libthread_db.so.1。 
在目标板上执行 
#cd \usr\lib 
#ln –s libthread_db-x.x.so libthread_db.so 
#ln –s libthread_db-x.x.so libthread_db.so.1 
5. 使用gdbserver 
在目标板上运行gdbserver 
在目标板上执行 
#./gdbserver 192.168.0.2:2345 hello 
其中192.168.0.2为目标板的IP。2345为gdbserver打开的端口，可以自己设置。 
6. 运行gdb客户端 
#./arm-linux-gdb hello 
(gdb)target remote 192.168.0.2:2345 
(gdb) list 
1 #include <stdio.h> 
2 #include <fcntl.h> 
3 
4 int main() 
5 { 
6 printf("hello world \n"); 
7 return 0; 
8 } 
(gdb)b main 
(gdb)c 
7. GDB的基本指令： 
load：装入一个程序 
symbol-file：装入符号库文件，可以是用-g参数编译的可执行文件。 
f(ile)：指定一个可执行文件进行调试，gdb将读取些文件的调试讯息，如f a.exe 
l(ist)：列程序出源文件 
r(un) ：装载完要调试的可执行文件后，可以用run命令运行可执行文件 
b(reak)：设置断点（break point），如b 25，则在源程序的第25行设置一个断点，当程序执行到第25行时，就会产生中断；也可以使用b funcname，funcname为函数的名称， 
当程序调用些函数时，则产生中断 
c(ontinue)：c命令可以另中断的程序继续执行，直到下一个中断点或程序结束 
p(rint)：输入某个变量的值，如程序定义了一个int aa的就是，p aa就会输出aa的当前值 
n(ext)：程序执行到断点时中断执行，可以用n指令进行单步执行 
s(tep)：程序执行到断点时中断执行，可以用s指令进行单步执行进某一函数 
q(uit)：退出GDB



我的gdbserver位于/sbin下，调试时要进入应用程序目录下；arm-linux-gdb位于/usr/bin/下。

调试步骤：

1、gdbserver 172.16.17.50:2345 XXX     开发板端

2、/usr/bin/arm-linux-gdb XXX PC端

3 、 target remote 172.16.17.50:2345 PC端









