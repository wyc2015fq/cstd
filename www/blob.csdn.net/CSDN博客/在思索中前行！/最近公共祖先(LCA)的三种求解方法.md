# 最近公共祖先(LCA)的三种求解方法 - 在思索中前行！ - CSDN博客





2017年03月21日 15:31:47[_Tham](https://me.csdn.net/txl16211)阅读数：475










## 转载来自：https://blog.andrewei.info/2015/10/08/e6-9c-80-e8-bf-91-e5-85-ac-e5-85-b1-e7-a5-96-e5-85-88lca-e7-9a-84-e4-b8-89-e7-a7-8d-e6-b1-82-e8-a7-a3-e6-96-b9-e6-b3-95/

## 简述




LCA（Least Common Ancestors），即最近公共祖先，是指这样一个问题：在有根树中，找出某两个结点 u 和 v 最近的公共祖先（另一种说法，离树根最远的公共祖先）。

## [](https://blog.andrewei.info/2015/10/08/e6-9c-80-e8-bf-91-e5-85-ac-e5-85-b1-e7-a5-96-e5-85-88lca-e7-9a-84-e4-b8-89-e7-a7-8d-e6-b1-82-e8-a7-a3-e6-96-b9-e6-b3-95/#%E7%AE%97%E6%B3%95)算法

### [](https://blog.andrewei.info/2015/10/08/e6-9c-80-e8-bf-91-e5-85-ac-e5-85-b1-e7-a5-96-e5-85-88lca-e7-9a-84-e4-b8-89-e7-a7-8d-e6-b1-82-e8-a7-a3-e6-96-b9-e6-b3-95/#RMQ-ST-%E5%9C%A8%E7%BA%BF%E7%AE%97%E6%B3%95)RMQ_ST 在线算法

#### [](https://blog.andrewei.info/2015/10/08/e6-9c-80-e8-bf-91-e5-85-ac-e5-85-b1-e7-a5-96-e5-85-88lca-e7-9a-84-e4-b8-89-e7-a7-8d-e6-b1-82-e8-a7-a3-e6-96-b9-e6-b3-95/#%E7%AE%80%E4%BB%8B)**简介**


RMQ（Range Minimum/Maximum Query）, 即区间最值查询, 是指这样一个问题: 对于长度为 n 的数列 A，回答若干询问 RMQ（A, i, j）(i, j <= n), 返回数列A中下标在i，j之间的最小/大值。所谓在线算法，是指用户每输入一个查询便马上处理一个查询。该算法一般用较长的时间做预处理，待信息充足以后便可以用较少的时间回答每个查询。ST（Sparse Table）算法是一个非常有名的在线处理RMQ问题的算法，**它可以在 O(nlogn) 时间内进行预处理，然后在 O(1) 时间内回答每个查询**。

#### [](https://blog.andrewei.info/2015/10/08/e6-9c-80-e8-bf-91-e5-85-ac-e5-85-b1-e7-a5-96-e5-85-88lca-e7-9a-84-e4-b8-89-e7-a7-8d-e6-b1-82-e8-a7-a3-e6-96-b9-e6-b3-95/#%E7%AE%97%E6%B3%95%E8%BF%87%E7%A8%8B)**算法过程**


首先是预处理，用动态规划（DP）解决。设 A[i] 是要求区间最值的数列， dp[i, j] 表示从第 i 个数起连续 2^j 个数中的最大值。例如数列 3 2 4 5 6 8 1 2 9 7 ，dp[1，0] 表示第 1 个数起，长度为 2 ^ 0 = 1 的最大值，其实就是 3 这个数。 dp[1, 2] = 5, dp[1, 3] = 8, dp[2, 0] = 2 , dp[2, 1] = 4 ……从这里可以看出 dp[i, 0] 其实就等于 A[i] 。这样，DP 的状态、初值都已经有了，剩下的就是状态转移方程。我们把
 dp[i, j] 平均分成两段（因为 dp[i, j] 一定是偶数个数字）, 从 i 到为一段， 到  为一段(长度都为 )
 。用上例说明，当 i = 1，j = 3 时就是 3,2,4,5 和 6,8,1,2 这两段。dp[i，j] 就是这两段的最大值中的最大值。于是我们得到了动态规划方程



然后是查询。取 



则有



举例说明，要求区间 [2，8] 的最大值，就要把它分成 [2, 5] 和 [5, 8] 两个区间，因为这两个区间的最大值我们可以直接由 dp[2, 2] 和 dp[5, 2] 得到。




参考代码：




```cpp
#include <cstdio>
const int MAXN = 100010;
int n, q;
int num[MAXN];
int dp[MAXN][20];
void ST(){
    for(int i = 1; i <= n; i++)
        dp[i][0] = num[i];
    for(int j = 1; j < 20; j++)
        for(int i = 1; i + (1 << j) - 1 <= n; i++)
            dp[i][j] = max(dp[i][j - 1], dp[i + (1 << (j - 1))][j - 1]);
}
int RMQ(int l, int r){
    if(l > r) return 0;
    int k = log((double)(r - l + 1)) / log(2.0);
    return max(dp[l][k], dp[r - (1 << k) + 1][k]);
}
int main(){
    while(scanf("%d%d", &n, &q) != EOF){
        for(int i = 1; i <= n; i++)
            scanf("%d", &num[i]);
        ST();
        int l, r;
        for(int i = 1; i <= q; i ++){
            scanf("%d%d", &l, &r);
            printf("%d\n", RMQ(l, r));
        }
    }
    return 0;
}
```



**利用 RMQ_ST 算法求解 LCA 问题**


思想是: 将树看成一个无向图，u 和 v 的公共祖先一定在 u 与 v 之间的最短路径上:
- DFS: 从树 T 的根开始, 进行深度优先遍历（将树 T 看成一个无向图）, 并记录下每次到达的顶点, 以及这个点的深度, 第一个的结点是 root(T), 每经过一条边都记录它的端点。由于每条边恰好经过 2 次，因此一共记录了 2n - 1 个结点，用 dfn[1, … , 2n-1] 来表示。
- 计算first: 用 first[i] 表示 dfn 数组中第一个值为 i 的元素下标，即如果 first[u] < first[v] 时，DFS 访问的顺序是 。虽然其中包含
 u 的后代，但深度最小的还是 u 与 v 的公共祖先。
- RMQ: 当  时，
- 否则 ,
 计算 RMQ。

由于 RMQ 中使用的ST算法是在线算法，所以这个算法也是在线算法。


参考代码：




```cpp
int pcnt = 0;               // 用来计算遍历序
int first[MAXN];
int dfn[2 * MAXN];          // 注意数组大小
int deepth[2 * MAXN];
int dp[2 * MAXN][20]; 
void dfs(int u, int fa, int dep){
    dfn[++pcnt] = u;
    first[u] = pcnt;
    deepth[pcnt] = dep;
    for(int i = head[u]; i + 1; i = edge[i].nxt){
        int v = edge[i].v;
        if(v == fa) continue;
        dfs(v, u, dep + 1);
        dfn[++pcnt] = u;
        deepth[pcnt] = dep;
    }
}
void ST(){
    for(int i = 1; i <= pcnt; i++)
        dp[i][0] = i;
    for(int j = 1; j < 20; j++)
        for(int i = 1; i + (1 << j) - 1 <= pcnt; i++){
            int a = dp[i][j - 1], b = dp[i + (1 << (j - 1))][j - 1];
            if(deepth[a] < deepth[b]) dp[i][j] = a;
            else dp[i][j] = b;
        }
}
int RMQ(int l, int r){
    int k = log((double)(r - l + 1)) / log(2.0);
    int a = dp[l][k], b = dp[r - (1 << k) + 1][k];
    if(deepth[a] < deepth[b]) return a;
    else return b;
}
int LCA(int L, int R){
    int l = first[L];
    int r = first[R];
    if(l > r) swap(l, r);
    int pos = RMQ(l, r);
    return dfn[pos];
}
```



离线 TarJan 算法

#### [](https://blog.andrewei.info/2015/10/08/e6-9c-80-e8-bf-91-e5-85-ac-e5-85-b1-e7-a5-96-e5-85-88lca-e7-9a-84-e4-b8-89-e7-a7-8d-e6-b1-82-e8-a7-a3-e6-96-b9-e6-b3-95/#%E7%AE%80%E8%BF%B0-1)简述


Tarjan算法（以发现者Robert Tarjan命名）是一个在图中寻找强连通分量的算法。算法的基本思想为：任选一结点开始进行深度优先搜索dfs（若深度优先搜索结束后仍有未访问的结点，则再从中任选一点再次进行）。搜索过程中已访问的结点不再访问。搜索树的若干子树构成了图的强连通分量。

应用到要解决的LCA问题上，则是：对于新搜索到的一个结点 u, 先创建由 u 构成的集合，再对 u 的每颗子树进行搜索，每搜索完一棵子树，这时候子树中所有的结点的最近公共祖先就是 u 了。

#### [](https://blog.andrewei.info/2015/10/08/e6-9c-80-e8-bf-91-e5-85-ac-e5-85-b1-e7-a5-96-e5-85-88lca-e7-9a-84-e4-b8-89-e7-a7-8d-e6-b1-82-e8-a7-a3-e6-96-b9-e6-b3-95/#%E7%AE%97%E6%B3%95%E6%80%9D%E6%83%B3)**算法思想**


Tarjan算法基于dfs的框架,对于新搜到的一个结点,首先创建由这个结点构成的集合,再对当前结点的每个子树进行搜索;

每搜索完一棵子树,则可确定子树内的LCA询问都已解决,其他的LCA询问的结果必然在这个子树之外;

这时把子树所形成的集合与当前结点的集合合并,并将当前结点设为这个集合的祖先;

之后继续搜索下一棵子树,直到当前结点的所有子树搜完;

这时把当前结点也设为已被检查过的,同时可以处理有关当前结点的LCA询问;

如果有一个从当前结点到结点v的询问,且v已经被检查过;

则由于进行的是dfs,当前结点与v的最近公共祖先一定还没有被检查;

而这个最近公共祖先的包含v的子树一定已经搜索过了,那么这个最近公共祖先一定是v所在集合的祖先;

#### [](https://blog.andrewei.info/2015/10/08/e6-9c-80-e8-bf-91-e5-85-ac-e5-85-b1-e7-a5-96-e5-85-88lca-e7-9a-84-e4-b8-89-e7-a7-8d-e6-b1-82-e8-a7-a3-e6-96-b9-e6-b3-95/#%E7%AE%97%E6%B3%95%E6%AD%A5%E9%AA%A4)**算法步骤**


对于每一个结点:
- 建立以u为代表元素的集合;
- 遍历与u相连的结点v,如果没有被访问过,对于v使用Tarjan_LCA算法,结束后将v的集合并入u的集合;
- 对于与u有关的询问(u,v),如果v被访问过,则结果就是v所在集合的代表元素;


参考代码:




```cpp
const int MAXN = 300100;
struct Edge{
    int v, nxt;
    int id;
};
bool vis[MAXN];
int n, q, ecnt, qcnt;
Edge tedge[MAXN * 2], qedge[MAXN * 2];
int thead[MAXN], qhead[MAXN], res[MAXN], par[MAXN];
void init(){
    ecnt = qcnt = 0;
    memset(vis, 0, sizeof(vis));
    memset(tedge, 0, sizeof(tedge));
    memset(qedge, 0, sizeof(qedge));
    memset(thead, -1, sizeof(thead));
    memset(qhead, -1, sizeof(qhead));
}
void addEdge(int *head, Edge *edge, int &cnt, int u, int v, int id){
    edge[cnt].v = v;
    edge[cnt].id = id;
    edge[cnt].nxt = head[u];
    head[u] = cnt++;
}
int Find(int x){
    if(par[x] != x) return par[x] = Find(par[x]);
    return par[x];
}
void Union(int u, int v){
    int fu = Find(u);
    int fv = Find(v);
    par[fu] = fv;
}
void Tarjan(int u){
    par[u] = u;
    vis[u] = true;
    for(int i = thead[u]; i + 1; i = tedge[i].nxt){
        int v = tedge[i].v;
        if(vis[v]) continue;
        Tarjan(v);
        Union(v, u);
    }
    for(int i = qhead[u]; i + 1; i = qedge[i].nxt){
        int v = qedge[i].v;
        int id = qedge[i].id;
        if(!vis[v]) continue;
        res[id] = Find(v);
    }
}
```



**(拓展)运用Tarjan算法求图上两点间最大边**


参考代码：



```cpp
const int MAXN = 50050;
struct Edge{
    int v, nxt;
    int index;
};
bool vis[MAXN];
int Min[MAXN], Max[MAXN];
int ecnt, qcnt, acnt, n, q;
int uu[MAXN], vv[MAXN], val[MAXN];
Edge tedge[2 * MAXN], qedge[2 * MAXN], aedge[2 * MAXN];
int thead[MAXN], qhead[MAXN], ahead[MAXN], par[MAXN], res[MAXN];
void init(){
    ecnt = qcnt = acnt = 0;
    memset(res, 0, sizeof(res));
    memset(val, 0, sizeof(val));
    memset(vis, 0, sizeof(vis));
    memset(tedge, 0, sizeof(tedge));
    memset(qedge, 0, sizeof(qedge));
    memset(aedge, 0, sizeof(aedge));
    memset(thead, -1, sizeof(thead));
    memset(qhead, -1, sizeof(qhead));
    memset(ahead, -1, sizeof(ahead));
}
void addEdge(int *head, Edge *edge, int u, int v, int index, int &cnt){
    edge[cnt].v = v;
    edge[cnt].index = index;
    edge[cnt].nxt = head[u];
    head[u] = cnt++;
}
int Find(int x){
    if(x == par[x]) return par[x];
    int temp = par[x];
    par[x] = Find(par[x]);
    Max[x] = max(Max[x], Max[temp]);
    Min[x] = min(Min[x], Min[temp]);
    return par[x];
}
void Tarjan(int u){
    vis[u] = true;
    par[u] = u;
    for(int i = qhead[u]; i + 1; i = qedge[i].nxt){
        int v = qedge[i].v, index = qedge[i].index;
        if(!vis[v]) continue;
        int lca = Find(v);
        addEdge(ahead, aedge, lca, v, index, acnt);
    }
    for(int i = thead[u]; i + 1; i = tedge[i].nxt){
        int v = tedge[i].v;
        if(vis[v]) continue;
        Tarjan(v);
        par[v] = u;
    }
    for(int i = ahead[u]; i + 1; i = aedge[i].nxt){
        int index = aedge[i].index;
        Find(uu[index]);
        Find(vv[index]);
        res[index] = max(Max[
        res[index] = max(max(Up[uu[index]], Down[vv[index]]), Max[vv[index]] - Min[uu[index]]); 
    }
}
```



### [](https://blog.andrewei.info/2015/10/08/e6-9c-80-e8-bf-91-e5-85-ac-e5-85-b1-e7-a5-96-e5-85-88lca-e7-9a-84-e4-b8-89-e7-a7-8d-e6-b1-82-e8-a7-a3-e6-96-b9-e6-b3-95/#%E5%80%8D%E5%A2%9E-LCA)简介倍增 LCA

#### [](https://blog.andrewei.info/2015/10/08/e6-9c-80-e8-bf-91-e5-85-ac-e5-85-b1-e7-a5-96-e5-85-88lca-e7-9a-84-e4-b8-89-e7-a7-8d-e6-b1-82-e8-a7-a3-e6-96-b9-e6-b3-95/#%E7%AE%80%E4%BB%8B-1)


倍增法LCA也是一个求最近公共祖先的在线算法，他利用了二分搜索的思想降低每次寻找最近公共祖先的复杂度，预处理的复杂度为 O(nlog(n)), 每次查询的复杂度为 O(log(n))。

#### [](https://blog.andrewei.info/2015/10/08/e6-9c-80-e8-bf-91-e5-85-ac-e5-85-b1-e7-a5-96-e5-85-88lca-e7-9a-84-e4-b8-89-e7-a7-8d-e6-b1-82-e8-a7-a3-e6-96-b9-e6-b3-95/#%E7%AE%97%E6%B3%95%E6%B5%81%E7%A8%8B)算法流程
- 初始化所有点的深度和第 2^0, 2^1, 2^2, … 2^n 个祖先；
- 从深度大的节点上升至深度小的节点同层，如果此时两节点相同直接返回此节点，即lca，否则，利用倍增法找到最小深度的p[a][j]!=p[b][j]，此时他们的父亲p[a][0]即lca。


参考代码:



```cpp
const int MAXN = 300010;
const int DEG = 20;

struct Edge{
    int v, nxt;
};

int ecnt, n, m;
Edge edge[MAXN * 2];
int fa[MAXN][20];
int head[MAXN], depth[MAXN];

void init(){
    ecnt = 0;
    memset(edge, 0, sizeof(edge));
    memset(head, -1, sizeof(head));
    memset(depth, 0, sizeof(depth));
}

void addEdge(int u, int v){
    edge[ecnt].v = v;
    edge[ecnt].nxt = head[u];
    head[u] = ecnt++;
}

void initfa(int root){
    queue <int> que;
    que.push(root);
    depth[root] = 0;
    fa[root][0] = root;
    while(!que.empty()){
        int u = que.front();
        que.pop();

        for(int i = 1; i < DEG; i++)
            fa[u][i] = fa[fa[u][i - 1]][i - 1];

        for(int i = head[u]; i + 1; i = edge[i].nxt){
            int v = edge[i].v;
            if(v == fa[u][0]) continue;
            depth[v] = depth[u] + 1;
            fa[v][0] = u;
            que.push(v);
        }
    }
}

int LCA(int u, int v){
    if(depth[u] > depth[v]) swap(u, v);
    int du = depth[u], dv = depth[v];
    int tu = u, tv = v;
    for(int det = dv - du, i = 0; det; det >>= 1, i++)
        if(det & 1) tv = fa[tv][i];
    if(tu == tv) return tu;
    for(int i = DEG - 1; i >= 0; i--){
        if(fa[tu][i] == fa[tv][i]) continue;
        tu = fa[tu][i];
        tv = fa[tv][i];
    }
    return fa[tu][0];
}
```






