# gym100269D——Dwarf Tower - westbrook1998的博客 - CSDN博客





2018年10月23日 22:57:44[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：23标签：[图论																[最短路](https://so.csdn.net/so/search/s.do?q=最短路&t=blog)](https://so.csdn.net/so/search/s.do?q=图论&t=blog)
个人分类：[ACM练习题](https://blog.csdn.net/westbrook1998/article/category/7652684)








感觉很经典的题目，虽然我还是不会做

给n个物品可以直接买，需要花费dis[i]，然后再给m个合成方法，两个其他的可以合成一个另外其他的，求得到物品1的最小的花费

最短路的题目，然而一直想不通怎么将合成方法转成边，看了题解都是用了spfa，然后边也是很奇怪
代码

```
#include <bits/stdc++.h>
using namespace std;
const int N=1e4+50;
const int M=1e6+50;
int dis[N];
struct Edge{
    int x,y,next;
}edge[M];
int cnt;
int head[N];
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
int a,b,c;
int inq[N];
int spfa(){
    queue<int> q;
    for(int i=1;i<=n;i++){
        q.push(i);
        inq[i]=1;
    }
    while(!q.empty()){
        int now=q.front();
        q.pop();
        inq[now]=0;
        for(int i=head[now];i!=-1;i=edge[i].next){
            int v=edge[i].x;
            //这里u其实是以前的权值，这里代表now和v可以合成的
            int u=edge[i].y;
            //松弛
            if(dis[v]+dis[now]<dis[u]){
                dis[u]=dis[v]+dis[now];
                if(!inq[u]){
                    inq[u]=1;
                    q.push(u);
                }
            }
        }
    }
    return dis[1];
}
int main(void){
    freopen("dwarf.in","r",stdin);
    freopen("dwarf.out","w",stdout);
    init();
    scanf("%d%d",&n,&m);
    for(int i=1;i<=n;i++){
        scanf("%d",&dis[i]);
    }
    while(m--){
        scanf("%d%d%d",&a,&b,&c);
        //最关键的建边，将合成的b和c作为边的端点，边的权值不再是单纯的权值
        //而是代表了能够合成的边，这样在松弛的时候需要不同的处理
        addEdge(b,c,a);
    }
    int ans=spfa();
    printf("%d\n",ans);
    return 0;
}
```





