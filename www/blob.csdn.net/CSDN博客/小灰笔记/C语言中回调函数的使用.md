# C语言中回调函数的使用 - 小灰笔记 - CSDN博客





2017年03月23日 00:43:16[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：181








       C语言中传递给函数的参数可以是一个指向函数的指针，这样，指针指向的函数在函数执行的时候被调用，因此叫做回调函数。如此，回调函数涉及的主要的知识点也就是函数指针。

       使用回调函数的场景常见的有两个：一个是实现通用性的函数；第二个是实现跳转表功能的简化。

       关于通用型函数的实例代码;

#include"stdio.h"



intCompareInt(void const *a,void const *b);

intCompareFloat(void const *a,void const *b);



int main(void)

{

       int a = 4;

       int b = 5;



       float c = 5.0f;

       float d = 1.0f;



       printf("%d\n",CompareInt(&a,&b));

       printf("%d\n",CompareFloat(&c,&d));

       return 0;

}



intCompareInt(void const *a,void const *b)

{

       if(*((int*)a) == *((int*)b))

       {

                     return 0;

       }

       else

       {

              return 1;

       }

}



intCompareFloat(void const *a,void const *b)

{

       if(*((float*)a) == *((float*)b))

       {

                     return 0;

       }

       else

       {

              return 1;

       }

}      

       如果是把printf语句当做一个函数的话，上面就能够看出回调函数的影子了。其实，换成函数的参数使用方法差不多。

       至于跳转表的实现，只是上面的一个在应用方向上的划分了。



