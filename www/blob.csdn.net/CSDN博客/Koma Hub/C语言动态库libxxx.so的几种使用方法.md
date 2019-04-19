# C语言动态库libxxx.so的几种使用方法 - Koma Hub - CSDN博客
2018年08月26日 10:22:25[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：435
首先先编写一个简单的函数func.h func.c
func.h
```cpp
#ifndef _FUNC_H
#define _FUNC_H
#include <stdio.h>
#include <stdlib.h>
void fun(char *str);
#endif
```
func.c
```cpp
#include <func.h>
void fun(char *str)
{
	printf("%s\n", str);
	
}
```
将这个函数编译成动态库
```bash
$ gcc func.c -I . -shared -fPIC -o libfunc.so
$ ls
func.c*  func.h*  libfunc.so*  main.c*
```
编写main.c程序
```
#include <func.h>
int main(int argc, char *argv[])
{
	fun("Hello World!\n");
	
	return 0;
}
```
# **编译方法一：**
```cpp
$ gcc main.c -I . ./libfunc.so
$ ./a.exe
Hello World!
```
查看可执行文件依赖：
```bash
$ ldd a.exe
        ntdll.dll => /cygdrive/c/Windows/SYSTEM32/ntdll.dll (0x77d20000)
        KERNEL32.DLL => /cygdrive/c/Windows/System32/KERNEL32.DLL (0x779b0000)
        KERNEL32.DLL => /cygdrive/c/Windows/System32/KERNEL32.DLL (0x779b0000)
        KERNELBASE.dll => /cygdrive/c/Windows/System32/KERNELBASE.dll (0x74e40000)
        libfunc.so => /home/rongtao/libfunc.so (0x690c0000)
        cygwin1.dll => /usr/bin/cygwin1.dll (0x61000000)
```
里面依赖的dll文件是因为我用的cywin，是windows下面的Linux仿真软件，所以会用到，我们主要关注这个libfunc.so库文件，关注它后面的路径，可以看到这个地方的路径即为编译时的路径。
我们再看看这个动态库内部的函数关系：
```bash
$ nm libfunc.so | grep T --color=auto
690c1690 T ___CTOR_LIST__
690c1104 T ___cxa_atexit
690c169c T ___DTOR_LIST__
690c10c0 T ___gcc_deregister_frame
690c1000 T ___gcc_register_frame
690c3264 R ___RUNTIME_PSEUDO_RELOC_LIST__
690c3264 R ___RUNTIME_PSEUDO_RELOC_LIST_END__
690c1690 T __CTOR_LIST__
690c12d0 T __cygwin_crt0_common@8
690c1150 T __cygwin_dll_entry@12
690c1250 T __cygwin_noncygwin_dll_entry@12
```
nm指令是查看库文件的声明、未声明的函数等作用。在编程过程中我们经常会遇到“未引用的定义”，这个其实就可以用nm命令查看。在这里我想说个题外话，消息队列的库是“-lrt”，之前编译的N多变没通过的原因就是没加这个库。
编译方法二：
如果我们把这个文件家内的libfunc.so文件删除，再运行可执行文件会出现如下错误：
```bash
$ ls
a.exe*  func.c*  func.h*  main.c*
$ ./a.exe
D:/Program Files (x86)/cygwin/home/rongtao/a.exe: error while loading shared libraries: libfunc.so: cannot open shared object file: No such file or directory
```
无法加载动态库文件。
这里我用的cywin，所以说和一般的Linux系统不一样，没法演示，具体流程是：
如果系统是x86_64的，那么在/usr/lib64文件夹下为系统库，这个库是可以直接调用的（例如我们的-lm， -pthread，-lrt等），如果我们将动态库文件放到这个文件夹下就可以直接用“-lfunc”来调用。此处不做过多讲解。
