# C中const 定义常量的使用 - 在思索中前行！ - CSDN博客





2014年05月23日 15:40:23[_Tham](https://me.csdn.net/txl16211)阅读数：609







- 先看如下代码
- 
```cpp
#include <stdio.h>
#include <string.h>

#define ARRSIZE(a) (sizeof(a)/sizeof(a[0]))
int main()
{
    const int i = 10;
    int *p = (int *)&i;
    int a[i];
    *p = 100;

    printf("%d\n", ARRSIZE(a));
    printf("%d\t%d\n", i, *p);
    return 0;
}
```



我用gcc编译运行，上面这段简单的代码的输出结果是：


100               # 这也是一个很奇怪的结果。


100    100





这个结果说明了2个问题，1：可以用const变量作为数组大小；2：可以修改const变量的值。





但难道真是这样吗？又或者说，难道在不同机器，不同编译器下都这样吗？





并不尽然，首先这个程序在VC编译器下，不能通过，VC编译器不允许用const变量作为数组大小。


即使把有关数组的给删掉，VC编译后的可执行文件不能执行，发生内存错误。这说明不可以修改const变量。





因此这个是与编译器有关的。





下面要思考的是：为什么gcc编译器能允许这事发生，而VC不允许。


对上面的代码略做修改，将const int i = 10放到main函数外面去。

```cpp
#include <stdio.h>
#include <string.h>

#define ARRSIZE(a) (sizeof(a)/sizeof(a[0]))
const int i = 10;
int main()
{
    int *p = (int *)&i;
        int a[i];

    printf("%d\n", ARRSIZE(a));
    *p = 100;
    printf("%d\t%d\n", i, *p);
    return 0;
}
```



这时gcc编译通过，运行出现段错误。段错误是经典错误，表明访问了不该访问的段。


因此这说明全局的const变量不能修改；局部的const能修改。





作如下推测：


gcc下，全局的const变量存储于只读的数据段，因此肯定不能修改。


局部的const存储在堆栈段，堆栈段是可读可写的；因此不能避免程序员去修改const。





虽然局部const在堆栈，且堆栈可写，但如下方式却行不通：


const int i = 10;


i = 100；        # 这里的错误是编译错误。


因此编译器不允许你对局部const变量[直接]修改，它能够识别你在修改const变量。


但正如前面的程序，你只要将局部const变量的地址赋给指针，就能骗过编译器了。


而在VC下，const变量总是在只读数据段里。





这时再来看数组

```cpp
#include <stdio.h>
#include <string.h>

#define ARRSIZE(a) (sizeof(a)/sizeof(a[0]))
const int i = 10;
int a[i];      // 将数组拿到main的外面来
int main()
{
    printf("%d\n", ARRSIZE(a));
    return 0;
}
```



这时会有编译错误：error: variably modified ‘a’ at file scope


如果将"int a[i]"放到main函数里，则正确。





我还不太清楚这样的具体原因。这里我只能大致得出结论是：不能用const变量来作为数组大小。


至于为什么将"int a[i]"放到main函数里时没有错误，我并不清楚，我看了下汇编码，发现gcc对这种情况进行了一些特殊的处理，并不是简单的分配堆栈空间，但更细致的原因，我并未深入研究。





另外在VC下，是不能用const变量来作为数组大小的，不管是局部还是全局的const。





因此不要试着去修改const变量，也不要用const变量来作为数组大小


- 



