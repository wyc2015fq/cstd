# Shortest Path（HDU-5636） - Alex_McAvoy的博客 - CSDN博客





2019年01月19日 16:18:18[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：28
个人分类：[HDU																[图论——Floyd](https://blog.csdn.net/u011815404/article/category/8312424)](https://blog.csdn.net/u011815404/article/category/7923257)








> 
# Problem Description

There is a path graph G=(V,E) with nn vertices. Vertices are numbered from 1 to n and there is an edge with unit length between i and i+1 (1≤i<n). To make the graph more interesting, someone adds three more edges to the graph. The length of each new edge is 1. 

You are given the graph and several queries about the shortest path between some pairs of vertices.

# Input

There are multiple test cases. The first line of input contains an integer T, indicating the number of test cases. For each test case: 

The first line contains two integer n and m (1≤n,m≤105) -- the number of vertices and the number of queries. The next line contains 6 integers a1,b1,a2,b2,a3,b3 (1≤a1,a2,a3,b1,b2,b3≤n), separated by a space, denoting the new added three edges are (a1,b1), (a2,b2), (a3,b3). 

In the next m lines, each contains two integers si and ti (1≤si,ti≤n), denoting a query. 

The sum of values of m in all test cases doesn't exceed 106.

# Output

For each test cases, output an integer S=(∑mi⋅zi) mod (109+7), where zi is the answer for i-th query.

# **Sample Input**

**110 22 4 5 7 8 101 53 1**

# Sample Output

**7**


题意：t 组数据，每组数据给出两个数 n、m 代表一个图有 n 个点，且相邻两个点 i 到 i+1 的距离是 1，另外再加三条边，边权同样为 1，m 代表 m 组查询，每组查询给出两个数 x、y ，要求将每组的查询结果 mi 乘以组号 i 后相加模 1E9+7 后输出

思路：实质就是一条线，线上有 n 个点，相邻两点距离为一，然后有 3 组点的距离改为 1，然后查询任意两点的距离。

由于数据量的问题，直接 Floyd，开数组 dis[1000000][1000000] 会爆，故此题需要技巧

通过题意可以看出，在不加新的三条边前，任意两点 x、y 的路径长度为 |x-y|，由于加 3 条边权为 1 的边后要求两点最短路，因此增加的边要具有缩短边权的功能，否则就没有任何作用

因此，可以直接去枚举新增的三条边，通过 Floyd 看能否缩短路径，如果可以的话就更新最小值，然后查询时，先通过给出的两点 x、y 判断原距离 |x-y|，然后与 Floyd 后的最小距离比较，找出最小的，最后直接将每组查询结果与组号相乘再相加取模即可

# Source Program

```cpp
#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<string>
#include<cstring>
#include<cmath>
#include<ctime>
#include<algorithm>
#include<utility>
#include<stack>
#include<queue>
#include<vector>
#include<set>
#include<map>
#define PI acos(-1.0)
#define E 1e-6
#define MOD 1000000007
#define INF 0x3f3f3f3f
#define N 1001
#define LL long long
using namespace std;
int n,m;
int a[N];
int dis[N][N];
void floyd(){
    for(int k=1;k<=6; k++)
        for(int i=1;i<=6;i++)
            for(int j=0;j<=6;j++)
                dis[i][j]=min(dis[i][j],dis[i][k]+dis[k][j]);
}
int main (){
    int t;
	scanf("%d",&t);
	while(t--){
		scanf("%d%d",&n,&m);
		for(int i=1;i<=6;i++) //六个点
			scanf("%d",&a[i]);

		for(int i=1;i<=6;i++)
            for(int j=1;j<=6;j++)
                dis[i][j]=abs(a[i]-a[j]);

		for(int i=1;i<=6;i+=2)//相邻距离为1
			dis[i][i+1]=dis[i+1][i]=1;

		floyd();

		LL res=0;
		for(int i=1;i<=m;i++){
            int x,y;
			scanf("%d%d",&x,&y);
			int len=abs(x-y);//原距离
			for(int j=1;j<=6;j++)
                for(int k=1;k<=6;k++)
                    len=min(len,abs(x-a[j])+abs(y-a[k])+dis[j][k]);

			res=(res+(LL)i*len%MOD)%MOD;
		}
		printf("%lld\n",res);
	}

    return 0;
}
```






