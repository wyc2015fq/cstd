# Allowance（POJ-3040） - Alex_McAvoy的博客 - CSDN博客





2018年07月26日 00:44:24[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：113








# Problem Description

As a reward for record milk production, Farmer John has decided to start paying Bessie the cow a small weekly allowance. FJ has a set of coins in N (1 <= N <= 20) different denominations, where each denomination of coin evenly divides the next-larger denomination (e.g., 1 cent coins, 5 cent coins, 10 cent coins, and 50 cent coins).Using the given set of coins, he would like to pay Bessie at least some given amount of money C (1 <= C <= 100,000,000) every week.Please help him ompute the maximum number of weeks he can pay Bessie.

# **Input**

* Line 1: Two space-separated integers: N and C 

* Lines 2..N+1: Each line corresponds to a denomination of coin and contains two integers: the value V (1 <= V <= 100,000,000) of the denomination, and the number of coins B (1 <= B <= 1,000,000) of this denomation in Farmer John's possession.

# Output

* Line 1: A single integer that is the number of weeks Farmer John can pay Bessie at least C allowance

# Sample Input

**3 610 11 1005 120**

# Sample Output

**111**

————————————————————————————————————————————————————

题意：每周至少给牛 c 元，有 n 种硬币，已知每种硬币币值和数量，求最多坚持的周数。

思路：将硬币从小到大排序，大于等于 c 元的硬币直接支付，然后将不能直接支付的先从大到小凑到接近 c ，再从小到大向上加，如果能达到就记录并开始下一轮，如果不能就退出。要注意的是，硬币无法拆分，因此只能多给不能少给。

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
#define N 1001
#define MOD 123
#define E 1e-6
using namespace std;
struct Node{
    int num;
    int value;
}coin[N];
int use[N];
int cmp(Node a,Node b)
{
    return a.value<b.value;
}
int main()
{
    int n,c;
    int cnt=0;
    scanf("%d%d",&n,&c);

    for(int i=1;i<=n;i++)
        scanf("%d %d",&coin[i].value,&coin[i].num);

    sort(coin+1,coin+1+n,cmp);

    for(int i=1;i<=n;i++)
    {
        if(coin[i].value>=c)//如果硬币币值大于 c 元
        {
            cnt+=coin[i].num;//直接支付，记录次数
            coin[i].num=0;//清零
        }
    }

    while(true)
    {
        bool flag=false;
        int sum=c;
        memset(use,0,sizeof(use));

        for(int i=n;i>0;i--)//对币值从大到小凑到接近c
        {
            if(coin[i].num>0)
            {
                use[i]=min(sum/coin[i].value,coin[i].num);//记录使用的次数
                sum-=use[i]*coin[i].value;//记录凑到的值
                if(sum==0)
                {
                    flag=true;
                    break;
                }
            }
        }

        if(sum>0)//对币值从小到大凑到c
        {
            for(int i=1;i<=n;i++)
            {
                if(coin[i].num-use[i]>0)//如果该种硬币未用光
                {
                    while(use[i]<coin[i].num)
                    {
                        sum-=coin[i].value;//记录凑到的值
                        use[i]++;//记录使用的次数
                        if(sum<=0)
                        {
                            flag=true;
                            break;
                        }
                    }
                }
                if(flag)
                    break;
            }
        }

        if(!flag)
            break;

        int minn=INF;
        for(int i=1;i<=n;i++)//记录最小的支付数
            if(use[i]>0)
                minn=min(minn,coin[i].num/use[i]);

        for(int i=1;i<=n;i++)//从硬币总数量中减去支付数
            if(use[i]>0)
                coin[i].num-=minn*use[i];
        cnt+=minn;
    }

    printf("%d\n",cnt);

    return 0;
}
```





