# BZOJ 3524 Couriers | 主席树 - 胡小兔 - 博客园







# [BZOJ 3524 Couriers | 主席树](https://www.cnblogs.com/RabbitHu/p/BZOJ3524.html)





# [BZOJ 3524](http://www.lydsy.com/JudgeOnline/problem.php?id=3524) Couriers

## 题意

求一个区间内出现超过区间长度的一半的数，如果没有则输出0。

## 题解

我可能太菜了吧……这道题愣是没想出来……

维护权值主席树，记录每个数都出现过多少次；

查询的时候，这个数要么在较小的那一部分，要么在较大的那一部分；如果两部分的数的总数都不大于一开始那个“区间长度的一半”，则不存在这样的数。

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

const int N = 500005, M = 20000005;
int n, m, a[N];
int idx, root[N], ls[M], rs[M], data[M];

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
int query(int old, int k, int l, int r, int x){
    if(l == r) return l;
    int mid = (l + r) >> 1;
    if(data[ls[k]] - data[ls[old]] > x) return query(ls[old], ls[k], l, mid, x);
    if(data[rs[k]] - data[rs[old]] > x) return query(rs[old], rs[k], mid + 1, r, x);
    return 0;
}

int main(){

    read(n), read(m);
    build(root[0], 1, n);
    for(int i = 1; i <= n; i++){
        read(a[i]);
        change(root[i - 1], root[i], 1, n, a[i]);
    }
    while(m--){
        int l, r;
        read(l), read(r);
        write(query(root[l - 1], root[r], 1, n, (r - l + 1) / 2)), enter;
    }

    return 0;
}
```












