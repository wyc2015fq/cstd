# GCC——C程序是如何编译成的 - ljx0305的专栏 - CSDN博客
2008年12月18日 11:30:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：1274标签：[gcc																[c																[编译器																[fckeditor																[include																[function](https://so.csdn.net/so/search/s.do?q=function&t=blog)](https://so.csdn.net/so/search/s.do?q=include&t=blog)](https://so.csdn.net/so/search/s.do?q=fckeditor&t=blog)](https://so.csdn.net/so/search/s.do?q=编译器&t=blog)](https://so.csdn.net/so/search/s.do?q=c&t=blog)](https://so.csdn.net/so/search/s.do?q=gcc&t=blog)
个人分类：[C++																[C](https://blog.csdn.net/ljx0305/article/category/394717)](https://blog.csdn.net/ljx0305/article/category/380566)
**摘要：**
    在windows环境，我们有集成开发环境（IDE），使得我们对编译器了解的很少。当我们专向linux时需要在命令行下编译自己的程序需要对编译器的 命令行参数比较熟悉。而如果是做嵌入开发构建自己的操作系统时失去了系统平台，需要我们对编译的过程以及可执行文件的内部结构有所了解。本文讲述了如何编 译可执行文件、静态库、动态库，如何优化编译器，如何编译无操作系统环境下的程序（自己的OS）等。
```
1.分析普通的helloworld程序
    先书写一下一个简单的helloworld程序如下：
/* hello.c */
int main(int argc, char * argv[])
{
    return 0
}
    编译程序：
gcc -o hello hello.c
    等价的编译方法：
gcc -c hello.c
gcc -o hello.my -nostartfiles hello.o /usr/lib/crt1.o /usr/lib/crti.o /usr/lib/crtn.o
    我在redflag workstation 5.0版本下用3.4.3版本GCC编译器编译出来的大小都是3589字节，并且用diff命令比较为相同的文件。由此证明gcc在编译并链接 hello.c文件时先将hello.c编译成hello.o，然后将它与crt1.o、crti.o、crtn.o链接在一起。
    如果要写一个没有main函数的程序，就需要自己实现crt1.o。程序入口为符号“_start”处。代码如下：
/* nomain.c */
void _start(void)
{
    _exit(0);
}
gcc -o nomain -nostartfiles hello.c
2.编译和使用库
    除了直接编译代码外还会使用到函数库。库有静态库和动态库之分。静态库是以.a结尾的文件，例如：libXXX.a；动态库是以.so结尾的文件，例如： libXXX.so。XXX之后或者.so和.a之后会跟版本号，例如：libc-2.3.4.so，或者libuuid.so.1.2。
    加入函数库的代码如下：
/* test.h */
int test1 (void);
/* test.c */
#include <stdio.h>
#include "libtest.h"
int test1 (void)
{
    printf("test1 function is called./n");
    return 0;
}
    当要编译成静态库的时候使用如下命令生成libtest.a：
gcc -c test.c
ar -r libtest.a test.o
    当要编译成动态链接库的时候使用如下命令生成libtest.so
gcc -c -fpic test.c
gcc -shared test.o -o libtest.so
或
gcc -fpic -shared test.c -o libtest.so
    当需要调用该库时使用如下代码。
/* calllib.c */
#include "test.h"
int main (int argc, char * argv[])
{
    test1();
}
    按如下方式编译：
gcc -o calllib.static calllib.c libtest.a
或
gcc -o calllib.dynamic calllib.c libtest.so
或
gcc -o calllib calllib.c -ltest
    前提是libtest.a或libtest.so在当前编译的目录。
    使用ldd显示calllib.static依赖的库只有libc和ld-linux，但calllib.dynamic就多了一个libtest.so。
3.库文件和头文件
    在程序中，使用#include <stdio.h>类似的头文件stdio.h在编译器的头文件路径中，#include "abc.h"中的stdio.h文件则应该在当前目录。通过对编译器指定参数-I<PATH>来指定头文件所在目录，可以用 #include <>来引用。例如：gcc -I./include hello.c，将从当前目录下的include目录下去寻找头文件。
    同理，程序中调用的库函数在编译时也需要指定路径，同时指定库。使用-L<PATH>参数指定库文件的目录，-l<FILE>指定包含的库文件。例如，要使用libXXX.so库，参数为-lXXX。
    一般一个库编译完成后有库文件和头文件。如果要使用这个库，可以将库文件目录和头文件目录分别用-I和-L参数指定，也可以将他们拷贝到编译器的include和lib目录下。
4.缩小程序体积
    代码如下：
/* nomain.c */
void _start(void)
{
    asm("movl , eax/n"
        "movl {fckeditor}, ebx/n"
        "int {fckeditor}x80"
    );
}
使用如下方法能使可执行文件体积最小（手工方法除外）。
gcc -o nomain -s -O3 nomain.c
objcopy -R .comment -R .data
    由此得到可执行文件的体积为352字节。
    GCC能编译出2种格式的可执行体：a.out格式、elf格式。其中.o目标文件、.a静态库文件
Appendix.1 常用命令
ldd：显示可执行文件或者库文件依赖的库文件。
objdump：显示elf可执行文件的内部信息。
    -h：显示
    -t：显示符号信息
    -T：显示动态符号信息。（例如引用动态链接库中的函数名称）
    -r：显示重定位入口信息。
    -R：显示动态重定位入口信息。（例如：动态链接库中的变量或者函数地址）
    -s：显示所有section内容。
    -S：反汇编代码段。
objcopy：copy elf文件内容
    -R：删除某个section
    -j：仅仅复制指定的section
```
引用:http://blog.chinaunix.net/u/1028/showart.php?id=1668046
