# Linux gcc 使用以及库文件的编译 - BlueSky - CSDN博客
2015年08月14日 20:27:29[ASIA_kobe](https://me.csdn.net/ASIA_kobe)阅读数：632
- 预处理, 生成 .i 的文件 [ 调用预处理器 cpp]
- 将预处理后的文件转换成汇编语言 , 生成文件 .s[ 调用编译器 gcc/egcs]
- 有汇编变为目标代码 ( 机器代码 ) 生成 .o 的文件 [ 调用汇编器 as]
- 连接目标代码 , 生成可执行程序 [ 调用链接器 ld]
gcc  -E hello.c -o hello.i     —预处理 
gcc  -S hello.i -o hello.s     —编译 
gcc  -c hello.s -o hello.o     —汇编 
gcc  hello.o  -o  hello         —链接
gcc  hello.c -o hello            —直接编译链接生成可执行文件 
gcc  -c hello.c 或者 gcc -c hello.c -o hello    —编译生成可重定位目标文件
-Wall 
    生成所有警告信息。一下是具体的选项，可以单独使用 
-lxx 
    表示动态加载libxx.so库 
-Lxx 
    表示增加目录xx，让编译器可以在xx下寻找库文件 
-Ixx 
   表示增加目录xx，让编译器可以在xx下寻找头文件 
   -w 
    不生成任何警告信息。 
有关宏定义的选项 
有两种定义的宏的方式：1：其他原文件中；2：在GCC的命令行中使用 -Dxxx 
当这些宏被定义后（#define xxx） 
在系统中使用时：#ifdef xxx 就会被预处理器扩展为有效代码；在系统中已经定义了一些系统命名空间内的宏，都是以  __ 开头的 （两条下划线） 
使用命令：cpp -dM /dev/null 可以查看到所有的预定义宏 
-g 
　　只是编译器，在编译的时候，产生调试信息。 
-m 
  选择不同的硬件型号 或 配置 — 例如, 68010 还是 68020, 有没有浮点协处理器. 通过指定选项, 安装编译器的一个版本能够为所有的型号或配置进行编译.有很多选项，这里不列举，如 -mshort，认为int类型为16bit 
**头文件**
头文件通常用来提供对常数的定义和对系统及库函数调用的声明。 
**库文件**
库文件是一些预先编译好的函数集合，那些函数都是按照可重用原则编写的。它们通常由一组互相关联的用来完成某项常见工作的函数构成。 
**头文件与库文件位置**
/usr/include以及子目录底下的include文件夹 
/usr/local/include以及子目录底下的include文件夹 
/usr/lib 
/usr/local/lib
gcc -wall main.c -o main -lm  —其中-l为引用其他的库 比如libm.so 则直接为-lm(lib省略)
**库搜索路径**
**静态库(.a)**
程序在编译链接的时候把哭的代码链接到可执行文件中。程序运行时讲不再需要静态库。 
ar是gnu归档工具，rcs表示（replace and create） 
比如：ar rcs libhello.a hello_1.o hello_2.o … 
  —生成了libhello.a静态库 （可以看成是.o文件的集合）（一般库文件为lib开始） 
  gcc -wall -L. main.c -o mian -lhello     —其中-L.表示在当前路径下查找库 
**共享库(.so或.sa)动态库**
程序在运行的时候才去链接共享库的代码，多个程序共享库的代码。一个与共享库链接的可执行文件仅仅包含它用到的函数入口地址的一个表，而不是外部函数所在目标文件的整个机器码。在可执行文件开始运行以前，尾部函数的机器码由该操作系统从磁盘上的该共享库中复制到内存中，这个过程称为动态链接库。 
**生成共享库**
shared：表示生成共享库格式 
fPIC：表示位置无关码 
gcc -shared -fPIC hello.o … -o libhello.so *验证下*
*运行共享库*
1、拷贝.so文件到系统共享库路径下，一般指/usr/lib 
2、更改LD_LIBRARY_PATH（即在~/.bansh_profile 添加路径 然后source下） 
3、配置ldconfig（即配置ld.so.conf）
