# C语言中数据类型的使用选择 - 小灰笔记 - CSDN博客





2017年03月28日 22:08:49[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：327








       在《C专家编程》书中有这么一段话：

*       尽量不要使用无符号数，以避免增加不必要的复杂性。尤其是不要仅仅因为无符号不存在负值而用它来表述数量。*

       说到了上面的原则，书中给出了一个例子。这里给一段示范代码：

#include"stdio.h"



int num = -1;



int main(void)

{

       printf("value after convert:%u\n",(unsigned int)num);



       return 0;

}

       代码的编译运行记录与结果如下：

E:\WorkSpace\01_编程语言\01_C语言\exp_11>gcc exp_11.c



E:\WorkSpace\01_编程语言\01_C语言\exp_11>a

value afterconvert: 4294967295

       确实是比较难以令人置信，经过了强制数据类型转换之后-1被转成了一个非常大的正整数。如果用到代码中，这确实是一个比较难以预测的，至少是跟我们的自然尝试逻辑大不一致。

       不过，在嵌入式中，似乎偏向用有符号的数值有时候确实有有点不便，那么在使用的时候加个简单的边界检验似乎还是很有用的。示范代码如下：

#include"stdio.h"



int num = -1;



int main(void)

{

       unsigned int my_num_u = 0U;



       if(num < 0)

       {

              my_num_u = 0U;

       }

       else

       {

              my_num_u = (unsigned int)num;

       }

       printf("value after convert:%u\n",my_num_u);



       num = 123;

       if(num < 0)

       {

              my_num_u = 0U;

       }

       else

       {

              my_num_u = (unsigned int)num;

       }

       printf("value after convert:%u\n",my_num_u);



       return 0;

}

       不一定合理，但是在我用到的绝大多数场景中这基本上是我想要的结果。



