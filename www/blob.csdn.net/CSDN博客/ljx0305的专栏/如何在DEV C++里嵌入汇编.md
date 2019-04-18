# 如何在DEV C++里嵌入汇编 - ljx0305的专栏 - CSDN博客
2010年08月23日 14:00:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：4701
呵呵，这方面的资料很多，但是东是东一鳞西一爪的。这篇也算我自己的一个小总结吧！
在Dev C++里使用内联汇编的格式不同于VC的，这一点需要注意：
VC里采用的是Intel的格式，而Dev C++采用的是AT&A的汇编格式。
如下例：
#include<stdio.h>
int main(){
  printf("Test inline asm/n");
  //在此插入一个int 3中断
  _asm int 3      //此处采用的就是Intel的汇编格式
  return 0;
}
这个可以在VC下编译通过，但用Dev C++则编译出错，原因就在内联的汇编！如果需要转移到Dev C++下，则应将那句汇编更改为：
__asm__("int $3");
完整的如下：
#include<stdio.h>
int main(){
  printf("Test inline asm/n");
  __asm__("int $3");
  return 0;
}
在AT&A里，引用值需要前加"$"标记，而且引用寄存器时也需要加一个前缀"%",如：
mov $789,%eax
另外，不可以直接引用变量，而只能引用全局变量，同时在引用时，需要在其前加一下划线"_",另外就是操作数的方向也与Intel的语法相反，如：
mov $789,%eax    //=============是把789赋值给EAX
其方向是:                       operator1------------------->operator2
而在Intel下则是反过来的：operator1<-------------------operator2
一个完整的示例如下：
#include<stdio.h>
int a;
int main(){
  printf("Test inline asm/n");
  __asm("mov $789,%eax");
  __asm("mov %eax,_a");
  printf("A is : %d/n",a);
  return 0;
}
//这里大家也应该注意到了，对于__asm__与__asm标签是不区分的！同样适用！！
当然，如果汇编的语句较多，可以用一个标记来标记，如：
__asm(
  "mov $123,%eax/n/
  mov %eax,_a"
);
转自：IaWeN's Blog
链接：[http://www.iawen.com/read.php?4](http://www.iawen.com/read.php?4)
