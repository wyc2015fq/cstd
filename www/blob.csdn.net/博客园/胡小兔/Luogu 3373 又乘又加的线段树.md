# Luogu 3373 又乘又加的线段树 - 胡小兔 - 博客园







# [Luogu 3373 又乘又加的线段树](https://www.cnblogs.com/RabbitHu/p/Luogu3373.html)





# [Luogu 3373](https://www.luogu.org/problemnew/show/P3373) 又乘又加的线段树

当给一个节点加上一个加法标记时，直接把加法标记 += 新值；

当给一个节点加上一个乘法标记时，把乘法标记和加法标记同时 *= 新值。(注意pushdown函数中也要这样！)

```
#include <cstdio>
#include <cstring>
#include <algorithm>
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
int n, m, P;
ll a[N], data[4*N], mul[4*N], add[4*N];
void calc(ll &x, ll y, ll z){ // x = x * y + z
    x = (x * y % P + z % P) % P;
}
void build(int k, int l, int r){
    mul[k] = 1;
    if(l == r) return (void)(data[k] = a[l]);
    int mid = (l + r) >> 1;
    build(k << 1, l, mid);
    build(k << 1 | 1, mid + 1, r);
    data[k] = (data[k << 1] + data[k << 1 | 1]) % P;
}
void pushdown(int k, int l, int r){
    if(mul[k] == 1 && add[k] == 0) return;
    int mid = (l + r) >> 1;
    calc(data[k << 1], mul[k], add[k] * (mid - l + 1));
    calc(data[k << 1 | 1], mul[k], add[k] * (r - mid));
    calc(mul[k << 1], mul[k], 0);
    calc(mul[k << 1 | 1], mul[k], 0);
    calc(add[k << 1], mul[k], add[k]);
    calc(add[k << 1 | 1], mul[k], add[k]);
    mul[k] = 1, add[k] = 0;
}
void change(int k, int l, int r, int ql, int qr, ll x, bool is_mul){
    if(ql <= l && qr >= r){
    if(is_mul){
        data[k] = data[k] * x % P;
        mul[k] = mul[k] * x % P;
        add[k] = add[k] * x % P;
    }
    else{
        data[k] = (data[k] + x * (r - l + 1) % P) % P;
        add[k] = (add[k] + x) % P;
    }
    return;
    }
    pushdown(k, l, r);
    int mid = (l + r) >> 1;
    if(ql <= mid) change(k << 1, l, mid, ql, qr, x, is_mul);
    if(qr > mid) change(k << 1 | 1, mid + 1, r, ql, qr, x, is_mul);
    data[k] = (data[k << 1] + data[k << 1 | 1]) % P;
}
ll query(int k, int l, int r, int ql, int qr){
    if(ql <= l && qr >= r) return data[k];
    pushdown(k, l, r);
    int mid = (l + r) >> 1;
    ll ret = 0;
    if(ql <= mid) ret = (ret + query(k << 1, l, mid, ql, qr)) % P;
    if(qr > mid) ret = (ret + query(k << 1 | 1, mid + 1, r, ql, qr)) % P;
    return ret;
}
int main(){
    read(n), read(m), read(P);
    for(int i = 1; i <= n; i++)
    read(a[i]), a[i] %= P;
    build(1, 1, n);
    while(m--){
    int op, l, r;
    read(op), read(l), read(r);
    if(op < 3){
        ll x;
        read(x);
        if(op == 2) change(1, 1, n, l, r, x, 0);
        else change(1, 1, n, l, r, x, 1);
    }
    else write(query(1, 1, n, l, r)), enter;
    }
    return 0;
}
```












