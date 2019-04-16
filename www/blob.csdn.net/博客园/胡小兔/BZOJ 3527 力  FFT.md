# BZOJ 3527 力 | FFT - 胡小兔 - 博客园







# [BZOJ 3527 力 | FFT](https://www.cnblogs.com/RabbitHu/p/BZOJ3527.html)





# [BZOJ 3527](http://www.lydsy.com/JudgeOnline/problem.php?id=3527) 力 | 分治

## 题意

给出数组q，$E_i = \sum_{i < j} \frac{q_i}{(i - j) ^ 2} - \sum_{i > j} \frac{q_i}{(i - j) ^ 2} $。

## 题解

求出减号前面一部分（设为A(i)），再求出减号后面的一部分（设为B(i)）。

具体怎么求呢？还是转换成多项式乘法。

设\(f(i) = q[i]\)，翻转后成为\(f'(i)\)。

设\(g(i) = \frac{1}{i^2}, g(0) = 0\)。

\[A(i) = \sum_{i = 0}^{j} f(i) * g(j - i)\]
\[\begin{align*}
B(i) &= \sum_{i = j}^{n - 1} f(i) * g(i - j)\\
&= \sum_{i = 0}^{n - 1 - j} f(i + j) * g(i)\\
&= \sum_{i = 0}^{n - 1 - j} f'(n - 1 - i - j) * g(i)
\end{align*}
\]

```
#include <cstdio>
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

const int N = 1000005;
const double PI = acos(-1);
typedef complex<double> cp;

int m, n = 1;
double q[N], res[N];
cp f[N], _f[N], g[N], omg[N], inv[N];

void init(){
    for(int i = 0; i < n; i++){
        omg[i] = cp(cos(2 * PI * i / n), sin(2 * PI * i / n));
        inv[i] = conj(omg[i]);
    }
}
void fft(cp *a, cp *omg){
    int lim = 0;
    while((1 << lim) < n) lim++;
    for(int i = 0; i < n; i++){
        int t = 0;
        for(int j = 0; j < lim; j++)
            if(i >> j & 1) t |= 1 << (lim - j - 1);
        if(i < t) swap(a[i], a[t]);
    }
    for(int l = 2; l <= n; l *= 2){
        int m = l / 2;
        for(cp *p = a; p != a + n; p += l)
            for(int i = 0; i < m; i++){
                cp t = omg[n / l * i] * p[m + i];
                p[m + i] = p[i] - t;
                p[i] += t;
            }
    }
}

int main(){

    read(m);
    for(int i = 0; i < m; i++){
        scanf("%lf", &q[i]);
        f[i].real(q[i]);
        _f[m - 1 - i].real(q[i]);
        if(i) g[i].real(1.0 / i / i);
    }
    while(n < 2 * m) n *= 2;
    init();
    fft(f, omg), fft(_f, omg), fft(g, omg);
    for(int i = 0; i < n; i++) f[i] *= g[i], _f[i] *= g[i];
    fft(f, inv), fft(_f, inv);
    for(int i = 0; i < m; i++)
        res[i] = f[i].real() / n - _f[m - 1 - i].real() / n;
    for(int i = 0; i < m; i++)
        printf("%lf\n", res[i]);

    return 0;
}
```












