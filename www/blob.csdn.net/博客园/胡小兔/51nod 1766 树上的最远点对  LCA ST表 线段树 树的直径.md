# 51nod 1766 树上的最远点对 | LCA ST表 线段树 树的直径 - 胡小兔 - 博客园







# [51nod 1766 树上的最远点对 | LCA ST表 线段树 树的直径](https://www.cnblogs.com/RabbitHu/p/51nod1766.html)





# [51nod 1766](http://www.51nod.com/onlineJudge/questionCode.html#!problemId=1766) 树上的最远点对 | LCA ST表 线段树 树的直径

## 题面

n个点被n-1条边连接成了一颗树，给出a~b和c~d两个区间，表示点的标号请你求出两个区间内各选一点之间的最大距离，即你需要求出max｛dis(i,j) |a<=i<=b,c<=j<=d｝

Input

第一行一个数字 n n<=100000。

第二行到第n行每行三个数字描述路的情况， x,y,z (1<=x,y<=n,1<=z<=10000)表示x和y之间有一条长度为z的路。

第n+1行一个数字m，表示询问次数 m<=100000。

接下来m行，每行四个数a,b,c,d。

Output

共m行，表示每次询问的最远距离

## 题解

刚才写了一大长串题解，好顿证明，发现证错了……

算了我不知道这玩意怎么证了(ノಠ益ಠ)ノ彡┻━┻

反正树上的一个“点的集合”的直径的端点，一定包含于把这个集合分成两部分、分别得到的直径的端点（共四个）中。

那么可以用线段树维护区间内直径的端点。

```
#include <cstdio>
#include <cstring>
#include <algorithm>
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

const int N = 100005;
int n, m;
int ecnt, adj[N], nxt[2*N], go[2*N], w[2*N];
int dep[N], pos[N], lst[2*N], idx, mi[2*N][20], lg[2*N];
struct Data {
    int fi, se;
} data[4*N];
void add(int u, int v, int ww){
    go[++ecnt] = v;
    nxt[ecnt] = adj[u];
    adj[u] = ecnt;
    w[ecnt] = ww;
}
void dfs(int u, int pre){
    lst[++idx] = u, pos[u] = idx;
    for(int e = adj[u], v; e; e = nxt[e])
        if(v = go[e], v != pre)
            dep[v] = dep[u] + w[e], dfs(v, u), lst[++idx] = u;
}
int Min(int a, int b){
    return dep[a] > dep[b] ? b : a;
}
int lca(int u, int v){
    int l = pos[u], r = pos[v];
    if(l > r) swap(l, r);
    int j = lg[r - l + 1];
    return Min(mi[l][j], mi[r - (1 << j) + 1][j]);
}
void st_init(){
    for(int i = 1, j = 0; i <= idx; i++){
        if(1 << (j + 1) == i) j++;
        lg[i] = j;
    }
    for(int i = 1; i <= idx; i++)
        mi[i][0] = lst[i];
    for(int j = 1; (1 << j) <= idx; j++)
        for(int i = 1; i + (1 << j) - 1 <= idx; i++)
            mi[i][j] = Min(mi[i][j - 1], mi[i + (1 << (j - 1))][j - 1]);
}
int dis(int u, int v){
    return dep[u] + dep[v] - 2 * dep[lca(u, v)];
}
Data Max(Data a, Data b){
    int t[4] = {a.fi, a.se, b.fi, b.se}, mx = 0;
    Data ret;
    for(int i = 0; i < 4; i++)
        for(int j = i + 1; j < 4; j++)
            if(dis(t[i], t[j]) > mx)
                mx = dis(t[i], t[j]), ret.fi = t[i], ret.se = t[j];
    return ret;
}
void build(int k, int l, int r){
    if(l == r) return (void)(data[k] = (Data){l, l});
    int mid = (l + r) >> 1;
    build(k << 1, l, mid);
    build(k << 1 | 1, mid + 1, r);
    data[k] = Max(data[k << 1], data[k << 1 | 1]);
}
Data query(int k, int l, int r, int ql, int qr){
    if(ql <= l && qr >= r) return data[k];
    int mid = (l + r) >> 1;
    if(qr <= mid) return query(k << 1, l, mid, ql, qr);
    if(ql > mid) return query(k << 1 | 1, mid + 1, r, ql, qr);
    return Max(query(k << 1, l, mid, ql, qr), query(k << 1 | 1, mid + 1, r, ql, qr));
}
                
int main(){
    read(n);
    for(int i = 1, u, v, ww; i < n; i++)
        read(u), read(v), read(ww), add(u, v, ww), add(v, u, ww);
    dfs(1, 0);
    st_init();
    build(1, 1, n);
    read(m);
    while(m--){
        int a, b, c, d, ans = 0;
        read(a), read(b), read(c), read(d);
        Data x = query(1, 1, n, a, b), y = query(1, 1, n, c, d);
        ans = max(dis(x.fi, y.fi), dis(x.fi, y.se));
        ans = max(ans, max(dis(x.se, y.fi), dis(x.se, y.se)));
        write(ans), puts("");
    }
    return 0;
}
```












