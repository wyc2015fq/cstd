# hdu  2899  Strange fuction（二分） - HJ - CSDN博客
2016年08月08日 22:33:03[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：215
个人分类：[====ACM====																[-----------搜索-----------																[---------OJ---------																[HDU																[二分&三分](https://blog.csdn.net/feizaoSYUACM/article/category/7238038)](https://blog.csdn.net/feizaoSYUACM/article/category/6308041)](https://blog.csdn.net/feizaoSYUACM/article/category/6308067)](https://blog.csdn.net/feizaoSYUACM/article/category/6307975)](https://blog.csdn.net/feizaoSYUACM/article/category/6308070)
Strange fuction
Problem Description 
Now, here is a fuction: 
  F(x) = 6 * x^7+8*x^6+7*x^3+5*x^2-y*x (0 <= x <=100) 
Can you find the minimum value when x is between 0 and 100.
Input 
The first line of the input contains an integer T(1<=T<=100) which means the number of test cases. Then T lines follow, each line has only one real numbers Y.(0 < Y <1e10)
Output 
Just the minimum value (accurate up to 4 decimal places),when x is between 0 and 100.
Sample Input 
2 
100 
200
Sample Output 
-74.4291 
-178.8534
解析：二分求解，需要先求一阶导数，再二分循环。
提示：二分的前提条件是单调，单调，单调！！！（重要事情说三遍）
代码如下：
```cpp
#include<iostream>
#include<cstdio>
#include<cmath>
using namespace std;
double y;
double sum(double a)
{
    return 42*pow(a,6)+48*pow(a,5)+21*a*a+10*a;    //一次求导之后函数公式
}
double ans(double x)
{
    return 6*pow(x,7)+8*pow(x,6)+7*pow(x,3)+5*pow(x,2)-y*x;   //原函数公式
}
int main()
{
    int t;
    cin>>t;
    while(t--)
    {
        cin>>y;
        double right,left,m;
        if(sum(100.0)-y<=0)
        {
            printf("%.4lf\n",ans(100.0));
            continue;
        }
        left=0.0;
        right=100.0;
        while(right-left>1e-8)         //二分精度取段
        {
             m=(right+left)/2;
            if(sum(m)-y<0)
                left=m;
            else
                right=m;
        }
        printf("%.4lf\n",ans((left+right)/2));
    }
    return 0;
}
```
