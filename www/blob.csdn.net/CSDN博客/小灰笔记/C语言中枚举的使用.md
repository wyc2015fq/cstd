# C语言中枚举的使用 - 小灰笔记 - CSDN博客





2017年04月03日 21:06:53[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：402








       在我这几年的嵌入式软件开发工作期间，使用枚举的次数十分有限。在别人的代码中倒是用过，自己创建的枚举类型真是屈指可数。在《C专家编程》中有一段描述：像C语言这样的弱类型语言而言，很少有什么事情只能够靠枚举完成而不能够用#define来实现。后者的功能在我的代码中确实是出现的很多，但是枚举类型的优势在于其名字在调试器中都是可见的，这样对于调试代码来说是更为便利一点的。

       在结构使用上，枚举的语法格式跟结构体相似，但是枚举的内容块中成员之间的分隔使用的是逗号而不是分号。

代码1：

#include"stdio.h"



enum demo_tag{

       a = 1,

       b = 3,

       c = 5

} var1 = a,var2 =b,var3 = c;



int main(void)

{

       printf("value of a:%d\n",var1);

       printf("value of b:%d\n",var2);

       printf("value of c:%d\n",var3);



       return 0;

}

       上述代码在编译后的执行结果：

E:\WorkSpace\01_编程语言\01_C语言\exp_24>a

value of a: 1

value of b: 3

value of c: 5

       在没有专门指定时，成员（其实不是成员，应该叫做标识符）的整型值默认从0开始，这让我觉得很多时候枚举不过是整型的一种可读性加强的替代。如果指定之后，某个紧跟其后的标识符的数值会比它大一个数。

代码2：

#include"stdio.h"



enum demo_tag{

       a = 1,

       b = 3,

       c = 5,

       d

} var1 = a,var2 =b,var3 = c,var4 = d;



int main(void)

{

       printf("value of a:%d\n",var1);

       printf("value of b:%d\n",var2);

       printf("value of c:%d\n",var3);

       printf("value of d:%d\n",var4);



       return 0;

}

       代码的编译执行结果：

E:\WorkSpace\01_编程语言\01_C语言\exp_25>gcc exp_25.c



E:\WorkSpace\01_编程语言\01_C语言\exp_25>a

value of a: 1

value of b: 3

value of c: 5

value of d: 6

       从上面的运行结果可以看出标识符的增长规律。不过，这种规则在使用不当的时候就会带来程序上的问题。比如如下代码：

#include"stdio.h"



enum demo_tag{

       a = 1,

       b = 3,

       c = 5,

       d,

       e = 6

} var1 = a,var2 =b,var3 = c,var4 = d,var5 = e;



int main(void)

{

       printf("value of a:%d\n",var1);

       printf("value of b:%d\n",var2);

       printf("value of c:%d\n",var3);

       printf("value of d:%d\n",var4);

       printf("value of e:%d\n",var5);



       return 0;

}

       程序的编译执行结果：

E:\WorkSpace\01_编程语言\01_C语言\exp_25>gcc exp_25.c



E:\WorkSpace\01_编程语言\01_C语言\exp_25>a

value of a: 1

value of b: 3

value of c: 5

value of d: 6

value of e: 6

       d和e所代表的数值都是6，两者相同。这虽然编译以及运行没有问题，但是通常却不是我们想要的结果。

       同struct结构体一样，使用typedef构造新的数据类型别名能够让枚举类型的定义更为简洁一点。具体如下：

#include"stdio.h"



typedef enumdemo_tag{

       a = 1,

       b = 3,

       c = 5,

       d,

       e = 6

} demo_t;



demo_t var1 =a,var2 = b,var3 = c,var4 = d,var5 = e;



int main(void)

{

       printf("value of a:%d\n",var1);

       printf("value of b:%d\n",var2);

       printf("value of c:%d\n",var3);

       printf("value of d:%d\n",var4);

       printf("value of e:%d\n",var5);



       return 0;

}

       代码的编译执行结果：

E:\WorkSpace\01_编程语言\01_C语言\exp_25>gcc exp_25.c



E:\WorkSpace\01_编程语言\01_C语言\exp_25>a

value of a: 1

value of b: 3

value of c: 5

value of d: 6

value of e: 6



