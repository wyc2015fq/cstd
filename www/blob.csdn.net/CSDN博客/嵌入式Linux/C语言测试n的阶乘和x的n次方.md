
# C语言测试n的阶乘和x的n次方 - 嵌入式Linux - CSDN博客

2018年12月12日 16:54:48[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：271


**题目描述**
输入一个正数x和一个正整数n，求下列算式的值。要求定义两个调用函数：fact(n)计算n的阶乘；mypow(x,n)计算x的n次幂（即xn），两个函数的返回值类型是double。
![x - x^{2}/2! + x^3/3! + ... + (-1)^{n-1}x^n/n!](http://www.zhihu.com/equation?tex=+x+-+x%5E%7B2%7D%2F2%21+%2B+x%5E3%2F3%21+%2B+...+%2B+%28-1%29%5E%7Bn-1%7Dx%5En%2Fn%21)
×输出保留4位小数。
**输入**
x n
**输出**
数列和
**样例输入**
2.0 3
**样例输出**
1.3333

## 答案
`/*************************************************************************
        > File Name: 2.c
        > Author: 
        > Mail: 
        > Created Time: Wed 12 Dec 2018 09:03:22 AM CST
 ************************************************************************/
#include<stdio.h>
double fact(int n)
{
        double s = 1.0;
    for(int i=1; i<= n; i++)
        {
                s=s*i;
        } 
        return s;
}
double mypow(double x,int n)
{                 
    double s = 1.0;
   
    //printf("%lf %d\n",x,n);
        if(n == 0)    
    {             
      return 1.0; 
    }             
                  
    if(n == 1)    
    {             
      return x;                                                                               
    }             
     
    s = x;
    for(int i =0;i<n-1;i++)
    {             
      s = x*s;   
    }
    //printf("%lf \n",s);
    return s;     
}   
void main(void)
{
    double x = 0.0;
    int n = 0;
    double s;
    scanf("%lf %d",&x,&n);
 
    //printf("%lf\n",mypow(-1.0,2));
    if(n == 1)
    {
        s = x;
    }
    else
    {
        s = x;
        for(int i=2;i<=n;i++)
        {
            s = s+ mypow(-1.0,i-1)*mypow(x,i)/fact(i);
        }
    }
    printf("%.4lf\n",s);
}`同事提供的答案，不用函数实现
`#include <stdio.h>
int main ()
{
double x, ret, tmp1, tmp2;
int n, i, j;
while (~scanf("%lf %d", &x, &n))
{
ret = 0;
for (i = 1; i <= n; i++)
{
tmp1 = 1;
for (j = 1; j <= i; j++)
{
tmp1 *= x;
}
tmp2 = 1;
for (j = 1; j <= i; j++)
{
tmp2 *= j;
}
if (i % 2 == 1)
{
ret += tmp1 / tmp2;
}
else
{
ret -= tmp1 / tmp2;
}
}
printf("%.04f\n", ret);
}
return 0;
}`
**感兴趣的可以自行测试**
[2004年秋浙江省计算机等级考试二级C 编程题(2) - C语言网](http://link.zhihu.com/?target=http%3A//www.dotcpp.com/oj/problem1066.html)
![](https://pic3.zhimg.com/80/v2-0320c5c677bcb4c44a9ff7c3912739a6_hd.jpg)
欢迎关注微信公众号-**嵌入式Linux**
觉得不错，请帮忙**转发，点赞**，您的每一次支持，我都将铭记于心

