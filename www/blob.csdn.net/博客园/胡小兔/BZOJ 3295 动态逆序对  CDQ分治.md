# BZOJ 3295 动态逆序对 | CDQ分治 - 胡小兔 - 博客园







# [BZOJ 3295 动态逆序对 | CDQ分治](https://www.cnblogs.com/RabbitHu/p/BZOJ3295.html)





# [BZOJ 3295](http://www.lydsy.com/JudgeOnline/problem.php?id=3295) 动态逆序对

这道题和三维偏序很类似。某个元素加入后产生的贡献 = time更小、pos更小、val更大的元素个数 + time更小、pos更大、val更小的元素个数。

分别用类似CDQ分治求三维偏序的方法求即可。

```
#include <cstdio>
#include <cstring>
#include <algorithm>
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
    if(x < 0) putchar('-'), x = -x;
    if(x >= 10) write(x / 10);
    putchar('0' + x % 10);
}

const int N = 1000005;
ll n, m, tr[N];
ll ans[N];
struct Point {
    ll pos, val, tim;
    bool operator < (const Point &b) const{
        return tim < b.tim;
    }
} p[N];
bool cmp(const Point &a, const Point &b){
    return a.val > b.val;
}
void add(int p, ll x){
    while(p <= n) tr[p] += x, p += p & -p;
}
ll ask(int p){
    ll ret = 0;
    while(p) ret += tr[p], p -= p & -p;
    return ret;
}
void solve(int l, int r){
    if(l == r) return;
    int mid = (l + r) >> 1;
    solve(l, mid), solve(mid + 1, r);
    sort(p + l, p + mid + 1, cmp);
    sort(p + mid + 1, p + r + 1, cmp);
    int pl = l, pr = mid + 1;
    while(pr <= r){
        while(pl <= mid && p[pl].val > p[pr].val)
            add(p[pl].pos, 1), pl++;
        ans[p[pr].tim] += ask(p[pr].pos);
        pr++;
    }
    while(--pl >= l) add(p[pl].pos, -1);
    pl = mid, pr = r;
    while(pr > mid){
        while(pl >= l && p[pl].val < p[pr].val)
            add(p[pl].pos, 1), pl--;
        ans[p[pr].tim] += ask(n) - ask(p[pr].pos);
        pr--;
    }
    while(++pl <= mid) add(p[pl].pos, -1);
}

int main(){

    read(n), read(m);
    for(int i = 1, x; i <= n; i++)
        read(x), p[x].val = x, p[x].pos = i;
    for(int i = 1, x; i <= m; i++)
        read(x), p[x].tim = n - i + 1;
    for(int i = 1, cnt = 1; i <= n; i++)
        if(!p[i].tim) p[i].tim = cnt++;
    sort(p + 1, p + n + 1);
    solve(1, n);
    for(int i = 2; i <= n; i++)
        ans[i] += ans[i - 1];
    for(int i = n; i > n - m; i--)
        write(ans[i]), enter;

    return 0;
}
```












