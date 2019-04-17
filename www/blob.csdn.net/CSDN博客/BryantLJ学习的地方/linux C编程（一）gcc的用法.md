# linux C编程（一）gcc的用法 - BryantLJ学习的地方 - CSDN博客





2016年08月03日 16:36:15[遍地流金](https://me.csdn.net/u012177034)阅读数：783








**一.编译环境搭建**


C/C++的编译环境：make工具，编译器gcc，调试器gdb，C/C++标准库。常用库信息列表build-essential包含gcc，g++，make，libc6-dev等写程序时基本会用到的工具，因此只安装build-essential，gcc，make等工具也会自动安装。不清楚的话全部都apt-get即可。


`sudo apt-get install gdb build-essential`


装完以后，查看gcc,make,gdb等版本



```
bryant@bryant:~$ gcc -v
Using built-in specs.
COLLECT_GCC=gcc
COLLECT_LTO_WRAPPER=/usr/lib/gcc/x86_64-linux-gnu/4.8/lto-wrapper
Target: x86_64-linux-gnu
Configured with: ../src/configure -v --with-pkgversion='Ubuntu 4.8.4-2ubuntu1~14.04.3' --with-bugurl=file:///usr/share/doc/gcc-4.8/README.Bugs --enable-languages=c,c++,java,go,d,fortran,objc,obj-c++ --prefix=/usr --program-suffix=-4.8 --enable-shared --enable-linker-build-id --libexecdir=/usr/lib --without-included-gettext --enable-threads=posix --with-gxx-include-dir=/usr/include/c++/4.8 --libdir=/usr/lib --enable-nls --with-sysroot=/ --enable-clocale=gnu --enable-libstdcxx-debug --enable-libstdcxx-time=yes --enable-gnu-unique-object --disable-libmudflap --enable-plugin --with-system-zlib --disable-browser-plugin --enable-java-awt=gtk --enable-gtk-cairo --with-java-home=/usr/lib/jvm/java-1.5.0-gcj-4.8-amd64/jre --enable-java-home --with-jvm-root-dir=/usr/lib/jvm/java-1.5.0-gcj-4.8-amd64 --with-jvm-jar-dir=/usr/lib/jvm-exports/java-1.5.0-gcj-4.8-amd64 --with-arch-directory=amd64 --with-ecj-jar=/usr/share/java/eclipse-ecj.jar --enable-objc-gc --enable-multiarch --disable-werror --with-arch-32=i686 --with-abi=m64 --with-multilib-list=m32,m64,mx32 --with-tune=generic --enable-checking=release --build=x86_64-linux-gnu --host=x86_64-linux-gnu --target=x86_64-linux-gnu
Thread model: posix
gcc version 4.8.4 (Ubuntu 4.8.4-2ubuntu1~14.04.3)
```


```
bryant@bryant:~$ make -v
GNU Make 3.81
Copyright (C) 2006  Free Software Foundation, Inc.
This is free software; see the source for copying conditions.
There is NO warranty; not even for MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE.

This program built for x86_64-pc-linux-gnu
```





本文重点讲述gcc




**二.g++与gcc的区别**



gcc，全称：GNU Compile Collection，是一组编译器集合，可以编译c，c++，object-c，java等。其功能是实现一个源代码文件向可执行文件的转换。


g++等效于 gcc -xc++ -lstdc++ -shared -libgcc

其中-x选项是指定gcc编译器的编译语言，由上述等价关系可知，

gcc对于不同的文件，可根据其后缀名采用不同编译规则，如.c，.cpp文件，而g++是将所有文件都当做c++来进行编译

由上述可知只需要gcc即可




**三.细说gcc**

gcc的对于hello.c编译流程分为四部分：

(1)预处理(Pre-Processing) 
hello.c -> hello.i 替换宏及头文件等


(2)编译(Compile) hello.i -> hello.s  将.c代码转为汇编代码


(3)汇编(Assembler) hello.s ->hello.o 将汇编代码转为二进制代码，该二进制代码中可能会引用其他.c或库里的函数，但是只是一个标志，没有具体的位置


(4)链接(Link) hello.o -> hello 将hello.o里引用的其他地方的函数和其他.c文件对应的.o全部链接为一个科执行文件




在链接外部库的时候有两种方式，一种是链接动态库.so（shared object），一种是静态库.a(Archive)，gcc -static 此选项会禁止使用动态库，因此编译完后的文件一般会比较大，运行时也不需要动态库；gcc -share将尽量使用动态库，生成文件较小，但运行时会需要动态库。




**在指定-llib编译源文件时，默认为shared参数，即默认为查找lib.so的共享库，编译时不讲lib链接进去；若显式指定-static参数，则首先查找lib.a静态库，然后编译时将静态库里的函数链接进去！**





其余常用gcc选项：




-c 激活前三步，生成.o文件 如gcc -c hello.c

-o 指定目标名称，如gcc -c hello.c -o hello.o

-Wall 显示警告信息

-O0 -O1 -O2 -O3 编译器的优化选项，-O0表示没有优化，-O1为default值，-O3优化级别最高




-llibrary 指定链接时使用library库，如gcc  hello.c -o hello -lcurses 表示将hello.c编译链接为hello可执行文件，且链接的库使用libcurses.so库

-Ldir 指定链接时寻找库文件的路径，否则gcc只会在默认的几个路径中寻找库（如/usr/lib等），一般用在使用自己的库或者第三方库时会使用

-include file指定编译某个.c时包含file头文件，其功能相当于在.c前端#include file

-Idir 指定编译某个.c时先在dir文件夹里寻找头文件，然后再去默认目录寻找

linux默认系统路径

头文件：/usr/include/  /usr/local/include/


库文件：/usr/lib/  /usr/local/lib/





-Dmacro 在编译该文件时定义macro宏

-Dmacro=defn 在编译该文件时定义macro宏=defn

-Umacro 在编译该文件时undef macro




-x language filename 设定文件所使用的语言，language可为c,c++等




