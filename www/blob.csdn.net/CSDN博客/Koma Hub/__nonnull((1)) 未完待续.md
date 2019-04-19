# __nonnull((1))  未完待续 - Koma Hub - CSDN博客
2019年01月24日 21:51:03[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：74
```cpp
#include <stdio.h>
int fun(char *str) __nonnull((1));
int main()
{
    fun(NULL);
}
int fun(char *str)
{
    printf("%p\n", str);
}
```
未完待续
