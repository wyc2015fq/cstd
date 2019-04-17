# 石子合并问题--直线版（Hrbust-1818） - Alex_McAvoy的博客 - CSDN博客





2018年09月01日 11:39:25[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：91








# Problem Description

一条直线上摆放着一行共n堆的石子。现要将石子有序地合并成一堆。规定每次只能选相邻的两堆合并成新的一堆，并将新的一堆石子数记为该次合并的得分。

请编辑计算出将n堆石子合并成一堆的最小得分和将n堆石子合并成一堆的最大得分。

# **Input**

输入有多组测试数据。

每组第一行为n(n<=100)，表示有n堆石子，。

二行为n个用空格隔开的整数，依次表示这n堆石子的石子数量ai（0<ai<=100）

# Output

每组测试数据输出有一行。输出将n堆石子合并成一堆的最小得分和将n堆石子合并成一堆的最大得分。 中间用空格分开。

# Sample Input

**3**

**1 2 3**

# Sample Output

**9 11**

————————————————————————————————————————————————————

思路：[点击这里](https://blog.csdn.net/u011815404/article/details/81773570)

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
#include<ctime>
#include<vector>
#define INF 0x3f3f3f3f
#define PI acos(-1.0)
#define N 1001
#define MOD 10007
#define E 1e-6
#define LL long long
using namespace std;
int a[N];
int sum[N];
int dp[N][N];
int main()
{
    int n;
    while(scanf("%d",&n)!=EOF)
    {
        for(int i=1;i<=n;i++)
        {
            scanf("%d",&a[i]);
            sum[i]=sum[i-1]+a[i];
        }


        memset(dp,INF,sizeof(dp));
        for(int i=1;i<=n;i++)
            dp[i][i]=0;
        for(int len=1;len<n;len++)
        {
            for(int i=1;i<=n-len;i++)
            {
                int j=i+len;
                for(int k=i;k<j;k++)
                    dp[i][j]=min(dp[i][j],dp[i][k]+dp[k+1][j]);
                dp[i][j]+=sum[j]-sum[i-1];
            }
        }
        printf("%d ",dp[1][n]);


        memset(dp,0,sizeof(dp));
        for(int i=1;i<=n;i++)
            dp[i][i]=0;
        for(int len=1;len<n;len++)
        {
            for(int i=1;i<=n-len;i++)
            {
                int j=i+len;
                for(int k=i;k<j;k++)
                    dp[i][j]=max(dp[i][j],dp[i][k]+dp[k+1][j]);
                dp[i][j]+=sum[j]-sum[i-1];
            }
        }
        printf("%d\n",dp[1][n]);
    }

    return 0;
}
```





