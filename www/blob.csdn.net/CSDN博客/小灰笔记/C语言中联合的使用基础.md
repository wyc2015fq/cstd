# C语言中联合的使用基础 - 小灰笔记 - CSDN博客





2017年03月23日 00:12:06[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：146
个人分类：[C](https://blog.csdn.net/grey_csdn/article/category/6639462)









       这又是一个时常看但是不常写的东西，在我现在的工作中用到的相关地方基本上也是处理器驱动对于寄存器的抽象。

       联合的定义方式其实跟结构体在语法上类似，不同的地方是结构体的成员占用不同的存储区域而联合的成员则是共享相同的存储。虽说如此，但是并不意味着联合的成员必须是数据类型相同的元素。

       基础知识点代码如下;

#include"stdio.h"

#include"string.h"



union

{

       int a;

       float b;

       char c[4];

} demo_union ={123};



int main(void)

{

       printf("a is%d\n",demo_union.a);

       printf("b is%f\n",demo_union.b);

       printf("c is%s\n",demo_union.c);



       demo_union.b = 234.0f;

       printf("a is%d\n",demo_union.a);

       printf("b is%f\n",demo_union.b);

       printf("c is%s\n",demo_union.c);



       strcpy(demo_union.c,"GGG");

       printf("a is%d\n",demo_union.a);

       printf("b is%f\n",demo_union.b);

       printf("c is%s\n",demo_union.c);



       return 0;

}

       代码编译运行结果：

E:\WorkSpace\01_编程语言\01_C语言\exp_07>gcc exp_07.c



E:\WorkSpace\01_编程语言\01_C语言\exp_07>a

a is 123

b is 0.000000

c is {

a is 1131020288

b is 234.000000

c is

a is 4671303

b is 0.000000

c is GGG



       结论：

       1，只要是占用的存储空间相同，数据类型可以不一致，甚至可以是数组（也可以是其他数据结构）；

       2，每次只能够有一个成员占用存储；

       3，能够进行初始化（但是，只能够初始化第一个成员）。



