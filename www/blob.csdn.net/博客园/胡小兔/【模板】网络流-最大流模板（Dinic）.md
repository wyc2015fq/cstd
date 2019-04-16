# 【模板】网络流-最大流模板（Dinic） - 胡小兔 - 博客园







# [【模板】网络流-最大流模板（Dinic）](https://www.cnblogs.com/RabbitHu/p/netflow.html)





```
#include <cstdio>
#include <cstring>
#include <algorithm>
#include <queue>
using namespace std;
typedef long long ll;
#define space putchar(' ')
#define enter putchar('\n')
#define INF 0x7fffffff
template <class T>
void read(T &x){
    char c;
    bool op = 0;
    while(c = getchar(), c < '0' || c > '9')
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
const int N = 1000005, M = 8000005;
int n, m, src, des;
int ecnt = 1, adj[N], cur[N], dis[N], nxt[M], go[M], cap[M];
int que[N], qr;
int ans;
void ADD(int u, int v, int w){
    go[++ecnt] = v;
    nxt[ecnt] = adj[u];
    adj[u] = ecnt;
    cap[ecnt] = w;
}
void add(int u, int v, int w){
    ADD(u, v, w);
    ADD(v, u, 0);
}
bool bfs(){
    for(int i = 1; i <= n; i++)
    dis[i] = -1, cur[i] = adj[i];
    dis[src] = 0, que[qr = 1] = src;
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
int dfs(int u, int flow){
    if(u == des) return flow;
    int ret = 0, delta;
    for(int &e = cur[u], v; e; e = nxt[e])
    if(cap[e] && dis[v = go[e]] == dis[u] + 1){
        delta = dfs(v, min(cap[e], flow - ret));
        if(delta){
        cap[e] -= delta;
        cap[e ^ 1] += delta;
        ret += delta;
        if(ret == flow) return ret;
        }
    }
    dis[u] = -1;
    return ret;
}
int main(){
    read(n), read(m), read(src), read(des);
    for(int i = 1, u, v, w; i <= m; i++)
    read(u), read(v), read(w), add(u, v, w);
    while(bfs()) ans += dfs(src, INF);
    write(ans), enter;
    return 0;
}
```












