# Biorhythms（POJ-1006） - Alex_McAvoy的博客 - CSDN博客





2018年07月31日 20:09:12[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：42
个人分类：[POJ																[数论——方程](https://blog.csdn.net/u011815404/article/category/8796916)](https://blog.csdn.net/u011815404/article/category/7504500)








> 
# Problem Description

人生来就有三个生理周期，分别为体力、感情和智力周期，它们的周期长度为23天、28天和33天。每一个周期中有一天是高峰。在高峰这天，人会在相应的方面表现出色。例如，智力周期的高峰，人会思维敏捷，精力容易高度集中。因为三个周期的周长不同，所以通常三个周期的高峰不会落在同一天。对于每个人，我们想知道何时三个高峰落在同一天。对于每个周期，我们会给出从当前年份的第一天开始，到出现高峰的天数（不一定是第一次高峰出现的时间）。你的任务是给定一个从当年第一天开始数的天数，输出从给定时间开始（不包括给定时间）下一次三个高峰落在同一天的时间（距给定时间的天数）。例如：给定时间为10，下次出现三个高峰同天的时间是12，则输出2（注意这里不是3）。

# **Input**

输入四个整数：p, e, i和d。 p, e, i分别表示体力、情感和智力高峰出现的时间（时间从当年的第一天开始计算）。d 是给定的时间，可能小于p, e, 或 i。 所有给定时间是非负的并且小于365, 所求的时间小于21252。 

当p = e = i = d = -1时，输入数据结束。

# Output

从给定时间起，下一次三个高峰同天的时间（距离给定时间的天数）。

# Sample Input

**0 0 0 00 0 0 1005 20 34 3254 5 6 7283 102 23 320203 301 203 40-1 -1 -1 -1**

# Sample Output

**Case 1: the next triple peak occurs in 21252 days.Case 2: the next triple peak occurs in 21152 days.Case 3: the next triple peak occurs in 19575 days.Case 4: the next triple peak occurs in 16994 days.Case 5: the next triple peak occurs in 8910 days.Case 6: the next triple peak occurs in 10789 days.**


思路：

根据题意可知，一个周期内有一天为峰值，即任意两个峰值间隔着整数倍的周期。

假设：一年的第 N 天到达峰值，周期是 T

则下一次到达峰值的时间为：![N+T*k](https://private.codecogs.com/gif.latex?N&plus;T*k)

假设三个峰值同时出现的那天为 S

因此三个峰值同时出现的那天满足：![S=N_1+T_1*k_1=N_2+T_2*k_2=N_3+T_3*k_3](https://private.codecogs.com/gif.latex?S%3DN_1&plus;T_1*k_1%3DN_2&plus;T_2*k_2%3DN_3&plus;T_3*k_3)

其中，![N_1,N_2,N_3](https://private.codecogs.com/gif.latex?N_1%2CN_2%2CN_3) 为体力、情感、智力出现峰值的日期，![T_1,T_2,T_3](https://private.codecogs.com/gif.latex?T_1%2CT_2%2CT_3) 为体力、情感、智力的周期，因此仅需求三个非负整数 ![k_1,k_2,k_3](https://private.codecogs.com/gif.latex?k_1%2Ck_2%2Ck_3) 使得等式成立。

故问题可转化为：S%23=P，S%28=E，S%33=I，P、E、I 为体力、情感、智力的出现的时间

由中国剩余定理可得以下同余方程组：

n1%(28*33) = 0, n1%23 = P

n2%(23*33) = 0, n2%28 = E

n3%(23*28) = 0, n3%33 = I

解得 S = (n1+n2+n3) % 21252

# Source Program

```cpp
#include<iostream>
#include<cstdio>
#include<cstring>
#include<cmath>
#include<algorithm>
#include<string>
#include<cstdlib>
#include<queue>
#include<set>
#include<map>
#include<stack>
#include<vector>
#define INF 0x3f3f3f3f
#define PI acos(-1.0)
#define N 10001
#define MOD 123
#define E 1e-6
using namespace std;
int main()
{
    int p, e, i, d;
    int n=0;//计录循环数的

    while(scanf("%d%d%d%d", &p, &e, &i, &d)!=EOF&&p>=0)
    {
        int n1,n2,n3;

        /*求得每个周期内的高峰在第几天*/
        p=p%23+1;
        e=e%28+1;
        i=i%33+1;

        /*计算 n1%(28*33) = 0, n1%23 = P*/
        for(int k=1;;k++)
        {
            int m=(k*28*33)%23;
            if(p%m==0)
            {
                n1=k*28*33*(p/m);
                break;
            }
        }
        
        /*n2%(23*33)=0,n2%28=E*/
        for(int k=1;;k++)
        {
            int m=(k*23*33)%28;
            if(e%m==0)
            {
                n2=k*23*33*(e/m);
                break;
            }
        }
        
        /*n3%(23*28)=0, n3%33=I*/
        for(int k=1;;k++)
        {
            int m=(k*23*28)%33;
            if(i%m==0)
            {
                n3=k*23*28*(i/m);
                break;
            }
        }


        int s=(n1+n2+n3-1)%21252;

        while(s-d<=0)
            s+=21252;

        printf("Case %d: the next triple peak occurs in %d days.\n",++n,s-d);
    }

    return 0;
}
```






