# c语言多源文件编译 - westbrook1998的博客 - CSDN博客





2018年03月04日 22:39:33[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：79








```
//test.h
#ifndef _Test_H

void fun1();
void fun2();
void fun3();

#endif

//function.c
/*
gcc -c function.c 生成二进制中间文件
*/

#include <stdio.h>
#include "test.h"
void fun1(){
    printf("fun1\n");
}
void fun2(){
    printf("fun2\n");
}
void fun3(){
    printf("fun3\n");
}

//main.c
/*
gcc -c main.c 生成二进制的中间文件

gcc function.o main.o 链接生成可执行文件(window下为a.exe  linux下为a.out)

./a.out 执行可执行文件
*/

#include <stdio.h>
#include "test.h"
int main(void){
    fun1();
    fun2();
    fun3();
    return 0;
}
```



