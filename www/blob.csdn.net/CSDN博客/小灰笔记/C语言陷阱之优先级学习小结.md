# C语言陷阱之优先级学习小结 - 小灰笔记 - CSDN博客





2017年02月16日 22:52:13[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：307








很多时候跟别人交流介绍自己工作的时候，经常听到别人说自己精通C语言。其实，我自己有时候自我介绍的时候也会这么说，不过随着自己工作的过程中各种书籍、文档等一些列的资料看下来，越觉得自己肤浅。其实，很多最基础的东西我还没有掌握。

今天看C语言陷阱的时候看到一个简单的小陷阱，如果是我自己写代码的话很可能就陷进去了，幸运的是这么多年来我的代码中确实是没有用到过这个操作。

测试代码如下：

  1 #include "stdio.h"

  2 

  3 int flag1 =
1;

  4 int flag2 =
2;

  5 

  6 int main()

  7 {

  8 if(flag1 & flag2)

  9     {

 10         printf("bit equal!\n");

 11     }

 12 else

 13     {

 14         printf("bit not equal!\n");

 15     }

 16 

 17 return0;

 18 }

 19 




编译运行如下：

GreydeMac-mini:exp01 greyzhang$ gcc exp01.c 

GreydeMac-mini:exp01 greyzhang$ ./a.out 

bit not equal!




为了可读性，修改代码如下：

  1 #include "stdio.h"

  2 

  3 int flag1 =
1;

  4 int flag2 =
2;

  5 

  6 int main()

  7 {

  8 if(flag1 & flag2 ==
1)

  9     {

 10         printf("bit equal!\n");

 11     }

 12 else

 13     {

 14         printf("bit not equal!\n");

 15     }

 16 

 17 return0;

 18 }




编译运行如下：




GreydeMac-mini:exp01 greyzhang$ gcc exp01.c 

**exp01.c:8:11: ****warning:****& has lower precedence than ==; == will be evaluated**

**      first [-Wparentheses]**

        if(flag1 & flag2 == 1)

**                 ^~~~~~~~~~~~**

**exp01.c:8:11: note: **place parentheses around the '==' expression to silence this

      warning

        if(flag1 & flag2 == 1)

**                 ^**

                   (         )

**exp01.c:8:11: note: **place parentheses around the & expression to evaluate it

      first

        if(flag1 & flag2 == 1)

**                 ^**

           (            )

1 warning generated.

GreydeMac-mini:exp01 greyzhang$ ./a.out 

bit not equal!




啊，这个结果有些出乎意料。不过，我们前面提到的陷阱问题倒是被编译器发掘出来了，我用的编译器是Mac自带的兼容gcc的编译器。不知道gcc的编译器中是否也会有相应的处理？当然，再怎么说这毕竟是编译器的处理，不同的编译器对此或许会有不同的处理方式。Anyway，这个陷阱还是确确实实存在的。



