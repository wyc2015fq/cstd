# 51nod 1206 Picture 矩形周长求并 | 线段树 扫描线 - 胡小兔 - 博客园







# [51nod 1206 Picture 矩形周长求并 | 线段树 扫描线](https://www.cnblogs.com/RabbitHu/p/51nod1206.html)





# [51nod 1206](https://www.51nod.com/onlineJudge/questionCode.html#!problemId=1206) Picture 矩形周长求并 | 线段树 扫描线

```
#include <cstdio>
#include <cmath>
#include <cstring>
#include <algorithm>
#include <iostream>
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
struct Query {
    int l, r, h, x;
    bool operator < (const Query &b) const{
        return h == b.h ? x > b.x : h < b.h;
        /*
          对于高度相同的修改操作，要让入边在前面，为的是应对这种数据：
          2
          0 0 1 1
          0 1 1 2
        */
    }
} Q[2 * N];
int n, lst[2 * N], tot, cnt[8 * N], len[8 * N], sum[8 * N], lsum[8 * N], rsum[8 * N];
ll ans;
/*
  变量解释：
  lst、tot: 用于离散化
  cnt: 记录线段树上一个节点是否已被覆盖
  len: 记录线段树上一个节点所代表的区间被覆盖了多长
  sum: 记录线段树上一个节点所代表的区间有多少条竖线
  lsum、rsum: 分别记录一个区间左右、端点是否有竖线，因为用儿子更新某父亲节点竖线数时，如果左儿子的右端点和右儿子的左端点都有竖线，那么这两条竖线其实是一条竖线。
 */
void pushup(int k, int l, int r){
    if(cnt[k]) {
        len[k] = lst[r + 1] - lst[l];
        lsum[k] = rsum[k] = 1;
        sum[k] = 2;
    }
    else if(l == r) sum[k] = len[k] = lsum[k] = rsum[k] = 0;
    else{
        len[k] = len[k << 1] + len[k << 1 | 1];
        lsum[k] = lsum[k << 1], rsum[k] = rsum[k << 1 | 1];
        sum[k] = sum[k << 1] + sum[k << 1 | 1];
        if(rsum[k << 1] && lsum[k << 1 | 1]) sum[k] -= 2;
    }
}
void change(int k, int l, int r, int ql, int qr, int x){
    if(ql <= l && qr >= r){
        cnt[k] += x;
        pushup(k, l, r);
        return;
    }
    int mid = (l + r) >> 1;
    if(ql <= mid) change(k << 1, l, mid, ql, qr, x);
    if(qr > mid) change(k << 1 | 1, mid + 1, r, ql, qr, x);
    pushup(k, l, r);
}
int getx(int x){
    return lower_bound(lst + 1, lst + tot + 1, x) - lst;
    //写给自己：注意离散化不要又双叒叕把数组范围打错！不要打成2*n！
}
int main(){
    read(n);
    for(int i = 1, xa, ya, xb, yb; i <= n; i++){
        read(xa), read(ya), read(xb), read(yb);
        lst[2 * i - 1] = xa, lst[2 * i] = xb;
        Q[2 * i - 1] = (Query){xa, xb, ya, 1};
        Q[2 * i] = (Query){xa, xb, yb, -1}; //进行愉快的离散化
    }
    sort(Q + 1, Q + 2 * n + 1);
    sort(lst + 1, lst + 2 * n + 1);
    tot = unique(lst + 1, lst + 2 * n + 1) - lst - 1;
    for(int i = 1, lastlen = 0; i <= 2 * n; i++){
        if(i != 1) ans += (ll) sum[1] * (Q[i].h - Q[i - 1].h);
        change(1, 1, tot, getx(Q[i].l), getx(Q[i].r) - 1, Q[i].x);
        ans += (ll) abs(lastlen - len[1]), lastlen = len[1];
    }
    write(ans), enter;
    return 0;
}
```












