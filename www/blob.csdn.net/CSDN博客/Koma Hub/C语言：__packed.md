# C语言：__packed - Koma Hub - CSDN博客
2019年01月26日 09:44:01[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：138

```cpp
#include <stdio.h>
#ifndef __packed
#define __packed __attribute__((packed))
#else
#pragma message "Already defined __packed"
#endif
struct node{
        int i;
        char c;
        float f;
        char ch;
        double d;
} __packed;
int main()
{
        printf("sizeof struct node is %d\n", sizeof(struct node));
        return 0;
}
```
__packed在linux内核中早有定义：
```cpp
#ifndef __packed
#define __packed __attribute((packed))
#endif
```
上面是在文件dump_psb.c中的一小段代码。
