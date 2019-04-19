# STM8串口打印调试信息 - xqhrs232的专栏 - CSDN博客
2017年11月07日 23:31:21[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：189
个人分类：[ARM/STM32/LPC17/单片机技术](https://blog.csdn.net/xqhrs232/article/category/939440)
原文地址::[http://blog.csdn.net/u011574686/article/details/53260192](http://blog.csdn.net/u011574686/article/details/53260192)
- [stm8S
 串口输出调试信息](http://blog.csdn.net/u011574686/article/details/53260192#stm8s-%E4%B8%B2%E5%8F%A3%E8%BE%93%E5%87%BA%E8%B0%83%E8%AF%95%E4%BF%A1%E6%81%AF)- [自己写的API函数](http://blog.csdn.net/u011574686/article/details/53260192#%E8%87%AA%E5%B7%B1%E5%86%99%E7%9A%84api%E5%87%BD%E6%95%B0)
- [printf
 函数重映射](http://blog.csdn.net/u011574686/article/details/53260192#printf-%E5%87%BD%E6%95%B0%E9%87%8D%E6%98%A0%E5%B0%84)
# stm8S 串口输出调试信息
## 自己写的API函数
```
static void uart1SendMsg(char msg[], uint16_t length)
{
       uint16_t i;
    for (i =0 ;i < length;i++)
    {
        UART1_SendData8(msg[i]);
        while(UART1_GetFlagStatus(UART1_FLAG_TC) == RESET);
    }
}
"`
void myPrintfAscii(char *msg)
{
    while((*msg) != '\0')
    {
        uart1SendMsg(msg, 1);
        msg++;
    }
}
void myPrintfInt8(uint8_t msg[], int length)
{
    char int2Ascii[64];
    uint16_t i, m, intLength;
    int n;
    long temp;
    for (i = 0;i < length;i++)
    {
        m = 0;
        temp = msg[i];
        while(m <= 64)
        {
            int2Ascii[m++] = temp % 10;
            temp /= 10;
            if (0 == temp)
            {
                break;
            }
        }
        intLength = m;
        for (n = intLength - 1;n >= 0;n--)
        {
            int2Ascii[n] += '0';
            uart1SendMsg(int2Ascii + n, 1);
        }
        myPrintfAscii(" ");
    }
}
void myPrintfInt16(uint16_t msg[], int length)
{
    char int2Ascii[64];
    uint16_t i, m, intLength;
    int n;
    long temp;
    for (i = 0;i < length;i++)
    {
        m = 0;
        temp = msg[i];
        while(m <= 64)
        {
            int2Ascii[m++] = temp % 10;
            temp /= 10;
            if (0 == temp)
            {
                break;
            }
        }
        intLength = m;
        for (n = intLength - 1;n >= 0;n--)
        {
            int2Ascii[n] += '0';
            uart1SendMsg(int2Ascii + n, 1);
        }
        myPrintfAscii("\r\n");
    }
}
void myPrintfInt32(uint32_t msg[], int length)
{
    char int2Ascii[64];
    uint16_t i, m, intLength;
    int n;
    long temp;
    for (i = 0;i < length;i++)
    {
        m = 0;
        temp = msg[i];
        while(m <= 64)
        {
            int2Ascii[m++] = temp % 10;
            temp /= 10;
            if (0 == temp)
            {
                break;
            }
        }
        intLength = m;
        for (n = intLength - 1;n >= 0;n--)
        {
            int2Ascii[n] += '0';
            uart1SendMsg(int2Ascii + n, 1);
        }
        myPrintfAscii(" ");
    }
}
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 24
- 25
- 26
- 27
- 28
- 29
- 30
- 31
- 32
- 33
- 34
- 35
- 36
- 37
- 38
- 39
- 40
- 41
- 42
- 43
- 44
- 45
- 46
- 47
- 48
- 49
- 50
- 51
- 52
- 53
- 54
- 55
- 56
- 57
- 58
- 59
- 60
- 61
- 62
- 63
- 64
- 65
- 66
- 67
- 68
- 69
- 70
- 71
- 72
- 73
- 74
- 75
- 76
- 77
- 78
- 79
- 80
- 81
- 82
- 83
- 84
- 85
- 86
- 87
- 88
- 89
- 90
- 91
- 92
- 93
- 94
- 95
- 96
- 97
- 98
- 99
- 100
- 101
- 102
- 103
- 104
- 105
- 106
- 107
- 108
- 109
- 110
上述方法相对笨一点，而且无法打印负数，浮点数。下面直接使用printf标准输入输出函数，将调试信息串口打印出来。
## printf 函数重映射
使用printf函数将调试信息用串口打印出来，首先在有printf函数的C文件下包含 `#include <stdio.h>`，其次将printf函数重定向串口输出1， 参考[网址1](http://www.openedv.com/posts/list/17302.htm)。贴代码：
重映射代码，myPrintf.c
```
#include "stm8s.h"
#include <stdio.h>
/*
 由于不同的编译器 putcha 和 getchar 形参和返回值略有不同。
 因此此处采用宏定义的方式区别。
 _RAISONANCE_ 和 _COSMIC_ 这2个宏是由编译器自动添加的预编译宏
*/
#ifdef _RAISONANCE_
 #define PUTCHAR_PROTOTYPE int putchar (char c)
 #define GETCHAR_PROTOTYPE int getchar (void)
#elif defined (_COSMIC_)
 #define PUTCHAR_PROTOTYPE char putchar (char c)
 #define GETCHAR_PROTOTYPE char getchar (void)
#else /* _IAR_ */
 #define PUTCHAR_PROTOTYPE int putchar (int c)
 #define GETCHAR_PROTOTYPE int getchar (void)
#endif /* _RAISONANCE_ */
PUTCHAR_PROTOTYPE
{
 /* 发送一个字符 c 到UART1 */
 UART1_SendData8(c);
 /* 等待发送完毕 */
 while (UART1_GetFlagStatus(UART1_FLAG_TXE) == RESET);
 return (c);
}
GETCHAR_PROTOTYPE
{
 #ifdef _COSMIC_
  char c = 0;
 #else
  int c = 0;
 #endif
 /* 等待新数据到达  */
 while (UART1_GetFlagStatus(UART1_FLAG_RXNE) == RESET);
 /* 读取数据寄存器 */
 c = UART1_ReceiveData8();
 return (c);
}
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 24
- 25
- 26
- 27
- 28
- 29
- 30
- 31
- 32
- 33
- 34
- 35
- 36
- 37
- 38
- 39
- 40
- 41
- 42
main.c 主要输出部分：
```cpp
#include <stdio.h>
void main(void)
{       
    myUart1Init (115200);
    printf ("STM8S Start.\r\n");
    while(1)
    {   
    }
}
```

