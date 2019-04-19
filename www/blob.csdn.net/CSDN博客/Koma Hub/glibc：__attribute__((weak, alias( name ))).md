# glibc：__attribute__((weak, alias("name"))) - Koma Hub - CSDN博客
2019年01月28日 21:40:02[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：34
```cpp
#include <stdio.h>
int fun()
{
        printf("func\n");
        return 0;
}
int f() __attribute__((weak, alias("fun")));
#define weak_alias(name, aliasname) \
        typeof(name) aliasname __attribute__((weak, alias(#name)))
weak_alias(fun, fff);
#define strong_alias(name, aliasname) \
        typeof(name) aliasname __attribute__((alias(#name)))
strong_alias(fun, ffff);
int main()
{
        f();
        fff();
        ffff();
        return 0;
}
```
```
$ ./a.exe
func
func
func
```
