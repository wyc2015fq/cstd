# Vasya And The Mushrooms（CF-1016C） - Alex_McAvoy的博客 - CSDN博客





2018年08月14日 15:46:13[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：75
个人分类：[CodeForces																[动态规划——线性 DP](https://blog.csdn.net/u011815404/article/category/7813170)](https://blog.csdn.net/u011815404/article/category/7923180)








# Problem Description

Vasya's house is situated in a forest, and there is a mushroom glade near it. The glade consists of two rows, each of which can be divided into n consecutive cells. For each cell Vasya knows how fast the mushrooms grow in this cell (more formally, how many grams of mushrooms grow in this cell each minute). Vasya spends exactly one minute to move to some adjacent cell. Vasya cannot leave the glade. Two cells are considered adjacent if they share a common side. When Vasya enters some cell, he instantly collects all the mushrooms growing there.

Vasya begins his journey in the left upper cell. Every minute Vasya must move to some adjacent cell, he cannot wait for the mushrooms to grow. He wants to visit all the cells exactly once and maximize the total weight of the collected mushrooms. Initially, all mushrooms have a weight of 0. Note that Vasya doesn't need to return to the starting cell.

Help Vasya! Calculate the maximum total weight of mushrooms he can collect.

# Input

The first line contains the number n (1 ≤ n ≤ 3·105) — the length of the glade.

The second line contains n numbers a1, a2, ..., an (1 ≤ ai ≤ 106) — the growth rate of mushrooms in the first row of the glade.

The third line contains n numbers b1, b2, ..., bn (1 ≤ bi ≤ 106) is the growth rate of mushrooms in the second row of the glade.

# Output

Output one number — the maximum total weight of mushrooms that Vasya can collect by choosing the optimal route. Pay attention that Vasya must visit every cell of the glade exactly once.

# Examples

**Input**

3

1 2 3

6 5 4

**Output**

70

**Input**

3

1 1000 10000

10 100 100000

**Output**

543210

————————————————————————————————————————————

题意：给一个 2*n 的矩阵，每一个点有一个值，从左上角出发，时间 t 从 0 开始，连续的走，将矩阵走完，每走一步，t++，并且得到 t*当前格子的值 的值，求最走完得到的最大权值和。

思路：[点击这里](https://blog.csdn.net/qq_40160605/article/details/81432788)

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
#define N 1000001
#define MOD 1e9+7
#define E 1e-6
#define LL long long
using namespace std;
LL a[N],b[N];
LL sum[N],suma[N],sumb[N],sumc[N];
int main()
{
    int n;
    cin>>n;
    for(int i=1;i<=n;i++)
        cin>>a[i];
    for(int i=1;i<=n;i++)
        cin>>b[i];

    for(int i=n;i>=1;i--)
        sum[i]=sum[i+1]+a[i]+b[i];
	for(int i=n,j=n;i>=1;i--,j++)
        suma[i]=suma[i+1]+(i-1)*a[i]+j*b[i];
	for(int i=n,j=n+1;i>=2;i--,j++)
        sumb[i]=sumb[i+1]+i*b[i]+j*a[i];

	sumb[1]=sumb[2]+b[1];

	LL ans=0,res=0;
	for(int i=1;i<=n;i++)
    {
		if(i%2)
		{
			res+=ans*a[i];
            ans++;
			res+=ans*b[i];
			ans++;
			sumc[i]=res;
		}
		else
        {
			res+=ans*b[i];
            ans++;
			res+=ans*a[i];
			ans++;
			sumc[i]=res;
		}
	}

	LL maxx=-INF;
    for(int i=0;i<=n;i++)
    {
        if(i%2)
            maxx=max(maxx,sumc[i]+sumb[i+1]+(i-1)*sum[i+1]);
        else
            maxx=max(maxx,sumc[i]+suma[i+1]+i*sum[i+1]);
    }

	cout<<maxx<<endl;

    return 0;
}
```






