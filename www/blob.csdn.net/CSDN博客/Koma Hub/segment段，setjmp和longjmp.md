# segment段，setjmp和longjmp - Koma Hub - CSDN博客
2019年02月11日 11:28:33[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：48
个人分类：[ANSI C																[C/C++](https://blog.csdn.net/Rong_Toa/article/category/7156199)](https://blog.csdn.net/Rong_Toa/article/category/8653945)
**目录**
[What Kinds of C Statements End Up in Which Segments](#What%20Kinds%20of%20C%20Statements%20End%20Up%20in%20Which%20Segments)
[How the Segments of an Executable are Laid Out in Memory](#How%20the%20Segments%20of%20an%20Executable%20are%20Laid%20Out%20in%20Memory)
[Virtual Address Space Layout, Showing Shared Libraries](#Virtual%20Address%20Space%20Layout%2C%20Showing%20Shared%20Libraries)
[An Activation Record is Created at Runtime for Each Function Call](#An%20Activation%20Record%20is%20Created%20at%20Runtime%20for%20Each%20Function%20Call)
[The following code shows an example of setjmp() and longjmp().](#The%20following%20code%20shows%20an%20example%20of%20setjmp()%20and%20longjmp().)
[Jump to It!](#Jump%20to%20It!)
### What Kinds of C Statements End Up in Which Segments
![What Kinds of C Statements End Up in Which Segments](https://img-blog.csdnimg.cn/20190211112224780.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1JvbmdfVG9h,size_16,color_FFFFFF,t_70)
### How the Segments of an Executable are Laid Out in Memory
![](https://img-blog.csdnimg.cn/20190211112339527.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1JvbmdfVG9h,size_16,color_FFFFFF,t_70)
### Virtual Address Space Layout, Showing Shared Libraries
![](https://img-blog.csdnimg.cn/20190211112400999.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1JvbmdfVG9h,size_16,color_FFFFFF,t_70)
### An Activation Record is Created at Runtime for Each Function Call
![](https://img-blog.csdnimg.cn/20190211112418860.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1JvbmdfVG9h,size_16,color_FFFFFF,t_70)
### The following code shows an example of setjmp() and longjmp().
```cpp
#include <setjmp.h>
jmp_buf buf;
#include <setjmp.h>
banana() {
  printf("in banana()\n");
  longjmp(buf, 1);
  /*NOTREACHED*/
  printf("you'll never see this, because I longjmp'd");
}
main()
{
  if (setjmp(buf))
    printf("back in main\n");
  else {
    printf("first time through\n");
    banana();
  }
}
```
结果：
```
% a.out
first time through
in banana()
back in main
```
或者一种牛叉的方法：
```cpp
switch(setjmp(jbuf)) 
{
  case 0:
    apple = *suspicious;
    break;
  case 1:
    printf("suspicious is indeed a bad pointer\n");
    break;
  default:
    die("unexpected value returned by setjmp");
}
```
### Jump to It!
Take the source of a program you have already written and add setjmp/longjmp to it, so that on receiving some particular input it will start over again.
The header file <setjmp.h> needs to be included in any source file that uses setjmp or longjmp.
Like **goto's**, **setjmp**/**longjmp **can make it hard to understand and debug a program. They are best avoided except in the specific situations described.
