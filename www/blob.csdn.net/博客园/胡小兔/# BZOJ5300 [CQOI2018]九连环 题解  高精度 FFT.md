# # BZOJ5300 [CQOI2018]九连环 题解 | 高精度 FFT - 胡小兔 - 博客园







# [# BZOJ5300 [CQOI2018]九连环 题解 | 高精度 FFT](https://www.cnblogs.com/RabbitHu/p/BZOJ5300.html)





今天做了传说中的CQOI六道板子题……有了一种自己很巨的错觉（雾

## 题面

求n连环的最少步数，n <= 1e5。

## 题解

首先……我不会玩九连环……

通过找规律~~（其实是百度搜索）~~可知，\(n\)连环的最少步数是\(\lfloor\frac{2^{n + 1}}{3}\rfloor\)。

（实际上，九连环的步骤恰好是一个叫【格雷码】的编码方式中的\(1\)一直到\(2^{n+1}-1\)！）

然后我们要输出这个\(\lfloor\frac{2^{n + 1}}{3}\rfloor\)就好了。

然后我们发现——毒瘤出题人让我们写高精度。

……那就只能写咯……

用FFT做高精度乘法，封装起来，然后正常进行快速幂即可。

```
#include <cstdio>
#include <cstring>
#include <cmath>
#include <algorithm>
#include <iostream>
#define enter putchar('\n')
#define space putchar(' ')
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
    if(op == 1) x = -x;
}
template <class T>
void write(T x){
    if(x < 0) putchar('-'), x = -x;
    if(x >= 10) write(x / 10);
    putchar('0' + x % 10);
}

const int N = 150000;
const double PI = acos(-1);
int T, x;

struct cp {
    double a, b;
    cp(){}
    cp(double x, double y): a(x), b(y){}
    cp operator + (const cp &obj) const {
    return cp(a + obj.a, b + obj.b);
    }
    cp operator - (const cp &obj) const {
    return cp(a - obj.a, b - obj.b);
    }
    cp operator * (const cp &obj) const {
    return cp(a * obj.a - b * obj.b, a * obj.b + b * obj.a);
    }
} inv[N], omg[N];

void init(int n){
    for(int i = 0; i < n; i++){
    omg[i] = cp(cos(2 * PI / n * i), sin(2 * PI / n * i));
    inv[i] = cp(omg[i].a, -omg[i].b);
    }
}
void fft(cp *a, int n, cp *omg){
    int lim = 0;
    while((1 << lim) < n) lim++;
    for(int i = 0; i < n; i++){
    int t = 0;
    for(int j = 0; j < lim; j++)
        if(i >> j & 1) t |= 1 << (lim - j - 1);
    if(i < t) swap(a[i], a[t]);
    }
    for(int l = 2; l <= n; l <<= 1){
    int m = l / 2;
    for(cp *p = a; p != a + n; p += l)
        for(int i = 0; i < m; i++){
        cp t = omg[n / l * i] * p[i + m];
        p[i + m] = p[i] - t;
        p[i] = p[i] + t;
        }
    }
}

struct big {
    int g[N], len;
    big(){
    memset(g, 0, sizeof(g));
    len = 1;
    }
    big(int x){
    memset(g, 0, sizeof(g));
    len = 0;
    if(!x){
        len = 1;
        return;
    }
    while(x) g[len++] = x % 10, x /= 10;
    }
    void out(){
    for(int i = len - 1; i >= 0; i--)
        printf("%d", g[i]);
    enter;
    }
    void operator /= (int x){
    int sum = 0, newlen = 0;
    for(int i = len - 1; i >= 0; i--){
        sum = sum * 10 + g[i];
        if(sum < x) g[i] = 0;
        else{
        if(!newlen) newlen = i + 1;
        g[i] = sum / x;
        sum %= x;
        }
    }
    len = max(newlen, 1);
    }
    void operator *= (const big &b){
    static cp A[N], B[N];
    int newlen = len + b.len - 1, n = 1;
    while(n < newlen) n <<= 1;
    for(int i = 0; i < n; i++){
        A[i] = cp(i < len ? g[i] : 0, 0);
        B[i] = cp(i < b.len ? b.g[i] : 0, 0);
    }
    init(n);
    fft(A, n, omg);
    fft(B, n, omg);
    for(int i = 0; i < n; i++)
        A[i] = A[i] * B[i];
    fft(A, n, inv);
    for(int i = 0; i < newlen; i++)
        g[i] = (int)floor(A[i].a / n + 0.5);
    g[len = newlen] = 0;
    for(int i = 0; i < len; i++)
        g[i + 1] += g[i] / 10, g[i] %= 10;
    if(g[len]) len++; 
    }
} ret, a;

int main(){

    read(T);
    while(T--){
    read(x), x++;
    ret = big(1), a = big(2);
    while(x){
        if(x & 1) ret *= a;
        a *= a;
        x >>= 1;
    }
    ret /= 3;
    ret.out();
    }

    return 0;
}
```












