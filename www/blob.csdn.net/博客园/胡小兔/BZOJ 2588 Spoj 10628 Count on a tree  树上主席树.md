# BZOJ 2588 Spoj 10628 Count on a tree | 树上主席树 - 胡小兔 - 博客园







# [BZOJ 2588 Spoj 10628 Count on a tree | 树上主席树](https://www.cnblogs.com/RabbitHu/p/BZOJ2588.html)





# [BZOJ 2588](http://www.lydsy.com/JudgeOnline/problem.php?id=2588) Count on a tree

## 题面

求树上两点之间路径上第k大的点权。

## 题解

一开始看到这道题觉得是树剖，然后又听说是主席树，然后以为是主席树+树剖，差点没吓死……

然后发现，如果每个点都挂一棵主席树，每棵都通过修改父亲的主席树得到，这样当询问路径(u, v)时，u的主席树+v的主席树-lca的主席树-fa[lca]的主席树就得到了路径上的主席树。

```
#include <cstdio>
#include <cmath>
#include <cstring>
#include <algorithm>
#define space putchar(' ')
#define enter putchar('\n')
using namespace std;
typedef long long ll;
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

const int N = 100005, M = 3000005;
int n, m, a[N], lst[N], tot, ans;
int ecnt, adj[N], nxt[2*N], go[2*N];
int fa[N], dep[N], lg[2*N], seq[2*N], cnt, pos[N], mi[2*N][20];
int idx, root[N], data[M], ls[M], rs[M];

void add(int u, int v){
    go[++ecnt] = v;
    nxt[ecnt] = adj[u];
    adj[u] = ecnt;
}
void dfs(int u, int pre){
    fa[u] = pre, dep[u] = dep[pre] + 1;
    seq[++cnt] = u, pos[u] = cnt;
    for(int e = adj[u], v; e; e = nxt[e])
        if(v = go[e], v != pre)
            dfs(v, u), seq[++cnt] = u;
}
int Min(int a, int b){
    return dep[a] < dep[b] ? a : b;
}
void init(){
    for(int i = 1, j = 0; i <= cnt; i++){
        lg[i] = i == (1 << (j + 1)) ? ++j : j;
        mi[i][0] = seq[i];
    }
    for(int j = 1; (1 << j) <= cnt; j++)
        for(int i = 1; i + (1 << j) - 1 <= cnt; i++)
            mi[i][j] = Min(mi[i][j - 1], mi[i + (1 << (j - 1))][j - 1]);
}
int getlca(int u, int v){
    int l = pos[u], r = pos[v];
    if(l > r) swap(l, r);
    int j = lg[r - l + 1];
    return Min(mi[l][j], mi[r - (1 << j) + 1][j]);
}
void build(int &k, int l, int r){
    k = ++idx;
    if(l == r) return;
    int mid = (l + r) >> 1;
    build(ls[k], l, mid);
    build(rs[k], mid + 1, r);
}
void change(int old, int &k, int l, int r, int p){
    k = ++idx;
    data[k] = data[old] + 1, ls[k] = ls[old], rs[k] = rs[old];
    if(l == r) return;
    int mid = (l + r) >> 1;
    if(p <= mid) change(ls[old], ls[k], l, mid, p);
    else change(rs[old], rs[k], mid + 1, r, p);
}
int query(int u, int v, int x){
    int lca = getlca(u, v), l = 1, r = n, mid, sum;
    int k[4] = {root[u], root[v], root[lca], root[fa[lca]]};
    while(l < r){
        mid = (l + r) >> 1, sum = data[ls[k[0]]] + data[ls[k[1]]] - data[ls[k[2]]] - data[ls[k[3]]];
        if(x <= sum){
            r = mid;
            for(int i = 0; i < 4; i++) k[i] = ls[k[i]];
        }
        else{
            l = mid + 1, x -= sum;
            for(int i = 0; i < 4; i++) k[i] = rs[k[i]];
        }
    }
    return lst[l];
}
void build_tree(){
    build(root[0], 1, tot);
    static int que[N], qr;
    que[qr = 1] = 1;
    for(int ql = 1; ql <= qr; ql++){
        int u = que[ql];
        change(root[fa[u]], root[u], 1, n, a[u]);
        for(int e = adj[u], v; e; e = nxt[e])
            if(v = go[e], v != fa[u])
                que[++qr] = v;
    }
}
int main(){
    read(n), read(m);
    for(int i = 1; i <= n; i++)
        read(a[i]), lst[i] = a[i];
    sort(lst + 1, lst + n + 1);
    tot = unique(lst + 1, lst + n + 1) - lst - 1;
    for(int i = 1; i <= n; i++)
        a[i] = lower_bound(lst + 1, lst + tot + 1, a[i]) - lst;
    for(int i = 1, u, v; i < n; i++)
        read(u), read(v), add(u, v), add(v, u);
    dfs(1, 0);
    init();
    build_tree();
    while(m--){
        int u, v, x;
        read(u), read(v), read(x);
        u ^= ans;
        ans = query(u, v, x);
        write(ans);
        if(m) enter;
    }
    return 0;
}
```












