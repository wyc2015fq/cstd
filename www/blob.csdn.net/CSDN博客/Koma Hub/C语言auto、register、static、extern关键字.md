# C语言auto、register、static、extern关键字 - Koma Hub - CSDN博客
2017年11月30日 20:23:17[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：191
**1.auto**
```cpp
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
int a = 0;
void show(){
    a++;
    printf("hello: %d\n",a);
}
void main(int *argv, char *args[]){
    show();
/**
Auto变量：
局部变量不作任何说明，都作为自动变量auto,及动态存储的。Auto可省略。
*/
    volatile unsigned auto int b = 10;
}
```
**2.register**
```cpp
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
int a = 0;
void show(){
    a++;
    printf("hello: %d\n",a);
}
void main(int *argv, char *args[]){
    show();
/**
Register变量：暗示编译程序相应的变量将将被频繁使用，如果可能的话，
应将其保存在CPU的寄存器中，以指加快其存取速度，有几点使用限制：
1 register变量必须是能被CPU寄存器所接受的类型
2为register变量可能不存放在内存中，所以不能用取址符运算符“& ”来获取取址符运算符
3只有局部变量和形参可以作为register变量，全局变量不行
4 80x86系列CPU最多可使用的register变量数目有限。int型可使用8个通用寄存器
5 静态变量不能定义为register。
*/
    register int b = 10;
}
```
**3.static**
```cpp
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
int A;
void show(){
    volatile static int a ;//静态局部变量(static) 静态局部变量定义时前面加static关键字。
    a++;
    f1(a);
    f2(a);
    //printf("show: B = %d\n",B);
}
void main(int *argv, char *args[]){
    show();
    show();
    show();
    show();
    int c;
    static int b = 10;
}
/**
//head1.c
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
static int B ;
int A;
void f1(int a){
    A++,B++;
    printf("head1.h:f1(%d), A = %d, B = %d\n",a,A,B);
}
//head2.c
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
static int B;
int A;
void f2(int a){
    A += 2, B += 2;
    printf("head2.h:f2(%d), A = %d, B = %d\n",a,A,B);
}
*/
/**
-bash-4.1$ gcc -o a Demo.c head*.c
-bash-4.1$ ./a
head1.h:f1(1), A = 1, B = 1
head2.h:f2(1), A = 3, B = 2
head1.h:f1(2), A = 4, B = 2
head2.h:f2(2), A = 6, B = 4
head1.h:f1(3), A = 7, B = 3
head2.h:f2(3), A = 9, B = 6
head1.h:f1(4), A = 10, B = 4
head2.h:f2(4), A = 12, B = 8
-bash-4.1$ 
*/
```
**4.extern**
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
/**
//extern.c
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
const int num = 3;
void show(){
    printf("show: num = %d\n",num);
}
*/
```
