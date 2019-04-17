# 【climit】INT_MIN & INT_MAX - CD's Coding - CSDN博客





2015年03月10日 23:46:16[糖果天王](https://me.csdn.net/okcd00)阅读数：852








众所周知，我们常常需要知道所谓的INF值，

那个2开头的一串数字代表int型的最大值，

虽然很多人都记住了，也有很多人用3f3f3f3f这样的方法来表示，

不过自然这种选择：limit.h里的INT_MIN & INT_MAX也常常被大家使用。

有时候会看到有人直接



```cpp
int minValue=-INT_MAX
```


这就需要吐槽了，为什么呢，在climit的定义里，它们是这样的



```cpp
#define INT_MAX 2147483647  
#define INT_MIN (- INT_MAX - 1)
```




网上查来的资料是这么说——



这里没有简单地将INT_MIN赋值成-2147483647，是因为-2147483648对于编译器而言是个表达式，而2147483648对于32-bit整数是无法表示的，所以经过这个表达式的结果是未定义的。在GCC上直接写-2147483648后，编译器给出了警告，说结果是unsigned。

*“An integer constant begins with a digit, but has no period or exponent part. It may have a prefix that specifies its base and a suffix that specifies its type.”*





The problem is that 2147483648 cannot be represented in a signed 32-bit integer, so it becomes either an unsigned long int or a long long int.


So we have to resort to a little trickery, and compute -2147483648 as (-2147483647 – 1), which all fit nicely into 32-bit signed integers, and INT_MIN gets
 the right type and value.

这里我解释一下：

这样一个程序可能可以帮上忙



```cpp
#include <stdio.h>
#include <limits.h>
#include <float.h>

int main(void)
{
	if (-2147483648 > 0)     printf("positive\n");
	if (-2147483647 - 1 < 0) printf("negative\n");
	if (INT_MIN == -INT_MIN) printf("equal\n");
	if (FLT_MIN > 0)         printf("floating\n");

	return 0;
}
```



所以，使用的话就加上头文件climit，

然后轻松愉快地用INT_MAX和INT_MIN吧



