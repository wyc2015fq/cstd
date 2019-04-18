# 068_C语言libc中的assert - 小灰笔记 - CSDN博客





2019年02月20日 21:00:56[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：32








         Assert，断言，大多时候适用于软件开发过程中的调试。大多数时候，我们开发过程中遇到故障的时候宁可希望软件在故障的位置停下来也不愿意跑一个不确定的结果，这个时候我们就需要用到assert。

         以下是一个例子：

```cpp
#include "stdio.h"
#include "assert.h"


int a = 123;
int b = 234;

int main(void)
{
    assert(a == 123);
    printf("a OK\n");
    assert(b == 123);
    printf("B OK\n");

    return 0;
}
```

         编译运行结果：

![](https://img-blog.csdnimg.cn/20190220205821923.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==,size_16,color_FFFFFF,t_70)

如果开发结束，这些测试代码也不需要删除，只需要使用NDEBUG的宏来屏蔽掉其执行效果即可。代码修改：

```cpp
#define NDEBUG

#include "stdio.h"
#include "assert.h"


int a = 123;
int b = 234;

int main(void)
{
    assert(a == 123);
    printf("a OK\n");
    assert(b == 123);
    printf("B OK\n");

    return 0;
}
```

         编译以及执行结果：

![](https://img-blog.csdnimg.cn/20190220205821853.png)

         需要注意的是，这个宏必须定义在assert.h包含之前，否则是不起作用的。例如如下代码：

```cpp
#include "stdio.h"
#include "assert.h"

#define NDEBUG

int a = 123;
int b = 234;

int main(void)
{
    assert(a == 123);
    printf("a OK\n");
    assert(b == 123);
    printf("B OK\n");

    return 0;
}
```



         编译以及运行结果：

![](https://img-blog.csdnimg.cn/20190220205821849.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==,size_16,color_FFFFFF,t_70)

         看得出，屏蔽没有起到作用。



