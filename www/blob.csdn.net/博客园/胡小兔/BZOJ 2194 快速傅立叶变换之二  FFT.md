# BZOJ 2194 快速傅立叶变换之二 | FFT - 胡小兔 - 博客园







# [BZOJ 2194 快速傅立叶变换之二 | FFT](https://www.cnblogs.com/RabbitHu/p/BZOJ2194.html)





# [BZOJ 2194](http://www.lydsy.com/JudgeOnline/problem.php?id=2194) 快速傅立叶变换之二

## 题意

给出两个长为\(n\)的数组\(a\)和\(b\)，\(c_k = \sum_{i = k}^{n - 1} a[i] * b[i - k]\)。

## 题解

我们要把这个式子转换成多项式乘法的形式。

一个标准的多项式乘法是这样的：
\[c_k = \sum_{i = 0}^{k} a[i] * b[k - i]\]

来看看原式：
\[c_k = \sum_{i = k}^{n - 1} a[i] * b[i - k]\]

将a翻转得到a':
\[c_k = \sum_{i = k}^{n - 1} a'[n - 1 - i] * b[i - k]\]

调整求和指标：
\[c_k = \sum_{i = 0}^{n - k - 1} a'[n - k - 1 - i] * b[i]\]

那么求出\(c_k\)，之后取\(c\)的前\(n\)位，倒着输出即可。

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

int len, ta[N], tb[N], res[N];
cp omg[N], inv[N];

void init(int n){
    for(int i = 0; i < n; i++){
        omg[i] = cp(cos(2 * PI * i / n), sin(2 * PI * i / n));
        inv[i] = conj(omg[i]);
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
void multiply(){
    static cp a[N], b[N];
    for(int i = 0; i < len; i++)
        a[i].real(ta[i]), b[i].real(tb[i]);
    int n = 1;
    while(n < 2 * len) n *= 2;
    init(n);
    fft(a, n, omg);
    fft(b, n, omg);
    for(int i = 0; i < n; i++)a[i] *= b[i];
    fft(a, n, inv);
    for(int i = 0; i < n; i++)
        res[i] = floor(a[i].real() / n + 0.5);
}

int main(){

    read(len);
    for(int i = 0; i < len; i++)
        read(ta[i]), read(tb[i]);
    for(int i = 0, j = len - 1; i < j; i++, j--)
        swap(ta[i], ta[j]);
    multiply();
    for(int i = len - 1; i >= 0; i--)
        write(res[i]), enter;

    return 0;
}
```












