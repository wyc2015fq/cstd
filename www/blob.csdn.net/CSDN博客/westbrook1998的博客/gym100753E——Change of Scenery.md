# gym100753E——Change of Scenery - westbrook1998的博客 - CSDN博客





2018年10月23日 18:42:48[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：22标签：[图论																[最短路条数																[Dijkstra](https://so.csdn.net/so/search/s.do?q=Dijkstra&t=blog)
个人分类：[ACM练习题](https://blog.csdn.net/westbrook1998/article/category/7652684)





求1到n是否有超过一条的最短路（another way）

读题读了半天…

双向边忘记把M开大点 re了一次
```
#include <bits/stdc++.h>
using namespace std;
const int N=1e4+50;
//边数要比题目给的范围多一倍
const int M=2*1e6+50;
const int INF=0x3f3f3f3f;
const int MOD=1e9+7;
int n,m,k;
int jun[N];
int u,v,w;
struct Edge{
    int v,w,next;
}edge[M];
struct node{
    int v,w;
    bool operator <(const node &r)const{
        return w>r.w;
    }
};
int cnt,head[N];
//记录到i的最短路条数
int pcnt[N];
int dis[N];
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
//堆优化的Dijkstra
void Dijkstra(){
    memset(dis,INF,sizeof(dis));
    memset(pcnt,0,sizeof(pcnt));
    priority_queue<node> q;
    dis[1]=0;
    pcnt[1]=1;
    q.push(node{1,0});
    node tmp;
    while(!q.empty()){
        tmp=q.top();
        q.pop();
        int u=tmp.v;
        if(tmp.w>dis[u]){
            continue;
        }
        for(int i=head[u];i!=-1;i=edge[i].next){
            int v=edge[i].v;
            int w=edge[i].w;
            if(dis[v]>dis[u]+w){
                dis[v]=dis[u]+w;
                //多加了一条u-v的边而已，所以到v的最短路条数和到u的相同
                pcnt[v]=pcnt[u];
                q.push(node{v,dis[v]});
            }
            else if(dis[v]==dis[u]+w){
                //相等的情况就多出了pcnt[u]条路了
                pcnt[v]+=pcnt[u];
            }
        }
    }
}
int main(void){
    //题目是要判断是否有多条最短路
    scanf("%d%d%d",&n,&m,&k);
    for(int i=0;i<k;i++){
        scanf("%d",&jun[i]);
    }
    init();
    for(int i=0;i<m;i++){
        scanf("%d%d%d",&u,&v,&w);
        addEdge(u,v,w);
    }
    Dijkstra();
    if(pcnt[n]>1){
        printf("yes\n");
    }
    else{
        printf("no\n");
    }
    return 0;
}
```](https://so.csdn.net/so/search/s.do?q=最短路条数&t=blog)](https://so.csdn.net/so/search/s.do?q=图论&t=blog)




