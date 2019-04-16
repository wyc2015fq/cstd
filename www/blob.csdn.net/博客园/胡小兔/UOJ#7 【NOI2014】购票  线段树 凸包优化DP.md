# UOJ#7. 【NOI2014】购票 | 线段树 凸包优化DP - 胡小兔 - 博客园







# [UOJ#7. 【NOI2014】购票 | 线段树 凸包优化DP](https://www.cnblogs.com/RabbitHu/p/UOJ7.html)





## 题目链接

[UOJ #7](http://uoj.ac/problem/7)

## 题解

首先这一定是DP！可以写出：
\[f[i] = \min_{ancestor\ j} \{f[j] + (d[j] - d[i]) * p[i] + q[i]\}\]

其中\(d[i]\)表示树上\(i\)的深度。

整理一下式子：
\[f[i] = \min_{ancestor\ j} \{f[j] - d[j] * p[i]\} + d[i] * p[i] + q[i]\]

看起来可以斜率优化？

推一下式子：设\(j < k\)，\(i\)从\(j\)转移优于从\(k\)转移：
\[f[j] - d[j] * p[i] < f[k] - d[k] * p[i]\]
\[\frac{f[k] - f[j]}{d[k] - d[j]} > p[i]\]

好的！

所以应该维护一个下凸壳，在上面二分即可。

可是由于限制条件，每个结点\(i\)对应的下凸壳都是不同的，怎么办呢？

考虑一条链的情况：每个\(f[i]\)都是可以由一个区间内的凸包得到。

可以用线段树维护当前处理完的所有点的凸包，线段树上每个节点上存储着一个凸包，查询的时候相当于在线段树上区间查询——如果当前节点所代表的区间完全包含在查询区间里面，则在这个凸包上二分查询这个区间可以带来的最优解，否则递归，就可以得到答案了。

现在再考虑把一条链上的情况推广到树上。

考虑DFS，栈中的节点组成从根到当前节点的一条链，如果线段树维护了这条链的信息，则可以像正常序列上的情况一样求当前点的\(f\)值。

如果当前点DFS完毕出栈时，可以在线段树上删除它，就可以不影响复杂度地保证时时刻刻线段树维护的都是栈中所有节点的信息，就可以求出答案了。

于是引入【可撤销的凸包】，每次可以【撤销】——回到上一次插入新节点的操作之前。

怎么实现？当插入一个新节点时，二分它要放到哪个位置，并记录当前的top和被新节点取代的节点是谁，当撤销这次插入时，恢复top和被取代的节点即可。这样插入是\(O(\log n)\)的，撤销操作是\(O(1)\)的，非常科学 =v=

代码比想象中好写（雾

```
#include <cstdio>
#include <cstring>
#include <cmath>
#include <algorithm>
#include <iostream>
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

const int N = 200005;
int n, T, adj[N], nxt[N], fa[N], d_top, id[N];
ll f[N], d[N], p[N], q[N], w[N], lim[N], d_stk[N];
int seg_dep[4*N], pre[N][20], last_top[N][20], top[4*N];
vector <int> stk[4*N];
typedef long double ld;
//pre[i][j]是i号节点在线段树上第j层加入所属的凸包中时, "取代"的点的编号, last[i][j]是加入前该凸包的大小
void build(int k, int l, int r){
    seg_dep[k] = seg_dep[k >> 1] + 1;
    stk[k].resize(r - l + 3);
    if(l == r) return;
    int mid = (l + r) >> 1;
    build(k << 1, l, mid);
    build(k << 1 | 1, mid + 1, r);
}
bool cmp1(int i, int j, int k){ // 判断(i, j, k)是否构成上凸
    return (ld)(d[j] - d[i]) * (f[k] - f[j]) < (ld)(f[j] - f[i]) * (d[k] - d[j]);
}
int find_pos(int k, int i){ // 在k号单调栈中插入i号点, 应该放在哪个位置
    if(!top[k]) return 1;
    int l = 2, r = top[k] + 1, mid; // 找到第一个和i上凸的位置(新来的i应该取代这个位置)
    while(l < r){
    mid = (l + r) >> 1;
    if(cmp1(stk[k][mid - 1], stk[k][mid], i)) r = mid;
    else l = mid + 1;
    }
    return l;
}
void push(int k, int i){
    int p = find_pos(k, i);
    last_top[i][seg_dep[k]] = top[k];
    pre[i][seg_dep[k]] = stk[k][p];
    top[k] = p;
    stk[k][p] = i;
}
void rollback(int k){
    int i = stk[k][top[k]];
    stk[k][top[k]] = pre[i][seg_dep[k]];
    top[k] = last_top[i][seg_dep[k]];
}
ll calc(int u, int v){
    return f[u] + (d[v] - d[u]) * p[v] + q[v];
}
bool cmp2(int i, int j, ll x){ // 判断i和j构成的斜率是否小于等于x
    return f[j] - f[i] <= (ld) x * (d[j] - d[i]);
}
ll ask(int k, int x){
    int l = 1, r = top[k];
    while(l < r){
    int mid = (l + r) >> 1;
    if(cmp2(stk[k][mid], stk[k][mid + 1], p[x])) l = mid + 1;
    else r = mid;
    }
    return calc(stk[k][l], x);
}
void insert(int k, int l, int r, int p, bool flag){
    flag ? push(k, id[p]) : rollback(k);
    if(l == r) return;
    int mid = (l + r) >> 1;
    if(p <= mid) insert(k << 1, l, mid, p, flag);
    else insert(k << 1 | 1, mid + 1, r, p, flag);
}
ll query(int k, int l, int r, int ql, int qr){
    if(ql <= l && qr >= r) return ask(k, id[qr + 1]);
    int mid = (l + r) >> 1;
    ll ret = 9e18;
    if(ql <= mid) ret = query(k << 1, l, mid, ql, qr);
    if(qr > mid) ret = min(ret, query(k << 1 | 1, mid + 1, r, ql, qr));
    return ret;
}
void dfs(int u){
    d_stk[++d_top] = d[u] = d[fa[u]] + w[u], id[d_top] = u;
    if(u != 1){
    int st = lower_bound(d_stk + 1, d_stk + d_top + 1, d[u] - lim[u]) - d_stk;
    f[u] = query(1, 1, n, st, d_top - 1);
    }
    insert(1, 1, n, d_top, 1);
    for(int v = adj[u]; v; v = nxt[v]) dfs(v);
    insert(1, 1, n, d_top, 0);
    d_top--;
}

int main(){
    read(n), read(T);
    build(1, 1, n);
    for(int i = 2; i <= n; i++){
    read(fa[i]), read(w[i]), read(p[i]), read(q[i]), read(lim[i]);
    nxt[i] = adj[fa[i]], adj[fa[i]] = i;
    }
    dfs(1);
    for(int i = 2; i <= n; i++)
    write(f[i]), enter;
    return 0;
}
```












