# Dima and Salad（CF-366C） - Alex_McAvoy的博客 - CSDN博客





2019年01月16日 20:23:59[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：27
个人分类：[CodeForces																[动态规划——背包问题](https://blog.csdn.net/u011815404/article/category/7813163)](https://blog.csdn.net/u011815404/article/category/7923180)








# Problem Description

Dima, Inna and Seryozha have gathered in a room. That's right, someone's got to go. To cheer Seryozha up and inspire him to have a walk, Inna decided to cook something.

Dima and Seryozha have *n* fruits in the fridge. Each fruit has two parameters: the taste and the number of calories. Inna decided to make a fruit salad, so she wants to take some fruits from the fridge for it. Inna follows a certain principle as she chooses the fruits: the total taste to the total calories ratio of the chosen fruits must equal *k*. In other words, ![](https://img-blog.csdnimg.cn/20190116195514779.jpg) , where *a**j* is the taste of the *j*-th chosen fruit and *b**j* is its calories.

Inna hasn't chosen the fruits yet, she is thinking: what is the maximum taste of the chosen fruits if she strictly follows her principle? Help Inna solve this culinary problem — now the happiness of a young couple is in your hands!

Inna loves Dima very much so she wants to make the salad from at least one fruit.

# Input

The first line of the input contains two integers n, k (1 ≤ n ≤ 100, 1 ≤ k ≤ 10). The second line of the input contains n integers a1, a2, ..., an (1 ≤ ai ≤ 100) — the fruits' tastes. The third line of the input contains n integers b1, b2, ..., bn (1 ≤ bi ≤ 100) — the fruits' calories. Fruit number i has taste ai and calories bi.

# Output

If there is no way Inna can choose the fruits for the salad, print in the single line number -1. Otherwise, print a single integer — the maximum possible sum of the taste values of the chosen fruits.

# Examples

**Input**

3 2

10 8 1

2 7 1

**Output**

18

**Input**

5 3

4 4 4 4 4

2 2 2 2 2

**Output**

-1

————————————————————————————————————————————

题意：给出 n 个物品与一个 k 值，每个物品有 ai、bi 两个值，要求保证 ![](https://img-blog.csdnimg.cn/20190116195514779.jpg) 的前提下，使得 ai 的和最大，最后输出这个和

思路： 01 背包的转换问题

实质是用容量为 0 的背包去装 n 个物品，物品重量是 a[i]-k*b[i]，价值是 a[i]，要求装满背包且总价值最大

用 dp[i][j] 表示前 i 个物品总重量为 j 的最大价值，这样 dp[n][0] 就是答案，根据题意，有 dp[i][j]=max( dp[i-1][j],dp[i-1][j-a[i]+k*b[i]] + a[i])

由于 j 可能为负数，因此需要对背包的整体重量加上一个值，使得不会出现负的情况

根据数据范围：1 ≤ n ≤ 100，1 ≤ k ≤ 10，1 ≤ ai ≤ 100，1 ≤ bi ≤ 100，极端情况下，100*(1-10*100)=100 000，因此可以将背包的整体数量加上 100 000 这个值，这样就不会出现负的情况

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
#define N 200001
#define LL long long
using namespace std;
int n,k;
int a[N],b[N];
int dp[101][200001];
int main()
{
    while(scanf("%d%d",&n,&k)!=EOF){
        for(int i=1;i<=n;i++)
            scanf("%d",&a[i]);
        for(int i=1;i<=n;i++)
            scanf("%d",&b[i]);

        memset(dp,-INF,sizeof(dp));
        dp[0][100000]=0;//整体扩容100000

        int maxV=200000;//扩容后的最大值
        for(int i=1;i<=n;i++){
            int w=a[i]-k*b[i];
            for(int j=maxV;j>=0;j--){
                if(j-w>=0&&j-w<=maxV){
                    dp[i][j]=max(dp[i-1][j],dp[i-1][j-w]+a[i]);
                }
            }
        }

        if(dp[n][100000]==0)
            dp[n][100000]=-1;
        printf("%d\n",dp[n][100000]);
    }
    return 0;
}
```






