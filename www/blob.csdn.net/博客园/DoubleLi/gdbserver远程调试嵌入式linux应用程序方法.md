# gdbserver远程调试嵌入式linux应用程序方法 - DoubleLi - 博客园






此处所讲的是基于gdb和gdbsever的远程调试方法。环境为：PC机：win7、虚拟机：10.04、下位机：飞嵌TE2440开发板。

嵌入式linux应用程序的开发一般都是在linux里面编写好代码，并用交叉编译器arm-linux-gcc编译好之后再下载到板子上运行。如果建立了远程调试（能够在PC机ubuntu中调试板子中的linux应用程序），这样对于开发程序是极其方便的。




以下是摘自网络上的一些方法。其中加入了自己所遇到的一些问题，并提出了解决方法。



1、首先建立nfs文件系统，让PC机ubuntu上的文件系统这里假设路径为：/home/xxx/work/可以挂载到板子上去。方法可以见本博的另一篇文章。其中IP环境是PC机win7的IP为192.168.0.11（这个不重要，只要保证在同一个网段即可）。虚拟机中IP为：192.168.0.57。下位机中的IP为192.168.0.16。



2、安装交叉编译环境arm-linux-gcc等工具。方法可以网上找。这里假设已经安装好，并且版本是3.4.1。路径为：/usr/local/arm/3.4.1/bin。并建好环境变量。



3、下面我们就来测试一个简单的例子。
/*Hello.c*/
#include 
int main()
{
printf(“hello word!\n”);
return 0;
}
程序输好以后确认无误，保存。进入程序文件所在目录
＃ arm-linux-gcc hello.c –o hello
将hello文件放入nfs共享目录中/home/xxx/work/中，并在串口终端中执行hello程序./hello。



4、安装gdb和gdbserver

要经行gdbserver远程调试，还必须安装gdb远程调试工具：
gdb的源代码包可以从http: //ftp.cs.pu.edu.tw/Linux/sourceware/gdb/releases/下载，我下载的是gdb-6.6。下载到某个目录，笔者下载到自己的用户目录
下载完后，进入该目录，配置编译步骤如下：
#tar jxvf gdb-6.6-tar-bz2
#cd gdb-6.6
#./configure --target=arm-linux --prefix=/usr/local/arm-gdb -v
#make

执行到此步之后我出现了如下的错误

cc1: warnings being treated as errors
/root/tools/gdb-6.6/bfd/elf32-arm.c: In function 'find_thumb_glue':
/root/tools/gdb-6.6/bfd/elf32-arm.c:2452: error: ignoring return value of 'asprintf', declared with attribute warn_unused_result
/root/tools/gdb-6.6/bfd/elf32-arm.c: In function 'find_arm_glue':
/root/tools/gdb-6.6/bfd/elf32-arm.c:2485: error: ignoring return value of 'asprintf', declared with attribute warn_unused_result

这个由于返回值没有被接收 导致编译中断 这是因为makefile中加入了-Werror导致将警告当成了错误

将下列文件中的-Werror选项删除后保存即可重新编译成功
bfd/Makefile:185:WARN_CFLAGS = -W -Wall -Wstrict-prototypes -Wmissing-prototypes -Werror
opcodes/Makefile:175:WARN_CFLAGS = -W -Wall -Wstrict-prototypes -Wmissing-prototypes -Werror

接下来执行：

#make install
#export PATH=$PATH:/usr/local/arm-gdb
进入gdbserver目录（在gdb目录下）：
#./configure --target=arm-linux –host=arm-linux
#make CC=/usr/local/arm/3.4.1/bin/arm-linux-gcc
(这一步要指定arm-linux-gcc的位置，可能跟你的不一样)

这时，可能又会遇到一个错误：

/usr/local/arm/3.4.1/bin/arm-linux-gcc -c -Wall -g -O2    -I. -I. -I./../regformats -I./../../include -I../../bfd -I./../../bfd linux-arm-low.c

linux-arm-low.c:35:21: sys/reg.h: No such file or directory

因为sys/reg.h在ubuntu中 /usr/include/sys/reg.h目录中，所以可以加上 -I/usr/include

然后单独运行：/usr/local/arm/3.4.1/bin/arm-linux-gcc -c -Wall -g -O2    -I. -I. -I./../regformats -I./../../include -I../../bfd -I./../../bfd -I/usr/include linux-arm-low.c

OK！！！！！

再运行一遍

#make CC=/usr/local/arm/3.4.1/bin/arm-linux-gcc

没有错误的话就在gdbserver目录下生成gdbserver可执行文件，把它烧写到flash的根文件系统分区，或通过nfs mount的方式都可以。只要保证gdbserver能在开发板上运行就行。



5、安装必要的库libthread_db-1.0.so

如果你的嵌入式linux没有安装libthread_db-1.0.so这个库的话，在板子中运行gdbserver会提示错误。解决方法如下：

将你的arm-linux-gcc的交叉编译环境中的/usr/local/arm/3.4.1/arm-linux/lib下的libthread_db-1.0.so文件拷贝到下位机TE2440开发板的根目录下的/lib中去（可以先放入nfs共享文件夹中然后再在串口终端中输入拷贝命令）。并在/lib目录建立一个一个指向libthread_db-1.0.so的软链接libthread_db.so.1。

命令如下：#ln -s libthread_db-1.0.so libthread_db.so.1

执行完之后，再次运行gdbserver  ./gdbserver之后不会再提示找不到库了。



6、调试hello程序

要进行gdb调试，首先要在目标系统上启动gdbserver服务。在gdbserver所在目录下输入命令：（串口终端中）
#cd /tmp
#./gdbserver 192.168.2.57:2345 hello
192.168.2.57为宿主机IP，在目标系统的2345端口开启了一个调试进程（避免使用1024以下的端口号），hello为要调试的程序。
出现提示：
Process /tmp/hello created: pid="80"
Listening on port 2345

在虚拟机ubuntu中的终端里面输入

#cd /

#export PATH=$PATH:/usr/local/arm-gdb/bin
#arm-linux-gdb hello
(gdb) target remote 192.168.2.223:2345
（192.168.2.223为开发板IP）
出现提示：
Remote debugging using 192.168.2.223:2345
[New thread 80]
[Switching to thread 80]
0x40002a90 in ??()
同时在串口中终端下提示：
Remote debugging from host 192.168.2.100
(gdb)
连接成功，这时候就可以输入各种gdb命令如list、run、next、step、break等进行程序调试了。

至于出现了0x40002a90 in ??()的问题是因为arm-linux-gdb的共享库路径没有进行设置。找不到库文件。通过输入下列语句可以解决问题。

set solib-absolute-prefix /usr/local/arm/4.4.3/arm-none-linux-gnueabi/sys-root/












