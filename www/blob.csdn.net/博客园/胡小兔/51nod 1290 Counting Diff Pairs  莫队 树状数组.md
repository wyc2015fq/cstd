# 51nod 1290 Counting Diff Pairs | 莫队 树状数组 - 胡小兔 - 博客园







# [51nod 1290 Counting Diff Pairs | 莫队 树状数组](https://www.cnblogs.com/RabbitHu/p/51nod1290.html)





[51nod 1290](http://www.51nod.com/onlineJudge/questionCode.html#!problemId=1290) Counting Diff Pairs | 莫队 树状数组

## 题面

一个长度为N的正整数数组A，给出一个数K以及Q个查询，每个查询包含2个数l和r，对于每个查询输出从A[i]到A[j]中，有多少对数，abs(A[i] - A[j]) <= K(abs表示绝对值)。

## 题解

莫队！//其实我就是搜索“51nod + 莫队”找到的这道题……

七级算法题！

一道320分！

你值得拥有！

题解就是……用个普通的[莫队](http://www.cnblogs.com/RabbitHu/p/MoDui.html)，加上树状数组来统计符合条件的数个数，就好啦。

当增加/删除一个数的时候，统计能和它组成合法数对的数的个数，然后对答案进行相应的增/减。

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
int n, d, m, a[N], lst[N], idx, tol[N], tor[N], tr[N], pl = 1, pr;
ll res, ans[N];
#define bel(x) (((x) - 1) / B + 1)
struct query {
    int id, l, r;
    bool operator < (const query &b) const {
    return bel(l) == bel(b.l) ? r < b.r : l < b.l;
    }
} q[N];
void init(){
    sort(lst + 1, lst + n + 1);
    idx = unique(lst + 1, lst + n + 1) - lst - 1;
    for(int i = 1; i <= n; i++)
    a[i] = lower_bound(lst + 1, lst + idx + 1, a[i]) - lst;
    int l = 1, r = 1;
    for(int i = 1; i <= idx; i++){
    while(l < i && lst[i] - lst[l] > d) l++;
    while(r < idx && lst[r + 1] - lst[i] <= d) r++;
    tol[i] = l, tor[i] = r;
    }
}
void add(int p, int x){
    while(p <= idx) tr[p] += x, p += p & -p;
}
int ask(int p){
    int res = 0;
    while(p) res += tr[p], p -= p & -p;
    return res;
}
int getres(int x){
    return ask(tor[x]) - ask(tol[x] - 1);
}
int main(){
    read(n), read(d), read(m);
    for(int i = 1; i <= n; i++)
    read(a[i]), lst[i] = a[i];
    init();
    for(int i = 1; i <= m; i++)
    q[i].id = i, read(q[i].l), q[i].l++, read(q[i].r), q[i].r++;
    sort(q + 1, q + m + 1);
    for(int i = 1; i <= m; i++){
    while(pl > q[i].l) res += getres(a[--pl]), add(a[pl], 1);
    while(pr < q[i].r) res += getres(a[++pr]), add(a[pr], 1);
    while(pl < q[i].l) add(a[pl], -1), res -= getres(a[pl++]);
    while(pr > q[i].r) add(a[pr], -1), res -= getres(a[pr--]);
    ans[q[i].id] = res;
    }
    for(int i = 1; i <= m; i++)
    write(ans[i]), enter;
    return 0;
}
```












