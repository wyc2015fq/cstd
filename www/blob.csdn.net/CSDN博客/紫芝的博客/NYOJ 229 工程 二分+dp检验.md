# NYOJ 229 工程 二分+dp检验 - 紫芝的博客 - CSDN博客





2018年09月02日 16:03:45[紫芝](https://me.csdn.net/qq_40507857)阅读数：83








# [工程](http://acm.nyist.edu.cn/JudgeOnline/problem.php?pid=229)

时间限制：1000 ms  |  内存限制：65535 KB

描述

有n个工人做两个工程A和B，每个工程都被分为相同的m份，给你第i个工人做A中的一份需要的时间Xi秒，和做B中的一份所需时间Yi秒，问最短需要多少时间可以完成这两项工程。

输入

第一行是一个整数t (1 <= t <= 100),表示有t组测试数据;

每组测试数据第一行有两个整数 n (1 <= n <= 100), m (1 <= m <= 100).

接下来的n行,每行有两个整数Xi,Yi;

输出

输出最短时间，占一行。

样例输入

```
1
3 20
1 1
2 4
1 6
```

样例输出
`18`
分析：

首先，二分的下界是0，上界是完成A，B两个工程所需的最长时间，然后二分答案。

其次，对于一个答案mid是否符合条件（完成两个工程），任选一个人，初始化dp数组，他完成i个工作A,之后还可以完成dp[i]个工作B，然后判断剩余的n-1个人是否可以完成其余的工作

```cpp
#include<iostream>
#include<cstdio>
#include<cstring>
using namespace std;
 
const int MAXN = 110;
 
int Ap[MAXN];
int Bp[MAXN];
int dp[MAXN];//dp[i]表示完成i个工作A，之后还能完成多少个工作B
int n,m;
 
int MAX(int a, int b)
{
    return a > b ? a : b;
}
 
int slove( int mid )
{
    int i,j,k;
    memset(dp, -1, sizeof(dp));
 
    for(i = 0; i <= m; ++i)//第一个人完成i个A，还可以完成多少个B
        if(mid >= i*Ap[1])
            dp[i] = (mid-i*Ap[1])/Bp[1];
        else
            break;
 
    if(dp[m] >= m)
//如果第一个人完成A之后，还有足够的时间来完成B，那么就说明这个mid时间是可以的
        return 1;
    //如果剩下的时间不足以完成B，或者给定mid的时间完不成A， 剩下的就是背包思想
    for(i = 2; i <= n; ++i)
    {
//既然第一个人不能在给定的mid时间内完成两个任务，那么接下来在让其他人上,看前i是否可以完成任务
        for(j = m; j >= 0; --j)
        {
            for(k = 0; k <= j && k*Ap[i] <= mid; ++k)
            {//如果第i个人可以完成k份工作A，并且前i-1个人已完成了j-k个工作A，
//(即是否可以留更多的时间来做更多的工作)那么前i个工人还可以完成多少份工作？
                if(dp[j-k] != -1)
                    dp[j] = MAX(dp[j], dp[j-k]+(mid-k*Ap[i])/Bp[i]);
            }
        }
//如果前i(包括i)个工人完成A之后还能完成B，那么返回1，说明mid满足
        if(dp[m] >= m)
            return 1;
    }
    return 0;
}
 
int main()
{
    int i,T;
    scanf("%d", &T);
    while(T--)
    {
        int maxtime = 0;
        scanf("%d%d",&n,&m);
        for(i = 1; i <= n; ++i)
        {
            scanf("%d%d",&Ap[i],&Bp[i]);
            maxtime = MAX(maxtime, MAX(Ap[i], Bp[i]));
        }
 
        int L = 0,R = (maxtime*m) << 1;
        while(L < R)
        {
            int mid = (L + R)>>1;
 
            if(slove(mid))
//如果给定的mid时间一直都满足，那么继续收缩区间，直到找到一个最小的时间
                R = mid;
            else
                L = mid + 1;
        }
        printf("%d\n",L);
    }
    return 0;
}
```





