# Luogu 3810 & BZOJ 3262 陌上花开/三维偏序 | CDQ分治 - 胡小兔 - 博客园







# [Luogu 3810 & BZOJ 3262 陌上花开/三维偏序 | CDQ分治](https://www.cnblogs.com/RabbitHu/p/Luogu3810.html)





# [Luogu 3810 & BZOJ 3263](https://www.luogu.org/problemnew/show/3810) 陌上花开/三维偏序 | CDQ分治

## 题面

\(n\)个元素，每个元素有三个值：\(a_i\), \(b_i\) 和 \(c_i\)。定义一个元素的偏序是三个值都小于等于它的值的元素的个数，对于\([0, n)\)的每个值\(i\)，求偏序为\(i\)的元素个数。

## 题解

这道题我使用的是**CDQ分治**。

这道题有三个维度，每个维度都要对应一个数据结构/算法，来逐个击破。

首先，按照\(a\)从小到大把所有元素排序，保证\(a\)从小到大。

然后，对于第二维进行分治：首先对mid两边的子区间分别处理，然后处理左边子区间内的元素对有边子区间内元素的贡献。

处理跨mid贡献，我们需要使用树状数组。把左右两个子区间分别按照\(b\)排序，对于右边的每个元素，先把左边所有b比它小、尚未加入树状数组的元素加入树状数组，即树状数组中左区间当前元素的\(c\)的对应位值上的值增加；然后计算右区间当前元素的\(c\)的对应位置的前缀和，把右区间当前元素的答案加上这个前缀和。

需要注意：

数据中有些元素是完全相同的，做的时候要去重，把原来的相同元素的个数当做新的唯一元素的权值。

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

const int N = 2000005;
int n, k, tot, tr[N], ans[N];
struct mem{
    int a, b, c, cnt, sum;
    bool operator < (const mem &B) const{
        if(a != B.a) return a < B.a;
        if(b != B.b) return b < B.b;
        return c < B.c;
    }
    bool operator == (const mem &B) const{
        return a == B.a && b == B.b && c == B.c;
    }
} m[N], t[N];
bool cmp(const mem &A, const mem &B){
    return A.b < B.b;
}
void add(int p, int x){
    while(p <= k) tr[p] += x, p += p & -p;
}
int ask(int p){
    int ret = 0;
    while(p) ret += tr[p], p -= p & -p;
    return ret;
}
void solve(int l, int r){
    if(l == r) return;
    int mid = (l + r) >> 1;
    solve(l, mid), solve(mid + 1, r);
    sort(m + l, m + mid + 1, cmp);
    sort(m + mid + 1, m + r + 1, cmp);
    int pl = l, pr = mid + 1;
    while(pr <= r){
        while(pl <= mid && m[pl].b <= m[pr].b)
            add(m[pl].c, m[pl].cnt), pl++;
        m[pr].sum += ask(m[pr].c);
        pr++;
    }
    for(int i = l; i < pl; i++)
        add(m[i].c, -m[i].cnt);
}
int main(){
    read(n), read(k), tot = n;
    for(int i = 1, a, b, c; i <= n; i++)
        read(a), read(b), read(c), t[i] = (mem){a, b, c, 0, 0};
    sort(t + 1, t + n + 1);
    n = 0;
    for(int i = 1; i <= tot; i++){
        if(i == 1 || !(t[i] == t[i - 1]))
            m[++n] = t[i];
        m[n].cnt++;
    }
    solve(1, n);
    for(int i = 1; i <= n; i++)
        ans[m[i].sum + m[i].cnt - 1] += m[i].cnt;
    for(int i = 0; i < tot; i++)
        write(ans[i]), enter;
    return 0;
}
```












