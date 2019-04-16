# ZOJ 1314 Reactor Cooling | 上下界无源汇可行流 - 胡小兔 - 博客园







# [ZOJ 1314 Reactor Cooling | 上下界无源汇可行流](https://www.cnblogs.com/RabbitHu/p/ZOJ1314.html)





# ZOJ 1314 Reactor Cooling | 上下界无源汇可行流

## 题意

有一个网络，每条边有流量的上界和下界，求一种方案，让里面的流可以循环往复地流动起来。

## 题解

上下界无源汇可行流的模型：
- 设置虚拟源点和虚拟汇点。
- 如果一条边\(u \to v\)的下界是\(mi\)、上界是\(ma\)，则在图中建一条\(u \to v\)的边，流量是\(ma - mi\)，同时记录\(oud[u] += mi, ind[v] += mi\)，分别代表\(u\)实际比图上多流出的流量与\(v\)实际比图上多流入的流量。
- 对于每个节点\(u\)，如果\(ind[u] > oud[u]\)，即这个节点需要额外流入一些流量，则这些让虚拟源点提供这些流量，即连接一条边\(S \to u\)，流量为\(ind[u] - oud[u]\)；反之，如果\(ind[u] < oud[u]\)，即这个节点需要额外流出一些流量，则这些流量流入了虚拟汇点，即连接一条边\(u \to T\)，流量为\(oud[u] - ind[u]\)。

然后求一下这个图的最大流，如果\(S\)的所有出边都流满了，则有解，否则无解。

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
const int N = 205, M = 100005, INF = 0x3f3f3f3f;
int T, n, m, src, des, ans, sum, u[M], v[M], mi[M], ma[M], ind[N], oud[N];
int ecnt = 1, adj[N], cur[N], dis[N], nxt[M], go[M], cap[M];
void init(){
    ans = sum = 0;
    ecnt = 1;
    for(int i = 1; i <= des; i++)
        ind[i] = oud[i] = adj[i] = 0;
}
void ADD(int u, int v, int _cap){
    go[++ecnt] = v;
    nxt[ecnt] = adj[u];
    adj[u] = ecnt;
    cap[ecnt] = _cap;
}
void add(int u, int v, int _cap){
    ADD(u, v, _cap);
    ADD(v, u, 0);
}
bool bfs(){
    static int que[N], qr;
    for(int i = 1; i <= des; i++)
        cur[i] = adj[i], dis[i] = -1;
    dis[src] = 1, que[qr = 1] = src;
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
                if(ret == flow) break;
            }
        }
    return ret;
}
int main(){
    read(T);
    while(T--){
        read(n), read(m), src = n + 1, des = n + 2;
        init();
        for(int i = 1; i <= m; i++){
            read(u[i]), read(v[i]), read(mi[i]), read(ma[i]);
            add(u[i], v[i], ma[i] - mi[i]);
            oud[u[i]] += mi[i], ind[v[i]] += mi[i];
        }
        for(int i = 1; i <= n; i++)
            if(ind[i] > oud[i])
                add(src, i, ind[i] - oud[i]), sum += ind[i] - oud[i];
            else
                add(i, des, oud[i] - ind[i]);
        while(bfs()) ans += dfs(src, INF);
        if(ans < sum)
            puts("NO");
        else{
            puts("YES");
            for(int i = 1; i <= m; i++)
                write(mi[i] + cap[i * 2 + 1]), enter;
        }
    }
    return 0;
}
```












