# Codeforces 438D (今日gg模拟第二题) | 线段树 考察时间复杂度的计算 -_-||| - 胡小兔 - 博客园







# [Codeforces 438D (今日gg模拟第二题) | 线段树 考察时间复杂度的计算 -_-|||](https://www.cnblogs.com/RabbitHu/p/codeforces438D.html)





# [Codeforces 438D](http://codeforces.com/problemset/problem/438/D) The Child and Sequence

> 
给出一个序列，进行如下三种操作：
- 区间求和
- 区间每个数模x
- 单点修改


如果没有第二个操作的话，就是一棵简单的线段树。那么如何处理这个第二个操作呢？

对于一个数a，如果模数 x > a ，则这次取模是没有意义的，直接跳过；

如果 x > a/2 ，则取模结果小于 a / 2; 如果 x < a / 2，取模结果小于x，则也小于 a / 2。

所以对于一个数，最多只会做log a次取模操作。这是可以接受的！

对于一个区间，维护最大值，如果模数x > 最大值，直接跳过即可。

否则继续往下像单点修改一样。

时间复杂度不会超过 \(O(n \log ^2 n)\) ……吧。

```
#include <cstdio>
#include <cstring>
#include <algorithm>
using namespace std;
typedef long long ll;
#define INF 0x3f3f3f3f
#define space putchar(' ')
#define enter putchar('\n')
template <class T>
bool read(T &x){
    char c;
    bool op = 0;
    while(c = getchar(), c < '0' || c > '9')
        if(c == '-') op = 1;
        else if(c == EOF) return 0;
    x = c - '0';
    while(c = getchar(), c >= '0' && c <= '9')
        x = x * 10 + c - '0';
    if(op) x = -x;
    return 1;
}
template <class T>
void write(T x){
    if(x < 0) putchar('-'), x = -x;
    if(x >= 10) write(x / 10);
    putchar('0' + x % 10);
}

const int N = 100005;
int n, m, a[N], ma[4*N];
ll sum[4*N];

void build(int k, int l, int r){
    if(l == r) return (void) (ma[k] = sum[k] = a[l]);
    int mid = (l + r) >> 1;
    build(k << 1, l, mid);
    build(k << 1 | 1, mid + 1, r);
    ma[k] = max(ma[k << 1], ma[k << 1 | 1]);
    sum[k] = sum[k << 1] + sum[k << 1 | 1];
}
void change(int k, int l, int r, int p, int x){
    if(l == r) return (void) (ma[k] = sum[k] = x);
    int mid = (l + r) >> 1;
    if(p <= mid) change(k << 1, l, mid, p, x);
    else change(k << 1 | 1, mid + 1, r, p, x);
    ma[k] = max(ma[k << 1], ma[k << 1 | 1]);
    sum[k] = sum[k << 1] + sum[k << 1 | 1];
}
void mo(int k, int l, int r, int ql, int qr, int x){
    if(ma[k] < x) return;
    if(l == r) return (void) (ma[k] %= x, sum[k] %= x);
    int mid = (l + r) >> 1;
    if(ql <= mid) mo(k << 1, l, mid, ql, qr, x);
    if(qr > mid) mo(k << 1 | 1, mid + 1, r, ql, qr, x);
    ma[k] = max(ma[k << 1], ma[k << 1 | 1]);
    sum[k] = sum[k << 1] + sum[k << 1 | 1];
}
ll query(int k, int l, int r, int ql, int qr){
    if(ql <= l && qr >= r) return sum[k];
    int mid = (l + r) >> 1;
    ll ret = 0;
    if(ql <= mid) ret += query(k << 1, l, mid, ql, qr);
    if(qr > mid) ret += query(k << 1 | 1, mid + 1, r, ql, qr);
    return ret;
}

int main(){
    read(n), read(m);
    for(int i = 1; i <= n; i++) read(a[i]);
    build(1, 1, n);
    int op, l, r, x;
    for(int i = 1; i <= m; i++){
        read(op);
        if(op == 1){
            read(l), read(r);
            write(query(1, 1, n, l, r)), enter;
        }
        else if(op == 2){
            read(l), read(r), read(x);
            mo(1, 1, n, l, r, x);
        }
        else{
            read(l), read(x);
            change(1, 1, n, l, x);
        }
    }
    return 0;
}
```












