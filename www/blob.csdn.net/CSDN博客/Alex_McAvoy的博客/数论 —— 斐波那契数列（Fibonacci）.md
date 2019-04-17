# 数论 —— 斐波那契数列（Fibonacci） - Alex_McAvoy的博客 - CSDN博客





2018年04月23日 21:44:02[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：128








# 【概述】

斐波那契数列（Fibonacci sequence），又称黄金分割数列，其指的是这样一个数列：1, 1, 2, 3, 5, 8, 13, 21, 34, 55, 89, 144, 233，377，610，987，1597，2584，4181，6765，10946，17711，28657，46368....这个数列从第 3 项开始，每一项都等于前两项之和。

斐波那契数列除应用递推、递归定义解题外，还可用于改进二分查找——斐波那契数列查找。

关于斐波那契查找：[点击这里](https://blog.csdn.net/u011815404/article/details/80143165)

# 【特性】
- 奇数项求和：![f_1+f_3+f_5+...+f_{2n-1}=f_{2n}-f_2+f_1](https://private.codecogs.com/gif.latex?f_1&plus;f_3&plus;f_5&plus;...&plus;f_%7B2n-1%7D%3Df_%7B2n%7D-f_2&plus;f_1)
- 偶数项求和：![f_2+f_4+f_6+...+f_{2n}=f_{2n+1}-f_1](https://private.codecogs.com/gif.latex?f_2&plus;f_4&plus;f_6&plus;...&plus;f_%7B2n%7D%3Df_%7B2n&plus;1%7D-f_1)
- 平方求和：![f_1^2+f_2^2+f_3^2+...+f_{n}^2=f_{n}*f_{n+1}](https://private.codecogs.com/gif.latex?f_1%5E2&plus;f_2%5E2&plus;f_3%5E2&plus;...&plus;f_%7Bn%7D%5E2%3Df_%7Bn%7D*f_%7Bn&plus;1%7D)
- 二倍项关系：![f_{2n}/f_{n}=f_{n-1}+f_{n+1}](https://private.codecogs.com/gif.latex?f_%7B2n%7D/f_%7Bn%7D%3Df_%7Bn-1%7D&plus;f_%7Bn&plus;1%7D)
- 平方项与隔项：![\left\{\begin{matrix}f_{n-1}f_{n+1}-f_n^2=(-1)^n \\ f_n^2-f_{n-1}f_{n+1}=(-1)^{n-1} \end{matrix}\right.](https://private.codecogs.com/gif.latex?%5Cleft%5C%7B%5Cbegin%7Bmatrix%7Df_%7Bn-1%7Df_%7Bn&plus;1%7D-f_n%5E2%3D%28-1%29%5En%20%5C%5C%20f_n%5E2-f_%7Bn-1%7Df_%7Bn&plus;1%7D%3D%28-1%29%5E%7Bn-1%7D%20%5Cend%7Bmatrix%7D%5Cright.)

# 【公式】

## 1.通项公式

![](https://img-blog.csdn.net/20180731202542443?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

## 2.递推公式

根据定义可知：F0=0，F1=1，F2=F1+F0=1，F3=F2+F1=2，F4=F3+F2=3，F5=F4+F3=5，……

```cpp
int Fibonacci(int n)  
{  
    if (n<0)  
        return -1;  
    
    int n1=1,n2=2,n3=3;
    for (int i=3;i<=n;++i)  
    {  
        n3=n1+n2;  
        n1=n2;  
        n2=n3;  
    }  
    
    return n3;  
}
```

## 3.递归公式

如果设 F(n）为该数列的第n项，那么斐波那契数列可以写成如下形式：![F(0)=0,F(1)=1,F(n)=F(n-1)+F(n-2)](https://private.codecogs.com/gif.latex?F%280%29%3D0%2CF%281%29%3D1%2CF%28n%29%3DF%28n-1%29&plus;F%28n-2%29)

```cpp
int fibonacci(int n)
{
    if (n == 1 || n == 2)// 递归终止条件
        return 1;// 简单情景
    return fibonacci(n-1)+fibonacci(n-2); // 相同重复逻辑，缩小问题的规模
}
```

# 【例题】
- 斐波那契数列（信息学奥赛一本通-T1201）**(递归实现)**：[点击这里](https://blog.csdn.net/u011815404/article/details/80261447)
- 斐波那契数列（信息学奥赛一本通-T1188）**(递推实现)**：[点击这里](https://blog.csdn.net/u011815404/article/details/80085033)
- 爬楼梯（信息学奥赛一本通-T1204）**(推导+斐波那契数列)**：[点击这里](https://blog.csdn.net/u011815404/article/details/80275407)
- 一只小蜜蜂（HDU-2044）**(推导+斐波那契数列)**：[点击这里](https://blog.csdn.net/u011815404/article/details/79721181)
- 骨牌铺方格（HDU-2046）**(推导+斐波那契数列)**：[点击这里](https://blog.csdn.net/u011815404/article/details/79747960)
- 数楼梯（洛谷-P1255）**(高精度+斐波那契数列)**：[点击这里](https://blog.csdn.net/u011815404/article/details/79921483)



