# Find the most comfortable road（HDU-1598） - Alex_McAvoy的博客 - CSDN博客





2018年05月22日 13:54:16[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：46
个人分类：[图论——最小生成树																[HDU](https://blog.csdn.net/u011815404/article/category/7923257)](https://blog.csdn.net/u011815404/article/category/7813283)








> 
# Problem Description

XX星有许多城市，城市之间通过一种奇怪的高速公路SARS(Super Air Roam Structure---超级空中漫游结构）进行交流，每条SARS都对行驶在上面的Flycar限制了固定的Speed，同时XX星人对 Flycar的“舒适度”有特殊要求，即乘坐过程中最高速度与最低速度的差越小乘坐越舒服 ,(理解为SARS的限速要求，flycar必须瞬间提速/降速，痛苦呀 ),但XX星人对时间却没那么多要求。

要你找出一条城市间的最舒适的路径。(SARS是双向的）。

# Input

输入包括多个测试实例，每个实例包括：

第一行有2个正整数n (1<n<=200)和m (m<=1000),表示有N个城市和M条SARS。

接下来的行是三个正整数StartCity,EndCity,speed,表示从表面上看StartCity到EndCity,限速为speedSARS。speed<=1000000

然后是一个正整数Q（Q<11),表示寻路的个数。

接下来Q行每行有2个正整数Start,End, 表示寻路的起终点。

# Output

每个寻路要求打印一行，仅输出一个非负整数表示最佳路线的舒适度最高速与最低速的差。如果起点和终点不能到达，那么输出-1。

# Sample Input

**4 41 2 22 3 41 4 13 4 221 31 2**

# Sample Output

**1**

**0**


思路：并查集+贪心+kruskal，将路径升序排序， 然后依次枚举最小速度， 再寻找目标起点到目标终点的路径中的最大速度，为了使差更小，就要使最大速度尽量的小，按照kruskal算法从最小路径开始构造生成树，直到所连出的树含有起点和终点为止。

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
#define INF 999999
#define N 201
#define MOD 1000000007
#define E 1e-12
using namespace std;
struct Node
{
    int x;
    int y;
    int w;
}a[5*N];
int n,m;
int father[N];
int Find(int x)
{
    int r=x;
    while(r!=father[r])
        r=father[r];

    int i=x,j;
    while(i!=r)
    {
        j=father[i];
        father[i]=r;
        i=j;
    }
    return r;
}
bool cmp(Node a,Node b)
{
    return a.w<b.w;
}
int main()
{
    while(scanf("%d%d",&n,&m)!=EOF)
    {
        for(int i=0;i<m;i++)
            scanf("%d%d%d",&a[i].x,&a[i].y,&a[i].w);
        sort(a,a+m,cmp);

        int t;
        scanf("%d",&t);
        for(int i=0;i<t;i++)
        {
            int s,e;
            scanf("%d%d",&s,&e);
            int MST=INF;
            for(int j=0;j<m;j++)
            {
                for(int i=1;i<=n;i++)
                    father[i]=i;

                for(int k=j;k<m;k++)
                {
                    int x=Find(a[k].x);
                    int y=Find(a[k].y);
                    if(x!=y)
                        father[x]=y;

                    if(Find(s)==Find(e))
                    {
                        MST=min(MST,a[k].w-a[j].w);
                        break;
                    }
                }
            }
            if(MST!=INF)
                printf("%d\n",MST);
            else
                printf("-1\n");
        }
    }
    return 0;
}
```






