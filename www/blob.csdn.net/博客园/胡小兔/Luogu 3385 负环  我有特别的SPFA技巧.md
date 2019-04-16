# Luogu 3385 负环 | 我有特别的SPFA技巧 - 胡小兔 - 博客园







# [Luogu 3385 负环 | 我有特别的SPFA技巧](https://www.cnblogs.com/RabbitHu/p/Luogu3385.html)





这样似乎跑得快：

初始化所有的dis是0，然后枚举每个点作为起点，用DFS更新所有点的dis；

如果更新到一个栈中节点，那么有负环。

```
#include <cstdio>
#include <cstring>
#include <algorithm>
#define INF 0x3f3f3f3f
#define space putchar(' ')
#define enter putchar('\n')
using namespace std;
typedef long long ll;
template <class T>
bool read(T &x){
    char c;
    bool op = 0;
    while(c = getchar(), c < '0' || c > '9')
        if(c == '-') op = 1;
        else if(c == EOF) return 0;
    x = c - '0';
    while(c = getchar(), c >= '0' && c <= '9')
        x = x * 10 + c - '0';
    if(op) x = -x;
    return 1;
}
template <class T>
void write(T x){
    if(x < 0) putchar('-'), x = -x;
    if(x >= 10) write(x / 10);
    putchar('0' + x % 10);
}
const int N = 200005;
int T, n, m;
int ecnt, adj[N], go[2*N], nxt[2*N];
ll w[2*N], dis[N];
bool ins[N], done;
void add(int u, int v, int ww){
    go[++ecnt] = v;
    nxt[ecnt] = adj[u];
    adj[u] = ecnt;
    w[ecnt] = ww;
}
void dfs(int u){
    if(done) return;
    ins[u] = 1;
    for(int e = adj[u], v; e; e = nxt[e]){
        if(done) return;
        if(v = go[e], w[e] < dis[v] - dis[u]){
            dis[v] = dis[u] + w[e];
            if(ins[v]) return (void)(done = 1);
            else dfs(v);
        }
    }
    ins[u] = 0;
}
void init(){
    for(int i = 1; i <= n; i++)
        ins[i] = adj[i] = dis[i] = 0;
    ecnt = done = 0;
}
int main(){
    read(T);
    while(T--){
        read(n), read(m);
        init();
        for(int i = 1, u, v, ww; i <= m; i++){
            read(u), read(v), read(ww);
            add(u, v, ww);
            if(ww >= 0) add(v, u, ww);
        }
        for(int i = 1; i <= n; i++){
            dfs(i);
            if(done) break;
        }
        if(done) puts("YE5");
        else puts("N0");
    }
    return 0;
}
```












