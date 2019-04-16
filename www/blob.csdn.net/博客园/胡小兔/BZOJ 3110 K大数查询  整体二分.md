# BZOJ 3110 K大数查询 | 整体二分 - 胡小兔 - 博客园







# [BZOJ 3110 K大数查询 | 整体二分](https://www.cnblogs.com/RabbitHu/p/BZOJ3110.html)





[BZOJ 3110](http://www.lydsy.com/JudgeOnline/problem.php?id=3110) K大数查询

## 题面

有N个位置，M个操作。操作有两种，每次操作如果是1 a b c的形式表示在第a个位置到第b个位置，每个位置加入一个数c

如果是2 a b c形式，表示询问从第a个位置到第b个位置，第C大的数是多少。

## 题解

这道题一个好写的做法是“整体二分”。

我做的上一道整体二分的题目在[这里](http://www.cnblogs.com/RabbitHu/p/BZOJ2738.html)。

整体二分的主要过程是：二分答案，然后按照 答案比当前mid小/答案比当前mid大 将询问分为两组，一组放在左边，另一组放在右边，然后递归进行左右两边的二分。

这道题有修改操作，但是也没什么关系，不要被吓住了 =_=

我们的分治策略保证了当前要处理的的询问在时间上是单调的。于是我们从左到右枚举当前要处理的询问，如果是修改操作，则：若修改的c <= mid，则在树状数组上直接进行修改，同时把这个询问放在左边；否则直接放到右边，不修改；如果是查询操作，则看对应区间大于等于当前mid的数的个数（就是树状数组内记录的区间和）是否超过k，决定这个询问应该放在哪一边。

关于树状数组如何记录区间和，请访问我的[这篇博客](http://www.cnblogs.com/RabbitHu/p/BIT.html)。

这道题要注意每次进入分治函数，树状数组都是要清空的，然而我们不能直接memset（会T），于是记录树状数组上每一位是否在本轮更新过（vis数组），如果没更新过，先清零再操作。

下面是我的代码。因为这道题求第k“大”很难受，于是我预处理，把数字重新排序，变成了第k小问题……于是代码长了些。

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
const int N = 50005;
int n, m, times, vis[N], idx, seq[N], buf1[N], buf2[N];
ll ta[N], tb[N], lst[N], ans[N];
struct query{
    int type, l, r;
    ll k;
} q[N];
void single_add(int p, ll x){
    for(int i = p; i <= n; i += i & -i){
    if(vis[i] != times)
        vis[i] = times, ta[i] = tb[i] = 0;
    ta[i] += x, tb[i] += p * x;
    }
}
void add(int l, int r, ll x){
    single_add(l, x);
    single_add(r + 1, -x);
}
ll single_ask(int p){
    ll ret = 0;
    for(int i = p; i; i -= i & -i)
    if(vis[i] == times)
        ret += (p + 1) * ta[i] - tb[i];
    return ret;
}
ll ask(int l, int r){
    return single_ask(r) - single_ask(l - 1);
}
void solve(int ql, int qr, int l, int r){
    if(ql > qr) return;
    if(l == r){
    for(int i = ql; i <= qr; i++)
        ans[seq[i]] = lst[l];
    return;
    }
    times++;
    int mid = (l + r) >> 1, p1 = 0, p2 = 0;
    for(int i = ql; i <= qr; i++){
    if(q[seq[i]].type == 1){
        if(q[seq[i]].k <= mid){
        add(q[seq[i]].l, q[seq[i]].r, 1), buf1[++p1] = seq[i];
        }
        else buf2[++p2] = seq[i];
    }
    else{
        ll sum = ask(q[seq[i]].l, q[seq[i]].r);
        if(sum < q[seq[i]].k) buf2[++p2] = seq[i], q[seq[i]].k -= sum;
        else buf1[++p1] = seq[i];
    }
    }
    for(int i = 1; i <= p1; i++) seq[ql + i - 1] = buf1[i];
    for(int i = 1; i <= p2; i++) seq[ql + p1 - 1 + i] = buf2[i];
    solve(ql, ql + p1 - 1, l, mid);
    solve(ql + p1, qr, mid + 1, r);
}
int main(){
    read(n), read(m);
    for(int i = 1; i <= m; i++){
    read(q[i].type), read(q[i].l), read(q[i].r), read(q[i].k);
    if(q[i].type == 1) lst[++idx] = q[i].k;
    }
    sort(lst + 1, lst + idx + 1);
    idx = unique(lst + 1, lst + idx + 1) - lst - 1;
    for(int i = 1; i <= m; i++)
    if(q[i].type == 1)
        q[i].k = idx + 1 - (lower_bound(lst + 1, lst + idx + 1, q[i].k) - lst);
    for(int i = 1, j = idx; i < j; i++, j--) swap(lst[i], lst[j]);
    for(int i = 1; i <= m; i++) seq[i] = i;
    solve(1, m, 1, idx);
    for(int i = 1; i <= m; i++)
    if(q[i].type == 2)
        write(ans[i]), enter;
    return 0;
}
```












