# CodeForces 1110F Nearest Leaf | 线段树/换根 - 胡小兔 - 博客园







# [CodeForces 1110F Nearest Leaf | 线段树/换根](https://www.cnblogs.com/RabbitHu/p/10358470.html)





> 
我……又诈尸了……

代码几乎都不会写了，打场CF居然上分啦，开心！（虽然还是比不过列表里的各路神仙）


## 题目链接

## 题目描述

一棵\(n\)个点的有根树，规定一种dfs序（规则：编号小的点优先dfs），\(m\)次询问一个点\(u\)和一个区间\([l, r]\)，求dfs序在这个区间内的叶子中，到\(u\)最小的距离。

\(n, m \le 500000\)

## 题解

这题……很简单……

题面一上来给个什么欧拉遍历定义……我吓得比赛中没看这题……（实际上码量对于代码几乎都不会敲的退役选手来说，不是非常友好 = = 当时做了可能也会写跪）

用线段树维护所有叶子到“当前点”（一开始是\(1\)号节点）的距离\(dis\)。

一开始以\(1\)号节点为“当前点”，dfs求距离，建树。这样\(u = 1\)的询问就可以解决了。

怎么解决其他\(u\)的询问呢？考虑移动“当前点”时，线段树会如何变化。

因为是DFS序，所以每棵子树在dfs序上都挨在一起。当“当前点”从父亲\(u\)移到儿子\(v\)\(w(u, v)\)时，子树\(v\)内所有点的\(dis\)都减去了\(w(u, v)\)（<u, v>这条边的长度），而子树\(v\)外的所有点\(dis\)都加上了\(w(u, v)\)。只需在线段树上区间修改即可。

显然，先把询问都读进来，离线处理非常优秀。空间允许的话似乎也可以主席树？（没试过 = =）

## 代码

```
#include <cstdio>
#include <cstring>
#include <cmath>
#include <algorithm>
#include <iostream>
#include <cassert>
#include <vector>
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

const int N = 500005;
const ll INF = 0x3f3f3f3f3f3f3f3f;
int n, m;
int ncnt, dfn[N], idx[N], ed[N];
int lcnt, llst[N], lnum[N], prel[N], nxtl[N];
ll w[N], dis[N], data[4*N], lazy[4*N], ans[N];
vector <int> son[N];

struct Query {
    int id, u, l, r;
    bool operator < (const Query &b) const {
        return dfn[u] < dfn[b.u];
    }
} qry[N];
int qpos = 1;

void dfs1(int u, int pre){
    dfn[u] = ++ncnt;
    idx[ncnt] = u;
    for(auto v : son[u]){
        dis[v] = dis[u] + w[v];
        dfs1(v, u); 
    }
    if(u != 1 && ncnt == dfn[u]){
        lnum[u] = ++lcnt;
        llst[lcnt] = u;
    }
    ed[u] = ncnt;
}

void modify(int k, ll x){
    data[k] += x, lazy[k] += x;
}
void pushdown(int k){
    if(!lazy[k]) return;
    modify(k << 1, lazy[k]);
    modify(k << 1 | 1, lazy[k]);
    lazy[k] = 0;
}
void change(int k, int l, int r, int ql, int qr, ll x){
    if(ql <= l && qr >= r) return void(modify(k, x));
    pushdown(k);
    int mid = (l + r) >> 1;
    if(ql <= mid) change(k << 1, l, mid, ql, qr, x);
    if(qr > mid) change(k << 1 | 1, mid + 1, r, ql, qr, x);
    data[k] = min(data[k << 1], data[k << 1 | 1]);
}
ll query(int k, int l, int r, int ql, int qr){
    if(ql <= l && qr >= r) return data[k];
    pushdown(k);
    int mid = (l + r) >> 1;
    ll ret = INF;
    if(ql <= mid) ret = query(k << 1, l, mid, ql, qr);
    if(qr > mid) ret = min(ret, query(k << 1 | 1, mid + 1, r, ql, qr));
    return ret;
}
void build_tree(int k, int l, int r){
    if(l == r) return void(data[k] = dis[llst[l]]);
    int mid = (l + r) >> 1;
    build_tree(k << 1, l, mid);
    build_tree(k << 1 | 1, mid + 1, r);
    data[k] = min(data[k << 1], data[k << 1 | 1]);
}

void dfs2(int u, int pre){
    while(qry[qpos].u == u){
        ans[qry[qpos].id] = query(1, 1, lcnt, qry[qpos].l, qry[qpos].r);
        qpos++;
    }
    for(auto v : son[u]){
        change(1, 1, lcnt, 1, lcnt, w[v]);
        change(1, 1, lcnt, nxtl[dfn[v]], prel[ed[v]], -2 * w[v]);
        dfs2(v, u);
        change(1, 1, lcnt, 1, lcnt, -w[v]);
        change(1, 1, lcnt, nxtl[dfn[v]], prel[ed[v]], 2 * w[v]);
    }
}

int main(){

    read(n), read(m);
    for(int i = 2, u; i <= n; i++){
        read(u), read(w[i]);
        son[u].push_back(i);
    }
    dfs1(1, 0);
    for(int i = 1, t = 1; i <= n; i++){
        if(lnum[idx[i]]) t = lnum[idx[i]];
        prel[i] = t;
    }
    for(int i = n, t = lcnt; i; i--){
        if(lnum[idx[i]]) t = lnum[idx[i]];
        nxtl[i] = t;
    }
    for(int i = 1; i <= m; i++){
        qry[i].id = i, read(qry[i].u), read(qry[i].l), read(qry[i].r);
        qry[i].l = nxtl[qry[i].l], qry[i].r = prel[qry[i].r];
    }
    sort(qry + 1, qry + m + 1);
    build_tree(1, 1, lcnt);
    dfs2(1, 0);
    for(int i = 1; i <= m; i++)
        write(ans[i]), enter;

    return 0;
}
```












