# C语言陷阱与缺陷学习之函数调用 - 小灰笔记 - CSDN博客





2017年02月24日 00:29:01[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：285








       首先看一下如下的代码：

#include"stdio.h"



int Func(void);



int main(void)

{

       Func;

       printf("Function ran!");

       return 0;

}



int Func(void)

{

       return 7;

}

       主函数的第一行，只是写了一下函数名，其他什么都没加。这样的代码编译的时候有问题吗？实际上，编译的时候一点错误都没有出现，而这个语句也仅仅是计算了一下函数的入口地址之后什么也不做，程序继续往下走。

![](https://img-blog.csdn.net/20170224002912579?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




       加入更多的猜想与测试，继续修改代码如下;

#include"stdio.h"



int Func(void);



int (*fun_p)();



int main(void)

{

       fun_p = Func;

       printf("%p\n",fun_p);

       printf("%p\n",Func);

       printf("%p\n",*Func);

       printf("%d\n",Func());

       printf("%d\n",fun_p());

       printf("Function ran!\n");

       return 0;

}



int Func(void)

{

       return 7;

}
       程序编译运行结果如下：

![](https://img-blog.csdn.net/20170224003002830?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




       通过代码以及运行结果基本能够得出以下的知识总结：1，函数名称代表的是函数的入口，其实是入口地址；2，通过函数指针可以实现函数的调用；3，函数名可以看做是一个表达式，广义上的表达式，它的作用是计算函数的地址；4，函数指针的书写方式有两种，一种是常见的指针语法格式，另一种是省略*的表示方式。通常，我们使用的时候省略星号。这个知识点被我遗忘了，让我纠结了一小阵子。




