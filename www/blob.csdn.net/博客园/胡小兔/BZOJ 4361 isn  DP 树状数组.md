# BZOJ 4361 isn | DP 树状数组 - 胡小兔 - 博客园







# [BZOJ 4361 isn | DP 树状数组](https://www.cnblogs.com/RabbitHu/p/BZOJ4361.html)





## 链接

[BZOJ 4361](http://www.lydsy.com/JudgeOnline/problem.php?id=4361)

## 题面

给出一个长度为n的序列A(A1,A2...AN)。如果序列A不是非降的，你必须从中删去一个数，

这一操作，直到A非降为止。求有多少种不同的操作方案，答案模10^9+7。

N <= 2000。

## 题解

中国非著名数学老师张军说过：正难则反……

答案就是所有最后剩下一个非降序列的方案 - 不合法的最后剩下一个非降序列的方案。

什么是不合法的、最后剩下一个非降序列的方案呢？就是中间过程中已经形成非降序列的方案。它们的共同特点就是——若最后剩下的序列长度为i，则一定是长度为(i + 1)的另一个非降序列删去(i + 1)个数中的一个数后形成的。要把这部分减去。

设f[i]表示长为i的非降序列的个数。

所以答案就是\[ans = \sum_{i = 1}{n} (f[i] * (n - i) ! - f[i + 1] * (n - i - 1) ! * (i + 1)\]。

问题就是怎么求出f[i]。

设g[i][j]为长为i、最后一位是原序列中第j个数的不降序列数。

则有\[g[i][j] = \sum_{k \le j, a[k] \le a[j]} g[i - 1][k]\]。

求g可以用树状数组优化，总复杂度\(O(n^2 \log n)\)

那么f[i] 就是所有 g[i][j] 之和。

```
#include <cstdio>
#include <cmath>
#include <cstring>
#include <algorithm>
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

const int N = 2005, P = 1000000007;
int n, a[N], g[N][N], f[N], lst[N], idx, sum[N];
ll ans, fac[N];

void add(int p, int x){
    while(p <= n) sum[p] = (sum[p] + x) % P, p += p & -p;
}
int ask(int p){
    int ret = 0;
    while(p) ret = (ret + sum[p]) % P, p -= p & -p;
    return ret;
}

int main(){

    read(n);
    for(int i = 1; i <= n; i++)
        read(a[i]), lst[i] = a[i];
    sort(lst + 1, lst + n + 1);
    idx = unique(lst + 1, lst + n + 1) - lst - 1;
    for(int i = 1; i <= n; i++)
        a[i] = lower_bound(lst + 1, lst + idx + 1, a[i]) - lst;
    for(int i = 1; i <= n; i++)
        g[1][i] = 1;
    for(int i = 2; i <= n; i++){
        memset(sum, 0, sizeof(sum));
        for(int j = 1; j <= n; j++)
            g[i][j] = ask(a[j]), add(a[j], g[i - 1][j]);
    }
    for(int i = 1; i <= n; i++)
        for(int j = 1; j <= n; j++)
            f[i] = (f[i] + g[i][j]) % P;
    fac[0] = 1;
    for(int i = 1; i <= n; i++)
        fac[i] = fac[i - 1] * i % P;
    for(int i = 1; i <= n; i++)
        ans = ((ans + f[i] * fac[n - i] % P
                - f[i + 1] * fac[n - i - 1] % P * (i + 1) % P) % P + P) % P;
    write(ans), enter;

    return 0;
}
```












