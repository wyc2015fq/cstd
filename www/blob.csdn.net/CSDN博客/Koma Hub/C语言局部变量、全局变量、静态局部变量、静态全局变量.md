# C语言局部变量、全局变量、静态局部变量、静态全局变量 - Koma Hub - CSDN博客
2017年11月21日 22:21:01[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：169
建立三个文件，文件名分别为main.c \ head1.c \  head2.c
main.c
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
```
head1.c
```cpp
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
static int B ;
int A;
void f1(int a){
    A++,B++;
    printf("head1.h:f1(%d), A = %d, B = %d\n",a,A,B);
}
```
head2.c
```cpp
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
static int B;
int A;
void f2(int a){
    A += 2, B += 2;
    printf("head2.h:f2(%d), A = %d, B = %d\n",a,A,B);
}
```
编译与运行结果：
```cpp
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
```
