# 【C语言】深入理解const,volatile,static关键字 - csdn_baotai的博客 - CSDN博客

2018年05月05日 16:45:56[csdn-[baotai]](https://me.csdn.net/csdn_baotai)阅读数：81


我们先来看一段代码：

```cpp
#include <stdio.h>
int main()
{
const int n = 10;
int *p = (int *)&n;
*p = 20;
printf("%d\n", n);
return 0;
}
```

//看看程序输出的结果是什么？

只要编译器适当的对代码进⾏优化，这⾥就可能输出： 10，⽽不是我们改变之后的值。

这是为什么呢？
是编译器在编译期间，可能对代码进⾏优化。
当编译器看到这⾥的num被const修饰，从语义上讲这⾥的num是不期望被改变（不改变）

的，那优化的时候就可以把num的值存放到寄存器（以提⾼访问的效率）中。以后只要使⽤

num的地⽅都去寄存器中取，那即使num对应的内存中的值发⽣变化，寄存器也是感知不到

的。所以造成输出10的结果。

当我们对代码进⾏如下修改：

const的深度讲解及volatile关键字介绍

```cpp
#include <stdio.h>
int main()
{
//使⽤volatile关键字对num修饰。
volatile const int n = 10;
int *p = (int *)&n;
*p = 20;
printf("%d\n", n);
return 0;
}
```

结果⼜会是什么呢？

这⾥我们可以看到，当我们对*p做了修改之后， num的输出变成了20。

这⾥ volatile 这个关键字起到关键的作业。

那我们说说 volatile 关键字的作⽤和使⽤场景。

volatile作⽤：
编译时不优化，执⾏时不缓存，每次需从内存中读出（保证内存的可⻅性）。

场景：
⽤于多线程或多CPU编程。
如果不显示的初始化，静态变量的初始化为0，

⽽⾃动变量是不做任何初始化的，是随机值

### 程序运行结果：![](https://img-blog.csdn.net/20180505164343792?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2NzZG5fYmFvdGFp/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

## static的总结：

1. 当它作⽤于函数定义时，或者⽤于代码块之外的变量声明时， static关键字⽤于修改标示符

的链接属性。外部链接属性变为内部链接属性，标识符的存储类型和作⽤域不受影响。也就

是说变量或者函数只能在当前源⽂件中访问。不能在其他源⽂件中访问。

2. 当它⽤于代码块内部的变量声明时， static关键字⽤于修改变量的存储类型。从⾃动变量变

为静态变量。变量的属性和作⽤域不受影响。

