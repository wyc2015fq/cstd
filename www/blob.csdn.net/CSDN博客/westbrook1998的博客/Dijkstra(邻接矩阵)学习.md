# Dijkstra(邻接矩阵)学习 - westbrook1998的博客 - CSDN博客





2018年05月11日 15:49:17[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：95








```cpp
#include <cstdio>
#include <cstring>
#include <stack>
using namespace std;
const int MAXN=1010;
const int INF=0x3f3f3f3f;
bool vis[MAXN];
//pre数组记录路径
int pre[MAXN];
//邻接表记录边权
int cost[MAXN][MAXN];
//最短路值数组
int lowcost[MAXN];
//节点数
int n;
//边数
int m;
//边的端点，权值
int u,v,c;
//起点和终点
int s,e;
void Dijkstra(int cost[][MAXN],int lowcost[],int n,int s){
    //初始化
    for(int i=1;i<=n;i++){
        lowcost[i]=INF;
        vis[i]=false;
        pre[i]=-1;
    }
    //注意这个，如果lowcost全都初始化为INF，后面就等于没用了
    //这里要将起点lowcost设为0
    lowcost[s]=0;
    for(int i=1;i<=n;i++){
        int k=-1;
        int Min=INF;
        //i到n循环，每次找出未访问过的lowcost最小的一个节点
        for(int j=1;j<=n;j++){
            if(!vis[j] && lowcost[j]<Min){
                Min=lowcost[j];
                k=j;
            }
        }
        printf("k:%d\n",k);
        //找不到，退出，结束函数
        if(k==-1){
            break;
        }
        vis[k]=true;
        //以该节点出发，遍历有相连的节点（如果没有相连，cost是为INF，加起来不可能lowcost[j]小）
        for(int j=1;j<=n;j++){
            //如果以该节点（k）出发的最短路加上k，j之间的边权小于以j出发的最短路，那么最短路就要加上这个k
            if(!vis[j] && lowcost[k]+cost[k][j]<lowcost[j]){
                lowcost[j]=lowcost[k]+cost[k][j];
                pre[j]=k;
            }
        }
        for(int i=1;i<=n;i++){
            printf("%d ",lowcost[i]);
        }
        printf("\n");
    }
}
int main(void){
    /*
        测试数据
        5 7
        1 2 1
        1 3 3
        2 4 2
        3 4 5
        3 5 4
        4 5 6
        1 5 20  
     */
    scanf("%d%d",&n,&m);
    s=1;
    e=5;
    //注意要把cost初始化为INF
    memset(cost,INF,sizeof(cost));
    for(int i=0;i<m;i++){
        scanf("%d%d%d",&u,&v,&c);
        cost[u][v]=c;
        cost[v][u]=c;
    }
    /*
        cost数组
          1  2  3  4  5
        1    1  3     20
        2 1        2
        3 3        5  4
        4    2  5     6
        5 20    4  6
    */
    Dijkstra(cost,lowcost,n,s);
    printf("%d到%d的最短路权值和为：%d\n",s,e,lowcost[e]);
    stack<int> path;
    int l=e;
    while(l!=-1){
        path.push(l);
        l=pre[l];
    }
    printf("%d到%d的最小权值路径：",s,e);
    while(!path.empty()){
        printf("%d ",path.top());
        path.pop();
    }
    printf("\n");
    return 0;
}
```



