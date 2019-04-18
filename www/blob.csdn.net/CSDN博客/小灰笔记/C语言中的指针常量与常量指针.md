# C语言中的指针常量与常量指针 - 小灰笔记 - CSDN博客





2017年03月17日 01:14:52[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：196








       很多人纠结过这两个名词，也有人写文章区分两者的区别。其实很简单，常量指针和指针常量一个是指针、一个是常量。

       const int *p_var;与

       int const *p_var;

       的功能是相同的，都是一个指向整型常量的指针。指针的数值可以修改，但是当前指针指向的量不可修改。该如何去确认p_var的含义呢？我从一片老外写的文章中学了一招，无论什么时候，都把*读作“value of address”就容易了。按照这个方法，这个生命表达的含义应该是：

       地址p_var的值（*p_var）指向整型常量，那么p_var便是一个指向常量的指针了。写一段代码测试：

#include"stdio.h"



const int a = 123;

const int *p_var1= &a;

int const *p_var2= &a;



int main(void)

{

       printf("value of p_var1 is:%p\n",p_var1);

       printf("value of p_var2 is:%p\n",p_var2);



       printf("value-1:%d\n",*p_var1);

       printf("value-2:%d\n",*p_var2);



       p_var1 += 1;

       p_var2 += 1;



       return 0;

}

       编译运行结果：

E:\WorkSpace\01_编程语言\01_C语言\03_declare>gcc exp_33.c



E:\WorkSpace\01_编程语言\01_C语言\03_declare>a

value of p_var1is: 00404024

value of p_var2is: 00404024

value-1: 123

value-2: 123

       从运行得出结论：

       1，常量指针两种定义方式都可以；

       2，常量指针可以修改；

       上面的代码做一下修改，增加对初始指向常量的修改：

#include"stdio.h"



const int a = 123;

const int *p_var1= &a;

int const *p_var2= &a;



int main(void)

{

       printf("value of p_var1 is:%p\n",p_var1);

       printf("value of p_var2 is:%p\n",p_var2);



       *p_var1 = 234;



       printf("value-1:%d\n",*p_var1);

       printf("value-2:%d\n",*p_var2);



       p_var1 += 1;

       p_var2 += 1;



       return 0;

}

       编译结果如下：

E:\WorkSpace\01_编程语言\01_C语言\03_declare>gcc exp_33.c

exp_33.c: Infunction 'main':

exp_33.c:12:10:error: assignment of read-only location '*p_var1'

  *p_var1 = 234;

          ^

       从上面的结果可以看出，初始指针指向的位置是常量，不能够修改。对代码进行进一步修改如下：

#include"stdio.h"



const int a = 123;

const int *p_var1= &a;

int const *p_var2= &a;



int main(void)

{

       printf("value of p_var1 is:%p\n",p_var1);

       printf("value of p_var2 is:%p\n",p_var2);



       p_var1 += 8;

       *p_var1 = 234;



       printf("value-1:%d\n",*p_var1);

       printf("value-2:%d\n",*p_var2);



       p_var1 += 1;

       p_var2 += 1;



       return 0;

}

       编译：

E:\WorkSpace\01_编程语言\01_C语言\03_declare>gcc exp_33.c

exp_33.c: Infunction 'main':

exp_33.c:13:10:error: assignment of read-only location '*p_var1'

  *p_var1 = 234;

          ^

       从这里可以看出，尽管指针修改了，但是其指向的空间依然是不可修改的，否则会引起编译器报错。

       接下来看看指针常量，再一次分析一下语义：这是一个指针，但是是一个不可修改的指针。这样，联想常量的定义就可以知道，如果想让它具有什么值得化需要在初始化的时候就完成。而个指针的特点是，本身不能够修改，但是指向的内容是可以修改的。指针常量的定义声明方式如下：

       首先利用前面的“value ofaddress”方法描述，应该是：value of address 一个常量指针指向XXX。如此，

Int * const p_var = XXX;

       写代码如下;

#include"stdio.h"



int a = 123;

const int b = 234;



int *const p_var1= &a;

int *const p_var2= &b;



int main(void)

{

       printf("value at p_var1 is:%d\n",*p_var1);

       printf("value at p_var2 is:%d\n",*p_var2);



       a += 1;



       printf("value at p_var1 is:%d\n",*p_var1);



       return 0;

}

       编译运行：

E:\WorkSpace\01_编程语言\01_C语言\03_declare>gcc exp_34.c

exp_34.c:7:21:warning: initialization discards 'const' qualifier from pointer target type

 int *const p_var2 = &b;

                     ^



E:\WorkSpace\01_编程语言\01_C语言\03_declare>a

value at p_var1is: 123

value at p_var2is: 234

value at p_var1is: 124

       虽说是遇到了警告，但是编译还是通过了而且可以运行。由此，可以得出结论：

       指针常量指向的值可以是变量也可以是常量。

       再次修改代码;

#include"stdio.h"



int a = 123;

const int b = 234;



int *const p_var1= &a;

int *const p_var2 =&b;



int main(void)

{

       p_var1 += 1;



       printf("value at p_var1 is:%d\n",*p_var1);

       printf("value at p_var2 is:%d\n",*p_var2);



       a += 1;



       printf("value at p_var1 is:%d\n",*p_var1);



       return 0;

}

       编译：

E:\WorkSpace\01_编程语言\01_C语言\03_declare>gcc exp_34.c

exp_34.c:7:21:warning: initialization discards 'const' qualifier from pointer target type

 int *const p_var2 = &b;

                     ^

exp_34.c: Infunction 'main':

exp_34.c:11:9:error: assignment of read-only variable 'p_var1'

  p_var1 += 1;

         ^

       这次是直接遇到了错误而停止了编译，可以看出这个指针实际上不可修改的。



