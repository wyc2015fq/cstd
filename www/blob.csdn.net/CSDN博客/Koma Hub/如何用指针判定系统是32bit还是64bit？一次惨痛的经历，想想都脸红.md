# 如何用指针判定系统是32bit还是64bit？一次惨痛的经历，想想都脸红 - Koma Hub - CSDN博客
2018年03月10日 12:15:20[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：198
一次惨痛的经历：
manager：简单的用C语言判断系统是多少位的。
me：额~~这个~~不是很了解。
manager：知道指针吗？
me：知道。
manager：那指针是什么？
me：地址。
manager：那简单的出个题目：
```cpp
char *c;
int *i;
printf("%d\n",sizeof(c));
printf("%d\n",sizeof(i));
```
的输出结果是什么呢？
me：额~~1和4吗？不太清楚。
manager：偶。。。。
多么痛的领悟，果断试了一下：
```cpp
#include<stdio.h>
#include<stdlib.h>
int main(int argc, char **argv)
{
    char *ch;
    int *in;
    in = (int*)malloc(sizeof(int)*100);
    printf("%d,  %d\n",sizeof(ch),sizeof(in));
}
```
输出结果：
```cpp
$ gcc test.c
$ ./a.out 
8,  8
```
me：why？？why？一脸懵逼
基础知识太薄弱，然后查了一下，我的运行环境是：
```cpp
$ uname -a
Linux AMAX 2.6.32-573.12.1.el6.x86_64 #1 SMP Tue Dec 15 21:19:08 UTC 2015 x86_64 x86_64 x86_64 GNU/Linux
```
64bit，嗯！想想原来
```cpp
/* 1Byte = 8bit */
```
偶偶，原来代码中的c和i是系统单位内存地址的字节数，8*8=64bit，恍然大明白。
基础知识太薄弱是大事！！！
