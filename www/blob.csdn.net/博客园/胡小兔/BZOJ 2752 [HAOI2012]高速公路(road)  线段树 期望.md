# BZOJ 2752 [HAOI2012]高速公路(road) | 线段树 期望 - 胡小兔 - 博客园







# [BZOJ 2752 [HAOI2012]高速公路(road) | 线段树 期望](https://www.cnblogs.com/RabbitHu/p/BZOJ2752.html)





## 传送门

[BZOJ 2752](http://www.lydsy.com/JudgeOnline/problem.php?id=2752)

## 题解

先澄清一下题意——显然最后一个收费站是没有用的！相当于只有n - 1个点，每个点代表一条道路，当题目输入\(l, r\)时，实际涉及的区间是\([l, r - 1]\)。

要求的是期望嘛，就是所有方案价值之和 / 总方案数。

当然不可能枚举每种方案，所以我们考虑每一个收费站的贡献。

设这个收费站\(i\)的当前价格是\(v[i]\)，当前询问区间\([l ,r]\)。

则总共有\((r - l + 2) * (r - l + 1) / 2\)种方案，其中有\((i - l + 1)*(r - i + 1)\)种方案会经过这个收费站。

那么这个收费站的贡献就是\(v[i] * (i - l + 1) * (r - i + 1)\)，展开后得到\(- v[i] * i * i + v[i] * i * (l + r) + v[i] * (r - l + 1 - l * r)\)。

那么分别用线段树维护\(v[i] * i * i, v[i] * i, v[i]\)就可以了！

注意过程中不要爆long long……

```
#include <cstdio>
#include <cmath>
#include <cstring>
#include <algorithm>
#include <ctime>
#include <cstdlib>
using namespace std;
typedef long long ll;
#define enter putchar('\n')
#define space putchar(' ')
template <class T>
void read(T &x){
    char c;
    bool op = 0;
    while(c = getchar(), c > '9' || c < '0')
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

const int N = 100005;
int n, m;
ll v[4*N], vi[4*N], vii[4*N], lazy[4 * N], sumi[N], sumii[N];

void init(){
    for(ll i = 1; i <= n; i++){
        sumi[i] = sumi[i - 1] + i;
        sumii[i] = sumii[i - 1] + i * i;
    }
}
void single_modify(int k, int l, int r, ll x){
    lazy[k] += x;
    v[k] += x * (r - l + 1);
    vi[k] += x * (sumi[r] - sumi[l - 1]);
    vii[k] += x * (sumii[r] - sumii[l - 1]);
}
void pushdown(int k, int l, int r){
    if(!lazy[k]) return;
    int mid = (l + r) >> 1;
    single_modify(k << 1, l, mid, lazy[k]);
    single_modify(k << 1 | 1, mid + 1, r, lazy[k]);
    lazy[k] = 0;
}
void pushup(int k){
    v[k] = v[k << 1] + v[k << 1 | 1];
    vi[k] = vi[k << 1] + vi[k << 1 | 1];
    vii[k] = vii[k << 1] + vii[k << 1 | 1];
}
void modify(int k, int l, int r, int ql, int qr, ll x){
    if(ql <= l && qr >= r) return single_modify(k, l, r, x);
    pushdown(k, l, r);
    int mid = (l + r) >> 1;
    if(ql <= mid) modify(k << 1, l, mid, ql, qr, x);
    if(qr > mid) modify(k << 1 | 1, mid + 1, r, ql, qr, x);
    pushup(k);
}
ll query(int k, int l, int r, int ql, int qr){
    if(ql <= l && qr >= r)
        return -vii[k] + vi[k] * (qr + ql) + v[k] * (qr - ql + 1 - (ll)ql * qr);
    pushdown(k, l, r);
    int mid = (l + r) >> 1;
    ll ret = 0;
    if(ql <= mid) ret += query(k << 1, l, mid, ql, qr);
    if(qr > mid) ret += query(k << 1 | 1, mid + 1, r, ql, qr);
    return ret;
}
ll gcd(ll a, ll b){
    return b ? gcd(b, a % b) : a;
}

int main(){

    read(n), read(m), n--;
    init();
    char op[2];
    int l, r, x;
    while(m--){
        scanf("%s%d%d", op, &l, &r), r--;
        if(op[0] == 'C'){
            read(x);
            modify(1, 1, n, l, r, x);
        }
        else{
            ll a = query(1, 1, n, l, r), b = ((ll)r - l + 2) * (r - l + 1) / 2, g = gcd(a, b);
            write(a / g), putchar('/'), write(b / g), enter;
        }
    }

    return 0;
}
```












