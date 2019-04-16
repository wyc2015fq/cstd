# CodeForces 464E The Classic Problem | 呆克斯歘 主席树维护高精度 - 胡小兔 - 博客园







# [CodeForces 464E The Classic Problem | 呆克斯歘 主席树维护高精度](https://www.cnblogs.com/RabbitHu/p/CF464E.html)





## 题意描述

> 
有一个\(n\)点\(m\)边的无向图，第\(i\)条边的边权是\(2^{a_i}\)。求点\(s\)到点\(t\)的最短路长度（对\(10^9 + 7\)取模）。


## 题解

思路很简单——用主席树维护每个点的\(dis\)。因为每次更新某个点\(v\)的\(dis_v\)的时候，新的\(dis_v\)都是某个点\(u\)的\(dis_u + 2^{w_{u, v}}\)，相当于在原先\(u\)对应的主席树基础上修改，得到新的一棵主席树，作为\(v\)对应的主席树。

主席树（线段树）维护二进制高精度怎么维护呢？像[松松松](http://www.cnblogs.com/RabbitHu/p/UOJ314.html)那么维护就好了 = =

需（wǒ）要（fàn）注（guò）意的问题：
- 如果你用`priority_queue`来做Dijkstra，又中途修改了节点对应的dis，会影响堆的性质，会WA。正确做法是在`priority_queue`传`pair<节点编号，当前dis>`。
- 主席树的空间要适当优化优化？例如查询操作的时候，pushdown会创造新的节点，但是以后就不会用到这群节点了，于是一次完整的查询操作之后把这些新节点都删掉就好了，空间可以得到明显优化。

## 代码

```
#include <cstdio>
#include <cmath>
#include <cstring>
#include <algorithm>
#include <iostream>
#include <queue>
#define space putchar(' ')
#define enter putchar('\n')
typedef long long ll;
using namespace std;
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

const int N = 100100, M = 40000007, mod = 1000000007, P = 1000000021;
int n, s, t, maxn = 100098, m, hsh100[N], hsh111[N], ans100[N], ans111[N];
int ecnt, adj[N], nxt[2*N], go[2*N], w[2*N], pre[N], stk[N], top;
int ls[M], rs[M], hsh[M], ans[M], tot, root[N];
bool lazy[M], vis[N];
void adde(int u, int v, int ww){
    go[++ecnt] = v;
    nxt[ecnt] = adj[u];
    adj[u] = ecnt;
    w[ecnt] = ww;
}
int newnode(int old){
    int k = ++tot;
    ls[k] = ls[old], rs[k] = rs[old];
    hsh[k] = hsh[old], ans[k] = ans[old], lazy[k] = lazy[old];
    return k;
}
int pushdown(int k){
    if(!lazy[k]) return newnode(k);
    k = newnode(k);
    lazy[k] = 0;
    ls[k] = newnode(ls[k]), rs[k] = newnode(rs[k]);
    lazy[ls[k]] = lazy[rs[k]] = 1;
    hsh[ls[k]] = hsh[rs[k]] = ans[ls[k]] = ans[rs[k]] = 0;
    return k;
}
int change0(int k, int l, int r, int ql, int qr){
    if(ql <= l && qr >= r) return k = newnode(k), lazy[k] = 1, hsh[k] = ans[k] = 0, k;
    k = pushdown(k);
    int mid = (l + r) >> 1;
    if(ql <= mid) ls[k] = change0(ls[k], l, mid, ql, qr);
    if(qr > mid) rs[k] = change0(rs[k], mid + 1, r, ql, qr);
    hsh[k] = (hsh[ls[k]] + (ll)hsh[rs[k]] * hsh100[mid - l + 1]) % P;
    ans[k] = (ans[ls[k]] + (ll)ans[rs[k]] * ans100[mid - l + 1]) % mod;
    return k;
}
int change1(int k, int l, int r, int p){
    if(l == r) return k = newnode(k), lazy[k] = 0, hsh[k] = ans[k] = 1, k;
    k = pushdown(k);
    int mid = (l + r) >> 1;
    if(p <= mid) ls[k] = change1(ls[k], l, mid, p);
    else rs[k] = change1(rs[k], mid + 1, r, p);
    hsh[k] = (hsh[ls[k]] + (ll)hsh[rs[k]] * hsh100[mid - l + 1]) % P;
    ans[k] = (ans[ls[k]] + (ll)ans[rs[k]] * ans100[mid - l + 1]) % mod;
    return k;
}
int find0(int k, int l, int r, int ql, int qr){
    if(hsh[k] == hsh111[r - l + 1] && ans[k] == ans111[r - l + 1]) return -1;
    if(l == r) return l;
    k = pushdown(k);
    int mid = (l + r) >> 1;
    if(ql > mid) return find0(rs[k], mid + 1, r, ql, qr);
    int ret = find0(ls[k], l, mid, ql, qr);
    if(ret != -1) return ret;
    return find0(rs[k], mid + 1, r, ql, qr);
}
int add(int k, int p){
    int mem_tot = tot;
    int q = find0(k, 0, maxn, p, maxn);
    tot = mem_tot;
    k = change1(k, 0, maxn, q);
    if(p < q) k = change0(k, 0, maxn, p, q - 1);
    return k;
}
bool diff(int k1, int k2, int l, int r){
    if(l == r) return hsh[k1] < hsh[k2];
    k1 = pushdown(k1), k2 = pushdown(k2);
    int mid = (l + r) >> 1;
    if(hsh[rs[k1]] == hsh[rs[k2]] && ans[rs[k1]] == ans[rs[k2]])
        return diff(ls[k1], ls[k2], l, mid);
    else return diff(rs[k1], rs[k2], mid + 1, r);
}
struct Data {
    int node, root;
    bool operator < (const Data &b) const {
        int mem_tot = tot;
        bool ret = diff(b.root, root, 0, maxn);
        tot = mem_tot;
        return ret;
    }
};
priority_queue <Data> que;

int main(){

    read(n), read(m);
    hsh100[0] = ans100[0] = 1;
    for(int i = 1; i <= maxn + 1; i++){
        hsh100[i] = hsh100[i - 1] * 2 % P;
        ans100[i] = ans100[i - 1] * 2 % mod;
        hsh111[i] = (hsh100[i] - 1 + P) % P;
        ans111[i] = (ans100[i] - 1 + mod) % mod;
    }
    for(int i = 1, u, v, ww; i <= m; i++)
        read(u), read(v), read(ww), adde(u, v, ww), adde(v, u, ww);
    read(s), read(t);
    root[0] = add(0, maxn - 1);
    for(int i = 1; i <= n; i++)
        if(i != s) root[i] = root[0];
    que.push((Data){s, root[s]});
    while(!que.empty()){
        int u = que.top().node;
        que.pop();
        if(vis[u]) continue;
        vis[u] = 1;
        for(int e = adj[u], v; e; e = nxt[e]){
            v = go[e];
            int tmp = add(root[u], w[e]);
            if(diff(tmp, root[v], 0, maxn))
                root[v] = tmp, pre[v] = u, que.push((Data){v, root[v]});
        }
    }
    if(ans[root[t]] == ans100[maxn - 1] && hsh[root[t]] == hsh100[maxn - 1])
        return puts("-1"), 0;
    write(ans[root[t]]), enter;
    stk[++top] = t;
    while(pre[stk[top]]) stk[top + 1] = pre[stk[top]], top++;
    write(top), enter;
    while(top) write(stk[top--]), top ? space : enter;

    return 0;
}
```












