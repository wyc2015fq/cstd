# 躁动的小Z 最短路+路径记录 - 紫芝的博客 - CSDN博客





2018年07月27日 00:16:16[紫芝](https://me.csdn.net/qq_40507857)阅读数：273








## 躁动的小Z

时间限制: 1 Sec  内存限制: 128 MB

## 题目描述

你猜怎么样？小Z追到Gakki了！Gakki邀请小Z去她家共进晚餐，小Z喜出望外。小Z的家和Gakki的家隔着几个街区，所以他决定乘坐公交车前往

Gakki家赴约。小Z的家在公交站台的起始站（编号为1），而Gakki家正好在末站（编号为n）。城市中有许多公交站台，有些站台之间可以通过公交

线路互相到达。现在给你n个公交站台和m条不同的公交线路的时间花费，请你帮助小Z分析一下最短的可以从家里来到Gakki身边的路径？

## 输入

有多组测试样例。



第一行两个正整数n,m(2≤n≤10^5,0≤m≤10^5)，代表站台数与公交线路数量。



接下来m行每行三个正整数a[i],b[i],w[i],代表从公交站a[i]到b[i]需要花费的时间为w[i]。(1≤a[i],b[i]≤n,1≤w[i]≤10^6)



注意：公交线路可能会产生环，并且两个站台之间可能有多条公交线路。

## 输出

单独一行，输出花费时间最小时小Z经过的公交站台编号，以空格隔开；如果小Z无法到达Gakki的家，则输出-1.

## 样例输入

```
5 6
1 2 2
2 5 5
2 3 4
1 4 1
4 3 3
3 5 1
```

```
10 10
1 5 12
2 4 140
2 10 149
3 6 154
3 7 9
3 8 226
3 10 132
4 10 55
5 8 33
7 8 173
```

## 样例输出
`1 4 3 5``1 5 8 7 3 10`
### 题解

在dijkstra算法模板的基础上加上一个pre数组，用于记录该节点的上一个节点，即该点是经过哪一点才到达该点的。pre数组具体在边松弛的过程中进行重新赋值，松弛成功就将pre值记录k点，及该点是由起点经过k点后所得到的。最后把pre数组中的值递归输出一遍即可。

```cpp
#include<bits/stdc++.h>
using namespace std;
const int maxn = 1e3+9;
#define inf 0x3f3f3f3f //需将road及dis初始化为正无穷inf
int n,m;

int pre[maxn];//记录到达该点的上一个点的坐标
int dis[maxn];  //储存各个点到源点的最短距离,dis[s]为0
int road[maxn][maxn];  //两点之间直接距离关系
bool vis[maxn];     //判断源点到该点的距离是否为最短距离

void dijkstra(int s,int e)
{//s为起点，e为终点
    memset(vis, false, sizeof(vis));//标记是否求出最短路径
    vis[s] = true;//标记起点到这一点的最小距离已经求出
    for(int i = 1; i <= n; i++){
        dis[i] = road[s][i];//初始化起点到每一个点的距离
        pre[i]=s;//初始化路径，每个点的上一个点为起点
    }
    for(int u = 1; u < n; u++)
    {
        int minD = inf,k = -1;
        for(int i = 1; i<= n; i++)
        {   //寻找没有访问过的最短路
            if(!vis[i]&&dis[i]<minD)
            {
                k = i;//记录下标
                minD = dis[i];//记录最小值
            }
        }
        if(k==e)    break;
        vis[k] = true;//标记已经访问过
        //松弛操作
        for(int i = 1; i<= n; i++)
        {
            if(!vis[i]&&dis[k]+road[k][i]<dis[i])
            {
                dis[i]=dis[k]+road[k][i];
                pre[i]=k;
            }//if
        }//for
    }
}
void print(int cur)
{
    if(cur==1){
        printf("1");
        return;
    }
    print(pre[cur]);
    printf(" %d",cur);
}
int main()
{
    while(scanf("%d%d",&n,&m)!=EOF){
        memset(road,inf,sizeof(road));
        memset(pre,0,sizeof(pre));
        for(int i=1;i<=m;i++)
        {
            int a,b,d;
            scanf("%d%d%d",&a,&b,&d);
            road[a][b]=road[b][a]=min(d,road[a][b]);
            //起点是fans[i],终点是S
        }
        dis[1]=0;//起点
        dijkstra(1,n);
            if(dis[n]==inf)    printf("-1");
            else   {
    //printf("dis=%d\n%d:%d\n",dis[i],i,pre[i]);
                print(n);
            }
            printf("\n");
        }
    return 0;
}
```





