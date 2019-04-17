# Telephone Wire（POJ-3612） - Alex_McAvoy的博客 - CSDN博客





2018年06月06日 17:52:53[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：62
个人分类：[POJ																[动态规划——线性 DP](https://blog.csdn.net/u011815404/article/category/7813170)](https://blog.csdn.net/u011815404/article/category/7504500)








# Problem Description

Farmer John's cows are getting restless about their poor telephone service; they want FJ to replace the old telephone wire with new, more efficient wire. The new wiring will utilize N (2 ≤ N ≤ 100,000) already-installed telephone poles, each with some heighti meters (1 ≤ heighti ≤ 100). The new wire will connect the tops of each pair of adjacent poles and will incur a penalty cost C × the two poles' height difference for each section of wire where the poles are of different heights (1 ≤ C ≤ 100). The poles, of course, are in a certain sequence and can not be moved.

Farmer John figures that if he makes some poles taller he can reduce his penalties, though with some other additional cost. He can add an integer X number of meters to a pole at a cost of X2.

Help Farmer John determine the cheapest combination of growing pole heights and connecting wire so that the cows can get their new and improved service.

# **Input**

Line 1: Two space-separated integers: N and C

Lines 2..N+1: Line i+1 contains a single integer: heighti

# Output

Line 1: The minimum total amount of money that it will cost Farmer John to attach the new telephone wire.

# Sample Input

**5 2**

**23514**

# Sample Output

**15**

———————————————————————————————————————————————————————

题意：要改造n个电线杆，相邻两电线杆改造费用=C*两电线杆的高度差，也可对任意电线杆进行加长x，但要花费加长费用x^2，求改造电线杆的最少的费用。

思路

用f[i][j]表示第i棵树高度为j的时候的最小代价，枚举相邻两棵树高度即可，状态方程：f[i][j]=min(f[i][k]+abs(j-k)+(j-a[i])^2)，测试后发现超时，不知道怎么优化，看了他人题解。

对状态方程进行优化，利用分类讨论j<k和j>=k，将k维度用两个数组预处理，即：high[j]=min(f[i-1][k]-k*c) (j>=k)，low[j]=min(f[i-1][k]+k*c) (j<k)，原状态方程就变为：f[i][j]=(j-a[i])^2+min(high[j]+j*c,low[j]-j*c);

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
#include<vector>
#define INF 0x3f3f3f3f
#define PI acos(-1.0)
#define N 105
#define MOD 100001
#define E 1e-12
using namespace std;
int low[N],high[N],f[N];
int main()
{
    int n,m;
    while(scanf("%d%d",&n,&m)!=EOF)
    {
        int a;
        scanf("%d",&a);
        for(int i=1;i<=100;i++)
        {
            if(i<a)
                f[i]=INF;
            else
                f[i]=(a-i)*(a-i);
        }

        for(int i=1;i<n;i++)
        {
            int temp=INF;
            scanf("%d",&a);
            for(int j=100;j>0;j--)//对low预处理
            {
                temp=min(temp,f[j]+j*m);
                low[j]=temp;
            }
            for(int j=1;j<=100;j++)//对high预处理
            {
                temp=min(temp,f[j]-j*m);
                high[j]=temp;
                f[j]=INF;
            }
            for(int j=a;j<=100;j++)
                f[j]=(j-a)*(j-a)+min(low[j]-j*m,high[j]+j*m);
        }

        int ans=INF;
        for(int i=1;i<=100;i++)
            ans=min(ans,f[i]);
        printf("%d\n",ans);
    }
}
```






