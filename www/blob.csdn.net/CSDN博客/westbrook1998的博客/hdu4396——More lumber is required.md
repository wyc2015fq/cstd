# hdu4396——More lumber is required - westbrook1998的博客 - CSDN博客





2018年11月11日 15:40:21[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：26标签：[bfs																[最短路](https://so.csdn.net/so/search/s.do?q=最短路&t=blog)](https://so.csdn.net/so/search/s.do?q=bfs&t=blog)
个人分类：[ACM练习题](https://blog.csdn.net/westbrook1998/article/category/7652684)








题目比较好懂的，给个无向图，每条边有个距离，而且走过每条边会获得10个东西，从s出发到t，求要得到k个东西的最短路是多少

如果没这个要拿k个东西的条件，那就是一个简单的最短路，然后这题数据不是很大，可以用bfs来求最短路，当s==t && tk>=k就结束，而与普通的bfs不同的就是我们还要加上一个松弛操作

其中dp[i][j]表示节点i获得j个东西的最短路
```
if(dp[v][tk+1]>dp[tp][tk]+edge[i].w){
                dp[v][tk+1]=dp[tp][tk]+edge[i].w;
                q.push(node{v,tk+1,dp[v][tk+1]});
            }
```

代码：

```
#include <bits/stdc++.h>
using namespace std;
const int M=200050;
const int N=5050;
const int INF=0x3f3f3f3f;
struct Edge{
    int v,w,next;
}edge[M];
int cnt,head[N];
void init(){
    cnt=0;
    memset(head,-1,sizeof(head));
}
void addEdge(int u,int v,int w){
    edge[cnt]=Edge{v,w,head[u]};
    head[u]=cnt++;
    edge[cnt]=Edge{u,w,head[v]};
    head[v]=cnt++;
}
int n,m;
int u,v,w;
int s,t,k;
struct node{
    int pp,kk,cc;

    friend bool operator <(node a,node b){
        return a.cc>b.cc;
    }
}tmp;
//dp[i][j]表示i点得到j*10个的最短路
int dp[N][60];
int bfs(){
    memset(dp,INF,sizeof(dp));
    priority_queue<node> q;
    q.push(node{s,0,0});
    dp[s][0]=0;
    while(!q.empty()){
        tmp=q.top();
        q.pop();
        int tp=tmp.pp;
        int tk=tmp.kk;
        int tc=tmp.cc;
        if(tp==t && tk>=k){
            return tc;
        }
        for(int i=head[tp];i!=-1;i=edge[i].next){
            int v=edge[i].v;
            if(dp[v][tk+1]>dp[tp][tk]+edge[i].w){
                dp[v][tk+1]=dp[tp][tk]+edge[i].w;
                q.push(node{v,tk+1,dp[v][tk+1]});
            }
        }
    }
    return -1;
}
int main(void){
    while(~scanf("%d%d",&n,&m)){
        init();
        while(m--){
            scanf("%d%d%d",&u,&v,&w);
            addEdge(u,v,w);
        }
        scanf("%d%d%d",&s,&t,&k);
        if(k%10){
            k=k/10+1;
        }else{
            k=k/10;
        }
        int ans=bfs();
        printf("%d\n",ans);
    }
    return 0;
}
```





