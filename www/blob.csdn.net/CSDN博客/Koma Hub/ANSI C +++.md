# ANSI C:+++ - Koma Hub - CSDN博客
2019年01月31日 21:48:08[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：43
```cpp
#include <stdio.h>
#define _s(i) printf("%s: %d\n", #i, i)
int main()
{
        int x = 1;
        int y = 2;
        int z = x+++y;
        _s(x);_s(y);_s(z);
        //z = x+++++y; //error
        z = x++ + ++y;
        _s(x);_s(y);_s(z);
}
```
