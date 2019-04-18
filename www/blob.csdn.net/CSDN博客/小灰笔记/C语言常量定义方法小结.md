# C语言常量定义方法小结 - 小灰笔记 - CSDN博客





2017年03月17日 00:41:02[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：4010








       之前一直只了解一种定义C语言中常量的方法，今天看书学到了第二种。其实，两种都需要掌握以下，否则指针部分的学习还真会有一点障碍。

       两种定义方式：

       1, const int a = 123;

       2, int const a = 123;

       上面两种其实都正确，但是第二种对我来说看起来确实是有些觉得不是很舒服，一直以来我都是使用第一种。写个测试代码：

#include"stdio.h"



const int a = 123;

int const b = 234;



int main(void)

{

       printf("value of a: %d\n",a);

       printf("value of b: %d\n",b);

       return 0;

}

       编译运行结果：

E:\WorkSpace\01_编程语言\01_C语言\03_declare>gcc exp_32.c



E:\WorkSpace\01_编程语言\01_C语言\03_declare>a

value of a: 123

value of b: 234

       由上面的结果可以看出，gcc下这两种定义方式全都可以正确识别且没有任何警告。换用了CodeWarrior进行了一下测试，也没有因此而产生任何警告。



