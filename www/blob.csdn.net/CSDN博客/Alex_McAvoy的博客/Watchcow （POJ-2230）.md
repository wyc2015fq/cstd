# Watchcow （POJ-2230） - Alex_McAvoy的博客 - CSDN博客





2018年07月28日 21:55:23[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：74








> 
# Problem Description

Bessie's been appointed the new watch-cow for the farm. Every night, it's her job to walk across the farm and make sure that no evildoers are doing any evil. She begins at the barn, makes her patrol, and then returns to the barn when she's done. 

If she were a more observant cow, she might be able to just walk each of M (1 <= M <= 50,000) bidirectional trails numbered 1..M between N (2 <= N <= 10,000) fields numbered 1..N on the farm once and be confident that she's seen everything she needs to see. But since she isn't, she wants to make sure she walks down each trail exactly twice. It's also important that her two trips along each trail be in opposite directions, so that she doesn't miss the same thing twice. 

A pair of fields might be connected by more than one trail. Find a path that Bessie can follow which will meet her requirements. Such a path is guaranteed to exist.

# **Input**

* Line 1: Two integers, N and M. 

* Lines 2..M+1: Two integers denoting a pair of fields connected by a path.

# Output

* Lines 1..2M+1: A list of fields she passes through, one per line, beginning and ending with the barn at field 1. If more than one solution is possible, output any solution.

# Sample Input

**4 51 21 42 32 43 4**

# Sample Output

**12342143241**


题意：给出 n 个点，m 条路，从第一个农场走到最后一个农场，要求走的路要走两遍，且两遍必须方向不同，输出经过的点。

思路：欧拉回路裸题，每条边正反存两条，记录点的序号，再用 dfs 搜索，即可得到点的顺序。

要注意的是，由于数据量较大，因此要选用链表存储

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
#define N 100001
#define MOD 123
#define E 1e-6
using namespace std;
int n,m;
int cnt,k;
int head[N],path[N];
int vis[N];
struct Node{
    int to;
    int next;
}edge[N];
void add(int x,int y)
{
    edge[cnt].to=y;
    edge[cnt].next=head[x];
    head[x]=cnt;
    cnt++;
}
void dfs(int x)
{
    for(int y=head[x];y!=-1;y=edge[y].next)
    {
        int next=edge[y].to;
        if(!vis[y])
        {
            vis[y]=1;
            dfs(next);
        }
    }
    path[k]=x;
    k++;
}
int main()
{
	scanf("%d%d",&n,&m);

	memset(vis,0,sizeof(vis));
	memset(head,-1,sizeof(head));

	for(int i=1;i<=m;i++)
    {
        int x,y;
        scanf("%d%d",&x,&y);
        add(x,y);
        add(y,x);
    }

    dfs(1);

    for(int i=0;i<k;i++)
        printf("%d\n",path[i]);
        
	return 0;
}
```





