# 少年，想学带修改主席树吗 | BZOJ1901 带修改区间第k小 - 胡小兔 - 博客园







# [少年，想学带修改主席树吗 | BZOJ1901 带修改区间第k小](https://www.cnblogs.com/RabbitHu/p/BZOJ1901.html)





# 少年，想学带修改主席树吗 | BZOJ1901 带修改区间第k小

有一道题（[BZOJ 1901](http://www.lydsy.com/JudgeOnline/problem.php?id=1901)）是这样的：n个数，m个询问，询问有两种：修改某个数/询问区间第k小。

不带修改的区间第k小用主席树很好写，不会的同学可以看一下[这个](http://www.cnblogs.com/RabbitHu/p/segtree.html)。

加上修改怎么做呢？我们可以用数学老师成天讲的类比思想：

可以发现，不修改的区间k小问题中，每加入一个原序列中的数，对应的主席树在上一个的基础上进行修改，而查询的时候用右端点主席树减去左端点左边的主席树。这样的操作就像是维护前缀和：每次加入一个元素的时候，sum[i] = sum[i - 1] + a[i]；询问的时候，则是sum[r] - sum[l - 1]。

sum数组可以用来不带修改求前缀和，那么假如我们要求带修改的前缀和呢？树状数组可以做到。每次在位置p加入一个元素x的时候，对树状数组中每个 {p, p + (p & -p), ...} 都加上x；询问前缀和的时候，则是求树状数组中每个 {p, p - (p & -p), ...} 的和。

现在我们来看看能否将树状数组和主席树结合起来，实现动态修改。

一开始，树状数组的每个位置都对应着一棵主席树——虽然实际上它们对应的都是同一棵空空荡荡的主席树。

然后我们该往里面加数了：当在位置p加入一个数x的时候，对树状数组中位置 {p, p + (p & -p), ...} 上的主席树都进行“加入数x”的操作，也就是把数x对应的位置++。

然后可以处理询问了。对于查询，可以采用非递归的方式：维护两个cur数组，分别记录左端点（的左边）对应的主席树 {l, l - (l & -l), ...} 上的当前节点和右端点对应主席树 {r, r - (r & -r), ...} 上的当前节点。每次对两个cur数组上记录的节点的data求和，然后相减，可以得到区间内的data。这个操作实际上就是树状数组求前缀和的操作。

询问的代码如下（写得比较臃肿……但是意思应该是比较清楚的了）：

```
int query(int ql, int qr, int k){
    int l = 1, r = idx;
    for(int p = ql; p; p -= p & -p) cur1[p] = root[p];
    for(int p = qr; p; p -= p & -p) cur2[p] = root[p];
    while(l < r){
        int mid = (l + r) >> 1, sum1 = 0, sum2 = 0;
        for(int p = ql; p; p -= p & -p) sum1 += data[ls[cur1[p]]];
        for(int p = qr; p; p -= p & -p) sum2 += data[ls[cur2[p]]];
        if(sum2 - sum1 >= k){
            for(int p = ql; p; p -= p & -p) cur1[p] = ls[cur1[p]];
            for(int p = qr; p; p -= p & -p) cur2[p] = ls[cur2[p]];
            r = mid;
        }
        else{
            l = mid + 1, k -= sum2 - sum1;
            for(int p = ql; p; p -= p & -p) cur1[p] = rs[cur1[p]];
            for(int p = qr; p; p -= p & -p) cur2[p] = rs[cur2[p]];
        }
    }
    return lst[l];
}
```

那么对于修改操作呢？当然是和初始化时加入每个数时一样，对于树状数组中每个 {p, p + (p & -p), ...} 位置上的主席树都进行修改咯。

```
void change(int old, int &k, int l, int r, int p, int x){
    k = ++tot;
    data[k] = data[old] + x, ls[k] = ls[old], rs[k] = rs[old];
    if(l == r) return;
    int mid = (l + r) >> 1;
    if(p <= mid) change(ls[old], ls[k], l, mid, p, x);
    else change(rs[old], rs[k], mid + 1, r, p, x);
}
void add(int p, int num, int x){
    while(p <= n) change(root[p], root[p], 1, idx, num, x), p += p & -p;
}
```

那么这道题就做完啦。

下面是完整的代码，加上近三十行的读入优化后也只有100行，比起学长让我近期写的各种树套树……应该算是比较短小好写的吧。

树状数组天下第一！

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
const int N = 100005, M = 6000005;
int n, m, a[N], lst[N], idx;
int tot, root[N], data[M], ls[M], rs[M], cur1[N], cur2[N];
int qtype[N], q1[N], q2[N], q3[N];
void build(int &k, int l, int r){
    k = ++tot;
    if(l == r) return;
    int mid = (l + r) >> 1;
    build(ls[k], l, mid);
    build(rs[k], mid + 1, r);
}
void change(int old, int &k, int l, int r, int p, int x){
    k = ++tot;
    data[k] = data[old] + x, ls[k] = ls[old], rs[k] = rs[old];
    if(l == r) return;
    int mid = (l + r) >> 1;
    if(p <= mid) change(ls[old], ls[k], l, mid, p, x);
    else change(rs[old], rs[k], mid + 1, r, p, x);
}
void add(int p, int num, int x){
    while(p <= n) change(root[p], root[p], 1, idx, num, x), p += p & -p;
}
int query(int ql, int qr, int k){
    int l = 1, r = idx;
    for(int p = ql; p; p -= p & -p) cur1[p] = root[p];
    for(int p = qr; p; p -= p & -p) cur2[p] = root[p];
    while(l < r){
        int mid = (l + r) >> 1, sum1 = 0, sum2 = 0;
        for(int p = ql; p; p -= p & -p) sum1 += data[ls[cur1[p]]];
        for(int p = qr; p; p -= p & -p) sum2 += data[ls[cur2[p]]];
        if(sum2 - sum1 >= k){
            for(int p = ql; p; p -= p & -p) cur1[p] = ls[cur1[p]];
            for(int p = qr; p; p -= p & -p) cur2[p] = ls[cur2[p]];
            r = mid;
        }
        else{
            l = mid + 1, k -= sum2 - sum1;
            for(int p = ql; p; p -= p & -p) cur1[p] = rs[cur1[p]];
            for(int p = qr; p; p -= p & -p) cur2[p] = rs[cur2[p]];
        }
    }
    return lst[l];
}
int getpos(int x){
    return lower_bound(lst + 1, lst + idx + 1, x) - lst;
}
bool isQ(){
    char c;
    while(c = getchar(), c != 'Q' && c != 'C');
    return c == 'Q';
}
int main(){
    read(n), read(m), idx = n;
    for(int i = 1; i <= n; i++)
        read(a[i]), lst[i] = a[i];
    for(int i = 1; i <= m; i++){
        qtype[i] = isQ(), read(q1[i]), read(q2[i]);
        if(qtype[i]) read(q3[i]);
        else lst[++idx] = q2[i];
    }
    sort(lst + 1, lst + idx + 1);
    idx = unique(lst + 1, lst + idx + 1) - lst - 1;
    build(root[0], 1, idx);
    for(int i = 1; i <= n; i++) root[i] = root[0];
    for(int i = 1; i <= n; i++) add(i, getpos(a[i]), 1);
    for(int i = 1; i <= m; i++){
        if(qtype[i]) write(query(q1[i] - 1, q2[i], q3[i])), enter;
        else{
            add(q1[i], getpos(a[q1[i]]), -1);
            a[q1[i]] = q2[i];
            add(q1[i], getpos(a[q1[i]]), 1);
        }
    }
    return 0;
}
```












