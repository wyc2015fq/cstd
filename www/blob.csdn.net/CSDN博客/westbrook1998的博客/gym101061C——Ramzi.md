# gym101061C——Ramzi - westbrook1998的博客 - CSDN博客





2018年10月24日 20:32:09[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：19标签：[最短路																[Dijkstra](https://so.csdn.net/so/search/s.do?q=Dijkstra&t=blog)](https://so.csdn.net/so/search/s.do?q=最短路&t=blog)
个人分类：[ACM练习题](https://blog.csdn.net/westbrook1998/article/category/7652684)








这几天做的第二道这种路的类型不一样，分策略进行松弛的最短路题，这道和上一道就是一模一样

给出m条路，有car的walk的，要使walk尽量小的情况下再使总路途最小

核心就是Dijk的队列节点要维护两个权值，然后优先级是walk的优先，然后松弛的时候要分情况
```
#include <bits/stdc++.h>
using namespace std;
#define bug printf("BUG\n")
const int N=4050;
const int M=80050;
const int INF=0x3f3f3f3f;
struct Edge{
    int v,w,next;
    bool isWalk;
}edge[M];
int cnt;
int head[N];
void init(){
    cnt=0;
    memset(head,-1,sizeof(head));
}
void addEdge(int u,int v,int w,int ch){
    bool isWalk;
    if(ch==1){
        isWalk=true;
    }
    else{
        isWalk=false;
    }
    edge[cnt]=Edge{v,w,head[u],isWalk};
    head[u]=cnt++;
    edge[cnt]=Edge{u,w,head[v],isWalk};
    head[v]=cnt++;
}
int n,m,p;
int u,v,w;
int s,t;
int isWalk;
int walk[N],sum[N];
bool vis[N];
struct node{
    int v,o,s;
    bool operator <(const node &r)const{
        //先优先walk小的
        if(o!=r.o){
            return o>r.o;
        }
        return s>r.s;
    }
};
void Dijkstra(int s){
    for(int i=1;i<=n;i++){
        vis[i]=false;
        walk[i]=INF;
        sum[i]=INF;
    }
    walk[s]=0;
    sum[s]=0;
    priority_queue<node> q;
    q.push(node{s,0,0});
    node tmp;
    while(!q.empty()){
        tmp=q.top();
        q.pop();
        int u=tmp.v;
        if(vis[u]){
            continue;
        }
        vis[u]=true;
        for(int i=head[u];i!=-1;i=edge[i].next){
            int v=edge[i].v;
            int w=edge[i].w;
            bool flag=edge[i].isWalk;
            //这一段不是步行的
            if(!flag){
            	//步行的dis需要更新，总的也要更新
                if(walk[v]>walk[u]){
                    walk[v]=walk[u];
                    sum[v]=sum[u]+w;
                }
                //不行不需要更新，总的更新
                else if(walk[v]==walk[u] && sum[v]>sum[u]+w){
                    sum[v]=sum[u]+w;
                }
            }
            //步行的路
            else{
            	//注意这里要加上w，因为这一段是步行的
            	//步行和总dis都要更新
                if(walk[v]>walk[u]+w){
                    walk[v]=walk[u]+w;
                    sum[v]=sum[u]+w;
                }
                //只更新总dis
                else if(walk[v]==walk[u]+w && sum[v]>sum[u]+w){
                    sum[v]=sum[u]+w;
                }
            }
            q.push(node{v,walk[v],sum[v]});
        }
    }
}
int T;
int main(void){
    scanf("%d",&T);
    while(T--){
        scanf("%d%d",&n,&m);
        init();
        for(int i=0;i<m;i++){
            scanf("%d%d%d%d",&u,&v,&w,&isWalk);
            addEdge(u,v,w,isWalk);
        }
        scanf("%d%d",&s,&t);
        memset(walk,0,sizeof(walk));
        memset(sum,0,sizeof(sum));
        Dijkstra(s);
        if(sum[t]!=INF){
            printf("%d %d\n",walk[t],sum[t]);
        }
        else{
            printf("-1\n");
        }
    }
    return 0;
}
```





