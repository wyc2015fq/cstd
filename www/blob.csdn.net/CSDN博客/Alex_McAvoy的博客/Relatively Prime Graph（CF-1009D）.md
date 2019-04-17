# Relatively Prime Graph（CF-1009D） - Alex_McAvoy的博客 - CSDN博客





2018年08月23日 20:56:12[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：121








# Problem Description

Let's call an undirected graph G=(V,E) relatively prime if and only if for each edge (v,u)∈E(v,u)，GCD(v,u)=1 (the greatest common divisor of v and u is 1). If there is no edge between some pair of vertices vv and uu then the value of GCD(v,u) doesn't matter. The vertices are numbered from 1 to |V|.

Construct a relatively prime graph with n vertices and m edges such that it is connected and it contains neither self-loops nor multiple edges.

If there exists no valid graph with the given number of vertices and edges then output "Impossible".

If there are multiple answers then print any of them.

# Input

The only line contains two integers n and m (1≤n,m≤105) — the number of vertices and the number of edges.

# Output

If there exists no valid graph with the given number of vertices and edges then output "Impossible".

Otherwise print the answer in the following format:

The first line should contain the word "Possible".

The ii-th of the next mm lines should contain the i-th edge (vi,ui) of the resulting graph (1≤vi,ui≤n,vi≠ui). For each pair (v,u) there can be no more pairs (v,u) or (u,v). The vertices are numbered from 11 to nn.

If there are multiple answers then print any of them.

# Examples

**Input**

5 6

**Output**

Possible

2 5

3 2

5 1

3 4

4 1

5 4

**Input**

6 12

**Output**

Impossible

————————————————————————————————————————————

题意：构造一个 n 个点 m 条边的图，要求图联通且GCD(u，v)=1，u~v 才可以建边

思路：暴力大法好。。。枚举所有的组合，如果满足了 n 个即可，还要判断图是否联通，边数是否到达 m 个，判断是否有答案。

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
int GCD(int a,int b)
{
    return b==0?a:GCD(b,a%b);
}
struct Node{
    int u;
    int v;
}a[N];
int main()
{
    int n,m;
    cin>>n>>m;

    int k=0;
    for(int i=1;i<=n && k<m;i++)
    {
        for(int j=i+1;j<=n && k<m;j++)
        {
            if(GCD(i,j)==1)
            {
                a[k].u=i;
                a[k].v=j;
                k++;
            }
        }
    }

    if(m<n-1||k<m)
        cout<<"Impossible"<<endl;
    else
    {
        cout<<"Possible"<<endl;
        for(int i=0;i<k;i++)
            cout<<a[i].u<<" "<<a[i].v<<endl;
    }


    return 0;
}
```





