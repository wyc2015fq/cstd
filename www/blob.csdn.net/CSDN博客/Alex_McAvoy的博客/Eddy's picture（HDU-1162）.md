# Eddy's picture（HDU-1162） - Alex_McAvoy的博客 - CSDN博客





2018年05月19日 19:25:21[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：49
个人分类：[图论——最小生成树																[HDU](https://blog.csdn.net/u011815404/article/category/7923257)](https://blog.csdn.net/u011815404/article/category/7813283)








> 
# Problem Description

Eddy begins to like painting pictures recently ,he is sure of himself to become a painter.Every day Eddy draws pictures in his small room, and he usually puts out his newest pictures to let his friends appreciate. but the result it can be imagined, the friends are not interested in his picture.Eddy feels very puzzled,in order to change all friends 's view to his technical of painting pictures ,so Eddy creates a problem for the his friends of you.

Problem descriptions as follows: Given you some coordinates pionts on a drawing paper, every point links with the ink with the straight line, causes all points finally to link in the same place. How many distants does your duty discover the shortest length which the ink draws?

# **Input**

The first line contains 0 < n <= 100, the number of point. For each point, a line follows; each following line contains two real numbers indicating the (x,y) coordinates of the point. 

Input contains multiple test cases. Process to the end of file.

# Output

Your program prints a single real number to two decimal places: the minimum total length of ink lines that can connect all the points. 

# Sample Input

**3**

**1.0 1.02.0 2.02.0 4.0**

# Sample Output

**3.41**


题意：给出n个点，将所有点相连，求最短距离。

思路：最小生成树问题，使用Prim算法即可。

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
double x[N],y[N];
double dis[N];
bool vis[N];
double calculate(double x1,double y1,double x2,double y2)
{
    return sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2));
}
int main()
{
    int n;
    while(cin>>n)
    {
        for(int i=1;i<=n;i++)
        {
            cin>>x[i]>>y[i];
            dis[i]=INF;
        }
        double MST=0;
        dis[1]=0;
        memset(vis,0,sizeof(vis));

        for(int i=1;i<=n;i++)
        {
            int k=0;
            double minn=INF;
            for(int j=1;j<=n;j++)//寻找与白点相连的权值最小的蓝点k
                if(vis[j]==0&&dis[j]<minn)
                {
                    k=j;
                    minn=dis[j];
                }
            vis[k]=1;//蓝点k加入生成树，标记为白点
            MST+=minn;

            for(int j=1;j<=n;j++)//修改所有与k相连的蓝点
            {
                double len=calculate(x[k],y[k],x[j],y[j]);
                if(vis[j]==0&&len<dis[j])
                    dis[j]=len;
            }
        }
        printf("%.2lf\n",MST);
    }
    return 0;
}
```






