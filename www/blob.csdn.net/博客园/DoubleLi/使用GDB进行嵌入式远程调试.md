# 使用GDB进行嵌入式远程调试 - DoubleLi - 博客园






PC主机：Ubuntu 10.4 

目标板：TQ2440开发板，linux内核2.6.30

NOTE：为了使用gdb进行调试，强烈建议使用nfs服务，否则调试会非常麻烦。

使用nfs服务可以参考：[S3C2440挂载NFS文件系统](http://blog.csdn.net/yj4231/article/details/7742804)

## 1. 概述

所谓远程调试，就是开发板上建立一个gdb服务端，同时待调试的程序也位于开发板，然后在PC机上使用gdb发起远程连接来进行调试。也就是说，在PC端调试开发板上的程序。请注意，在PC端需要包含被调试程序的符号调试信息（symbolic debug information），因此强烈建议使用NFS，否则需要两份被调试的应用程序，一份供gdb使用，另一份供gdbserver使用。

## 2. 安装GDB服务

下载gdb安装包，下载地址：[http://ftp.gnu.org/gnu/gdb/](http://http//ftp.gnu.org/gnu/gdb/)

这里使用的是7.4。

解压安装包：

yj423@ubuntu:~/work_yj423$ tar -xvf gdb-7.4/

yj423@ubuntu:~/work_yj423/gdb-7.4$ pwd
/home/yj423/work_yj423/gdb-7.4

在开始编译之前，你必须知道你的交叉编译器的名字。我的交叉编译器是arm-unknown-linux-gnueabi-gcc，在下面将会用到该名字。



首先编译gdbserver，该程序运行在开发板上。

执行下列命令：

yj423@ubuntu:~/work_yj423/gdb-7.4$ cd gdb/gdbserver/
yj423@ubuntu:~/work_yj423/gdb-7.4/gdb/gdbserver$ ./configure --host=arm-unknown-linux-gnueabi --target=arm-unknown-linux-gnueab
yj423@ubuntu:~/work_yj423/gdb-7.4/gdb/gdbserver$ make

在make以后，会在当前目录下生成可执行文件gdbserver。



接着编译gdb，该程序运行在PC机上。

执行下列命令：

yj423@ubuntu:~/work_yj423/gdb-7.4/gdb/gdbserver$ cd ../../
yj423@ubuntu:~/work_yj423/gdb-7.4$ sudo ./configure --targe=arm-unknown-linux-gnueabi
yj423@ubuntu:~/work_yj423/gdb-7.4$ make

请注意执行命令时所在的目录。执行完以后会在gdb-7.4/gdb/下生成可执行文件gdb。我将gdb改命为armgdb。

## 3. 使用gdb调试

测试程序为hello.c，程序如下：





**[cpp]**[view plain](http://blog.csdn.net/yj4231/article/details/7829686)[copy](http://blog.csdn.net/yj4231/article/details/7829686)



- #include <stdio.h>  
- 
- void main()  
- {  
-     printf("hello world\n");  
-     printf("hello world\n");  
- }  



使用交叉编译器编译该文件，使用-g参数，生成hello。将gdbserver和hello复制到NFS的挂载点，我的挂载点为/home/yj423/nfswork。



yj423@ubuntu:~/nfswork$ ls
bin   dev  gdbserver  home  linuxrc  proc  sbin  tmp  var
boot  etc  hello      lib   mnt      root  sys   usr  welcome

可以看到gdbserver和hello。

接着，在开发板上使用NFS：
[root@yj423 /]#mount -o nolock 192.168.1.103:/home/yj423/nfswork /mnt/nfs
[root@yj423 /]#cd /mnt/nfs
[root@yj423 nfs]#ls
bin        dev        gdbserver  home       linuxrc    proc       sbin       tmp        var
boot       etc        hello      lib        mnt        root       sys        usr        welcome



然后执行gdbserver：

[root@yj423 nfs]#./gdbserver localhost:2001 hello
Process hello created; pid = 948
Listening on port 2001

2001为端口号，hello表示要调试的程序。此时gdbserver等待PC机进行链接。



在PC机上执行gdb：




yj423@ubuntu:~$ ./armgdb -q /home/yj423/nfswork/hello

Reading symbols from /home/yj423/nfswork/hello...done.

执行远程链接：

(gdb) target remote 192.168.1.6:2001
Remote debugging using 192.168.1.6:2001
warning: Unable to find dynamic linker breakpoint function.
GDB will be unable to debug shared library initializers
and track explicitly loaded dynamic code.
0x400007b0 in ?? ()
(gdb)

这里的192.168.1.6为开发板的IP地址。

至此PC端的gdb和开发板的gdbserver已经建立连接，接下来可以调试。

(gdb) b main
Cannot access memory at address 0x0
Breakpoint 1 at 0x83e0: file hello.c, line 5.
(gdb) c
Continuing.
warning: `/lib/libc.so.6': Shared library architecture unknown is not compatible with target architecture arm.
warning: Could not load shared library symbols for /lib/ld-linux.so.3.
Do you need "set solib-search-path" or "set sysroot"?
Breakpoint 1, main () at hello.c:5
5        printf("hello world\n");
(gdb) n
6        printf("hello world\n");
(gdb) n
7    }

这里只是简单的调试。后面还会有共享库调试和多进程调试，尽请期待!

未完待续~~~~~~~~~









