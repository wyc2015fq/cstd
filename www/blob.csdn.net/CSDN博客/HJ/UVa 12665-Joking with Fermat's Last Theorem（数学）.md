# UVa 12665 - Joking with Fermat's Last Theorem（数学） - HJ - CSDN博客
2017年04月08日 18:07:06[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：323
个人分类：[====ACM====																[----------数学----------																[---------OJ---------](https://blog.csdn.net/feizaoSYUACM/article/category/6308067)](https://blog.csdn.net/feizaoSYUACM/article/category/6130298)](https://blog.csdn.net/feizaoSYUACM/article/category/6308070)
题目链接：[https://uva.onlinejudge.org/index.php?option=com_onlinejudge&Itemid=8&page=show_problem&problem=4403](https://uva.onlinejudge.org/index.php?option=com_onlinejudge&Itemid=8&page=show_problem&problem=4403)
题目大意：
给出一个范围[x,y]，要求在这个范围内找出任意三个数a、b、c，满足
```
a^3+b^3=c*10+3
```
求出能满足的个数有多少。
解题思路： 
因为给的数据太大，我们需要从c来入手，因为c*10+3=a^3+b^3，其中a、b、c最大为10的八次方，那么就可以推出a、b的数据范围不会超过1000。接着就直接暴力好了。
```
#include<iostream>
#include<cstdio>
#include<cmath>
using namespace std;
int main()
{
    int x,y,tt=0;
    while(cin>>x>>y)
    {
        int k=pow(y*10+3,1.0/3),ans=0;
        for(int i=x;i<=k;i++)
          for(int j=x;j<=k;j++)
            if(((i*i*i+j*j*j)%10==3)&&((i*i*i+j*j*j)<=y*10+3))
              ans++;
        printf("Case %d: %d\n",++tt,ans);
    }
    return 0;
}
```
