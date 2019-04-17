# memmove、memcpy和memccpy简介 - 工作笔记 - CSDN博客





2012年01月21日 10:01:55[Eason.wxd](https://me.csdn.net/App_12062011)阅读数：5405







memmove、memcpy和memccpy三个函数都是内存的拷贝，从一个缓冲区拷贝到另一个缓冲区。

memmove(void *dest,void*src,int count)

memcpy(void *dest,void *src,int count)

memccpy(void*dest,void*src,int ch,int count)


表头文件: #include <string.h>

定义函数: void *memcpy(void *dest, const void *src, size_t n)

函数说明: memcpy()用来拷贝src所指的内存内容前n个字节到dest所指的内存地址上。与strcpy()不同的是,memcpy()会完整的复制n个字节,不会因为遇到字符串结束'\0'而结束

返回值:   返回指向dest的指针


表头文件: #include <string.h>

定义函数: void *memccpy(void *dest, const void *src, int c, size_t n);

函数说明: memccpy()用来拷贝src所指的内存内容前n个字节到dest所指的地址上。与memcpy()不同的是,memccpy()如果在src中遇到某个特定值(int c)立即停止复制。

返回值:   返回指向dest中值为c的下一个字节指针。返回值为0表示在src所指内存前n个字节中没有值为c的字节。

表头文件: #include <string.h>

定义函数: void *memmove(void *dest, const void *src, size_t n);

函数说明:memmove()是从一个缓冲区移动到另一个缓冲区中。 

返回值:   返回指向dest指针。


当dest <= src-count 或dest >= src+count时，以上三个函数均不会产生覆盖问题，即源数据不会被更改。
若不在以上范围内，则源数据会被更改。


如:

char a[]={'a','b'};

char b[]={'c','d','e','f','g','h'};

memmove(a,b,sizeof(b));

或是直接char *p=b+2;memmove(p,b,sizeof(b));

输出数据会发现b中数据输出已被更改。

发现即使a数组指向的空间不够存储数据，也能够移动成功。

原因|dest - src |<count


如果在使用这些函数时，分配给足够的空间，然后再使用就不会出现覆盖问题。也就是说如果外部分配给的空间不足以存储要拷贝的数据时，就有可能出现源数据被覆盖更改的问题。


#include <stdio.h>

#include <stdlib.h>

#include <string.h>


void main(void)

{

 int i=0; 

    char a[9]={'a','b','c','d','e','f','g','h','\0'};

 char p[2]={'q','w'};//或char *p=a+2;

 memmove(p,a,sizeof(a));

    puts(a);

 printf("_____________________________________________\n");

 puts(p);

 printf("_____________________________________________\n");

  for(i =0;i<10;i++)

   printf("%c %d \n",*(a+i),a+i);

 printf("_____________________________________________\n");

 for(i =0;i<8;i++)

   printf("%c %d \n",*(p+i),p+i); 

}

观察输出结果。

把memmove(p,a,sizeof(a));改为memcpy(p,a,sizeof(a));或memccpy(p,a,'e',sizeof(a));再观察输出结果。

可以看出在目的存储空间不足时，便会出现源数据被覆盖改变的问题。

如果目的存储空间分配足够的空间，则便不会出现覆盖问题。




