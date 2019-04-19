# C语言求一个数的倒数的平方根近似值 - Koma Hub - CSDN博客
2019年03月10日 15:09:39[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：40
先来看一个程序：
```cpp
#include <stdio.h>
int main()
{
    float a;
    printf("数据排列:\n\n");
    printf("请输入相应的数据:");
    scanf("%f", &a);
    printf("输出的数据:%-80.5s\n", a);
    
    return 0;
}
```
再来看一个程序
```cpp
#include <stdio.h>
float invSqrt(float x)
{
    int i = *(int *)&x;
    i = 1597463007-(i>>1);
    x = *(float*)&i;
    return x;
}
```
这是一段德国某位高中生用C语言写的求一个数的倒数的平方根近似值的程序。如果你能在一星期内通过任何途径，包括看书，google资料，自己动手编译等，能找出这段只有5行有效代码的程序的意义的话，你应该是C语言的真正的高手了。
转自微信：[这段c语言代码牛逼在哪？](http://mp.weixin.qq.com/s?__biz=MzIzNjgxNjA3MQ==&mid=2247484915&idx=1&sn=fe188561ed2f4e8820e0cb32d4402a6e&chksm=e8d3565bdfa4df4df17940fdd009c0a8fd0a74bce0b7ed07109833de850a13d01c9a72782fbd&mpshare=1&scene=1&srcid=0309bCM9YFZmPPGw9vHqxzT9#rd)
