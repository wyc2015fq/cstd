# POJ 3255  Roadblocks （次短路问题） - HJ - CSDN博客
2016年12月20日 16:59:03[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：293
题目链接：[http://poj.org/problem?id=3255](http://poj.org/problem?id=3255)
题目大意：在一个图上有许多个农场，有个人从1农场出发，到他的朋友n农场去，他不想走一条最短路径，这次他想换条路走，要你帮他找一条次短路径，次短路的定义是，比最短路径长度短（可能有多条），但是不会比其他的路径长度长。而且告诉你数据中一定存在至少一条次短路。
常规方法：删除某一条边求最短路径，找出的最短路径比最短路径长，但是比其他路径短的结果就是次短路了。但是分析一下这样做的时间复杂度是，用spfa的时间复杂度大概是O(KE)，E为边的总数=200000，然后枚举边O(E)，那么总的时间复杂度就是O(KE^2)，这样肯定就会超时了。
首先看看思路：最短路明显不会是次短路,因为题目说了次短一定是存在的，那么他们不可能重合，这样次短路肯定是最短路中某一条边不走，而走了其他边再回到最短路上，而且不可能绕两个地方，只可能绕一个地方，因为明显绕两个地方比绕一个地方的路径长，明显不是次短路了。
方法一： 
我们可以枚举每条边s到t。然后用d[s]记录源点到s的最短距离，而用dr[t]记录t到汇点的最短距离，这样就只需要从t到s求一次最短路得到了len从s到t表示边s到t的这条边的长度。 
然后我们枚举每一条边有：tmp=d[s]+dr[t]+len从s到t 
找出其中比最短路小但是比其他路长的一个值就是次短路径了。
```cpp
#include<iostream>
#include<cstring>
#include<queue>
#include<cstdio>
using namespace std;
#define maxv 5010
#define maxe 200100
#define INF 0xffffff
struct node{
    int v,w,next;
}edge[maxe];
int d[maxv],dr[maxv];
int n,m,cnt;
int head[maxv];
bool vis[maxv];
void add(int u,int v,int w)            //数据处理 
{
    edge[cnt].v=v;
    edge[cnt].w=w;
    edge[cnt].next=head[u];
    head[u]=cnt++;
}
void spfa(int x,int dt[])             //SPFA找最短路 
{
    int i,v,u;
    queue<int>q;
    memset(vis,0,sizeof(vis));
    dt[x]=0;
    vis[x]=1;
    q.push(x); 
    while(!q.empty())
    {
        int u=q.front();
        q.pop();
        vis[u]=0;                                   //出栈之后标记为0 
        for(int i=head[u];i!=-1;i=edge[i].next)
        {
            int v=edge[i].v;
            if(dt[v]>dt[u]+edge[i].w)
            {               
                dt[v]=dt[u]+edge[i].w;
                if(!vis[v])
                {
                    vis[v]=1;
                    q.push(v);
                }
            }
        }
    }
}
int main()
{
    int a,b,c;
    int ans,tmp;
    while(cin>>n>>m)
    {
        memset(head,-1,sizeof(head));
        cnt=0;
        for(int i=1;i<=n;i++)               
          d[i]=dr[i]=INF;
        while(m--)
        {
            cin>>a>>b>>c;
            add(a,b,c);
            add(b,a,c); 
        }
        spfa(1,d);                       //以1为起点做单源最短路 ，数据保存到d数组里  
        spfa(n,dr);                      //以n为起点做单源最短路，数据保存到dr数组里 
        ans=INF;
        for(int i=1;i<=n;i++)
        {
            for(int j=head[i];j!=-1;j=edge[j].next)
            {
                b=edge[j].v;
                c=edge[j].w;
                tmp=d[i]+dr[b]+c;              //d[i]为源点到i得最短路，dr[b]为汇点到b的最短路，c为len<i,b>即从i到b的最短路 
                if(tmp>d[n]&&ans>tmp)          //找到比最短路要长但是比其他路径都要短的，即是次短路 
                  ans=tmp;
            }
        }
        cout<<ans<<endl;
    }
    return 0;
}
```
方法二： 
先来谈谈Dijkstra的优化。对于每次寻找到当前为访问过的点中距离最短的那一个，运用优先队列进行优化，避免全部扫描，每更新一个点的最短距离就加入优先队列。有人会问，一个点如果已经处理完成了，那它还留在队列中怎么办？我们放入队列时将一个点那时的顶点编号和最短距离进行打包，如果取出该点时，它当前的最短距离小于该点标记的最短距离，说明该点已经取到最短距离，不进行操作。或者直接用一个vis数组来记录某一个点是否已经取到最短距离；其次的优化是用邻接表存储与每一个点相连的所有边，方便处理。
而这道题的做法和最短路径基本一致，唯一的不同点在于，在求出最短路径的情况下必须要保留下次短路径。对于Dijkstra判断中取出的每一个点，如果到它的最短距离大于当前该点的次短距离，则当前该点已经取到最短距离和次短距离，不进行操作，否则进行两次判断：如果小于最短边，则赋给最短变，并将最短边赋给次短边；或者如果大于最短变且小于次短边，则赋给次短边。两次完成之后均要加入队列。要注意几点：
```
（1）由于是一张无向图，读入的时候必须正向、逆向分为两条边存储，所以实际有向边的数量为r的两倍，数组绝对不能开小！
（2）初始化时，源点的短边初始化为0，源点的次短边必须初始化为INF，而不是0。比如下面这组数据：
    4 2
    1 2 100
    2 4 200
答案应该是500，然而如果初始化为0则答案会输出700。因为500的结果是又1到2，在从2返回1，再到2，再到4,100+100+100+200=500得到的；如果次短边初始化为0，则次短路径不再返回源点，而是在2与4之间折返，会偏大。
（3）53行绝对不能直接赋值，而是要swap！因为最短边被修改后，它的值是要留给次短边的。
```
代码如下：
```cpp
#include<iostream>
#include<queue>
#include<cstring> 
using namespace std;
const int INF=0x7fffffff;
const int maxn=100000+10;
struct node{
    int num,len;
    bool operator < (const node &a) const
    {
        return len>a.len;                            //优先队列强制设置为以len为关键词的小顶堆 
    }
};
int u[maxn*2],v[maxn*2],w[maxn*2];                 //依次表示每条道路上的起始点、终点和长度 
int dis[maxn/2],second[maxn/2];                    //记录通往每一个路口的最短和次短距离 
int first[maxn/2],next[maxn*2];
int n,m;
void Dijkstra()
{
    priority_queue<node>q;
    for(int i=1;i<n;i++)
      dis[i]=second[i]=INF;
    dis[0]=0;                                   //源点的最短边可以初始化为0 
    second[0]=INF;                              //但是源点的次短边必须初始化为INF，而不是0（否则会出错） 
    node temp;
    temp.num=temp.len=0;
    q.push(temp);
    while(!q.empty())
    {
        node head=q.top();
        q.pop();
        if(head.len>second[head.num])  //对于该点的最短距离大于当前该点的次短距离，则当前该点已经取到最短和次短距离，不进行操作 
          continue;
        int k=first[head.num];
        while(k!=-1)        
        {
            int d=head.len+w[k];           //否则进行两次判断 
            if(dis[v[k]]>d)                //判断一：如果小于最短距离（把最短距离赋给次短距离，再把当前距离赋给最短距离） 
            {
                swap(dis[v[k]],d);         //实际上灵活运用下面一个if判断，只要把当前距离跟最短距离进行交换即可 
                temp.len=dis[v[k]];
                temp.num=v[k];
                q.push(temp);
            }
            if(dis[v[k]]<d&&second[v[k]]>d)    //判断二：如果大于最短距离并小于次短距离 （把当前距离赋给次短距离） 
            {
                second[v[k]]=d;
                temp.len=second[v[k]];
                temp.num=v[k];
                q.push(temp);
            }
            k=next[k];                   //枚举每一条边 
        }
    }
}
int main()
{
    while(cin>>n>>m)
    {
        memset(first,-1,sizeof(first));
        for(int i=0;i<m;i++)
        {
            cin>>u[i]>>v[i]>>w[i];
            u[i]--;
            v[i]--;
            next[i]=first[u[i]];
            first[u[i]]=i;
            v[i+m]=u[i];
            u[i+m]=v[i];
            w[i+m]=w[i];
            next[i+m]=first[u[i+m]];
            first[u[i+m]]=i+m;
        }
        Dijkstra();
        cout<<second[n-1]<<endl;
    }
    return 0;
}
```
