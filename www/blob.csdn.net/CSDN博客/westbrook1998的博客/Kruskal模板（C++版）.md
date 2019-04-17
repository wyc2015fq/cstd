# Kruskal模板（C++版） - westbrook1998的博客 - CSDN博客





2018年10月28日 16:55:10[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：110








hiho1098

并查集 对边权排序 贪心取边权小的边

```
#include <bits/stdc++.h>
using namespace std;
const int N=1e5+50;
const int M=1e6+50;
int n,m,u,v,w;
int p[N];
struct Edge{
    int u,v,w;
}edge[M];
int cnt;
void addEdge(int u,int v,int w){
    edge[cnt++]=Edge{u,v,w};
}
bool cmp(Edge a,Edge b){
    return a.w<b.w;
}
int find(int x){
    return p[x]==x ? x : p[x]=find(p[x]);
}
int Kruskal(){
    int ans=0;
    int c=0;
    for(int i=0;i<=n;i++){
        p[i]=i;
    }
    sort(edge,edge+cnt,cmp);
    for(int i=0;i<cnt;i++){
        int u=edge[i].u;
        int v=edge[i].v;
        int w=edge[i].w;
        int fa=find(u);
        int fb=find(v);
        if(fa!=fb){
            ans+=w;
            p[fa]=fb;
            c++;
        }
        if(c==n-1){
            break;
        }
    }
    if(c<n-1){
        //不连通
        return -1;
    }
    return ans;
}
int main(void){
    scanf("%d%d",&n,&m);
    cnt=0;
    while(m--){
        scanf("%d%d%d",&u,&v,&w);
        addEdge(u,v,w);
    }
    int ans=Kruskal();
    printf("%d\n",ans);
    return 0;
}
```



