# [CTSC2018] 假面 | 期望 DP - 胡小兔 - 博客园







# [[CTSC2018] 假面 | 期望 DP](https://www.cnblogs.com/RabbitHu/p/9040230.html)





## 题目链接

[LOJ 2552](https://loj.ac/problem/2552)
[Luogu P4564](https://www.luogu.org/problemnew/show/P4564)

考场上这道题我先是写了个70分暴力，然后发现似乎可以NTT，然鹅问题是——我没学过NTT，遂脑补之，脑补出来了，下午出成绩一看，卡成暴力分（70）……同是\(O(Qk^2\log k)\)，学姐的拉格朗日什么玩意就能过TAT……学姐太强了……

遂不忿，今天上午重写NTT，努力卡常，卡不进去……

那还是写正解吧。

首先，发现血量上限很少，0操作时，暴力维护每一时刻每个人是每种血量大小的概率即可。

1操作怎么办呢？设\(alive_i\)是\(i\)号人活着的概率，\(dead_i\)是他死了的概率，\(g_{i,j}\)是除\(i\)以外活了\(j\)个人的概率，\(i\)号人的答案就是\[alive_i * \sum_{j = 0}^{k - 1}\frac{1}{j + 1} * g_{i,j}\]

但是\(g_{i,j}\)怎么求呢？发现可以DP：设\(f_{i,j}\)表示前\(i\)个人有\(j\)个活着的概率，则\[f_{ij} = f_{i-1,j} * dead_i + f_{i-1,j-1} * alive_i\]

注意到最后的\(f\)和人的顺序无关，所以可以把人的顺序任意调换，把要求的这个\(i\)放在最后一个，这样\(f_{k - 1}\)就是\(g_{i}\)。

那么对于每个\(i\)求一遍\(f\)，复杂度是\(O(n^3)\)的，能得70分。

如何优化呢？考虑把\(i\)号人放在最后时，从\(f_k\)倒推到\(f_{k-1}\)：\[\frac{f_{k-1, j} = f_{k, j} - f_{k -1, j - 1} * alive_i}{dead_i}\]

注意到\(dead_i = 0\)时该式不能用，又发现此时\(f_{k-1, j} = f_{k, j+1}\)，所以也能直接求。

那么\(O(n^2)\)求出\(f_k\)，再\(O(n^2)\)倒推，直接可以获得答案！

```
#include <cstdio>
#include <cmath>
#include <cstring>
#include <algorithm>
#include <iostream>
#define space putchar(' ')
#define enter putchar('\n')
typedef long long ll;
using namespace std;
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

const int N = 256, P = 998244353;
int n, m, K, t[N], b[N], rate[N][105], iv[N];
ll qpow(ll a, ll x){
    ll ret = 1;
    while(x){
    if(x & 1) ret = ret * a % P;
    a = a * a % P;
    x >>= 1;
    }
    return ret;
}
void attack(int tar, ll x){
    ll y = (1 - x + P) % P;
    for(int i = 0; i <= b[tar]; i++){
    if(i) rate[tar][i] = rate[tar][i] * y % P;
    if(i < b[tar]) rate[tar][i] = (rate[tar][i] + rate[tar][i + 1] * x) % P;
    }
}
void query(){
    static ll f[N], g[N], h[N];
    memset(f, 0, sizeof(f));
    f[0] = 1;
    for(int i = 1; i <= K; i++)
    for(int j = i; j >= 0; j--)
        f[j] = ((j ? f[j - 1] * (1 - rate[t[i]][0]) : 0) + f[j] * rate[t[i]][0]) % P;
    for(int i = 1; i <= K; i++){
    h[i] = 0;
    if(!rate[t[i]][0])
        for(int j = 0; j < K; j++)
        h[i] += f[j + 1] * iv[j + 1] % P;
    else{
        int inv = qpow(rate[t[i]][0], P - 2);
        for(int j = 0; j < K; j++){
        g[j] = (f[j] - (j ? g[j - 1] * (1 - rate[t[i]][0]) : 0)) % P * inv % P;
        h[i] += iv[j + 1] * g[j] % P;
        }
    }
    h[i] %= P;
    h[i] = h[i] * (1 - rate[t[i]][0]) % P;
    if(h[i] < 0) h[i] += P;
    }
    for(int i = 1; i <= K; i++)
    write(h[i]), i == K ? enter: space; 
}

int main(){

    read(n);
    for(int i = 1; i <= n; i++)
    read(b[i]), rate[i][b[i]] = 1, iv[i] = qpow(i, P - 2);
    read(m);
    int op, x, u, v;
    while(m--){
    read(op);
    if(op == 0) read(x), read(u), read(v), attack(x, u * qpow(v, P - 2) % P);
    else{
        read(K);
        for(int i = 1; i <= K; i++) read(t[i]);
        query();
    }
    }
    for(int i = 1; i <= n; i++){
    ll sum = 0;
    for(int j = 1; j <= b[i]; j++)
        sum += (ll)j * rate[i][j] % P;
    write(sum % P), i == n ? enter: space;
    }
    
    return 0;
}
```












