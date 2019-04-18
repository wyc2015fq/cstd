# C语言中typedef与#define实现数据类型上的异同 - 小灰笔记 - CSDN博客





2017年04月02日 22:34:01[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：1036








       很多时候，实现一个数据类型的别名可以采用typedef或者#define两种方式，但是有些情况下两种实现方式的行为却有一些差异。

       typedef的作用是给数据类型取一个别名，但是别名产生之后却不一定具有之前类型的所有使用方式。例如，写如下代码：

#include"stdio.h"



typedef intint32_T;



int main(void)

{

       int32_T a = 0;

       unsigned int b = 0U;

       unsigned int32_T c = 0U;



       return 0;

}

       上述代码中a与b的定义都没有问题，但是到了c的定义却会遇到一点点异常。具体的编译提示如下：

E:\WorkSpace\01_编程语言\01_C语言\exp_21>gcc exp_21.c

exp_21.c: Infunction 'main':

exp_21.c:9:19:error: expected '=', ',', ';', 'asm' or '__attribute__' before 'c'

  unsigned int32_T c = 0U;

                   ^

exp_21.c:9:19:error: 'c' undeclared (first use in this function)

exp_21.c:9:19:note: each undeclared identifier is reported only once for each function itappears in

       而上述问题出现的原因就是使用typedef定义的新的数据类型别名并不是百分百具备原来的操作属性。而同样的情况下，使用#define却会避免相应的问题。因为#define的方式实际上是决定预处理阶段，而实现的方式是进行百分百的替换。代码修改如下：

#include"stdio.h"



/* 

 * typedef int int32_T;

 * */

#define int32_Tint



int main(void)

{

       int32_T a = 0;

       unsigned int b = 0U;

       unsigned int32_T c = 0U;



       return 0;

}

       对上述代码进行编译没有报出任何问题。不过，这并不是说明使用#define实现数据类型别名回避typedef好。相反，很多时候使用后者会是更好的选择。例如如下代码示例：

/* demo-1 */

typedef int *int_prt;



int_prt a,b;



/* demo-2 */



#define int*int_prt



int_prt a,b;

       两段代码，第一种能够实现b，b同时定义为一个指向整型的指针。而第二种实际上定义了一个指向整型的指针a而一个整型b。这样的用法会让第二种方式带来一定的代码可读性以及可维护性的障碍。

       摘录《C专家编程》中的一段话如下：

              typedef实现的数据类型能够保证声明的变量都为同一数据类型，而#define则不一定。其实，为了代码的安全性，最好还是使用typedef，我想这也是很多编码规范中对此要求的一个原因。



