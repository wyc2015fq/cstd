# hdu 5979 Convex（正弦定理求三角形面积） - HJ - CSDN博客
2017年10月27日 13:32:43[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：584
[http://acm.hdu.edu.cn/showproblem.php?pid=5979](http://acm.hdu.edu.cn/showproblem.php?pid=5979)
题目大意： 
给出n多边形的中心圆角大小，求这个多边形的面积。
解题思路： 
划分成n个三角形，每个三角形用正弦定理求面积累加即可。
```cpp
#include<iostream>
#include<cstdio>
#include<cmath>
using namespace std;
#define PI acos(-1.0)
int main()
{
    int n,d,x;
    while(cin>>n>>d)
    {
        double ans=0;
        for(int i=0;i<n;i++)
        {
            cin>>x;
            ans+=0.5*sin(1.0*x/180*PI)*d*d;
        }
        printf("%.3lf\n",ans);
    }
    return 0;
}
```
