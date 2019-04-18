# C语言移位知识小结 - 小灰笔记 - CSDN博客





2017年03月19日 18:37:20[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：576








       关于C语言移位功能的知识小结如下：

              1，进行2的n次方计算，使用移位效率会提升很多；

              2，移位有时候会导致数据丢失，但有时候这正是我们要的功能；

              3，右移位操作是不可移植的；

              4，移位的操作符好是负数的时候，行为不确定。

       关于第4点进行一点简单的验证，写代码如下：

#include"stdio.h"



int main(void)

{

       unsigned int a = 1;



       printf("left move 5 bit:%d\n",a << 5);

       printf("left move -5 bit:%d\n",a << -5);



       return 0;

}

       代码编译运行结果：

E:\WorkSpace\02_技术实践\01_编程语言\01_C语言\02_C和指针\exp02>gcc exp02.c

exp02.c: Infunction 'main':

exp02.c:8:2:warning: left shift count is negative

  printf("left move -5 bit: %d\n",a<< -5);

  ^



E:\WorkSpace\02_技术实践\01_编程语言\01_C语言\02_C和指针\exp02>a

left move 5 bit:32

left move -5 bit:134217728



E:\WorkSpace\02_技术实践\01_编程语言\01_C语言\02_C和指针\exp02>python

Python 3.6.0(v3.6.0:41df79263a11, Dec 23 2016, 08:06:12) [MSC v.1900 64 bit (AMD64)] onwin32

Type"help", "copyright", "credits" or"license" for more information.

>>>bin(134217728)

'0b1000000000000000000000000000'

>>>len(bin(134217728)) - 2

28

>>> 

       专门加入了Python处理了一下结果，得出结论如下：

       1，移位操作符是5的时候，计算正常没有什么需要探讨的；

       2，移位操作数是5的时候，实际的运算是左移操作了27位，这事儿很诡异的用法。很多书中会把这种操作定义为一种未定义的操作，因为不同的平台不同编译器会有不同的行为。如果非得找什么规律，单纯的一个验证能够脑补出来的也不过是27 + 5的值为32，正好是int长度。

       增加一段代码测试，代码如下;

#include"stdio.h"



int main(void)

{

       unsigned int a = 1;



       printf("left move 6 bit:%d\n",a << 6);

       printf("left move -6 bit:%d\n",a << -6);



       return 0;

}

       编译与运行结果：

E:\WorkSpace\02_技术实践\01_编程语言\01_C语言\02_C和指针\exp02>gcc exp02.c

exp02.c: Infunction 'main':

exp02.c:8:2:warning: left shift count is negative

  printf("left move -6 bit: %d\n",a<< -6);

  ^



E:\WorkSpace\02_技术实践\01_编程语言\01_C语言\02_C和指针\exp02>a

left move 6 bit:64

left move -6 bit:67108864



E:\WorkSpace\02_技术实践\01_编程语言\01_C语言\02_C和指针\exp02>python

Python 3.6.0(v3.6.0:41df79263a11, Dec 23 2016, 08:06:12) [MSC v.1900 64 bit (AMD64)] onwin32

Type"help", "copyright", "credits" or"license" for more information.

>>>len(bin(67108864)) - 2 - 1

26

>>> 

       从上面的结果还真看出了这个结论，如此初步得出的结论是：在Windows平台下使用gcc编译的时候这个功能类似于循环右移。而我用的操作系统是Windows 10 HB，而编译器的具体版本信息如下：

Using built-inspecs.

COLLECT_GCC=gcc

COLLECT_LTO_WRAPPER=C:/Strawberry/c/bin/../libexec/gcc/i686-w64-mingw32/4.9.2/lto-wrapper.exe

Target:i686-w64-mingw32

Configured with:../../../src/gcc-4.9.2/configure --host=i686-w64-mingw32--build=i686-w64-mingw32 --target=i686-w64-mingw32 --prefix=/mingw32--with-gxx-include-dir=/mingw32/i686-w64-mingw32/include/c++ --enable-shared--enable-static --disable-multilib --enable-languages=c,c++,fortran,lto--enable-libstdcxx-time=yes
 --enable-threads=posix --enable-libgomp--enable-libatomic --enable-lto --enable-graphite --enable-checking=release--enable-fully-dynamic-string --enable-version-specific-runtime-libs--enable-sjlj-exceptions --disable-isl-version-check--disable-cloog-version-check
 --disable-libstdcxx-pch --disable-libstdcxx-debug--disable-bootstrap --disable-rpath --disable-win32-registry --disable-nls--disable-werror --disable-symvers --with-gnu-as --with-gnu-ld --with-arch=i686--with-tune=generic --with-libiconv --with-system-zlib--with-gmp=/opt/build/prerequisites/i686-w64-mingw32-static--with-mpfr=/opt/build/prerequisites/i686-w64-mingw32-static--with-mpc=/opt/build/prerequisites/i686-w64-mingw32-static--with-isl=/opt/build/prerequisites/i686-w64-mingw32-static--with-cloog=/opt/build/prerequisites/i686-w64-mingw32-static--enable-cloog-backend=isl
 --with-pkgversion='i686-posix-sjlj, built bystrawberryperl.com project' CFLAGS='-O2 -pipe-I/opt/build/i686-492-posix-sjlj-rt_v402/mingw32/opt/include-I/opt/build/prerequisites/i686-zlib-static/include-I/opt/build/prerequisites/i686-w64-mingw32-static/include'
 CXXFLAGS='-O2 -pipe-I/opt/build/i686-492-posix-sjlj-rt_v402/mingw32/opt/include-I/opt/build/prerequisites/i686-zlib-static/include-I/opt/build/prerequisites/i686-w64-mingw32-static/include' CPPFLAGS=LDFLAGS='-pipe -L/opt/build/i686-492-posix-sjlj-rt_v402/mingw32/opt/lib-L/opt/build/prerequisites/i686-zlib-static/lib
 -L/opt/build/prerequisites/i686-w64-mingw32-static/lib-Wl,--large-address-aware'

Thread model:posix

gcc version 4.9.2(i686-posix-sjlj, built by strawberryperl.com project)



