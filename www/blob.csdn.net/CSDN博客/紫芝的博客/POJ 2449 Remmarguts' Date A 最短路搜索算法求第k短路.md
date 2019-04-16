# POJ 2449 Remmarguts' Date A*最短路搜索算法求第k短路 - 紫芝的博客 - CSDN博客





2018年09月13日 08:38:27[紫芝](https://me.csdn.net/qq_40507857)阅读数：31








Remmarguts' Date
|**Time Limit:** 4000MS||**Memory Limit:** 65536K|
|----|----|----|
|**Total Submissions:** 37284||**Accepted:** 10263|

Description

"Good man never makes girls wait or breaks an appointment!" said the mandarin duck father. Softly touching his little ducks' head, he told them a story. 


"Prince Remmarguts lives in his kingdom UDF – United Delta of Freedom. One day their neighboring country sent them Princess Uyuw on a diplomatic mission." 


"Erenow, the princess sent Remmarguts a letter, informing him that she would come to the hall and hold commercial talks with UDF if and only if the prince go and meet her via the K-th shortest path. (in fact, Uyuw does not want to come at all)" 


Being interested in the trade development and such a lovely girl, Prince Remmarguts really became enamored. He needs you - the prime minister's help! 


DETAILS: UDF's capital consists of N stations. The hall is numbered S, while the station numbered T denotes prince' current place. M muddy directed sideways connect some of the stations. Remmarguts' path to welcome the princess might include the same station twice or more than twice, even it is the station with number S or T. Different paths with same length will be considered disparate. 

Input

The first line contains two integer numbers N and M (1 <= N <= 1000, 0 <= M <= 100000). Stations are numbered from 1 to N. Each of the following M lines contains three integer numbers A, B and T (1 <= A, B <= N, 1 <= T <= 100). It shows that there is a directed sideway from A-th station to B-th station with time T. 


The last line consists of three integer numbers S, T and K (1 <= S, T <= N, 1 <= K <= 1000).

Output

A single line consisting of a single integer number: the length (time required) to welcome Princess Uyuw using the K-th shortest path. If K-th shortest path does not exist, you should output "-1" (without quotes) instead.

Sample Input

2 2
1 2 5
2 1 4
1 2 2

Sample Output

14
Source

[POJ Monthly](http://poj.org/searchproblem?field=source&key=POJ+Monthly),Zeyuan Zhu

**题意：**

给出一个n个节点和m条边的无向有环图，求起点到终点的第k短路，可以走重复的路，如果存在环，无论求第几短路都是存在的
**A*算法**

估价函数=当前值+当前位置到终点的距离，f(p)=g(p)+h(p);每次扩展估价函数中值最小的一个

对于k短路来说，g(p)为当前从s到p所走的长度，h(p)为从p到t的最短距离，f(p)就是从s走到p，再从p到t一共要走多远

即每次扩展都是有方向的扩展，可以提高求解速度和减少扩展的状态数

为了加速计算h(p),需要从A*开始进行预处理，将原图的边反向，再从终点t开始做一次单源最短路，就可以得到每个节点的h(p)

```cpp
#include<cstdio>
#include<cstring>
#include<queue>
#include<algorithm>
using namespace std;
const int maxm=1e5+10,maxn=1e3+7;
#define inf 0x3f3f3f3f
struct node
{
  int v,w,next;
}edge[maxm],revedge[maxm];
struct A
{
  int v;//当前走到的节点
  int g;//g函数
  int f;//f函数
  bool operator < (const A a)const {
    if(a.f==f)
    return a.g<g;
    return a.f<f;
  }
};
int e,d[maxn];
bool vis[maxn];
int q[maxm*5];
int head[maxn],revhead[maxn];
int n,m,s,t,k;
void init()
{
  e=0;
  memset(head,-1,sizeof head);
  memset(revhead,-1,sizeof revhead);
}
void insert(int x,int y,int w)
{
  edge[e].v=y;
  edge[e].w=w;
  edge[e].next=head[x];
  head[x]=e;

  revedge[e].v=x;
  revedge[e].w=w;
  revedge[e].next=revhead[y];
  revhead[y]=e++;
}

void spfa(int src)
{
  memset(vis,0,sizeof vis);
  memset(d,inf,sizeof d);

  d[src]=0;

  queue<int>que;

  que.push(src);
  while(!que.empty()){
    int u=que.front();
    que.pop();
    vis[u]=0;
    for(int i=revhead[u];i!=-1;i=revedge[i].next)
    {
      int v=revedge[i].v;
      int w=revedge[i].w;

      if(d[v]>d[u]+w)
      {
        d[v]=d[u]+w;
          if(!vis[v]){
            vis[v]=true;
            que.push(v);
          }//if
      }//if
    }//for
  }//while
}

/*
void spfa(int src)
{
  memset(vis,0,sizeof vis);
  memset(d,inf,sizeof d);
  int h=0,t=1;
  q[0]=src;
  d[src]=0;
  //printf("33333333\n");
  while(h<t){
    int u=q[h++];
    //printf("%d\n",u);
    vis[u]=0;
    for(int i=revhead[u];i!=-1;i=revedge[i].next){
      int v=revedge[i].v;
      int w=revedge[i].w;
      if(d[v]>d[u]+w)
      {
        d[v]=d[u]+w;
        if(!vis[v]){
          q[t++]=v;
          vis[v]=1;
        }
      }
    }
  }
}
*/
int Astar(int src,int des)
{
  int cnt=0;
  priority_queue<A>Q;
  if(src==des)
  k++;
  if(d[src]==inf) return -1;

  A start;
  start.v=src;
  start.g=0;
  start.f=start.g+d[src];
  Q.push(start);

  while(!Q.empty()){
    A tt=Q.top();
    Q.pop();
    if(tt.v==des){
      cnt++;
      if(cnt==k)  return tt.g;
    }
    for(int i=head[tt.v];i!=-1;i=edge[i].next){
      A t;
      t.v=edge[i].v;
      t.g=tt.g+edge[i].w;
      t.f=t.g+d[t.v];
      Q.push(t);
    }
  }
  return -1;
}
int main()
{
  while(scanf("%d%d",&n,&m)!=EOF){
    init();
    int x,y,w;
    int T;

    //scanf("%d",&T);
    for(int i=1;i<=m;i++)
    {
      scanf("%d%d%d",&x,&y,&w);
      insert(x,y,w);
    }

    scanf("%d%d%d",&s,&t,&k);
    spfa(t);
    /*
    int ans=Astar(s,t);
    if(ans==-1||ans>T)
    printf("Whitesnake!\n");
    else
    printf("yareyaredawa\n");
    */
    printf("%d\n",Astar(s,t));
  }
  return 0;
}
```





