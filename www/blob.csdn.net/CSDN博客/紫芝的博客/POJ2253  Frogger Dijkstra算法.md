# POJ2253   Frogger  Dijkstra算法 - 紫芝的博客 - CSDN博客





2018年04月23日 23:51:40[紫芝](https://me.csdn.net/qq_40507857)阅读数：41
个人分类：[最短路算法](https://blog.csdn.net/qq_40507857/article/category/7617885)









# Frogger
|**Time Limit:** 1000MS||**Memory Limit:** 65536K|
|----|----|----|
|**Total Submissions:** 58995||**Accepted:** 18505|

Description

Freddy Frog is sitting on a stone in the middle of a lake. Suddenly he notices Fiona Frog who is sitting on another stone. He plans to visit her, but since the water is dirty and full of tourists' sunscreen, he wants to avoid swimming and instead reach her by jumping. 

Unfortunately Fiona's stone is out of his jump range. Therefore Freddy considers to use other stones as intermediate stops and reach her by a sequence of several small jumps. 

To execute a given sequence of jumps, a frog's jump range obviously must be at least as long as the longest jump occuring in the sequence. 

The frog distance (humans also call it minimax distance) between two stones therefore is defined as the minimum necessary jump range over all possible paths between the two stones. 


You are given the coordinates of Freddy's stone, Fiona's stone and all other stones in the lake. Your job is to compute the frog distance between Freddy's and Fiona's stone. 

Input

The input will contain one or more test cases. The first line of each test case will contain the number of stones n (2<=n<=200). The next n lines each contain two integers xi,yi (0 <= xi,yi <= 1000) representing the coordinates of stone #i. Stone #1 is Freddy's stone, stone #2 is Fiona's stone, the other n-2 stones are unoccupied. There's a blank line following each test case. Input is terminated by a value of zero (0) for n.

Output

For each test case, print a line saying "Scenario #x" and a line saying "Frog Distance = y" where x is replaced by the test case number (they are numbered from 1) and y is replaced by the appropriate real number, printed to three decimals. Put a blank line after each test case, even after the last one.

Sample Input

```
2
0 0
3 4

3
17 4
19 4
18 5

0
```

Sample Output

```
Scenario #1
Frog Distance = 5.000

Scenario #2
Frog Distance = 1.414
```

Source

[Ulm Local 1997](http://poj.org/searchproblem?field=source&key=Ulm+Local+1997)

## 题意：

有一条小河，河中有青蛙a和青蛙b。青蛙a呆在石头1上，青蛙b呆在石头2上，

已知河中总共有n块石头，编号为1~n，已知所有石头的坐标。

现在青蛙a想去找青蛙b玩，求青蛙a所需的最小跳跃距离。


## 解析：

## 这个题意很简单，我们需要先预处理一下数据，建成一个无向图，然后就是dijkstra算法

但是注意，这里求的是青蛙a所在的石头到青蛙b所在石头的路径上的最大边的值，

所以松弛操作要稍微变一下形。

从第1点到第2点所有可达路径中,两点距离的所有最大值中的最小值

## Dijkstra算法

```cpp
#include<iostream>
#include<stdio.h>
#include<math.h>
#include<string.h>
#include<stdlib.h>
using namespace std;
#define maxn 205
#define inf 999999.9
double ma[maxn][maxn];//邻接矩阵存图
double dist[maxn];//dis[i]表示起点到i最短距离
int vis[maxn];//最短距离是否求出
double xy[maxn][2];//输入数据
int n;
//计算距离
double far(double x1,double y1,double x2,double y2)
{
    double f=(x1-x2)*(x1-x2)+(y1-y2)*(y1-y2);
    //printf("%.3lf\n",sqrt(f));
    return sqrt(f);
}
//起点为u
double dijkstra(int u)
{
    for(int i=1; i<=n; i++)//初始化
    {
        dist[i]=ma[u][i];
        //printf("%.3lf\n",dist[i]);
        vis[i]=0;
    }
    // printf("\n");
    vis[u]=1;//标记起点的最短距离已经求出
    dist[u]=0;

    for(int i=1; i<=n-1; i++)
    {
        double mi=inf;//最小距离
        int tm=0;//下标
        for(int j=1; j<=n; j++)
        {
            if(!vis[j]&&dist[j]<mi)
            {
                mi=dist[j];
                tm=j;
            }
        }
        vis[tm]=1;//标记
        if(tm==0)    break;
        for(int j=1; j<=n; j++)
        {
            if(!vis[j])
            {
                double maxx=max(dist[tm],ma[tm][j]);
//dist记录的不是最短路径而是最短路径中的两个结点间的最短长度
                if(dist[j]>maxx)
                     dist[j]=maxx;
            }
        }
    }
    return dist[2];
}
int main()
{
    int ca=1;
    while(scanf("%d",&n)!=EOF)
    {
        if(n==0)
            return 0;
        memset(ma,inf,sizeof(ma));
        memset(dist,inf,sizeof(dist));
        memset(vis,0,sizeof(vis));
        for(int i=1; i<=n; i++)
        {
            scanf("%lf %lf",&xy[i][0],&xy[i][1]);
        }
        for(int i=1; i<=n; i++)
        {
            for(int j=1; j<i; j++)
            {
                if(i!=j)
                {
                    ma[i][j]=ma[j][i]=far(xy[i][0],xy[i][1],xy[j][0],xy[j][1]);
                }
            }
            ma[i][i]=0;
        }
        double ans=dijkstra(1);
        printf("Scenario #%d\n",ca);
        ca++;
        printf("Frog Distance = %.3f\n\n",ans);
    }
}
```

### STL优先队列实现对dijkstra算法的优化

```cpp
/*
从第0点到第1点所有可达路径中,两点距离的所有最大值中的最小值
*/
#include<cstdio>
#include<queue>
#include<cmath>
#include<map>
using namespace std;
#define INF 0x3fffffff
#define maxn 205
double mp[205][205];//mp存储两点间的距离， 
double path[205];//path[i]表示起点到i点的最大距离的最小值 
bool flag[205];//标记是否被访问过
int n,cnt=1; 
map<int,pair<int,int> >m;//map存储点x和y
struct node{
	int num;//存储石头编号
	double dis;//存储从起点到num石头的最大距离的最小值
	node(int a,double b){//初始化函数 
		num=a;
		dis=b;
	} 
	bool friend operator < (node a,node b){//优先队列重载 < 
		return a.dis>b.dis;
	} 
}; 
//两点之间距离函数 
double dis(int a,int b)
{
	return sqrt(pow((double)(m[a].first-m[b].first),2)+pow((double)(m[a].second-m[b].second),2));
}
void dijkstra()
{
	priority_queue<node> q;//优先队列优化dijkstra算法
	node start(0,0);
	path[0]=0;
	q.push(start);//把第一个起点加入队列 
	while(!q.empty()){
		node p=q.top();q.pop();
		if(flag[p.num]==true)continue;
		for(int i=0;i<n;i++){
		
			double max_dis=max(mp[i][p.num],path[p.num]);//求每条可达路径中两点距离最大值
			if((path[i]>max_dis)&&!flag[i])//求所有最大值中的最小值 
			{
				path[i]=max_dis;//更新 
				node temp(i,path[i]);
				q.push(temp);
			}
		}
	} 
}
int main()
{
    while(scanf("%d",&n)!=EOF&&n){
       for(int i=0;i<maxn;i++){
       	fill(mp[i],mp[i]+maxn,INF);//初始化
		   mp[i][i]=0; 
	   }
	   fill(path,path+maxn,INF);
	   fill(flag,flag+maxn,false);
	   
        //map对于石头编号i到x,y的映射
		for(int i=0;i<n;i++)
		{
			scanf("%d%d",&m[i].first,&m[i].second);
			for(int j=0;j<i;j++)
			mp[i][j]=mp[j][i]=dis(i,j);//计算两点距离 
		 } 
		 dijkstra(); 
		printf("Scenario #%d\n",cnt++);
        printf("Frog Distance = %.3f\n\n",path[1]);
        //输出从第0点到第1点的距离 
    }
    return 0;
}
```





