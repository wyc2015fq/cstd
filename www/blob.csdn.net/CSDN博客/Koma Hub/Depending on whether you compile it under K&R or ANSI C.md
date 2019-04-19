# Depending on whether you compile it under K&R or ANSI C - Koma Hub - CSDN博客
2019年01月31日 20:54:08[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：37

```cpp
#include <stdio.h>
int array[] = { 23, 34, 12, 17, 204, 99, 16 };
#define TOTAL_ELEMENTS (sizeof(array) /sizeof(array[0]))
//#define TOTAL_ELEMENTS (sizeof(array) / sizeof(array[0]))
//instead of
//#define TOTAL_ELEMENTS (sizeof(array) / sizeof(int))
int main()
{
        if ( -1 < (unsigned char) 1 )
                printf("-1 is less than (unsigned char) 1: ANSI semantics \n");
        else
                printf("-1 NOT less than (unsigned char) 1: K&R semantics \n");
        printf("Sizeof array = %d\n", TOTAL_ELEMENTS);
        return 0;
}
```
```
$ ./a.exe
-1 is less than (unsigned char) 1: ANSI semantics
Sizeof array = 7
```
