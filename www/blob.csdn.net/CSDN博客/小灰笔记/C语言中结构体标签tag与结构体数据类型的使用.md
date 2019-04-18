# C语言中结构体标签tag与结构体数据类型的使用 - 小灰笔记 - CSDN博客





2017年04月02日 22:53:00[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：2812







C语言中的数据结构实现很多时候会用到结构体功能，而反复定义结构体有几种不同的方法。这里先把三种常用方法阐述一下然后针对两种简化模式的方法进行简单的示范与总结。

方法1：

#include "stdio.h"



struct {
char a;
short b;
int c;

} var1;



struct {
char a;
short b;
int c;

} var2;



int main(void)

{
return 0;

}
上述方法定义了两个结构体变量var1、与var2，两个变量的构造方式相同。但是，这种定义方式有着很大的不变，因为反复重写相同的东西。对此做一下修改，于是有定义方法2。

方法2：

#include "stdio.h"



struct {
char a;
short b;
int c;

} var1,var2;



int main(void)

{
return 0;

}
这种方式少写了一次结构体，同时还能够实现两个变量的定义。不过，如果定义的结构体变量过多的时候代码就会有可读性以及维护性上的问题。对此改进有两种可选方式，也就是方法3-结构体tag以及方法4-结构体数据类型。

方法3：

#include "stdio.h"



struct demo_tag {
char a;
short b;
int c;

} var1;



struct demo_tag var2;



int main(void)

{
return 0;

}
上述代码同样实现了两个结构体变量的定义，但是第二种方式采用了结构体的tag用以代替了结构体的描述。这种定义方式每次需要些struct关键字，因为tag只是代表了结构体的描述信息。不过，如果出现大量的变量定义需要时，这种方式显然要比第二种方式好得多。

方法4：

#include "stdio.h"



typedef struct demo_tag {
char a;
short b;
int c;

} demo_t;



demo_t var1,var2;



int main(void)

{
return 0;

}
代码中的结构体tag其实是没有用了的，可以去掉。我个人感觉，这种方法是最好的一种使用方式。只需要构造一个结构体数据类型，接下来的结构体对象构建起来就简单很多了。而且，当出现大量的结构体数据时，代码需要维护的时候工作量也会小很多。不仅仅在于代码的字符会看着少一些，同时这种方式也比较适合最基本的C语言定义规则。当然，这得建立在你对这种技术有一定的熟知的基础上。
            


