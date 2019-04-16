# UVA 10480 Sabotage 最大流（最小割） - 紫芝的博客 - CSDN博客





2018年10月07日 15:21:51[紫芝](https://me.csdn.net/qq_40507857)阅读数：30
个人分类：[最大流Dinic算法																[进击的网络流](https://blog.csdn.net/qq_40507857/article/category/7964413)](https://blog.csdn.net/qq_40507857/article/category/8104974)








# Sabotage

The regime of a small but wealthy dictatorship has been abruptly overthrown by an unexpected rebellion. Because of the enormous disturbances this is causing in world economy, an imperialist military super power has decided to invade the country and reinstall the old regime.

For this operation to be successful, communication between the capital and the largest city must be completely cut. This is a difficult task, since all cities in the country are connected by a computer network using the Internet Protocol, which allows messages to take any path through the network. Because of this, the network must be completely split in two parts, with the capital in one part and the largest city in the other, and with no connections between the parts.

There are large differences in the costs of sabotaging different connections, since some are much more easy to get to than others.

Write a program that, given a network specification and the costs of sabotaging each connection, determines which connections to cut in order to separate the capital and the largest city to the lowest possible cost.

## Input

Input file contains several sets of input. The description of each set is given below.

The first line of each set has two integers, separated by a space: First one the number of cities, *n*in the network, which is at most 50. The second one is the total number of connections, *m*, at most 500.

The following *m* lines specify the connections. Each line has three parts separated by spaces: The first two are the cities tied together by that connection (numbers in the range 1 - *n*). Then follows the cost of cutting the connection (an integer in the range 1 to 40000000). Each pair of cites can appear at most once in this list.

Input is terminated by a case where values of *n* and *m* are zero. This case should not be processed. For every input set the capital is city number 1, and the largest city is number 2.

## Output

For each set of input you should produce several lines of output. The description of output for each set of input is given below:

The output for each set should be the pairs of cities (i.e. numbers) between which the connection should be cut (in any order), each pair on one line with the numbers separated by a space. If there is more than one solution, any one of them will do.

Print a blank line after the output for each set of input.

## Sample Input

5 8
1 4 30
1 3 70
5 3 20
4 3 5
4 5 15
5 2 10
3 2 25
2 4 50
5 8
1 4 30
1 3 70
5 3 20
4 3 5
4 5 15
5 2 10
3 2 25
2 4 50
0 0

## Sample Output

4 1
3 4
3 5
3 2

4 1
3 4
3 5
3 2

**题意：****要把一个无向图分成两部分，要把点1和点2分开。割断每条边都有一个花费，求最小花费的情况下，输出应该切断哪些边？答案不唯一****分析：****这题很明显是最小割，也就是最大流。把1当成源点，2当成汇点。问题是要求最小割应该隔断那条边。****思路就是求最大流，BFS搜索增广路，然后残留网络下，和源点连通的分在源点一点，和汇点连通的分在汇点一边，每个点相邻边的流量为0时，说明该点已经被去除**
```cpp
#include<cstdio>
#include<cstdio>
#include<algorithm>
#include<cstring>
#include<vector>
#include<queue>
using namespace std;
#define INF 0x3f3f3f3f
const int maxn=1e3+10;
 int n,m;
int mp[maxn][maxn];//初始流量
int flow[maxn][maxn];//最大流
int path[maxn];//记录路径
int a[maxn];//i点在最大流中的流量
int start,End;
int x[maxn],y[maxn];
int maxflow()
{
    queue<int> q;
    memset(flow,0,sizeof flow);
    int max_flow=0;
    while(true){
        memset(a,0,sizeof a);//清空标记
        a[start]=INF;//源点权值无限大
        while(!q.empty())   q.pop();
        q.push(start);
        while(!q.empty()){
            int temp=q.front();
            q.pop();
            for(int i=1;i<=n;i++){
                //未走过且流量小于他的权值
                if(!a[i]&&flow[temp][i]<mp[temp][i])
                {
                    path[i]=temp;//记录前一个点
                    a[i]=min(a[temp],mp[temp][i]-flow[temp][i]);
                    q.push(i);
                }
            }

        }
        if(a[End]==0)   break;
        //更新剩余网络
        for(int i=End;i!=start;i=path[i]){
            flow[path[i]][i]+=a[End];//反向边加
            flow[i][path[i]]-=a[End];//正向边减
        }
        max_flow+=a[End];
    }
    return max_flow;
}
int main()
{
    while(scanf("%d%d",&n,&m)!=EOF&&n&&m){
        memset(mp,0,sizeof mp);

        for(int i=1;i<=m;i++)
        {
            int u,v,w;
            scanf("%d%d%d",&u,&v,&w);
            mp[u][v]=mp[v][u]=w;
            x[i]=u;
            y[i]=v;
        }
        start=1;End=2;
        int flow=maxflow();
        for(int i=1;i<=m;i++){//枚举每一条边的两个点
            //有一点的权值为0，说明当前点被删除
            if((!a[x[i]]&&a[y[i]])||(a[x[i]]&&!a[y[i]]))
            printf("%d %d\n",x[i],y[i]);
        }
        printf("\n");
    }
    return 0;
}

/*
5 8
1 4 30
1 3 70
5 3 20
4 3 5
4 5 15
5 2 10
3 2 25
2 4 50
5 8
1 4 30
1 3 70
5 3 20
4 3 5
4 5 15
5 2 10
3 2 25
2 4 50
0 0
*/
```






