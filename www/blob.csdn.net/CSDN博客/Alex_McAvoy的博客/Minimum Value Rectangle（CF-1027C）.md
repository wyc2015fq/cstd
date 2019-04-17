# Minimum Value Rectangle（CF-1027C） - Alex_McAvoy的博客 - CSDN博客





2018年08月24日 13:50:43[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：58
个人分类：[CodeForces																[基础算法——贪心](https://blog.csdn.net/u011815404/article/category/7628972)](https://blog.csdn.net/u011815404/article/category/7923180)








# Problem Description

You have nn sticks of the given lengths.

Your task is to choose exactly four of them in such a way that they can form a rectangle. No sticks can be cut to pieces, each side of the rectangle must be formed by a single stick. No stick can be chosen multiple times. It is guaranteed that it is always possible to choose such sticks.

Let S be the area of the rectangle and P be the perimeter of the rectangle.

The chosen rectangle should have the value P^2/S minimal possible. The value is taken without any rounding.

If there are multiple answers, print any of them.

Each testcase contains several lists of sticks, for each of them you are required to solve the problem separately.

# Input

The first line contains a single integer T (T≥1) — the number of lists of sticks in the testcase.

Then 2T lines follow — lines (2i−1) and 2i of them describe the ii-th list. The first line of the pair contains a single integer nn (4≤n≤106) — the number of sticks in the ii-th list. The second line of the pair contains nn integers a1,a2,…,an (1≤aj≤104) — lengths of the sticks in the i-th list.

It is guaranteed that for each list there exists a way to choose four sticks so that they form a rectangle.

The total number of sticks in all T lists doesn't exceed 106 in each testcase.

# Output

Print T lines. The i-th line should contain the answer to the i-th list of the input. That is the lengths of the four sticks you choose from the i-th list, so that they form a rectangle and the value P^2/S of this rectangle is minimal possible. You can print these four lengths in arbitrary order.

If there are multiple answers, print any of them.

# Examples

**Input**

3

4

7 2 2 7

8

2 8 1 4 8 2 1 5

5

5 5 5 5 5

**Output**

2 7 7 2

2 2 1 1

5 5 5 5

————————————————————————————————————————————

题意：给 n 个火柴棍的长度，建立一个矩形，且矩形的面积 S 和周长 P 要满足 P^2/S 尽可能的小，输出满足条件的火柴棍 

思路：

设长宽分别为 a、b，则 P^2/S = 4(a/b+b/a)+8，因此最小化 b/a 即可 

如果有4条相等的线段，直接选取，否则将数量不小于2的线段从小到大排序，贪心地选择相邻的线段，将当前的 a、b 与已知的 a、b 不断比较得出最优解

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
#define N 100001
#define MOD 1e9+7
#define E 1e-6
#define LL long long
using namespace std;
const int MAXN=1e4+5;
int a[MAXN];
int vis[MAXN];
int main()
{
    int t;
    scanf("%d",&t);
    while(t--)
    {
        memset(a,0,sizeof(a));
        memset(vis,0,sizeof(vis));

        int n;
        scanf("%d",&n);

        int k=0;
        bool flag=true;
        for(int i=0;i<n;i++)
        {
            int d;
            scanf("%d",&d);
            vis[d]++;

            if(vis[d]==4&&flag)
            {
                printf("%d %d %d %d\n",d,d,d,d);
                flag=false;
            }

            if(vis[d]==2&&flag)
                a[k++]=d;
        }

        if(flag)
        {
            sort(a,a+k);
            double minn=INF;
            int ans1,ans2;
            for(int i=0;i<k-1;i++)
            {
                double temp=(double)a[i]/a[i+1]+(double)a[i+1]/a[i];
                if(temp<minn)
                {
                    minn=temp;
                    ans1=a[i];
                    ans2=a[i+1];
                }
            }
            printf("%d %d %d %d\n",ans1,ans1,ans2,ans2);
        }
    }
    return 0;
}
```






