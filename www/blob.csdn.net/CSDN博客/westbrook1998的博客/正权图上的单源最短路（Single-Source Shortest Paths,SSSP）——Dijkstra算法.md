# 正权图上的单源最短路（Single-Source Shortest Paths,SSSP）——Dijkstra算法 - westbrook1998的博客 - CSDN博客





2018年05月07日 22:38:55[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：99








```cpp
#include <cstdio>
#include <vector>
#include <queue>
#include <cstring>
#include <iostream>
using namespace std;
const int maxn=1000007;
const int INF=1000008;
/**
 * 使用邻接表
*/
//节点个数n和边个数m
int n,m;
//保存节点u的第一条边的编号
int first[maxn];
//保存边的起点，终点，边权
//nxt数组保存编号e的边的下一条边（同一个节点多条边）
int u[maxn],v[maxn],w[maxn],nxt[maxn];
//读取图
void read_graph(){
    scanf("%d%d",&n,&m);
    //初始化节点，暂时没有边，所以为-1
    //或者说没有出度，这个样例是有向图
    for(int i=0;i<n;i++){
        first[i]=-1;
    }
    //读取边
    for(int e=0;e<m;e++){
        scanf("%d%d%d",&u[e],&v[e],&w[e]);
        //新的边添加在链表的首部而不是尾部，免去了对链表的遍历
        //把之前的第一条边作为现在第一条边的下一条边
        nxt[e]=first[u[e]];
        //把新的边作为该节点的的第一条边，相当于插入到链表的首部
        first[u[e]]=e;
    }
}
//测试方法 打印数组
void print_array(int* arr,int len){
    for(int i=0;i<len;i++){
        printf("%d ",arr[i]);
    }
    printf("\n");
}
/**
 * 更简单是使用vector数组封装数据结构
*/
struct Edge{
    //边的起点，终点，边权
    int from,to,dist;
    Edge(int u,int v,int d):from(u),to(v),dist(d){}
};
//封装一个结构体作为优先队列的元素
struct HeapNode{
    //边权d 节点编号u
    int d,u;
    //重载 < 操作符 使得优先队列优先取出最小值
    bool operator < (const HeapNode& rhs) const {
        //而且是根据边权的大小比较
        return d>rhs.d;
    }
};
struct D{
    int n,m;
    //边的vector
    vector<Edge> edges;
    //G数组每个值都是一个vector，用来保存每个节点的所有边
    vector<int> G[maxn];
    //标记数组
    bool done[maxn];
    //s到各个点的距离
    int d[maxn];
    //最短路中的一条弧
    int p[maxn];
};
struct Dijkstra{
    //节点个数，边的个数
    int n,m;
    //边的vector
    vector<Edge> edges;
    //G数组每个值都是一个vector，用来保存每个节点的所有边
    vector<int> G[maxn];
    //标记数组
    bool done[maxn];
    //s到各个点的距离
    int d[maxn];
    //最短路中的一条弧
    int p[maxn];

    //初始化
    void init(int n){
        this->n=n;
        for(int i=0;i<n;i++){
            G[i].clear();
        }
        edges.clear();
    }

    //添加一条边
    void AddEdge(int from,int to,int dist){
        //新建一条边加入vector
        edges.push_back(Edge(from,to,dist));
        //获取边的数量
        m=edges.size();
        //保存以from节点为起点的边（保存编号 m-1）
        G[from].push_back(m-1);    
    }

    void dijkstra(int s){
        //构造优先队列
        priority_queue<HeapNode> Q;
        //初始化，d[i]表示s到i的最短路权值和
        for(int i=0;i<n;i++){
            d[i]=INF;
        }
        d[s]=0;
        memset(done,0,sizeof(done));
        //将第一个元素压入栈中
        Q.push((HeapNode){0,s});
        //有点类似bfs，队列非空即取出元素
        while(!Q.empty()){
            HeapNode x=Q.top();
            Q.pop();
            //当前节点
            int u=x.u;
            //已访问
            if(done[u]){
                continue;
            }
            //未访问，标记访问
            done[u]=true;
            //依次遍历该节点所连的所有边（边的编号保存在G[u]这个vector里）
            for(int i=0;i<G[u].size();i++){
                //取出一条边的引用
                Edge& e=edges[G[u][i]];
                //相当于d[e.to]=min(d[e.to],d[u]+e.dist);
                if(d[e.to]>d[u]+e.dist){
                    d[e.to]=d[u]+e.dist;
                    //保存路径
                    p[e.to]=G[u][i];
                    //压入路径的下一个节点元素
                    Q.push((HeapNode){d[e.to],e.to});
                }
            }
        }
    }
};
int main(void){
    //read_graph();
    //print_array(first,n);
    //print_array(nxt,m);
    //结构体指针
    Dijkstra* d=(Dijkstra*)malloc(sizeof(Dijkstra));
    //初始化5个节点
    d->init(5);
    //添加边
    d->AddEdge(0,2,3);
    d->AddEdge(2,1,4);
    d->AddEdge(1,3,3);
    d->AddEdge(3,2,2);
    d->AddEdge(3,4,1);
    //定义起点节点和终点节点
    int s=0,e=4;
    //执行dijkstra算法求s到任何节点的最短路边权和
    d->dijkstra(s);
    //打印起点到终点的最短路边权和
    printf("%d\n",d->d[e]);
    //将最短路路径放入vector中，再反向打印出路径
    int pos=e;
    vector<int> path;
    path.push_back(d->edges[d->p[e]].to);
    while(pos!=s){
        path.push_back(d->edges[d->p[pos]].from);
        pos=d->edges[d->p[pos]].from;
    }
    for(int i=path.size()-1;i>=0;i--){
        printf("%d",path[i]);
        if(i!=0){
            printf("->");
        }
    }
    printf("\n");
    return 0;
}
```



