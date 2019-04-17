# Out of Hay（POJ-2395） - Alex_McAvoy的博客 - CSDN博客





2018年07月27日 23:24:30[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：35








> 
# Problem Description

The cows have run out of hay, a horrible event that must be remedied immediately. Bessie intends to visit the other farms to survey their hay situation. There are N (2 <= N <= 2,000) farms (numbered 1..N); Bessie starts at Farm 1. She'll traverse some or all of the M (1 <= M <= 10,000) two-way roads whose length does not exceed 1,000,000,000 that connect the farms. Some farms may be multiply connected with different length roads. All farms are connected one way or another to Farm 1. 

Bessie is trying to decide how large a waterskin she will need. She knows that she needs one ounce of water for each unit of length of a road. Since she can get more water at each farm, she's only concerned about the length of the longest road. Of course, she plans her route between farms such that she minimizes the amount of water she must carry. 

Help Bessie know the largest amount of water she will ever have to carry: what is the length of longest road she'll have to travel between any two farms, presuming she chooses routes that minimize that number? This means, of course, that she might backtrack over a road in order to minimize the length of the longest road she'll have to traverse.

# **Input**

* Line 1: Two space-separated integers, N and M. 

* Lines 2..1+M: Line i+1 contains three space-separated integers, A_i, B_i, and L_i, describing a road from A_i to B_i of length L_i.

# Output

* Line 1: A single integer that is the length of the longest road required to be traversed.

# Sample Input

**3 31 2 232 3 10001 3 43**

# Sample Output

**43**


题意：给出 n 个点，m 条路，ai、bi、li 表示从点 ai 到点 bi 的路程为 li，要从1号点开始选择最短路径访问所有点，求会经过的最大边。

思路：求最小生成树中的权值最大的边，Kruscal 算法即可。

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
int father[N];
struct Node{
	int a;
	int b;
	int l;
}edge[N];
int Find(int x)
{
	if(father[x]==x)
        return x;
    else
        return father[x]=Find(father[x]);
}
int cmp(Node x,Node y)
{
	return x.l<y.l;
}
int main()
{
    int n,m;
	scanf("%d%d",&n,&m);

	for(int i=1;i<=m;i++)
        scanf("%d%d%d",&edge[i].a,&edge[i].b,&edge[i].l);

	sort(edge+1,edge+m+1,cmp);
	for(int i=1;i<=n;i++)
        father[i]=i;

	int maxx=-INF;
	for(int i=1;i<=m;i++)
	{
		int x=Find(edge[i].a);
		int y=Find(edge[i].b);
		if(x!=y)
		{
			if(x>y)
                father[x]=y;
		    else
                father[y]=x;
			maxx=max(maxx,edge[i].l);
		}
	}

	printf("%d\n",maxx);
	return 0;
}
```





