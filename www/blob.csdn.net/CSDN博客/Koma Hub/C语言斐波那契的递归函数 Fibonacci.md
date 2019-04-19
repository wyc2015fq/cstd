# C语言斐波那契的递归函数 Fibonacci - Koma Hub - CSDN博客
2017年12月05日 20:52:48[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：747
```cpp
#include<stdio.h>
#include<stdlib.h>
/* 斐波那契的递归函数 */
int Fibonacci(int i)
{
    if(i<=1)
        return 1;
    else if(i==2)
        return 2;
    else 
        return Fibonacci(i-1) + Fibonacci(i-2);
}
int main(int argc, char*argv[])
{
    int n = 15;
    int i;
    for(i=0;i<n;i++)
        printf("%d - ",Fibonacci(i));
    printf("\n");
	return 0;
}
```
结果为：
```
-bash-4.1$ gcc -o a Demo.c
-bash-4.1$ ./a
1 - 1 - 2 - 3 - 5 - 8 - 13 - 21 - 34 - 55 - 89 - 144 - 233 - 377 - 610 -
```
