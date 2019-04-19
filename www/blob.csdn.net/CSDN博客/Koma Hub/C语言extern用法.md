# C语言extern用法 - Koma Hub - CSDN博客
2017年11月22日 15:07:00[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：2284
**1.用extern修饰变量**
```cpp
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
void show();
void main(int *argv, char *args[]){
    show();
    printf("num = %d\n",num);
}
const int num = 3;
void show(){
    printf("num = %d\n",num);
}
```
编译出错：
```cpp
-bash-4.1$ gcc -o a Demo.c
Demo.c: 在函数‘main’中:
Demo.c:11: 错误：‘num’未声明(在此函数内第一次使用)
Demo.c:11: 错误：(即使在一个函数内多次出现，每个未声明的标识符在其
Demo.c:11: 错误：所在的函数内也只报告一次。)
-bash-4.1$
```
在main函数中用extern声明变量num，如下：
```cpp
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
void show();
void main(int *argv, char *args[]){
    extern const int num;
    show();
    printf("num = %d\n",num);
}
const int num = 3;
void show(){
    printf("num = %d\n",num);
}
```
编译与运行：
```cpp
-bash-4.1$ gcc -o a Demo.c
-bash-4.1$ ./a
num = 3
num = 3
-bash-4.1$
```
这里需要说明，声明为extern的变量不能初始化，要另起一行赋值，如：
```cpp
extern const int num;
    num = 10;
```
当然，也可以用extern声明一个不再同一个文件内的全局变量（且必须为全局变量，如若不是则出错，编译不通过），如下两个文件：
main.c
```cpp
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
void main(int *argv, char *args[]){
    extern const int num;
    show();
    printf("num = %d\n",num);
}
```
extern.c
```cpp
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
const int num = 3;
void show(){
    printf("num = %d\n",num);
}
```
编译与运行：
```cpp
-bash-4.1$ gcc -o a main.c extern.c
-bash-4.1$ ./a
num = 3
num = 3
-bash-4.1$
```
**2.用extern修饰函数**
main.c
```cpp
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
void main(int *argv, char *args[]){
    extern const int num;
    extern void show();
    show();
    printf("main: num = %d\n",num);
    
}
```
extern.c
```cpp
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
const int num = 3;
void show(){
    printf("show: num = %d\n",num);
}
```
编译与运行：
```cpp
-bash-4.1$ gcc -o a Demo.c extern.c
-bash-4.1$ ./a
show: num = 3
main: num = 3
-bash-4.1$
```
**3.extern "C"{}修饰**
 此外，extern修饰符可用于指示C或者C＋＋函数的调用规范。比如在C＋＋中调用C库函数，就需要在C＋＋程序中用extern “C”声明要引用的函数。这是给链接器用的，告诉链接器在链接的时候用C函数规范来链接。主要原因是C＋＋和C程序编译完成后在目标代码中命名规则不同（http://blog.sina.com.cn/s/blog_52deb9d50100ml6y.html）。
