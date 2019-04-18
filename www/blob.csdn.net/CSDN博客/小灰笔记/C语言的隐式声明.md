# C语言的隐式声明 - 小灰笔记 - CSDN博客





2017年03月17日 00:29:13[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：523








       C语言中有几种声明类型可以省略，例如：函数不显示地声明返回值类型，它就默认返回整型。写如下代码：

#include"stdio.h"



Demo(void)

{

       return 8;

}



main(void)

{

       printf("return value of function:%d\n",Demo());

       return 0;

}

       编译运行结果：

E:\WorkSpace\01_编程语言\01_C语言\03_declare>gcc demo.c



E:\WorkSpace\01_编程语言\01_C语言\03_declare>a

return value offunction: 8

       通过上面可以得出结论：

              1，代码没问题，可以编译且运行；

              2，Demo隐式声明，但是成功返回了整型；

              3，主函数本该是int类型，也可以省略。

       再写点代码看一下变量的隐式声明：

#include"stdio.h"



int a;

b[10];



int main(void)

{

       printf("size of a:%d\n",sizeof(a));

       printf("size of array b:%d\n",sizeof(b));



       return 0;

}

       代码编译并运行：

E:\WorkSpace\01_编程语言\01_C语言\03_declare>gcc exp_31.c

exp_31.c:4:1:warning: data definition has no type or storage class

 b[10];

 ^



E:\WorkSpace\01_编程语言\01_C语言\03_declare>a

size of a: 4

size of array b:40

       其实，这种默认的隐式声明是K &R标准所支持的，我使用的编译器是gcc，虽说是报出了警告，但是依然可以编译通过并执行。编译器信息如下：

E:\WorkSpace\01_编程语言\01_C语言\03_declare>gcc -v

Using built-inspecs.

COLLECT_GCC=gcc

COLLECT_LTO_WRAPPER=C:/Strawberry/c/bin/../libexec/gcc/i686-w64-mingw32/4.9.2/lto-wrapper.exe

Target:i686-w64-mingw32

Configured with:../../../src/gcc-4.9.2/configure --host=i686-w64-mingw32--build=i686-w64-mingw32 --target=i686-w64-mingw32 --prefix=/mingw32--with-gxx-include-dir=/mingw32/i686-w64-mingw32/include/c++ --enable-shared--enable-static --disable-multilib --enable-languages=c,c++,fortran,lto--enable-libstdcxx-time=yes
 --enable-threads=posix --enable-libgomp--enable-libatomic --enable-lto --enable-graphite --enable-checking=release--enable-fully-dynamic-string --enable-version-specific-runtime-libs--enable-sjlj-exceptions --disable-isl-version-check--disable-cloog-version-check
 --disable-libstdcxx-pch --disable-libstdcxx-debug--disable-bootstrap --disable-rpath --disable-win32-registry --disable-nls--disable-werror --disable-symvers --with-gnu-as --with-gnu-ld --with-arch=i686--with-tune=generic --with-libiconv --with-system-zlib--with-gmp=/opt/build/prerequisites/i686-w64-mingw32-static--with-mpfr=/opt/build/prerequisites/i686-w64-mingw32-static--with-mpc=/opt/build/prerequisites/i686-w64-mingw32-static--with-isl=/opt/build/prerequisites/i686-w64-mingw32-static--with-cloog=/opt/build/prerequisites/i686-w64-mingw32-static
 --enable-cloog-backend=isl--with-pkgversion='i686-posix-sjlj, built by strawberryperl.com project'CFLAGS='-O2 -pipe -I/opt/build/i686-492-posix-sjlj-rt_v402/mingw32/opt/include-I/opt/build/prerequisites/i686-zlib-static/include-I/opt/build/prerequisites/i686-w64-mingw32-static/include'
 CXXFLAGS='-O2 -pipe-I/opt/build/i686-492-posix-sjlj-rt_v402/mingw32/opt/include-I/opt/build/prerequisites/i686-zlib-static/include-I/opt/build/prerequisites/i686-w64-mingw32-static/include' CPPFLAGS=LDFLAGS='-pipe -L/opt/build/i686-492-posix-sjlj-rt_v402/mingw32/opt/lib-L/opt/build/prerequisites/i686-zlib-static/lib-L/opt/build/prerequisites/i686-w64-mingw32-static/lib-Wl,--large-address-aware'

Thread model:posix

gcc version 4.9.2(i686-posix-sjlj, built by strawberryperl.com project)



