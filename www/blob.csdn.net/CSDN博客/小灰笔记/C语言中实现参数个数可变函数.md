# C语言中实现参数个数可变函数 - 小灰笔记 - CSDN博客





2017年03月22日 00:22:55[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：380








       其实，我总觉得有个结论应该是必然的。既然很多语言最初都是由C语言发展来的，还有很多语言的基础是用C语言实现的。那么，这些用C语言实现的高级语言的很多特性或者是功能在一定程度上应该是在C语言的开发中可以模拟的。正是如此，很多时候考虑诸如XXX是否可以用C语言实现的时候，应该无需考虑就能够给出一个肯定的答复。

       关于参数可变的函数的实现其实还不是这种情况，但是让我想到的是“是否能够实现参数个数可变的C语言功能”时，基本上可以给出一个可以的答案。

       在C语言中，参数个数可变的函数功能实现依赖于几个stdarg.h中定义的宏。在使用的行为上，它们看起来像是函数。几个宏清单如下：

       va_list：行为有点像数据类型

       va_start：行为有点像函数，参数为va_list类型的参数表以及参数个数

       va_arg：行为像函数，参数为va_list参数表以及参数数据类型

       va_end：行为像函数，参数为va_list的参数表

       示范代码如下;

#include"stdio.h"

#include"stdarg.h"



float avrg(intarg_num,...);



int main(void)

{

       printf("avrg value is:%f\n",avrg(4,1,5,3,6));

       return 0;

}



float avrg(intarg_num,...)

{

       va_list args;

       int i = 0;

       int sum = 0;

       float avrg_value = 0.0f;



       va_start(args,arg_num);



       for(i = 0;i < arg_num;i++)

       {

              sum += va_arg(args,int);

       }

       avrg_value = (float)sum / arg_num;



       va_end(args);



       return avrg_value;

}

       编译运行以及验证结果如下：

E:\WorkSpace\01_编程语言\01_C语言\exp_04>gcc exp_04.c



E:\WorkSpace\01_编程语言\01_C语言\exp_04>a

avrg value is:3.750000



E:\WorkSpace\01_编程语言\01_C语言\exp_04>python

Python 3.6.0(v3.6.0:41df79263a11, Dec 23 2016, 08:06:12) [MSC v.1900 64 bit (AMD64)] onwin32

Type"help", "copyright", "credits" or"license" for more information.

>>> (1 +5 + 3 + 6) / 4

3.75

       通过上面的结果可以看出，通过这种特定宏的方式可以实现多参数的C语言函数，只是在函数原型中只是指明了第一个参数的类型，其余的参数类型全都进行了缺省的设置。



