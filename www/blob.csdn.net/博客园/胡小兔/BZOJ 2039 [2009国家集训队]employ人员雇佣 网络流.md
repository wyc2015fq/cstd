# BZOJ 2039 [2009国家集训队]employ人员雇佣 网络流 - 胡小兔 - 博客园







# [BZOJ 2039 [2009国家集训队]employ人员雇佣 网络流](https://www.cnblogs.com/RabbitHu/p/BZOJ2039.html)





## 链接

[BZOJ 2039](http://www.lydsy.com/JudgeOnline/problem.php?id=2039)

## 题解

这题建图好神，自己瞎搞了半天，最后不得不求教了[企鹅学长的博客](http://oi.self-jqe.win/?p=531),,,,发现建图太神了！！

s向每个人连sum(e[i][x]) 的边，每个人向T连a[i]的边。两两人之间连2 * e[i][j]的边即可。

最后总的e – maxflow即为答案。

为什么我就没想到“源点向每个人连sum(e[i][x]) 的边”……

犯的错误：

为了方便，对于双向边，用`ADD(u, v, w), ADD(v, u, w)`代替了`ADD(u, v, w), ADD(v, u, 0), ADD(v, u, w), ADD(u, v, 0)`，但是却没注意到`ADD(u, v, w)`和`ADD(v, u, w)`一定要同时加，否则不满足反向边是e ^ 1这个性质……

```
#include <cstdio>
#include <cmath>
#include <cstring>
#include <algorithm>
#include <queue>
using namespace std;
typedef long long ll;
#define enter putchar('\n')
#define space putchar(' ')
template <class T>
void read(T &x){
    char c;
    bool op = 0;
    while(c = getchar(), c > '9' || c < '0')
    if(c == '-') op = 1;
    x = c - '0';
    while(c = getchar(), c >= '0' && c <= '9')
    x = x * 10 + c - '0';
    if(op) x = -x;
}
template <class T>
void write(T x){
    if(x < 0) putchar('-'), x = -x;
    if(x >= 10) write(x / 10);
    putchar('0' + x % 10);
}

const int N = 2005, M = 4000005;
ll INF = 0x3f3f3f3f3f3f3f3f;
int n, src, des, ecnt = 1, adj[N], cur[N], nxt[M], go[M];
ll a[N][N], dis[N], cap[M], tot;

void ADD(int u, int v, ll _cap){
    go[++ecnt] = v;
    nxt[ecnt] = adj[u];
    adj[u] = ecnt;
    cap[ecnt] = _cap;
}
void add(int u, int v, ll _cap){
    ADD(u, v, _cap);
    ADD(v, u, 0);
}
bool bfs(){
    static int que[N], qr;
    for(int i = 1; i <= des; i++)
    dis[i] = -1, cur[i] = adj[i];
    que[qr = 1] = src, dis[src] = 0;
    for(int ql = 1; ql <= qr; ql++){
    int u = que[ql];
    for(int e = adj[u], v; e; e = nxt[e])
        if(cap[e] && dis[v = go[e]] == -1){
        dis[v] = dis[u] + 1, que[++qr] = v;
        if(v == des) return 1;
        }
    }
    return 0;
}
ll dfs(int u, ll flow){
    if(u == des) return flow;
    ll ret = 0, delta;
    for(int &e = cur[u], v; e; e = nxt[e])
    if(cap[e] && dis[v = go[e]] == dis[u] + 1){
        delta = dfs(v, min(cap[e], flow - ret));
        cap[e] -= delta;
        cap[e ^ 1] += delta;
        ret += delta;
        if(ret == flow) return ret;
    }
    dis[u] = -1;
    return ret;
}
ll maxflow(){
    ll ret = 0;
    while(bfs()) ret += dfs(src, INF);
    return ret;
}

int main(){

    read(n), src = n + 1, des = src + 1;
    for(ll i = 1, t; i <= n; i++)
    read(t), add(i, des, t);
    for(int i = 1; i <= n; i++){
    ll sum = 0;
    for(int j = 1; j <= n; j++){
        read(a[i][j]);
        sum += a[i][j], tot += a[i][j];
        if(i >= j) continue;
        ADD(i, j, 2 * a[i][j]), ADD(j, i, 2 * a[i][j]);
    }
    add(src, i, sum);
    }
    write(tot - maxflow()), enter;

    return 0;
}
```












