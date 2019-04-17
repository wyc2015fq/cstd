# Bus System（HDU-1690） - Alex_McAvoy的博客 - CSDN博客





2018年05月21日 23:58:46[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：39
个人分类：[HDU																[图论——Floyd](https://blog.csdn.net/u011815404/article/category/8312424)](https://blog.csdn.net/u011815404/article/category/7923257)








> 
# Problem Description

Because of the huge population of China, public transportation is very important. Bus is an important transportation method in traditional public transportation system. And it’s still playing an important role even now.

The bus system of City X is quite strange. Unlike other city’s system, the cost of ticket is calculated based on the distance between the two stations. Here is a list which describes the relationship between the distance and the cost.

![](https://img-blog.csdn.net/20180522000539524)

Your neighbor is a person who is a really miser. He asked you to help him to calculate the minimum cost between the two stations he listed. Can you solve this problem for him?

To simplify this problem, you can assume that all the stations are located on a straight line. We use x-coordinates to describe the stations’ positions.

# Input

The input consists of several test cases. There is a single number above all, the number of cases. There are no more than 20 cases.

Each case contains eight integers on the first line, which are L1, L2, L3, L4, C1, C2, C3, C4, each number is non-negative and not larger than 1,000,000,000. You can also assume that L1<=L2<=L3<=L4.

Two integers, n and m, are given next, representing the number of the stations and questions. Each of the next n lines contains one integer, representing the x-coordinate of the ith station. Each of the next m lines contains two integers, representing the start point and the destination.

In all of the questions, the start point will be different from the destination.

For each case,2<=N<=100,0<=M<=500, each x-coordinate is between -1,000,000,000 and 1,000,000,000, and no two x-coordinates will have the same value.

# Output

For each question, if the two stations are attainable, print the minimum cost between them. Otherwise, print “Station X and station Y are not attainable.” Use the format in the sample.

# Sample Input

**2**

**1 2 3 4 1 3 5 74 212341 44 11 2 3 4 1 3 5 74 1123101 4**

# Sample Output

**Case 1:**

**The minimum cost between station 1 and station 4 is 3.The minimum cost between station 4 and station 1 is 3.Case 2:Station 1 and station 4 are not attainable.**


题意：输入所有点的坐标，求任意两点间的最短花费。

思路：求任意两点最短路，使用floyd算法最合适，注意用long long！

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
#define INF 0x3f7f7f7f7f7f7f7f
#define N 101
#define MOD 1000000007
#define E 1e-12
using namespace std;
long long n,m;
long long g[N][N],point[N];
long long l1,l2,l3,l4,c1,c2,c3,c4;
long long calculate(long long x)
{
    if(x<=l1)
        return c1;
    else if(x>l1&&x<=l2)
        return c2;
    else if(x>l2&&x<=l3)
        return c3;
    else if(x>l3&&x<=l4)
        return c4;
    else
        return INF;
}

int main()
{
    int t;
    scanf("%d",&t);
    for(long long k=1;k<=t;k++)
    {
        scanf("%lld%lld%lld%lld%lld%lld%lld%lld",&l1,&l2,&l3,&l4,&c1,&c2,&c3,&c4);
        scanf("%lld%lld",&n,&m);
        for(long long i=1;i<=n;i++)
            scanf("%lld",&point[i]);

        for(long long i=0;i<n;i++)
            for(long long j=0;j<n;j++)
                g[i][j]=INF;

        for(long long i=1;i<=n;i++)
            for(long long j=i+1;j<=n;j++)
            {
                long long dis=calculate(fabs(point[j]-point[i]));
                g[i][j]=dis;
                g[j][i]=dis;
            }

        for(long long k=1;k<=n;k++)
            for(long long i=1;i<=n;i++)
                for(long long j=1;j<=n;j++)
                    g[i][j]=min(g[i][j],g[i][k]+g[k][j]);

        printf("Case %lld:\n",k);
        for(long long i=1;i<=m;i++)
        {
            long long x,y;
            scanf("%lld%lld",&x,&y);
            if(g[x][y]!=INF)
                printf("The minimum cost between station %lld and station %lld is %lld.\n",x,y,g[x][y]);
            else
                printf("Station %lld and station %lld are not attainable.\n",x,y);
        }
    }
    return 0;
}
```






