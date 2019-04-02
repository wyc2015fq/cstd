# configure使用说明

2018年11月30日 16:06:45 [knowledgebao](https://me.csdn.net/knowledgebao) 阅读数：67



编译相关索引：<https://blog.csdn.net/knowledgebao/article/details/84973055>

------

./configure常用参数解释：

具体通过–help来查看具体支持什么功能。有时候编译不通过，可能依赖一些库，如果这些库关联的功能我们不需要，可以通过---disable-*lib来取消相关库的编译。

**--disable-FEATURE**
　　一些软件包可以选择这个选项来提供为大型选项的编译时配置,例如使用Kerberos认证系统或者一个实验性的编译器最优配置.如果默认是提供这些特性,可以使用'--disable-FEATURE'来禁用它,这里'FEATURE'是特性的名字.例如:
$ ./configure --disable-gui

**-enable-FEATURE[=ARG]**
　　相反的,一些软件包可能提供了一些默认被禁止的特性,可以使用'--enable-FEATURE'来起用它.这里'FEATURE'是特性的名字.一个特性可能会接受一个可选的参数.例如:
$ ./configure --enable-buffers=128
`--enable-FEATURE=no'与上面提到的'--disable-FEATURE'是同义的.

**--with-PACKAGE[=ARG]**
　　在自由软件社区里,有使用已有软件包和库的优秀传统.当用'configure'来配置一个源码树时,可以提供其他已经安装的软件包的信息.例如,倚赖于Tcl和Tk的BLT器件工具包.要配置BLT,可能需要给'configure'提供一些关于我们把Tcl和Tk装的何处的信息:
$ ./configure --with-tcl=/usr/local --with-tk=/usr/local
'--with-PACKAGE=no'与下面将提到的'--without-PACKAGE'是同义的.

**--without-PACKAGE**
　　有时候你可能不想让你的软件包与系统已有的软件包交互.例如,你可能不想让你的新编译器使用GNU ld.通过使用这个选项可以做到这一点:
$ ./configure --without-gnu-ld

**--enable-static:**编译静态库

**--enable-shared：**编译动态库

**--with-pic：**仅使用PIC编译，当编译出来的库被其他库调用的时候，其他库使用PIC编译，则依赖的库也需要加PIC

**–prefix=/home/carl：**指定安装位置，如果安装位置不指定，默认路径为/usr/local下的bin、lib等文件夹

**--exec-prefix=EPREFIX**与'--prefix'选项类似,但是他是用来设置结构倚赖的文件的安装位置.编译好的'emacs'二进制文件就是这样一个问件.如果没有设置这个选项的话,默认使用的选项值将被设为和'--prefix'选项值一样.

**--libdir=DIR**,指定库文件的安装位置.

**--includedir=DIR**,指定C头文件的安装位置.其他语言如C++的头文件也可以使用此选项.

**--build**: 执行代码编译的主机，正常的话就是你的主机系统。这个参数一般由config.guess来猜就可以。当然自己指定也可以。可以默认不写，默认为当前正在使用的主机，查看本机使用的可以在configure目录下执行./config.guess

**--host**：就是你编译好的程序可以运行的机器，如果没有指定,将会运行`config.guess’来检测。

**--target**： 这个选项只有在建立交叉编译环境的时候用到，正常编译和交叉编译都不会用到。他用build主机上的编译器，编译一个新的编译器（binutils, gcc,gdb等），这个新的编译器将来编译出来的其他程序将运行在target指定的系统上。如果不编译新的编译器，这个参数可以不填，或者与 host的参数一致。比如你build的机器是x86，想运行的host机器是arm，程序可以处理的程序target是mips

------

下边的选项需要卸载configure前边，比如CFLAGS="-g -O0" ./configure

**CC**：指定编译工具链的名字，指定GCC 交叉编译器命令，如果配置了，则使用CC配置的编译器，如果不配置则默认为host对应的GCC工具。如配置了 --host=arm-xilinx-linux-gnueabi，则默认CC的编译器为 arm-xilinx-linux-gnueabi-gcc，这个参数如无特殊指定，可以忽略不写。

**CFLAGS**：指定头文件（.h文件）的路径，如：CFLAGS=-I/usr/include -I/path/include

CFLAGS="-g -O0" ./configure添加-O0级别的调试信息

**LDFLAGS**：gcc 等编译器会用到的一些优化参数，也可以在里面指定库文件的位置。用法：LDFLAGS=-L/usr/lib -L/path/to/your/lib

**LIBS**：告诉链接器要链接哪些库文件，如LIBS = -lpthread -liconv

其他更详细的参数可以参考：<https://www.xuebuyuan.com/3196393.html>

------

1，为什么自己编译好的64位库没有安装到/usr/lib64目录下

方法一：cp /usr/lib/libXXX.so /usr/lib64/

方法二：./configure --prefix=/usr --libdir=/usr/lib64 --shlibdir=/usr/lib64

 

 

 