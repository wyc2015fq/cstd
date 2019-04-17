# C语言中指针的类型兼容性 - 小灰笔记 - CSDN博客





2017年03月27日 19:35:03[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：1062








       刚刚看完了《C和指针》，感觉自己对指针的理解应该更深了一点。今天又开始转向《C专家编程》，这一本稀里糊涂看了很多次但是并没有成为专家的书。开篇故事看得还是很不亦乐乎，遇到一个问题解释时似乎又有一点傻眼了。问题如下：

void foo(constchar **p)

{

}



int main(intargc,char **argv)

{

       foo(argv);

       return 0;

}      

上面的代码在编译的时候会给出一个警告，具体的信息如下：

E:\WorkSpace\01_编程语言\01_C语言\exp_10>gcc exp_10.c

exp_10.c: Infunction 'main':

exp_10.c:7:6:warning: passing argument 1 of 'foo' from incompatible pointer type

  foo(argv);

      ^

exp_10.c:1:6:note: expected 'const char **' but argument is of type 'char **'

 void foo(const char **p)

      ^

       确实，这段代码在编译的时候给出了一段警告。但是，最终代码还是能够编译成功。简单做一下分析，foo中的形参p的含义：*p是一个指向const char的指针，那么则是指向（指向const char的指针）的指针。从物理意义上进一步解释，p是一个指针，它所指向的对象是一个指向const char的地址。相应的，argv也是一个指针，它所指向的对象是一个指向char的地址。由此，这两个参数都是一个指向存储一个指向char的指针，只是其中的一个char是只读的。这样看，进行了一个隐性的强制数据类型转换程序的功能应该是可以实现的。简单的验证，修改代码如下：

#include "stdio.h"



void foo(constchar **p)

{

       while(*++p != NULL)

       {

              printf("%s\n",*p);

       }

}



int main(intargc,char **argv)

{

       foo(argv);

       return 0;

}

       代码编译并执行程序测试结果如下：

E:\WorkSpace\01_编程语言\01_C语言\exp_10>gcc exp_10.c

exp_10.c: Infunction 'main':

exp_10.c:13:6:warning: passing argument 1 of 'foo' from incompatible pointer type

  foo(argv);

      ^

exp_10.c:3:6:note: expected 'const char **' but argument is of type 'char **'

 void foo(const char **p)

      ^



E:\WorkSpace\01_编程语言\01_C语言\exp_10>a life is hard, you needpython.

life

is

hard,

you

need

python.

       通过上面的结果可以看出，虽说是程序编译遇到了警告，但是编译成功了，程序的运行也是没有问题的。那么，这个警告为什么出现呢？其实，说到了这个层次就得去研究C标准了。标准这东西相对枯燥，我自己是很难能够耐下性子好好读一下的。这里还是摘取一下《C专家编程中》的结论：

       两个操作数都是指向有限定符或者无限定符的相容类型的指针，左边指针所指向的类型必须具有右边指针所指向的类型的全部限定符。

       根据上面的结论再回看代码，两个指针本身都是没有限定符的，但是两个指针所指向的内容并不一致。其中一个是指向指向const char的指针的指针；而另一个是指向指向char的指针的指针。两个指针指向的内容并不相同，因此不相容。接下来在看，常用的函数传值所用的手段，具体的测试代码如下：



const char *p1;

char *p2;



int main(void)

{

       p1 = p2;

       p2 = p1;

       return 0;

}

       编译结果：

E:\WorkSpace\01_编程语言\01_C语言\exp_10>gcc exp_11.c

exp_11.c: Infunction 'main':

exp_11.c:8:5:warning: assignment discards 'const' qualifier from pointer target type

  p2 = p1;

     ^

       上面的记录中，p1 = p2位第7行。

       由此，可以看出前面《C专家编程中》总结出来的结论正确。左边不仅有右边的全部修饰符，而且还多了一个const。这种关系倒是有点像面向对象中的类的使用那么一种感觉。

       至于《C专家编程》中另外一句用于解释第一个例子的说法，我始终没有理解，总感觉是说反了。再贴一遍代码：

void foo(constchar **p)

{

}



int main(intargc,char **argv)

{

       foo(argv);

       return 0;

}

       《C专家编程》中的解释：每个实参都应该具有自己的类型，这样它的值就可以赋值给与它所对应的形参的对象（该对象的类型不含有限定符）。这就是说参数传递过程类似于赋值。所以，除非一个const char **类型的对象可以赋值给一个类型为char **的值，否则肯定会产生一条诊断信息。但是上面的代码中，赋值的方向似乎恰好相反，这是很让人寻味的。

       博客邻发出去之前，还是有点纠结，于是找来了英文原版的《Expert C Programming》，相关的原文如下：

       Thus, a diagnostic message must beproduced unless an object of type const char ** may be assigned a value of typechar **

       如此，我觉得描述是对的，而翻译的表述确实是有点问题。确实是把赋值给弄反了！



