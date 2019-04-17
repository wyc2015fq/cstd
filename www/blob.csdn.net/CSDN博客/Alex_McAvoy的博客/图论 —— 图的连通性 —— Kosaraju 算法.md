# 图论 —— 图的连通性 —— Kosaraju 算法 - Alex_McAvoy的博客 - CSDN博客





2019年01月30日 14:54:57[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：43








# 【概述】

Kosaraju 算法是最容易理解，最通用的求强连通分量的算法，其关键的部分是同时应用了原图 G 和反图 GT 。

# 【基本思想】

1.对原图 G 进行 DFS 搜索，计算出各顶点完成搜索的时间 f

2.计算图的反图 GT，对反图也进行 DFS 搜索，但此处搜索时顶点的访问次序不是按照顶点标号的大小，而是按照各顶点 f 值由大到小的顺序

3.反图 DFS 所得到的森林即对应连通区域。

原图

![](https://img-blog.csdn.net/20180720162508611?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

原图进行 DFS

![](https://img-blog.csdn.net/20180720162515970?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

反图

![](https://img-blog.csdn.net/20180720162536296?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

反图进行 DFS

![](https://img-blog.csdn.net/20180720162550837?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

上面提及原图 G 的反图 GT，其定义为 GT=(V,ET)，ET={(u,v):(v,u)∈E}

也就是说 GT 是由 G 中的边反向所组成的，通常也称之为图 G 的转置。

值得一提的是，逆图 GT 和原图 G 有着完全相同的连通分支，也就说，如果顶点 s 和 t 在 G 中是互达的，当且仅当 s 和 t 在 GT 中也是互达的。

# 【伪代码】

step1：对原图 G 进行 DFS 遍历，记录每个节点的离开时间 num[i]

step2：选择具有最晚离开时间的顶点，对反图 GT 进行遍历，删除能够遍历到的顶点，这些顶点构成一个强连通分量

step3：如果还有顶点没有删除，继续 step2，否则算法结束

# 【实现】

```cpp
#include<iostream>
#include<cstring>
#include<algorithm>
#include<vector>
using namespace std;
int n,m;
vector<int> g[N];
vector<int> gt[N];
bool vis[N];//访问标志数组
int color[N];//存储强连通分量,color[i]表示顶点i属于第color[i]个强连通分量
int num[N];//结束时间标记,num[i]表示离开时间为i的顶点
int sig;//强连通分量个数
void dfs1(int x){//第一次深搜,求numb[1..n]的值
    vis[x]=true;
    for(int i=0;i<g[x].size();i++){
        int y=g[x][i];
        if(vis[y]==false)
            dfs1(y);
    }
    num[sig++]=x;
}
void dfs2(int x){//第二次深搜,求color[1..n]的值
    vis[x]=true;
    color[x]=sig;
    for(int i=0;i<gt[x].size();i++){
        int y=gt[x][i];
        if(vis[y]==false)
            dfs2(y);
    }
}
int Kosaraju()//kosaraju算法
{    
    /*第一次深搜*/
    sig=1;
    memset(vis,0,sizeof(vis));
    for(int i=1;i<=n;i++)
        if(vis[i]==false)
            dfs1(i);
        
    /*第二次深搜*/
    sig=0;
    memset(vis,0,sizeof(vis));
    for(int i=n;i>=1;i--){
        if(vis[num[i]]==0){
            sig++;
            dfs2(num[i]);
        }
    }
}
int main(){
    while(scanf("%d%d",&n,&m)!=EOF){
        for(int i=1;i<=n;i++){
            g[i].clear();
            gt[i].clear();
        }
        for(int i=0;i<m;i++){
            int x,y;
            scanf("%d%d",&x,&y);
            g[x].push_back(y);
            gt[y].push_back(x);
        }
        Kosaraju();
        printf("%d",sig);
    }
    return 0;
}
```



