# Constructing Roads（HDU-1102） - Alex_McAvoy的博客 - CSDN博客





2018年05月19日 18:59:58[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：78
个人分类：[图论——最小生成树																[HDU](https://blog.csdn.net/u011815404/article/category/7923257)](https://blog.csdn.net/u011815404/article/category/7813283)








> 
# Problem Description

There are N villages, which are numbered from 1 to N, and you should build some roads such that every two villages can connect to each other. We say two village A and B are connected, if and only if there is a road between A and B, or there exists a village C such that there is a road between A and C, and C and B are connected. 

We know that there are already some roads between some villages and your job is the build some roads such that all the villages are connect and the length of all the roads built is minimum.

# **Input**

The first line is an integer N (3 <= N <= 100), which is the number of villages. Then come N lines, the i-th of which contains N integers, and the j-th of these N integers is the distance (the distance should be an integer within [1, 1000]) between village i and village j.

Then there is an integer Q (0 <= Q <= N * (N + 1) / 2). Then come Q lines, each line contains two integers a and b (1 <= a < b <= N), which means the road between village a and village b has been built.

# Output

You should output a line contains an integer, which is the length of all the roads to be built such that all the villages are connected, and this value is minimum. 

# Sample Input

**3**

**0 990 692990 0 179692 179 011 2**

# Sample Output

**179**


题意：有n个村庄，编号1-n，以矩阵的形式给出任意两个村庄之间的距离，然后告诉已经有q个村庄已经修好了路，问现在要打算使所有村庄都联通需要修路的最小长度。

思路：最小生成树问题，使用Kruskal算法即可。

# Source Program

```cpp
#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<cstring>
#include<cmath>
#include<algorithm>
#include<string>
#define INF 999999999
#define N 101
#define MOD 1000000007
#define E 1e-12
using namespace std;
int father[110];
struct node
{
    int x;
    int y;
    int w;
}a[10010];

int Find(int x)
{
    if(father[x]!=x)
        father[x]=Find(father[x]);
    return father[x];
}

int Union(int x,int y)
{
    x=Find(x);
    y=Find(y);
    if(x!=y)
    {
        father[y]=x;
        return 1;
    }
    return 0;
}

int cmp(node a,node b)
{
    return a.w<b.w;
}
int main()
{
    int n;
    while(scanf("%d",&n)!=EOF)
    {
        for(int i=1;i<=n;i++)//初始化
           father[i]=i;

        int k=0;
        for(int i=1;i<=n;i++)//数据处理
        {
            for(int j=1;j<=n;j++)
            {
                int w;
                cin>>w;
                if(j>i)
                {
                    a[k].x=i;
                    a[k].y=j;
                    a[k].w=w;
                    k++;
                }
            }
        }
        sort(a,a+k,cmp);//依照权值排序

        int edge;
        cin>>edge;
        for(int i=0;i<edge;i++)//遍历所有边，进行合并
        {
            int x,y;
            cin>>x>>y;
            Union(x,y);
        }
        int MST=0;
        for(int i=0;i<k;i++)//计算最小权值
            if(Union(a[i].x,a[i].y))
              MST+=a[i].w;

        cout<<MST<<endl;
    }
    return 0;
}
```






