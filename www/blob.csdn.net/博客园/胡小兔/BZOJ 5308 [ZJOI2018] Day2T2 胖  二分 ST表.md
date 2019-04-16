# BZOJ 5308 [ZJOI2018] Day2T2 胖 | 二分 ST表 - 胡小兔 - 博客园







# [BZOJ 5308 [ZJOI2018] Day2T2 胖 | 二分 ST表](https://www.cnblogs.com/RabbitHu/p/BZOJ5308.html)





## 题目链接

[LOJ 2529](https://loj.ac/problem/2529)

[BZOJ 5308](https://www.lydsy.com/JudgeOnline/problem.php?id=5308)

## 题解

这么简单的题

为什么考场上我完全想不清楚 = =

对于k个关键点中的每一个关键点\(a\)，二分它能一度成为哪些点的最短路起点（显然这些点在一段包含\(a\)的连续区间中），所以二分这个区间的左右端点。

如何判断某个点\(p\)是否在这个区间内呢？设\(d = |a - p|\)，则\(a\)可以更新\(p\)当且仅当区间\([p - d, p + d]\)中的关键点到\(p\)的距离没有比\(a\)更优的。

设\(sum_i\)点\(i\)到点\(1\)的距离，用ST表维护一个关键点区间中\((sum_i - l)_{\max}\)和\((sum_i + l)_{\min}\)，则\([p - d, p + d]\)中对于\(p\)的最优关键点可能是\([p - d, p]\)中\(sum_i - l\)最大的或\((p, p+d]\)中\(sum_i + l\)最小的。将他俩与\(a\)比较，如果\(a\)仍然是最优的，则\(a\)可以一度占领\(p\)这个点。

注意若两个关键点到\(p\)的距离相同，只能算一个，那么人为规定一下【离\(p\)边数最小的】是最优的，如果两个点这一项也一样，则取编号较小的即可。

```
#include <cstdio>
#include <cstring>
#include <cmath>
#include <algorithm>
#include <iostream>
#define enter putchar('\n')
#define space putchar(' ')
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
    if(op == 1) x = -x;
}
template <class T>
void write(T x){
    if(x < 0) putchar('-'), x = -x;
    if(x >= 10) write(x / 10);
    putchar('0' + x % 10);
}

const int N = 200005;
const ll INF = 1e18 + 233;
int n, m, K, lg[N];
ll sum[N], st1[N][20], st2[N][20], l1[N], l2[N];
struct Edge {
    int a;
    ll l;
    Edge(){}
    Edge(int x, ll y): a(x), l(y){}
    bool operator < (const Edge &b) const {
        return a < b.a;
    }
} e[N];

int Max(int a, int b){
    if(l1[a] == l1[b]) return max(a, b);
    return l1[a] > l1[b] ? a : b;
}
int Min(int a, int b){
    if(l2[a] == l2[b]) return min(a, b);
    return l2[a] < l2[b] ? a : b;
}
void init(){
    sort(e + 1, e + K + 1);
    for(int i = 1; i <= K; i++){
        l1[i] = sum[e[i].a] - e[i].l, l2[i] = sum[e[i].a] + e[i].l;
        st1[i][0] = st2[i][0] = i;
    }
    for(int j = 1; (1 << j) <= K; j++)
        for(int i = 1; i + (1 << j) - 1 <= K; i++){
            st1[i][j] = Max(st1[i][j - 1], st1[i + (1 << (j - 1))][j - 1]);
            st2[i][j] = Min(st2[i][j - 1], st2[i + (1 << (j - 1))][j - 1]);
        }
}
int get1(int l, int r){
    int j = lg[r - l + 1];
    return Max(st1[l][j], st1[r - (1 << j) + 1][j]);
}
int get2(int l, int r){
    int j = lg[r - l + 1];
    return Min(st2[l][j], st2[r - (1 << j) + 1][j]);
}
int better(int a, int b, int p){
    ll ans1 = e[a].l + abs(sum[p] - sum[e[a].a]);
    ll ans2 = e[b].l + abs(sum[p] - sum[e[b].a]);
    if(ans1 != ans2) return ans1 < ans2 ? a : b;
    if(abs(e[a].a - p) != abs(e[b].a - p))
        return abs(e[a].a - p) < abs(e[b].a - p) ? a : b;
    return min(a, b);
}
bool bel(int p, int k){
    int d = abs(e[k].a - p), best = k;
    int l = lower_bound(e + 1, e + K + 1, Edge(p - d, 0)) - e;
    int r = upper_bound(e + 1, e + K + 1, Edge(p + d, 0)) - e - 1;
    int mid = upper_bound(e + 1, e + K + 1, Edge(p, 0)) - e - 1;
    if(l <= mid)  best = better(best, get1(l, mid), p);
    if(mid < r) best = better(best, get2(mid + 1, r), p);
    return best == k;
}
ll solve(){
    ll ans = 0;
    for(int i = 1; i <= K; i++){
        int l = 1, r = e[i].a, mid, L;
        while(l < r)
            if(bel(mid = (l + r) >> 1, i)) r = mid;
            else l = mid + 1;
        L = l, l = e[i].a, r = n;
        while(l < r)
            if(bel(mid = (l + r + 1) >> 1, i)) l = mid;
            else r = mid - 1;
        ans += (r - L + 1);
    }
    return ans;
}

int main(){

    for(int i = 2; i < N; i++)
        lg[i] = lg[i >> 1] + 1;
    read(n), read(m);
    for(int i = 2; i <= n; i++)
        read(sum[i]), sum[i] += sum[i - 1];
    while(m--){
        read(K);
        for(int i = 1; i <= K; i++)
            read(e[i].a), read(e[i].l);
        init();
        write(solve()), enter;
    }

    return 0;
}
```












