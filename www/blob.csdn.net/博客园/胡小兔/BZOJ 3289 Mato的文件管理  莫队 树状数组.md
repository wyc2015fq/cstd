# BZOJ 3289 Mato的文件管理 | 莫队 树状数组 - 胡小兔 - 博客园







# [BZOJ 3289 Mato的文件管理 | 莫队 树状数组](https://www.cnblogs.com/RabbitHu/p/BZOJ3289.html)





# [BZOJ 3289](http://www.lydsy.com/JudgeOnline/problem.php?id=3289) Mato的文件管理

## 题意

求区间逆序对。

## 题解

在莫队的基础上使用树状数组求逆序对。

在当前区间左侧加入一个数时，res += 原区间比它小的数的个数；

在当前区间右侧加入一个数时，res += 原区间比它大的数的个数；

在当前区间左侧删除一个数时，res -= 原区间比它小的数的个数；

在当前区间右侧删除一个数时，res -= 原区间比它大的数的个数；

```
#include <cstdio>
#include <cmath>
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
#define space putchar(' ')
#define enter putchar('\n')

const int N = 50005, B = 233;
int n, m, a[N], lst[N], idx, tr[N], pl = 1, pr;
ll res, ans[N];
#define bel(x) (((x) - 1) / B + 1)
struct query {
    int id, l, r;
    bool operator < (const query &b) const {
    return bel(l) == bel(b.l) ? r < b.r : l < b.l;
    }
} q[N];
void add(int p, int x){
    while(p <= idx) tr[p] += x, p += p & -p;
}
int ask(int p){
    int res = 0;
    while(p) res += tr[p], p -= p & -p;
    return res;
}
int main(){
    read(n);
    for(int i = 1; i <= n; i++)
    read(a[i]), lst[i] = a[i];
    sort(lst + 1, lst + n + 1);
    idx = unique(lst + 1, lst + n + 1) - lst - 1;
    for(int i = 1; i <= n; i++)
    a[i] = lower_bound(lst + 1, lst + idx + 1, a[i]) - lst;
    read(m);
    for(int i = 1; i <= m; i++)
    q[i].id = i, read(q[i].l), read(q[i].r);
    sort(q + 1, q + m + 1);
    for(int i = 1; i <= m; i++){
    while(pl > q[i].l) res += ask(a[--pl] - 1), add(a[pl], 1);
    while(pr < q[i].r) res += ask(idx) - ask(a[++pr]), add(a[pr], 1);
    while(pl < q[i].l) add(a[pl], -1), res -= ask(a[pl++] - 1);
    while(pr > q[i].r) add(a[pr], -1), res -= ask(idx) - ask(a[pr--]);
    ans[q[i].id] = res;
    }
    for(int i = 1; i <= m; i++)
    write(ans[i]), enter;
    return 0;
}
```












