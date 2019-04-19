# C语言__builtin_choose_expr实现函数重载 - Koma Hub - CSDN博客
2019年01月24日 21:32:09[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：61
__builtin_choose_expr是编译阶段的行为，不是运行时的行为。
```cpp
#include <stdlib.h>
#include <stdio.h>
void show_c(char a)
{
    printf("char: %c\n", a);
}
void show_f(float a)
{
    printf("float: %f\n", a);
}
#define _C(a, b) __builtin_types_compatible_p(a, b)
#define _E(exp, e1, e2) __builtin_choose_expr(exp, e1, e2)
#define show(a) \
    _E(_C(typeof(a), char), show_c(a), \
        _E(_C(typeof(a), float), show_f(a), printf("nothing\n")))
    
int main()
{
    show('a');
    show((int)'a');
    show((char)'a');
    show((float)'a');
}
```
结果：
```
$ ./a.exe
nothing
nothing
char: a
float: 97.000000
```
