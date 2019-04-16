# BZOJ 3809 Gty的二逼妹子序列 | 莫队 + 分块 - 胡小兔 - 博客园







# [BZOJ 3809 Gty的二逼妹子序列 | 莫队 + 分块](https://www.cnblogs.com/RabbitHu/p/BZOJ3809.html)





# [BZOJ 3809](http://www.lydsy.com/JudgeOnline/problem.php?id=3809) Gty的二逼妹子序列 | 莫队 + 分块

> 
Autumn和Bakser又在研究Gty的妹子序列了！但他们遇到了一个难题。

对于一段妹子们，他们想让你帮忙求出这之内美丽度∈[a,b]的妹子的美丽度的种类数。

为了方便，我们规定妹子们的美丽度全都在[1,n]中。

给定一个长度为n(1<=n<=100000)的正整数序列s(1<=si<=n)，对于m(1<=m<=1000000)次询问“l,r,a,b”，每次输出sl...sr中，权值∈[a,b]的权值的种类数。


莫队+树状数组的方法是可以想到的：维护一个树状数组表示当前区间内的每个权值是否出现过，然后求[a, b]之间有多少出现过；当加入或删除一个数时，如果出现某数数量由０变１或由１变０，则在树状数组内修改。复杂度是\(O(m \sqrt n \log n)\)。

但是这样的复杂度是不够的。能不能把树状数组换成别的呢？

可以发现，在莫队中，修改操作很多（\(O(m \sqrt n)\)）但查询操作较少（\(O(m)\)），有没有修改是\(O(1)\)的算法呢？显然是——分块！

模仿权值树状数组，建立权值分块，统计每个块中包含的权值是否出现，每次修改是\(O(1)\)，而查询是\(O(\sqrt n)\)，与莫队综合起来，复杂度是\(O(n \sqrt n)\)的！

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

const int N = 100005, M = 1000005, B = 340;
int n, m, a[N], cnt[N], bcnt[B], ans[M], pl = 1, pr;
#define st(x) (((x) - 1) * B + 1)
#define ed(x) min(n, (x) * B)
#define bel(x) (((x) - 1) / B + 1)
struct query {
    int id, l, r, a, b;
    bool operator < (const query &b) const {
    return bel(l) == bel(b.l) ? r < b.r : l < b.l;
    }
} q[M];

void add(int p, int x){
    if(x == 1 && !cnt[p]) bcnt[bel(p)]++;
    cnt[p] += x;
    if(x == -1 && !cnt[p]) bcnt[bel(p)]--;
}
int single_ask(int l, int r){
    int ret = 0;
    for(int i = l; i <= r; i++)
    ret += cnt[i] > 0;
    return ret;
}
int ask(int l, int r){
    if(bel(l) == bel(r))
    return single_ask(l, r);
    int ret = single_ask(l, ed(bel(l))) + single_ask(st(bel(r)), r);
    for(int i = bel(l) + 1; i < bel(r); i++)
    ret += bcnt[i];
    return ret;
}

int main(){

    read(n), read(m);
    for(int i = 1; i <= n; i++) read(a[i]);
    for(int i = 1; i <= m; i++)
    q[i].id = i, read(q[i].l), read(q[i].r), read(q[i].a), read(q[i].b);
    sort(q + 1, q + m + 1);
    for(int i = 1; i <= m; i++){
    while(pl > q[i].l) add(a[--pl], 1);
    while(pr < q[i].r) add(a[++pr], 1);
    while(pl < q[i].l) add(a[pl++], -1);
    while(pr > q[i].r) add(a[pr--], -1);
    ans[q[i].id] = ask(q[i].a, q[i].b);
    }
    for(int i = 1; i <= m; i++)
    write(ans[i]), enter;
    
    return 0;
}
```












