# BZOJ 2527 Meteors | 整体二分 - 胡小兔 - 博客园







# [BZOJ 2527 Meteors | 整体二分](https://www.cnblogs.com/RabbitHu/p/BZOJ2527.html)





# [BZOJ 2527](http://www.lydsy.com/JudgeOnline/problem.php?id=2527) Meteors

## 题意

一个圆环上有m个位置，编号为1~m，分别属于n个国家。

有k个时刻，每个时刻都会给圆环上的一个区间中每个位置的值加上一个数。

每个国家有一个目标，问对于每个国家，哪个时刻它拥有的每个位置的数值之和会达到它的目标。

## 题解

这道题也是一道整体二分题。二分到达目标的时间mid，将l~mid的每个修改操作在树状数组中执行，然后看每个当前询问集合中的国家是否达到了目标，达到的放左边，没达到的放右边。

```
#include <cstdio>
#include <cmath>
#include <cstring>
#include <algorithm>
#include <complex>
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
const int N = 300005, INF = 0x3f3f3f3f;
int n, m, K, adj[N], nxt[N], buf1[N], buf2[N], seq[N], ans[N], L[N], R[N], A[N], vis[N], times;
ll tr[N], tar[N];
void single_add(int p, ll x){
    while(p <= m){
    if(vis[p] != times) vis[p] = times, tr[p] = 0;
    tr[p] += x, p += p & -p;
    }
}
void add(int l, int r, ll x){
    if(l <= r) single_add(l, x), single_add(r + 1, -x);
    else add(l, m, x), add(1, r, x);
}
ll ask(int p){
    ll ret = 0;
    for(; p; p -= p & -p)
    if(vis[p] == times)
        ret += tr[p];
    return ret;
}
void solve(int ql, int qr, int l, int r){
    if(ql > qr) return;
    if(l == r){
    for(int i = ql; i <= qr; i++)
        ans[seq[i]] = l;
    return;
    }
    ++times;
    int mid = (l + r) >> 1, p1 = 0, p2 = 0;
    for(int i = l; i <= mid; i++)
    add(L[i], R[i], A[i]);
    for(int i = ql; i <= qr; i++){
    ll sum = 0;
    for(int j = adj[seq[i]]; j && sum < tar[seq[i]]; j = nxt[j])
        sum += ask(j);
    if(sum >= tar[seq[i]]) buf1[++p1] = seq[i];
    else tar[seq[i]] -= sum, buf2[++p2] = seq[i];
    }
    for(int i = 1; i <= p1; i++) seq[ql - 1 + i] = buf1[i];
    for(int i = 1; i <= p2; i++) seq[ql + p1 - 1 + i] = buf2[i];
    solve(ql, ql + p1 - 1, l, mid);
    solve(ql + p1, qr, mid + 1, r);
}
int main(){
    read(n), read(m);
    for(int i = 1, t; i <= m; i++)
    read(t), nxt[i] = adj[t], adj[t] = i;
    for(int i = 1; i <= n; i++)
    read(tar[i]);
    read(K);
    for(int i = 1; i <= K; i++)
    read(L[i]), read(R[i]), read(A[i]);
    L[++K] = 1, R[K] = m, A[K] = INF;
    for(int i = 1; i <= n; i++) seq[i] = i;
    solve(1, n, 1, K);
    for(int i = 1; i <= n; i++)
    if(ans[i] == K) puts("NIE");
    else write(ans[i]), enter;
    return 0;
}
```












