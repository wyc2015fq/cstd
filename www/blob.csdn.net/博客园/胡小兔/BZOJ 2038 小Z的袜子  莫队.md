# BZOJ 2038 小Z的袜子 | 莫队 - 胡小兔 - 博客园







# [BZOJ 2038 小Z的袜子 | 莫队](https://www.cnblogs.com/RabbitHu/p/BZOJ2038.html)





# [BZOJ 2038](http://www.lydsy.com/JudgeOnline/problem.php?id=2038) 小Z的袜子

莫队（普通版……）最大的优点可能就是好写？这道莫队棵题是我吃完午饭15分钟写完的……可能大佬们写得要更快吧。

## 题意

给出一个序列，每次询问一个区间，问：在区间内随机取出两个数，相同的概率是多少。

## 题解

莫队棵题！

维护cnt[]数组，表示一个数在当前区间内出现了多少次。

维护res，记录当前区间内有多少对相同的数。

当加入一个数x时，res += cnt[x]++;

当删除一个数x时，res -= --cnt[x]。

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
int n, m, pl = 1, pr = 0, ql[N], qr[N], a[N], cnt[N];
ll res, ans[N], all;
#define bel(x) (((x) - 1) / B + 1)
struct query {
    int id, l, r;
    bool operator < (const query &b) const {
    return bel(l) == bel(b.l) ? r < b.r: l < b.l;
    }
} q[N];
void add(int x){res += cnt[x]++;}
void del(int x){res -= --cnt[x];}
ll gcd(ll a, ll b){return b ? gcd(b, a % b) : a;}
int main(){
    read(n), read(m);
    for(int i = 1; i <= n; i++) read(a[i]);
    for(int i = 1; i <= m; i++)
    q[i].id = i, read(ql[i]), read(qr[i]), q[i] = (query){i, ql[i], qr[i]};
    sort(q + 1, q + m + 1);
    for(int i = 1; i <= m; i++){
    while(pl > q[i].l) add(a[--pl]);
    while(pr < q[i].r) add(a[++pr]);
    while(pl < q[i].l) del(a[pl++]);
    while(pr > q[i].r) del(a[pr--]);
    ans[q[i].id] = res;
    }
    for(int i = 1; i <= m; i++){
    all = ((ll)qr[i] - ql[i]) * (qr[i] - ql[i] + 1) / 2;
    ll g = gcd(all, ans[i]);
    ans[i] /= g, all /= g;
    write(ans[i]), putchar('/'), write(all), enter;
    }
    return 0;
}
```












