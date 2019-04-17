# 最小树形图——朱刘算法(Edmonds) - 在思索中前行！ - CSDN博客





2017年03月14日 16:47:47[_Tham](https://me.csdn.net/txl16211)阅读数：3438








定义：一个有向图，存在从某个点为根的，可以到达所有点的一个最小生成树，则它就是最小树形图。




朱刘[算法](http://lib.csdn.net/base/datastructure)实现过程： 【在选出入边集后（看步骤1），若有向图中不存在有向环，说明该图就是最小树形图】




1，选入边集——找到除root点之外，每一个点的所有入边中权值最小的，用数组in[]记录下这个最小权值，用pre[]记录到达该点的前驱；（若图中存在独立点，最小树形图是不存在的，所以在该步骤结束后，要判断一下）

2，找有向环，并用数组id[]记录节点所属环的编号。

3，找到环后，缩点，并更新权值。（感觉和SCC缩点差不多）

4，以环数为下一次查找的点数，继续执行上述操作，直到没有环 或者 判定出不存在最小树形图为止。




给个图：




![](https://img-blog.csdn.net/20150826092343443?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





详看代码，有详细注释：点的编号是从0开始的



```cpp
/* 
最小树形图 
朱刘算法模板 
时间复杂度O(nm) 
数据为int型 
*/  
#include <cstdio>  
#include <cstring>  
#include <algorithm>  
#define MAXN 1010  
#define MAXM 1000000+10  
#define INF 0x3f3f3f3f  
using namespace std;  
struct Edge  
{  
    int from, to, cost;  
};  
Edge edge[MAXM];  
int pre[MAXN];//存储父节点  
int vis[MAXN];//标记作用  
int id[MAXN];//id[i]记录节点i所在环的编号  
int in[MAXN];//in[i]记录i入边中最小的权值  
int zhuliu(int root, int n, int m, Edge *edge)//root根 n点数 m边数  
{  
    int res = 0, u, v;  
    while(1)  
    {  
        for(int i = 0; i < n; i++)  
            in[i] = INF;//初始化  
        for(int i = 0; i < m; i++)  
        {  
            Edge E = edge[i];  
            if(E.from != E.to && E.cost < in[E.to])  
            {  
                pre[E.to] = E.from;//记录前驱  
                in[E.to] = E.cost;//更新  
            }  
        }  
        for(int i = 0; i < n; i++)  
            if(i != root && in[i] == INF)  
                return -1;//有其他孤立点 则不存在最小树形图  
        //找有向环  
        int tn = 0;//记录当前查找中 环的总数  
        memset(id, -1, sizeof(id));  
        memset(vis, -1, sizeof(vis));  
        in[root] = 0;//根  
        for(int i = 0; i < n; i++)  
        {  
            res += in[i];//累加  
            v = i;  
            //找图中的有向环 三种情况会终止while循环  
            //1,直到出现带有同样标记的点说明成环  
            //2,节点已经属于其他环  
            //3,遍历到根  
            while(vis[v] != i && id[v] == -1 && v != root)  
            {  
                vis[v] = i;//标记  
                v = pre[v];//一直向上找  
            }  
            //因为找到某节点属于其他环  或者 遍历到根  说明当前没有找到有向环  
            if(v != root && id[v] == -1)//必须上述查找已经找到有向环  
            {  
                for(int u = pre[v]; u != v; u = pre[u])  
                    id[u] = tn;//记录节点所属的 环编号  
                id[v] = tn++;//记录节点所属的 环编号  环编号累加  
            }  
        }  
        if(tn == 0) break;//不存在有向环  
        //可能存在独立点  
        for(int i = 0; i < n; i++)  
            if(id[i] == -1)  
                id[i] = tn++;//环数累加  
        //对有向环缩点  和SCC缩点很像吧  
        for(int i = 0; i < m; i++)  
        {  
            v = edge[i].to;  
            edge[i].from = id[edge[i].from];  
            edge[i].to = id[edge[i].to];  
            //<u, v>有向边   
            //两点不在同一个环 u到v的距离为 边权cost - in[v]  
            if(edge[i].from != edge[i].to)  
                edge[i].cost -= in[v];//更新边权值 继续下一条边的判定  
        }  
        n = tn;//以环总数为下次操作的点数 继续执行上述操作 直到没有环  
        root = id[root];  
    }  
    return res;  
}  
int main()  
{  
    int N, M;//N个点 M条有向边  
    while(scanf("%d%d", &N, &M) != EOF)  
    {  
        getMap();//建图  注意去除自环  自己到自己的权值为无穷大  
        int ans = zhuliu(0, N, M, edge);  
        if(ans == -1)  
            printf("-1\n");//不存在  
        else  
            printf("%d\n", ans);  
    }  
    return 0;  
}
```








