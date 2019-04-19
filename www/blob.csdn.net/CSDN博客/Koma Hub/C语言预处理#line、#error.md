# C语言预处理#line、#error - Koma Hub - CSDN博客
2017年11月30日 20:06:49[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：174
#line 的作用是改变当前行数和文件名称，它们是在编译程序中预先定义的标识符命令的基本形式如下：
```cpp
#line number["filename"]
```
其中[]内的文件名可以省略。例如：
```cpp
#line 30 a.h //其中，文件名a.h 可以省略不写。
```
这条指令可以改变当前的行号和文件名，例如上面的这条预处理指令就可以改变当前的行号为30，文件名是a.h。初看起来似乎没有什么用，不过，他还是有点用的，那就是用在编译器的编写中，我们知道编译器对C 源码编译过程中会产生一些中间文件，通过这条指令，可以保证文件名是固定的，不会被这些中间文件代替，有利于进行分析。
```cpp
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define pi 3.14
#undef pi
int a = 0;
void show(){
    a++;
    printf("hello: %d\n",a);
}
void main(int *argv, char *args[]){
    /**
    __DATE__, __TIME__ 
    */
    printf("current line number %d ,current file %s:%s\n",__LINE__,__FILE__, __FUNCTION__);
      
    #line 10 "Demo_01_pointer_ifdef_enum.c"
    printf("current line number %d ,current file %s\n",__LINE__,__FILE__);
}
```
编译与运行：
```
-bash-4.1$ gcc -o a Demo_06_#line__LINE__FILE__FUNCTION__TIME__DATA__.c
-bash-4.1$ ./a
current line number 20 ,current file Demo_06_#line__LINE__FILE__FUNCTION__TIME__DATA__.c:main
current line number 10 ,current file Demo_01_pointer_ifdef_enum.c
```
**#error**
如果程序中有#error这个预处理指令，那么程序在编译阶段就会直接报错：
```cpp
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define PI 3.14
#undef pi
int a = 0;
void show(){
    a++;
    printf("hello: %d\n",a);
}
void main(int *argv, char *args[]){
#ifndef PI  
#error First include then compile  
#else 
    show();
#endif
#ifdef pi
#error pi was defined
#else 
#error pi wasnt defined
#endif
}
```
编译：
```cpp
-bash-4.1$ gcc -o a Demo_07_#error.c
Demo_07_#error.c:26:2: 错误：#error pi wasnt defined
-bash-4.1$
```
