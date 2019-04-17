# 最小生成树prim和kruskal模板 - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)





prim：

```
int cost[MAX_V][MAX_V]; //cost[u][v]表示边e=(u,v)的权值(不存在的情况下设为INF)
int mincost[MAX_V];     //从集合X出发的每个变得最小权值
bool used[MAX_V];       //顶点i是包含在集合X中
int V;                  //顶点数
int prim()
{
    int res = 0;
    for(int i=0;i < V;i++)
    {
        mincost[i] = INF;
        used[i] = false;
    }
    mincost[0] = 0;
    while(1)
    {
        int v = -1;
        for(int u=0;u < V;u++) //从点集外找权值最小的路径
        {
            if(!used[u]&&(v == -1 || mincost[u] < mincost[v])) //因为c++从左到右所以这样写不会越界
                v = u;
        }
        if(v == -1) break;
        used[v] = true;
        res += mincost[v];       //把每次的权值加上去
        for(int u=0;u < V;u++)  //因为新把 v 加入点集，因此Mincost[]需要重新计算
        {
            mincost[u] = min(mincost[u],cost[v][u]);
        }
    }
    return res;
}
```

kruskal + 并查集:

```
const int MAX_V = 510;


struct edge
{
    int u;
    int v;
    int cost;
};

bool comp(const edge &e1,const edge &e2)
{
    return e1.cost < e2.cost;
}

edge es[MAX_V];
int V,E;             //顶点数和边数

int kruskal()
{
    sort(es,es+E,comp); //将边按照cost的大小顺序排序
    init_union_find(V); //并查集初始化
    int res = 0;
    for(int i=0;i < E;i++)
    {
        edge e = es[i];
        if(!same(e.u,e.v))  //如果一条边的两个端点不在同一个集合之内则合并两个点集
        {
            unite(e.u,e.v);
            res += e.cost;
        }
        
    }
    return res;
}
```

——











