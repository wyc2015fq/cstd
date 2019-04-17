# C语言主函数返回值含义 - 小灰笔记 - CSDN博客





2017年03月14日 19:36:09[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：6603








       上大学时候我学习C语言的时候主函数的类型还可以用void类型，毕业之后想再次加深一下编程的水平发现我找的编译器居然要求主函数必须具有int类型的返回值。简单的看了一点教程以及网络上的介绍之后，也不管三七二十一直接照抄教程中的说明，主函数返回值写上一个return 0。可这到底什么意思呢？

       写一个常用的简单代码如下：

#include"stdio.h"



int main(void)

{

       printf("test\n");

       return 0;

}

       编译运行结果：

E:\WorkSpace\01_编程语言\01_C语言\02_main_return_value>gcc test.c



E:\WorkSpace\01_编程语言\01_C语言\02_main_return_value>a

Test

       那再修改一下，返回值修改一下：

#include"stdio.h"



int main(void)

{

       printf("test\n");

       return 1;

}

       编译运行结果：

E:\WorkSpace\01_编程语言\01_C语言\02_main_return_value>gcc test.c



E:\WorkSpace\01_编程语言\01_C语言\02_main_return_value>a

Test

       运行也没有问题。如此看来，这个返回值似乎可以随意一些。不过，今天看了一段程序之后倒是觉得这个却也不能够那么随意。示范代码改为如下：

#include"stdio.h"

#include"stdlib.h"



int main(void)

{

       printf("test\n");

       return EXIT_SUCCESS;

}

       由此，看来最后一个返回状态还是有很多特殊的用途的。我查看了stdlib.h的头文件，找到了两个相关的宏定义如下：

#defineEXIT_SUCCESS 0

#defineEXIT_FAILURE 1

       由此看来，一般返回值为0的时候代表的是程序的一种正常状态，而返回值为1的时候则代表程序运行遇到问题失败。那么，换做其他的返回值是否有问题呢？在这个头文件中并没有找到其他的宏定义，因此只能够随意试一下了。修改代码如下;

#include"stdio.h"

#include"stdlib.h"



int main(void)

{

       printf("test\n");

       return 300;

}

       代码改的有点随意，编译运行结果如下：

E:\WorkSpace\01_编程语言\01_C语言\02_main_return_value>gcc test.c



E:\WorkSpace\01_编程语言\01_C语言\02_main_return_value>a

Test

     没有什么问题，看来这个返回值应该是大家形成共识作为一个参考参数的。而未定义的返回值则很难让人去琢磨你程序的运行状态。如此想来，如果没有什么特殊的需求最好是让程序能够正常结束，给出一个正常的状态。或者，加一个笼统一点的分支，在失败的时候给出一个运行失败的提示。



