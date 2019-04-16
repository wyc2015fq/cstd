# BZOJ5298 [CQOI2018] 交错序列 | 矩阵乘法和一个trick - 胡小兔 - 博客园







# [BZOJ5298 [CQOI2018] 交错序列 | 矩阵乘法和一个trick](https://www.cnblogs.com/RabbitHu/p/BZOJ5298.html)





## 　题面

> 
求所有长度为\(n\)的、没有相邻的1的01序列中，若0有\(x\)个、1有\(y\)个，\(x^ay^b\)之和（对\(m\)取模）。
\(n \le 10^7, m \le 10^8, 0 \le a, b \le 45\)


## 题解

本题麻烦的地方在于这个\(x^ay^b\)怎么处理。

\[x^ay^b = (n - y)^ay^b = \sum_{i = 0}^{a}C_a^in^i(-y)^{a - i}y^b = \sum_{i = 0}^{a}(-1)^{a - i}C_a^in^iy^{a+b-i}\]

所以可以求出对于所有\(i \in [0, a]\)的\(y^i\)之和，然后枚举\(i\)乘上对应的系数，加起来即可。

那么如何求出\(y^i\)之和呢？

设\(f[k][i][0/1]\)表示长度为\(k\)、结尾是0/1的序列中“1的个数”（即\(y\)）的\(i\)次方之和。

0结尾的序列可以从0/1序列转移过来，而1的出现次数不会变。
\[f[k][i][0] = f[k - 1][i][0] + f[k - 1][i][1]\]

1结尾的序列只能从0结尾的转移过来，1的出现次数会+1，也就是新的\(y' = (y + 1)^i = \sum_{j = 0}^{i}C_i^j y\)。
\[f[k][i][1] = \sum_{j = 0}^{i}C_i^jf[k - 1][j][0]\]

然后构建矩阵就可以做了！

```
#include <cstdio>
#include <cstring>
#include <cmath>
#include <algorithm>
#include <iostream>
#include <set>
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

const int N = 185;
int n, a, b, P, sze1, sze2;
ll c[N][N], ans;
struct matrix {
    ll g[N][N];
    matrix(){
    memset(g, 0, sizeof(g));
    }
    matrix operator * (const matrix &b) const {
    matrix c;
    for(int i = 0; i < sze2; i++)
        for(int j = 0; j < sze2; j++){
        for(int k = 0; k < sze2; k++)
            c.g[i][j] += g[i][k] * b.g[k][j];
        c.g[i][j] %= P;
        }
    return c;
    }
    friend matrix qpow(matrix a, int x){
    matrix ret;
    for(int i = 0; i < sze2; i++)
        ret.g[i][i] = 1;
    while(x){
        if(x & 1) ret = ret * a;
        a = a * a;
        x >>= 1;
    }
    return ret;
    }
} op;

int main(){
    read(n), read(a), read(b), read(P);
    sze1 = a + b + 1, sze2 = 2 * sze1;
    c[0][0] = 1;
    for(int i = 1; i <= a + b; i++){
    c[i][0] = 1;
    for(int j = 1; j <= i; j++)
        c[i][j] = (c[i - 1][j - 1] + c[i - 1][j]) % P;
    }
    for(int i = 0; i < sze1; i++){
    op.g[i][i] = op.g[i][sze1 + i] = 1;
    for(int j = 0; j <= i; j++)
        op.g[sze1 + i][j] = c[i][j];
    }
    op = qpow(op, n);
    ll pw = 1;
    for(int i = 0; i <= a; i++){
    ans += (((a - i) & 1) ? -1 : 1) * c[a][i] * pw % P * (op.g[a + b - i][0] + op.g[sze1 + a + b - i][0]) % P;
    pw = pw * n % P;
    }
    write((ans % P + P) % P), enter;
    return 0;
}
```












