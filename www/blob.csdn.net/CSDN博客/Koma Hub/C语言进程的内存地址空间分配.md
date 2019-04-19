# C语言进程的内存地址空间分配 - Koma Hub - CSDN博客
2017年11月30日 21:12:04[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：746
![](https://img-blog.csdn.net/20171130211049293?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvUm9uZ19Ub2E=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
图为一个执行进程的内存地址空间。
代码段就是存储程序文本的，所以有时候也叫做文本段，指令指针中的指令就是从这里取得。数据段是存储数据用的，还可以分成初始化为非零的数据区，BSS，和堆(Heap)三个区域。初始化非零数据区域一般存放静态非零数据和全局的非零数据。BSS是Block Started by Symbol的缩写，原本是汇编语言中的术语。该区域主要存放未初始化的全局数据和静态数据。还有就是堆了，这个区域是给动态分配内存是使用的，也就是用malloc等函数分配的内存就是在这个区域里的。它的地址是向上增长的。最后一个堆栈段（注意，堆栈是Stack,堆是Heap，不是同一个东西），堆栈可太重要了，这里存放着局部变量和函数参数等数据。
需要注意的是，代码段和数据段之间有明确的分隔，但是数据段和堆栈段之间没有，而且栈是向下增长，堆是向上增长的，因此理论上来说堆和栈会“增长到一起”，但是操作系统会防止这样的错误发生，所以不用过分担心。
```cpp
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
void *getP(){
    printf("---1\n");
    char *p = "rodfqeqwerng\n";
    return p;
    
}
void GetMemory(char **p, int num) 
{ 
    *p = (char *)malloc(num); 
}
void GetMemory2(char **p, int num)
{
    *p = (char *)malloc(sizeof(char) * num);
}
char *GetMemory3(int num)
{
    char *p = (char *)malloc(sizeof(char) * num);
    return p;
}
void main(int *argv, char *args[]){
    char *p = NULL;
    p = getP();
    printf(p);
/*解析：（1）能够输出hello 
          （2）内存泄漏*/
/*
    char *str= NULL; 
    GetMemory(&str, 100); 
    strcpy(str, "hello"); 
    printf(str);
*/
/*解析：篡改动态内存区的内容，后果难以预料，非常危险。
            因为free(str);之后，str成为野指针，if(str!= NULL)语句不起作用。*/
/*
    char *str= (char *) malloc(100); 
    strcpy(str, "hello\n"); 
    free(str); 
    if(str!= NULL) 
    { 
        printf(str);
        strcpy(str, "world\n"); 
        printf(str); 
    }
*/
/*解析：如果非得要用指针参数去申请内存，那么
        应该改用“指向指针的指针”.由于“指向指针的指针”
        这个概念不容易理解，我们可以用函数返回值来传
        递动态内存。*/
    char *str= NULL;
    GetMemory2(&str, 100); // 注意参数是&str，而不是str
    strcpy(str, "hello\n");
    printf(str);
    free(str);
/*解析：用函数返回值来传递动态内存这种方法虽然好用，
        但是常常有人把return 语句用错了。这里强调不要用return
        语句返回指向“栈内存”的指针*/
/*
    char *str= NULL;
    str= GetMemory3(100);
    strcpy(str, "hello\n");
    printf(str);
    free(str);
*/
}
```
