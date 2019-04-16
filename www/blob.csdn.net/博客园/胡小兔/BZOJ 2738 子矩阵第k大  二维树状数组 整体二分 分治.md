# BZOJ 2738 子矩阵第k大 | 二维树状数组 整体二分 分治 - 胡小兔 - 博客园







# [BZOJ 2738 子矩阵第k大 | 二维树状数组 整体二分 分治](https://www.cnblogs.com/RabbitHu/p/BZOJ2738.html)





# [BZOJ 2738](http://www.lydsy.com/JudgeOnline/problem.php?id=2738) “矩阵乘法”（子矩阵第k大）

## 题意

给出一个矩阵，多次询问子矩阵中第k大的数是多少。

## 题解

我做这道题之前先照着这道题出了一道题，是这道题的一维版本，在这里：[https://vijos.org/d/contest/p/5a26541bd3d8a11cef1706aa](https://vijos.org/d/contest/p/5a26541bd3d8a11cef1706aa)。

思路是这样的：二分答案mid，将所有小于mid的位置都在树状数组上 +1，对于每个询问，如果子矩阵所有”+1“之和 >= 这个询问的k，则把询问划分到”左边那一组”，否则划分到“右边那一组”，之后对它们分别处理。

核心代码：

```
void solve(int l, int r, int ql, int qr){
    if(ql > qr) return;
    if(l == r){
    for(int i = ql; i <= qr; i++)
        ans[q[i].id] = a[l].val;
    return;
    }
    int mid = (l + r) >> 1, cnt = 0;
    for(int i = l; i <= mid; i++)
    add(a[i].x, a[i].y, 1);
    for(int i = ql; i <= qr; i++){
    sum[i] = ask(q[i]);
    if(sum[i] >= q[i].k) cnt++;
    }
    for(int i = ql, pl = ql, pr = ql + cnt; i <= qr; i++)
    if(sum[i] >= q[i].k) tmp[pl++] = q[i];
    else q[i].k -= sum[i], tmp[pr++] = q[i];
    for(int i = ql; i <= qr; i++) q[i] = tmp[i];
    for(int i = l; i <= mid; i++)
    add(a[i].x, a[i].y, -1);
    solve(l, mid, ql, ql + cnt - 1);
    solve(mid + 1, r, ql + cnt, qr);
}
```

完整代码：

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

const int N = 505, M = 60005;
int n, m, tr[N][N], ans[M], idx, sum[M];
struct element {
    int x, y, val;
    bool operator < (const element &b) const{
    return val < b.val;
    }
} a[N*N];
struct query {
    int id, xa, ya, xb, yb, k;
} q[M], tmp[M];

void add(int x, int y, int val){
    for(int px = x; px <= n; px += px & -px)
    for(int py = y; py <= n; py += py & -py)
        tr[px][py] += val;
}
int single_ask(int x, int y){
    int ret = 0;
    for(int px = x; px; px -= px & -px)
    for(int py = y; py; py -= py & -py)
        ret += tr[px][py];
    return ret;
}
int ask(query Q){
    return single_ask(Q.xb, Q.yb) - single_ask(Q.xa - 1, Q.yb) - single_ask(Q.xb, Q.ya - 1) + single_ask(Q.xa - 1, Q.ya - 1);
}
void solve(int l, int r, int ql, int qr){
    if(ql > qr) return;
    if(l == r){
    for(int i = ql; i <= qr; i++)
        ans[q[i].id] = a[l].val;
    return;
    }
    int mid = (l + r) >> 1, cnt = 0;
    for(int i = l; i <= mid; i++)
    add(a[i].x, a[i].y, 1);
    for(int i = ql; i <= qr; i++){
    sum[i] = ask(q[i]);
    if(sum[i] >= q[i].k) cnt++;
    }
    for(int i = ql, pl = ql, pr = ql + cnt; i <= qr; i++)
    if(sum[i] >= q[i].k) tmp[pl++] = q[i];
    else q[i].k -= sum[i], tmp[pr++] = q[i];
    for(int i = ql; i <= qr; i++) q[i] = tmp[i];
    for(int i = l; i <= mid; i++)
    add(a[i].x, a[i].y, -1);
    solve(l, mid, ql, ql + cnt - 1);
    solve(mid + 1, r, ql + cnt, qr);
}

int main(){
    read(n), read(m);
    for(int i = 1; i <= n; i++)
    for(int j = 1; j <= n; j++)
        a[++idx].x = i, a[idx].y = j, read(a[idx].val);
    sort(a + 1, a + idx + 1);
    for(int i = 1; i <= m; i++)
    q[i].id = i, read(q[i].xa), read(q[i].ya), read(q[i].xb), read(q[i].yb), read(q[i].k);
    solve(1, idx, 1, m);
    for(int i = 1; i <= m; i++)
    write(ans[i]), enter;
    return 0;
}
```












