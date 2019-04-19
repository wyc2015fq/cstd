# C++实现费氏数列算法 - fanyun的博客 - CSDN博客
2019年01月31日 19:58:51[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：59
1.用C++实现费氏数列
2.费氏数列
     Fibonacci为1200年代的欧洲数学家，在他的着作中曾经提到：若有一只兔子每个月生一只小兔子，一个月后也开
始生产。起初只有一只兔子，一个月后就有两只兔子，二个月后就有三只兔子，三个月后有五只兔子(小兔子投入
生产)……
如果不太理解这个例子的话，举个图就知道了，注意新生的小兔子需一个月成长期才会投入生产，类似的道理也可
以用于植物生长这就是Fibonacci数列，一般习惯称之为费式数列，例如一下：1，1，2，3，5，8，13，21，34，
55，89。
3.实现思路：
Fn = Fn-1 + Fn-2， n > 1 
Fn = n, n = 0 或 1 
*/ 
4.实现代码如下
```cpp
#include <stdio.h>
#include <stdlib.h>
#define N 20
int main(void)
{
    int Fib[N] = {0};
    int i;
    
    Fib[0] = 0;
    Fib[1] = 1;
    
    for(i = 2; i < N; i++)
        Fib[i] = Fib[i-2] + Fib[i-1];
        
    for(i = 0; i < N; i++)
    {
        printf("%d", Fib[i]);
        printf(" ");
    }
        
    printf("\n"); 
    return 0;
}
```
5.执行结果
![](https://img-blog.csdnimg.cn/20190131082837899.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Zhbnl1bl8wMQ==,size_16,color_FFFFFF,t_70)
