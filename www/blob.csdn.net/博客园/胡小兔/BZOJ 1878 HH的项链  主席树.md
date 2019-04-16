# BZOJ 1878 HH的项链 | 主席树 - 胡小兔 - 博客园







# [BZOJ 1878 HH的项链 | 主席树](https://www.cnblogs.com/RabbitHu/p/BZOJ1878.html)





## 题意

询问区间有多少不同的数。

## 题解

和[Luogu 1903](http://www.cnblogs.com/RabbitHu/p/Luogu1903.html)一样，这道题也是用pre数组来求区间不同数的个数，这里pre[i]表示a[i]上一次出现的位置 +１，询问相当于查询区间内有多少pre小于等于左端点。

```
#include <cstdio>
#include <cstring>
#include <algorithm>
#include <set>
using namespace std;
typedef long long ll;
#define space putchar(' ')
#define enter putchar('\n')
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
    if(x < 0) x = -x, putchar('-');
    if(x >= 10) write(x / 10);
    putchar('0' + x % 10);
}
const int N = 50005;
int n, m, l, r, ans, a[N], last[1000005], pre[N];
int idx, ls[N*50], rs[N*50], data[N*50], root[N];
void build(int &k, int l, int r){
    k = ++idx;
    if(l == r) return;
    int mid = (l + r) >> 1;
    build(ls[k], l, mid);
    build(rs[k], mid + 1, r);
}
void change(int old, int &k, int l, int r, int p, int x){
    k = ++idx;
    ls[k] = ls[old], rs[k] = rs[old], data[k] = data[old];
    if(l == r) return (void)(data[k] += x);
    int mid = (l + r) >> 1;
    if(p <= mid) change(ls[old], ls[k], l, mid, p, x);
    else change(rs[old], rs[k], mid + 1, r, p, x);
    data[k] = data[ls[k]] + data[rs[k]];
}
int query(int old, int k, int l, int r, int ql, int qr){
    if(ql <= l && qr >= r) return data[k] - data[old];
    int mid = (l + r) >> 1, ret = 0;
    if(ql <= mid) ret += query(ls[old], ls[k], l, mid, ql, qr);
    if(qr > mid) ret += query(rs[old], rs[k], mid + 1, r, ql, qr);
    return ret;
}
int main(){
    read(n);
    build(root[0], 1, n);
    for(int i = 1; i <= n; i++){
    read(a[i]);
    pre[i] = last[a[i]] + 1;
    last[a[i]] = i;
    change(root[i - 1], root[i], 1, n, pre[i], 1);
    }
    read(m);
    while(m--){
    read(l), read(r);
    write(query(root[l - 1], root[r], 1, n, 1, l)), enter;
    }
    return 0;
}
```












