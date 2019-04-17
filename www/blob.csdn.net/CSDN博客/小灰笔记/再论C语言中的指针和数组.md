# 再论C语言中的指针和数组 - 小灰笔记 - CSDN博客





2017年03月22日 00:44:34[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：150








       之前指针与数组的总结性的小文已经整理过多次了，自我自己的使用只是范畴中基本上够用了。不过，今天看书的时候发现有一个知识点遗漏，这里做一下补充。

       遗漏的知识点是关于数组名称所等同的指针的具体数据类型问题。其实，大多数能够找到的教程或者是文章基本上会总结到除了用于sizeof的参数，其他时候数组的名称基本上与指向数组元素的首元素的指针等同。不过，这个等效指针如果看做指针的话应该是一个什么数据类型呢？

       关于上面的问题，其实看过map或者elf文件，联想一下其中的信息基本上也可以得出答案。数组名对应的地址是固定不变的，在链接之后就有了固定的地址。既然有了固定的地址，那么考虑到指针或者地址的使用时就应该注意到，这个量应该是只能够读取但是不能够修改的。

       写一段代码如下：

#include"stdio.h"



int a[] ={1,2,3,4,5};

unsigned char b[]= {1,2,3,4,5};



int main(void)

{

       a++;

       return 0;

}

       代码编译结果：

E:\WorkSpace\01_编程语言\01_C语言\exp_05>gcc exp_05.c

exp_05.c: Infunction 'main':

exp_05.c:8:3:error: lvalue required as increment operand

  a++;

   ^

       虽然没有提示对常量进行赋值，但是提示出了如此修改是非法的。其实，便于理解，这种类似的现象我们可以理解为数组的名称是一个指针常量。

       代码修改，做一点其他的验证测试：

#include"stdio.h"



int a[] ={1,2,3,4,5};

unsigned char b[]= {'1','2','3','4','5'};



int main(void)

{

       printf("value of a element:%d\n",*a);

       printf("value of b element:%c\n",*b);



       printf("value of a element:%d\n",*(a + 1));

       printf("value of b element:%c\n",*(b + 1));



       return 0;

}

       代码编译运行结果：

E:\WorkSpace\01_编程语言\01_C语言\exp_05>gcc exp_05.c



E:\WorkSpace\01_编程语言\01_C语言\exp_05>a

value of aelement: 1

value of belement: 1

value of aelement: 2

value of belement: 2

       这里其实能够得出一个结论：作为指针的偏移增加或者指针变量的修改，变化值并一定是字节数乘以偏移量或者是整型大小乘以偏移量。指针指向地址的变化实际上是指针指向类型大小乘以偏移量。这是很“聪明”的一种处理，让我们少了很多判断的麻烦。尤其是当我们面临复杂的数据结构时，我们的思维可以放在数据对象本身上面而少一点考虑计算机存储的模型。



