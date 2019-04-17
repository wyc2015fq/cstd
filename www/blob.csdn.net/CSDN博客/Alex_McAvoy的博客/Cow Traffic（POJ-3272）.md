# Cow Traffic（POJ-3272） - Alex_McAvoy的博客 - CSDN博客





2018年06月15日 23:07:09[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：49
个人分类：[POJ																[图论——AOV网与拓扑排序](https://blog.csdn.net/u011815404/article/category/7813285)](https://blog.csdn.net/u011815404/article/category/7504500)








> 
# Problem Description

The bovine population boom down on the farm has caused serious congestion on the cow trails leading to the barn. Farmer John has decided to conduct a study to find the bottlenecks in order to relieve the 'traffic jams' at milking time.

The pasture contains a network of M (1 ≤M ≤ 50,000) one-way trails, each of which connects exactly two different intersections from the set ofN (1 ≤ N ≤ 5,000) intersections conveniently numbered 1..N; the barn is at intersection numberN. Each trail connects one intersection point to another intersection point with a higher number. As a result, there are no cycles and, as they say on the farm, all trails lead to the barn. A pair of intersection points might be connected by more than one trail.

During milking time rush hour, the cows start from their respective grazing locations and head to the barn. The grazing locations are exactly those intersection points with no trails connecting into them. Each cow traverses a 'path', which is defined as a sequence of trails from a grazing location to the barn.

Help FJ finding the busiest trail(s) by computing the largest number of possible paths that contain any one trail. The answer is guaranteed to fit in a signed 32-bit integer.

# **Input**

Line 1: Two space-separated integers:N and M. 

Lines 2..M+1: Two space-separated intersection points.

# Output

Line 1: The maximum number of paths passing through any one trail.

# Sample Input

**7 7**

**1 33 43 54 62 35 66 7**

# Sample Output

**4**


题意：给出一个无环图，求从每个入度为零的点到个出度为零的点的所有走法里，经过次数最多的一条边被经过的次数

思路：出度为零的点是唯一的，进行正反两次拓扑排序，对任一结点，标记出他作为弧尾时的入度，每条弧正向的入度与他反向的入度相乘，就是这条边被经过的次数。

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
#define N 50001
#define MOD 123
#define E 1e-6
using namespace std;
struct Node{
    int next[N];//后继
    int pre[N];//前驱
    int v[N];
    int d[N];//当前入度
    int w[N];//入度总数量
}p1,p2;
int main()
{
    int n,m;
    while(scanf("%d%d",&n,&m)!=EOF)
    {
        queue<int>q1;
        queue<int>q2;

        memset(p1.next,-1,sizeof(p1.next));
        memset(p1.pre,-1,sizeof(p1.pre));
        memset(p1.v,-1,sizeof(p1.v));
        memset(p1.d,0,sizeof(p1.d));
        memset(p1.w,0,sizeof(p1.w));

        memset(p2.next,-1,sizeof(p2.next));
        memset(p2.pre,-1,sizeof(p2.pre));
        memset(p2.v,-1,sizeof(p2.v));
        memset(p2.d,0,sizeof(p2.d));
        memset(p2.w,0,sizeof(p2.w));

        for(int i=1;i<=m;i++)
        {
            int a,b;
            scanf("%d%d",&a,&b);

            /*正向*/
            p1.next[i]=p1.v[a];
            p1.pre[i]=b;
            p1.v[a]=i;
            p1.d[b]++;

            /*反向*/
            p2.next[i]=p2.v[b];
            p2.pre[i]=a;
            p2.v[b]=i;
            p2.d[a]++;
        }

        for(int i=1;i<=n;i++)
        {
            if(p1.d[i]==0)//寻找正向始点
            {
                p1.w[i]=1;
                q1.push(i);
            }
            if(p2.d[i]==0)//寻找反向始点
            {
                p2.w[i]=1;
                q2.push(i);
            }
        }

        while(!q1.empty())//正向拓扑排序
        {
            int x=q1.front();
            q1.pop();
            for(int i=p1.v[x];i!=-1;i=p1.next[i])
            {
                p1.w[p1.pre[i]]+=p1.w[x];
                p1.d[p1.pre[i]]--;
                if(p1.d[p1.pre[i]]==0)
                    q1.push(p1.pre[i]);
            }
        }
        while(!q2.empty())//反向拓扑排序
        {
            int x=q2.front();
            q2.pop();
            for(int i=p2.v[x];i!=-1;i=p2.next[i])
            {
                p2.w[p2.pre[i]]+=p2.w[x];
                p2.d[p2.pre[i]]--;
                if(p2.d[p2.pre[i]]==0)
                    q2.push(p2.pre[i]);
            }
        }

        int maxx=1;
        for(int i=1;i<=m;i++)
        {
            int temp=p1.w[p2.pre[i]]*p2.w[p1.pre[i]];//正向入度与反向入度乘积
            if(maxx<temp)
                maxx=temp;
        }
        printf("%d\n",maxx);
    }
    return 0;
}
```






