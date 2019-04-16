# 2018ACM-ICPC Asia Nanjing Regional Contest - 紫芝的博客 - CSDN博客





2018年11月22日 19:31:28[紫芝](https://me.csdn.net/qq_40507857)阅读数：219








# [Magic Potion](https://nanti.jisuanke.com/t/33678)

网络流 二分图模型建图
- 第一个限制，左边的点`（每个英雄）`最多可以流出2的流量，必须让流入左边点的流量为2
- 第二个限制，附加流量只有K，不能让所有的附加边连接到超级源点，需要限流
**建图**
附加一个源点，超级源点和这个源点建一条容量为K的边，表示有K个瓶子，这个源点和每个英雄建一条容量为1的边，表示每个瓶子只能被一个英雄用一次

![在这里插入图片描述](https://img-blog.csdnimg.cn/20181123135312995.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQwNTA3ODU3,size_16,color_FFFFFF,t_70)
```cpp
#include <bits/stdc++.h>
#include<cstring>
#define ll long long
using namespace std;
const int MAXN = 3010;
const int MAXM = 1200001;
const int INF = 0x3f3f3f3f;
struct Edge
{
    int to,next,cap,flow,from;
}edge[MAXM];
int tot,head[MAXN];
void init()
{
    memset(head,-1,sizeof(head));
    tot =0;
}
void addEdge(int u,int v,int w,int rw = 0)
{
    edge[tot].to = v,edge[tot].from = u,edge[tot].cap = w,edge[tot].flow = 0;
    edge[tot].next = head[u];head[u] = tot++;
    edge[tot].to = u,edge[tot].from = v,edge[tot].cap = rw,edge[tot].flow = 0;
    edge[tot].next = head[v];head[v] = tot++;
}
int Q[MAXN];
int dep[MAXN],cur[MAXN],sta[MAXN];
bool bfs(int s,int t,int n)
{
    int front = 0,tail = 0;
    memset(dep,-1,sizeof(dep));
    dep[s] = 0;
    Q[tail++] = s;
    while(front < tail)
    {
        int u  =Q[front++];
        for(int i = head[u];i != -1;i=edge[i].next)
        {
            int v = edge[i].to;
            if(edge[i].cap > edge[i].flow && dep[v] == -1)
            {
                dep[v]= dep[u] + 1;
                if(v == t) return 1;
                Q[tail++] = v;
            }
        }
    }
    return 0;
}
//s为源点，t为汇点，n为最大流(s+t)
int dinic(int s,int t,int n)
{
    int maxflow = 0;
    while(bfs(s,t,n))
    {
        for(int i = 0;i<n;i++) cur[i] = head[i];
        int u = s,tail = 0;
        while(cur[s] != -1)
        {
            if(u == t)
            {
                int tp = INF;
                for(int i = tail-1;i >= 0;i--)
                {
                    tp = min(tp,edge[sta[i]].cap -edge[sta[i]].flow);
                }
                maxflow += tp;
                for(int i = tail-1;i >= 0;i--)
                {
                    edge[sta[i]].flow += tp;
                    edge[sta[i]^1].flow -= tp;
                    if(edge[sta[i]].cap -edge[sta[i]].flow == 0) tail = i;
                }
                u = edge[sta[tail]^1].to;
            }
            else if(cur[u] != -1 && edge[cur[u]].cap > edge[cur[u]].flow
                    && dep[u]+1 == dep[edge[cur[u]].to])
            {
                sta[tail++] = cur[u];
                u = edge[cur[u]].to;
            }
            else
            {
                while(u != s && cur[u] == -1)
                {
                    u = edge[sta[--tail]^1].to;
                }
                cur[u] = edge[cur[u]].next;
            }
        }
    }
    return maxflow;
}
void debug()
{
    for(int i = 0;i<tot;i += 2)
    {
        printf("%d->%d : cap = %d flow = %d\n",edge[i].from,edge[i].to,edge[i].cap,edge[i].flow);
    }
}

int main(){
    int n,m,k;
    init();
    scanf("%d%d%d",&n,&m,&k);
    int src=0;//源点
    int tar=1+n+m+1;//汇点
    int B=n+m+1;//瓶子节点，可以增加k的流量
    addEdge(0,B,k);//源点连接瓶子，最多有K瓶

    for(int i=1;i<=n;i++){
        addEdge(src,i,1);//源点连接英雄，每个英雄最多杀一个怪兽
        addEdge(B,i,1);//瓶子连接每个英雄，每个英雄最多用一瓶药
        int t,x;
        scanf("%d",&t);
        while(t--){
            scanf("%d",&x);
            addEdge(i,x+n,1);//英雄连接可以杀的怪兽
        }
    }

    for(int i=1;i<=m;i++)
        addEdge(i+n,tar,1);//每个怪兽连接汇点

    int ans=dinic(src,tar,n+m+4);
    printf("%d\n",ans);
}
```

# [Country Meow](https://nanti.jisuanke.com/t/33673)

最小球覆盖模板题
**模拟淬火算法，起始温度10w，渐进0.99就过啦**

```cpp
#include<iostream>
#include<map>
#include<string>
#include<cstring>
#include<vector>
#include<algorithm>
#include<set>
#include<sstream>
#include<cstdio>
#include<cmath>
#include<climits>
#include<cstdlib>
using namespace std;
const double eps=1e-3;
//POJ2069 最小球覆盖
struct POINT{
    double x,y,z;
}p[110];//N个点
POINT op;//最小球的球心
int n;
inline double dist(POINT &a,POINT &b){//两点距离
    return sqrt((a.x-b.x)*(a.x-b.x)+(a.y-b.y)*(a.y-b.y)+(a.z-b.z)*(a.z-b.z));
}
double solve(){
    double ret,delta=100000.0;//温度
    double maxDis,tempDis;
    int i,id;
    while(delta>eps){
        id=0;
        maxDis=dist(op,p[id]);
        for(i=1;i<n;i++){
            tempDis=dist(op,p[i]);
            if(tempDis>maxDis){
                maxDis=tempDis;
                id=i;
            }
        }
        ret=maxDis;
        op.x+=(p[id].x-op.x)/maxDis*delta;
        op.y+=(p[id].y-op.y)/maxDis*delta;
        op.z+=(p[id].z-op.z)/maxDis*delta;
        delta*=0.999;
    }
    return ret;//最小球半径
}
int main(){
    while(scanf("%d",&n)!=EOF&&n){
        for(int i=0;i<n;i++){
            scanf("%lf%lf%lf",&p[i].x,&p[i].y,&p[i].z);
        }
        printf("%lf\n", solve());
    }
    return 0;
}
```

# [Prime Game](https://nanti.jisuanke.com/t/33679)

[参考博客](https://blog.csdn.net/ftx456789/article/details/83116668)

```cpp
#include <iostream>
#include <cstdio>
#include <cstring>
#include <algorithm>
#include <math.h>
#include <stack>
#include <list>
#include<bits/stdc++.h>
#define MAX 1000005//求MAX范围内的素数
using namespace std;
const int N=1e6+5;
long long su[MAX],cnt;
bool isprime[MAX];
void prime()
{
    cnt=1;
    memset(isprime,1,sizeof(isprime));//初始化认为所有数都为素数
    isprime[0]=isprime[1]=0;//0和1不是素数
    for(long long i=2; i<MAX; i++)
    {
        if(isprime[i])
            su[cnt++]=i;//保存素数i
        for(long long j=1; j<cnt&&su[j]*i<MAX; j++)
        {
            isprime[su[j]*i]=0;//筛掉小于等于i的素数和i的积构成的合数
        }
    }
}
int a[N];
vector<long long>mp[MAX];
void init(int x,int pos)
{
    for(int i=1; su[i]*su[i]<=x; i++)
    {
        if(x%su[i]==0)
        {
            mp[su[i]].push_back(pos);
            while(x%su[i]==0)
                x/=su[i];
        }
    }
    if(x>1)
    {
        mp[x].push_back(pos);
    }
}
int main()
{
    prime();
    long long n;
    while(scanf("%lld",&n)!=EOF)
    {
        for(int i=1;i<cnt;i++)
            mp[su[i]].clear();
        for(int i=1; i<=n; i++)
        {
            scanf("%d",&a[i]);
            init(a[i],i);
        }
        long long ans=0;
        for(int i=1; i<cnt; i++)
        {
            if(mp[su[i]].size()==0)
                continue;
            else
            {
                ans+=mp[su[i]][0]*(n-mp[su[i]][0]+1);
                for(int j=1; j<mp[su[i]].size(); j++)
             ans+=(mp[su[i]][j]-mp[su[i]][j-1])*(n-mp[su[i]][j]+1);
            }
        }
        printf("%lld\n",ans);
    }
    return 0;
}
```

[线性筛模板](https://blog.csdn.net/qq_40038776/article/details/82286870)

```cpp
//Mark数组存该下标是否为素数，值为0为素数，值为1为合数
int Mark[Max];  
int prime[Max];  
  
//判断是否是一个素数  Mark 标记数组 index 素数个数  
int Prime(){  
    int index = 0;  
    memset(Mark,0,sizeof(Mark));  
    for(int i = 2; i < Max; i++){  
        //如果未标记则得到一个素数  
        if(Mark[i] == 0){  
            prime[index++] = i;  
        }  
        //标记目前得到的素数的i倍为非素数  
        for(int j=0; j<index && prime[j]*i < Max; j++){  
            Mark[i * prime[j]] = 1;  
            if(i % prime[j] == 0){  
                break;  
            }  
        }  
    }  
    return index;  //返回值为素数个数，其实不太需要。。
}
```




