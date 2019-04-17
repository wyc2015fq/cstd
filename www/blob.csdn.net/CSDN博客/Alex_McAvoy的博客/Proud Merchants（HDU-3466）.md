# Proud Merchants（HDU-3466） - Alex_McAvoy的博客 - CSDN博客





2018年03月31日 21:41:58[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：56
个人分类：[动态规划——背包问题																[HDU](https://blog.csdn.net/u011815404/article/category/7923257)](https://blog.csdn.net/u011815404/article/category/7813163)








# Problem Description

    Recently, iSea went to an ancient country. For such a long time, it was the most wealthy and powerful kingdom in the world. As a result, the people in this country are still very proud even if their nation hasn’t been so wealthy any more.

    The merchants were the most typical, each of them only sold exactly one item, the price was Pi, but they would refuse to make a trade with you if your money were less than Qi, and iSea evaluated every item a value Vi.

    If he had M units of money, what’s the maximum value iSea could get?

# Input

    There are several test cases in the input.

    Each test case begin with two integers N, M (1 ≤ N ≤ 500, 1 ≤ M ≤ 5000), indicating the items’ number and the initial money.

    Then N lines follow, each line contains three numbers Pi, Qi and Vi (1 ≤ Pi ≤ Qi ≤ 100, 1 ≤ Vi ≤ 1000), their meaning is in the description.

    The input terminates by end of file marker.

# Output

    For each test case, output one integer, indicating maximum value iSea could get.<

# Sample Input

**2 1010 15 105 10 53 105 10 53 5 62 7 3**

# Sample Output

**511**

————————————————————————————————————————————

思路： 

这道题和01背包类似，唯一不同的就是买每件物品时多了一个限制条件，p，q，v，需要钱数大于q才能买，所以买东西需要有先后顺序。

以前的01背包不用排序，dp[j] = max(dp[j],dp[j-w[i]]+v[i])表达的是这第i个物品要不要买，但同时也在判断第i个物品要不要先买，如果先买剩下的空间随便放，看是否得到的价值会更大。

A物品  p1,q1,v1;

B物品  p2,q2,v2

若先买A，至少需要p1+q2 (p1 == q1情况下）若现买B，至少需要p2+q1

若A<B，应先买A，即：q1-p1 > q2-p2;即差值大的先买，所以先买的应排到后面

# Source Program

```cpp
#include<iostream>
#include<cstring>
#include<algorithm>
using namespace std;

struct node
{
    int p;
    int q;
    int v;
}a[500];
int dp[5001];

bool cmp(node a,node b)
{
    return a.q-a.p<b.q-b.p;
}

int main()
{
    int n,m;
    int i,j;

    while(cin>>n>>m)
    {

        memset(dp,0,sizeof(dp));
        for(i=1;i<=n;i++)
            cin>>a[i].p>>a[i].q>>a[i].v;

        sort(a+1,a+n,cmp);

        for(i=1;i<=n;i++)
            for(j=m;j>=a[i].q;j--)
                dp[j]=max(dp[j],dp[j-a[i].p]+a[i].v);

        cout<<dp[m]<<endl;
    }
    return 0;
}
```






