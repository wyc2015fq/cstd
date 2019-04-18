# C语言结构体成员内存分配 - 小灰笔记 - CSDN博客





2017年04月02日 17:26:10[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：660








       C语言中结构体内存分配曾经把我迷惑了好一阵子，确切说可以说是结构体对象所占用的空间判断让我迷惑了一阵子。把我之前探索的过程记录下来，做一下简单的学习总结。在此之前，我已经了解到了一点基本的信息，也就是：存储的分配一般是需要照顾到同类元素的连续，而不是做一个简单的数位累计。

       简单写一段代码，定义结构体同时输出一些可以用于判断的信息如下：

#include"stdio.h"



struct demo_tag{

       char a;

       int b;

       char c;

};



struct demo_tagvar1;



int main(void)

{

       printf("size of struct object is:%d\n",sizeof(var1));



       return 0;

}

       上面的代码中，由于a、c成员并不是连续的，因此所占用的空间并不能够合并。而由于其相邻的元素大小决定其占用空间。因此，两个char所占用的空间其实都是向int看齐。这样，三个元素一共占据12个字节的内存。代码的编译运行结果如下：

E:\WorkSpace\01_编程语言\01_C语言\exp_15>gcc exp_15.c



E:\WorkSpace\01_编程语言\01_C语言\exp_15>a

size of structobject is: 12

       如果相同类型的成员连续，那么占用的内存可能就会有压缩的可能性。例如如下的代码：

#include"stdio.h"



struct demo_tag{

       char a;

       char b;

       int c;

};



struct demo_tagvar1;



int main(void)

{

       printf("size of struct object is:%d\n",sizeof(var1));



       return 0;

}

       简单的预判分析：前面的两个char由于是连续分配，内存占用可以压缩占用2个字节。而这两个区域总和的内存占用需要向int看齐，因此会扩展到4个字节。如果，整个结构体对象会占用8个字节。代码的编译执行结果如下：

E:\WorkSpace\01_编程语言\01_C语言\exp_16>gcc exp_16.c



E:\WorkSpace\01_编程语言\01_C语言\exp_16>a

size of structobject is: 8

       结果跟预想一致。通过上面的两个字其实已经可以看出数据结构设计对于资源消耗的一点优化方向了，两个结构体包含的成员一样但是所占用的存储大小却不一样。占用空间小的方式无疑是一种能够优化资源占用的手段。

       再做一个稍微复杂一点的例子：

#include"stdio.h"



struct demo_tag{

       char a;

       short b;

       int c;

};



struct demo_tagvar1;



int main(void)

{

       printf("size of struct object is:%d\n",sizeof(var1));



       return 0;

}

       上面代码中，前两个元素由于所占用的空间不一致首先会有一个对齐的要求。这样，第一个char占用2个字节，第二个short占用2个字节。再加上接下来的int占用4个字节。整个结构体对象能够占用8个字节。代码编译验证结果：

E:\WorkSpace\01_编程语言\01_C语言\exp_17>gcc exp_17.c



E:\WorkSpace\01_编程语言\01_C语言\exp_17>a

size of structobject is: 8

       从上面的结果来看，推理的过程还是正确的。再做一个简单的小例子：

#include"stdio.h"



struct demo_tag{

       char a;

       int b;

       short c;

};



struct demo_tagvar1;



int main(void)

{

       printf("size of struct object is:%d\n",sizeof(var1));



       return 0;

}

       这段代码相应的分析其实应该跟第一个一致，进行编译运行验证推测是否正确：

E:\WorkSpace\01_编程语言\01_C语言\exp_18>gcc exp_18.c



E:\WorkSpace\01_编程语言\01_C语言\exp_18>a

size of structobject is: 12

       由上面的结果可以看出，推测正确。

       再看一个稍微复杂一点的例子：

#include"stdio.h"



struct demo_tag{

       char a;

       char b;

       char c;

       short d;

       int e;

};



struct demo_tagvar1;



int main(void)

{

       printf("size of struct objects is:%d\n",sizeof(var1));



       return 0;

}

       上面的代码中，前面的三个char共站3个字节，但是为了能够保持对齐这三个字节会与int对齐。因此，这三个成员会占用4个字节，接下来的short也会占用4个字节。再加上后面的整型，最终占用12个字节。代码的编译运行结果如下：

E:\WorkSpace\01_编程语言\01_C语言\exp_19>gcc exp_19.c



E:\WorkSpace\01_编程语言\01_C语言\exp_19>a

size of struct objectsis: 12

       同样的分析，应用于一个再加复杂一点的代码如下：

#include"stdio.h"



struct demo_tag{

       char a;

       char b;

       char c;

       short d;

       short e;

       int f;

};



struct demo_tagvar1;



int main(void)

{

       printf("size of struct object is:%d\n",sizeof(var1));



       return 0;

}

       与前面的代码不同的地方在于两个short，连续的short会进行统一的存储分配，可能会涉及到压缩。但是这里面的的元素恰好是4个字节，跟一个int一致。因此，存储的分配应该跟之前没有差异。代码的编译执行结果如下：

E:\WorkSpace\01_编程语言\01_C语言\exp_20>gcc exp_20.c



E:\WorkSpace\01_编程语言\01_C语言\exp_20>a

size of structobject is: 12

       从上面的结果可以看出，运行的结果与推断结果一致。

       总体看来，上面的推断中用到了一点之前我没有注意到的信息，而相应的原则我是从互联网上找到的，那就是：最终的补充对齐原则会保证结构体的总体内存分配会是最大成员存储的整数倍。通过这些简单的原则，基本上能够推断出一个结构体所占用的存储空间。



