# C语言switch使用之诡异用法 - 小灰笔记 - CSDN博客





2017年03月19日 18:12:47[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：2648








       关于switch的用法这里不再做什么总结了，其实这个是一个便捷的快速跳转条件切换器。而关于这个功能最常用的技术讨论点在于case后面的break以及default。这里不讨论这些，直接看下面的代码：

#include"stdio.h"



int main(void)

{

       int num = 0;

       switch(num)

       {

              printf("functionrun!\n");

       }

       return 0;

}

     上面的代码中用到了一个switch，但是代码块中没有任何case以及default。代码语法有问题吗？编译一下如下：

E:\WorkSpace\02_技术实践\01_编程语言\01_C语言\02_C和指针\switch>gcc switch.c



E:\WorkSpace\02_技术实践\01_编程语言\01_C语言\02_C和指针\switch>

       由上面的结果可以看到，编译没有任何问题。那么运行会是什么状态呢？会把这个唯一的printf执行输出相应的语句？运行如下：

E:\WorkSpace\02_技术实践\01_编程语言\01_C语言\02_C和指针\switch>a



E:\WorkSpace\02_技术实践\01_编程语言\01_C语言\02_C和指针\switch>

       如此看，就有点诡异了。这个printf语句是没有执行到的！也就是说，switch语句中的代码执行必须得有case标签指示代码的入口。这种行为很让人联想到一种情况的程序执行结果，代码如下：

#include"stdio.h"



int main(void)

{

       int num = 0;

       switch(num)

       {

              int i = 123;

              printf("functionrun!\n");

              default:

                     printf("value of iis:%d\n",i);

                     break;

       }

       return 0;

}

       代码是在之前的代码中修改的，代码的第二个printf会执行，可是执行的时候i的数值是多少呢？从之前的测试可以看出，局部变量后面的代码是没有执行过的，那么我这个i在每次函数执行到这里的时候会每次动态性初始化吗？代码编译运行结果：

E:\WorkSpace\02_技术实践\01_编程语言\01_C语言\02_C和指针\switch>gcc switch.c



E:\WorkSpace\02_技术实践\01_编程语言\01_C语言\02_C和指针\switch>a

value of i is:2



E:\WorkSpace\02_技术实践\01_编程语言\01_C语言\02_C和指针\switch>

       从上面的结果可以得出两个结论：

       1，代码块中的声明定义是起作用的；

       2，i的数值不是123证明这部分的局部变量并没有进行每次的动态初始化。

       说起来这也是够奇怪的一个特点，印象中之前看《C专家编程》的时候似乎看到过一个类似的描述，但是那时候C语言的水平看《C专家编程》一点也看不懂。只是留下了一个模模糊糊的印象，至于是否是一个问题，有空还是得会去查查。



