# Buns（CF-106C） - Alex_McAvoy的博客 - CSDN博客





2019年01月14日 22:22:43[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：42
个人分类：[CodeForces																[动态规划——背包问题](https://blog.csdn.net/u011815404/article/category/7813163)](https://blog.csdn.net/u011815404/article/category/7923180)








# Problem Description

Lavrenty, a baker, is going to make several buns with stuffings and sell them.

Lavrenty has n grams of dough as well as m different stuffing types. The stuffing types are numerated from 1 to m. Lavrenty knows that he has ai grams left of the i-th stuffing. It takes exactly bi grams of stuffing i and ci grams of dough to cook a bun with the i-th stuffing. Such bun can be sold for di tugriks.

Also he can make buns without stuffings. Each of such buns requires c0 grams of dough and it can be sold for d0 tugriks. So Lavrenty can cook any number of buns with different stuffings or without it unless he runs out of dough and the stuffings. Lavrenty throws away all excess material left after baking.

Find the maximum number of tugriks Lavrenty can earn.

# Input

The first line contains 4 integers n, m, c0 and d0 (1 ≤ n ≤ 1000, 1 ≤ m ≤ 10, 1 ≤ c0, d0 ≤ 100). Each of the following m lines contains 4 integers. The i-th line contains numbers ai, bi, ci and di (1 ≤ ai, bi, ci, di ≤ 100).

# Output

Print the only number — the maximum number of tugriks Lavrenty can earn.

# Examples

**Input**

10 2 2 1

7 3 2 100

12 3 1 10

**Output**

241 

**Input**

100 1 25 50

15 5 20 10

**Output**

200

————————————————————————————————————————————

题意：给出 n 克的面和 m 种馅，如果只用面做面包，花费 c0 克的面，做一个价值为 d0 的馒头。给出的 m 种馅每种都有 a、b、c、d 种属性，a 表示馅有多少克，b 为做一个含有该馅的面包所需的馅的量，c 是做含有该馅的面包所需的面的量，d 为做出的面包具有的价值，问最多能卖多少钱

思路：多重背包模板题，找准容量、价值即可

# Source Program

```cpp
#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<string>
#include<cstring>
#include<cmath>
#include<ctime>
#include<algorithm>
#include<utility>
#include<stack>
#include<queue>
#include<vector>
#include<set>
#include<map>
#define PI acos(-1.0)
#define E 1e-6
#define MOD 16007
#define INF 0x3f3f3f3f
#define N 10001
#define LL long long
using namespace std;
int dp[N];
int main()
{
    int n,m,c0,d0;
    while(scanf("%d%d%d%d",&n,&m,&c0,&d0)!=EOF){
        memset(dp,0,sizeof(dp));
        for(int i=c0;i<=n;i++)//初始价值，即i克面团不使用馅料所能够卖价格
            dp[i]=i/c0*d0;

        for(int i=0;i<m;i++){//对每一种馅
            int a,b,c,d;
            scanf("%d%d%d%d",&a,&b,&c,&d);
            for(int j=1;j<=a/b;j++)//使用i馅最多生成a/b个面包
                for(int k=n;k>=c;k--)//消耗的面的数量
                    dp[k]=max(dp[k-c]+d,dp[k]);
        }
        printf("%d\n",dp[n]);
    }
    return 0;
}
```






