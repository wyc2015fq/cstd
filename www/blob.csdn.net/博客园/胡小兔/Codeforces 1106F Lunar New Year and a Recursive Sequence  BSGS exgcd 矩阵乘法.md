# Codeforces 1106F Lunar New Year and a Recursive Sequence | BSGS/exgcd/矩阵乘法 - 胡小兔 - 博客园







# [Codeforces 1106F Lunar New Year and a Recursive Sequence | BSGS/exgcd/矩阵乘法](https://www.cnblogs.com/RabbitHu/p/10349388.html)





> 
我诈尸啦！

高三退役选手好不容易抛弃天利和金考卷打场CF，结果打得和shi一样……还因为queue太长而unrated了！一个学期不敲代码实在是忘干净了……

没分该没分，考题还是要订正的 =v= 欢迎阅读本题解！

P.S. 这几个算法我是一个也想不起来了 TAT


## 题目链接

[Codeforces 1106F Lunar New Year and a Recursive Sequence 新年和递推数列](https://codeforces.com/contest/1106/problem/F)

## 题意描述

某数列\(\{f_i\}\)递推公式：\[f_i = (\prod_{j=1}^kf_{i-j}^{b_j}) \bmod p\]

其中\(b\)是已知的长度为\(k\)的数列，\(p = 998244353\)，\(f_1 = f_2 = ... = f_{k-1} = 1\)，\(f_k\)未知。

给出两个数\(n, m\)，构造一个\(f_k\)使得\(f_n = m\)，无解输出-1。

\(k \le 100, n \le 10^9\)

## 题解

数论！真令人头秃！

首先这个数据范围让人想到什么？矩阵乘法！

矩阵乘法想推这个全是乘法和乘方的递推数列咋办？取对数！离散对数！

于是这道题关键的两个考点就被你发现啦！

（然而我太菜了，并不能发现 = =）

> 
#### 什么是离散对数？

众所周知（众==学过NTT的人等），这个喜闻乐见的模数\(p = 998244353\)有个原根\(g=3\)，\(g^i(0\le i < P - 1)\)和\(1\le x < P\)一一对应。那么类比我们学过的对数，称这个\(i\)为\(x\)的离散对数。


令数列\(h_i\)为\(f_i\)的离散对数。

那么有递推式：\[h_i = (\sum_{j=1}^kb_j\cdot h_{i-j}) \bmod (p - 1)\]

其中\(h_1 = h_2 = ... = h_{k-1} = 0\)。注意模数变成了\(p - 1\)（费马小定理）。

这个就可以用矩阵加速了！如果我们把\(h_k\)设为1带进去，求得\(h_n = c\)，那么有\(h_n = c \cdot h_k \bmod (p - 1)\)；

\(h_n\)即为\(m\)的离散对数，用BSGS可求；

exgcd解刚才这个同余方程即可得到\(h_k\)；

\(f_k = g^{h_k}\)，快速幂即可得到\(f_k\)。

如果exgcd发现没有解的话就输出-1。

是不是思路非常清晰啊~

## 代码

```
#include <cstdio>
#include <cstring>
#include <cmath>
#include <algorithm>
#include <iostream>
#include <cassert>
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

const int N = 102, P = 998244353, P2 = 998244352, G = 3;
int K;
ll b[N], n, m, C;

namespace BSGS {
    const int S = 32000, M = 2000000;
    int cnt = 0, adj[M + 5], nxt[S + 5];
    ll key[S + 5], val[S + 5];
    void insert(ll K, ll V){
        int p = K % M;
        key[++cnt] = K;
        val[cnt] = V;
        nxt[cnt] = adj[p];
        adj[p] = cnt;
    }
    ll search(ll K){
        for(int u = adj[K % M]; u; u = nxt[u])
            if(key[u] == K) return val[u];
        return -1;
    }
    void init(){
        ll sum = 1;
        for(int i = 1; i <= S; i++)
            sum = sum * G % P;
        ll tot = 1;
        for(int i = 1; (i - 1) * S < P - 1; i++)
            tot = tot * sum % P, insert(tot, i * S);
    }
    ll log(ll x){
        ll sum = 1, ret;
        for(int i = 1; i <= S; i++){
            sum = sum * G % P;
            ret = search(sum * x % P);
            if(~ret && ret < P - 1) return ret - i;
        }
        assert(0);
        return -1;
    }
}

struct matrix {
    ll g[N][N];
    matrix(){
        memset(g, 0, sizeof(g));
    }
    matrix(int x){
        memset(g, 0, sizeof(g));
        for(int i = 1; i <= K; i++)
            g[i][i] = 1;
    }
    matrix operator * (const matrix &b){
        matrix c;
        for(int i = 1; i <= K; i++)
            for(int j = 1; j <= K; j++)
                for(int k = 1; k <= K; k++)
                    c.g[i][j] = (c.g[i][j] + g[i][k] * b.g[k][j]) % P2;
        return c;
    }
};

ll qpow(ll a, ll x){
    ll ret = 1;
    while(x){
        if(x & 1) ret = ret * a % P;
        a = a * a % P;
        x >>= 1;
    }
    return ret;
}
matrix qpow(matrix a, ll x){
    matrix ret(1);
    while(x){
        if(x & 1) ret = ret * a;
        a = a * a;
        x >>= 1;
    }
    return ret;
}
ll calcC(){
    matrix ret, op;
    ret.g[K][1] = 1;
    for(int i = 1; i < K; i++)
        op.g[i][i + 1] = 1;
    for(int i = 1; i <= K; i++)
        op.g[K][i] = b[K - i + 1];
    ret = qpow(op, n - K) * ret;
    return ret.g[K][1];
}
void exgcd(ll a, ll b, ll &g, ll &x, ll &y){
    if(!b) return (void)(x = 1, y = 0, g = a);
    exgcd(b, a % b, g, y, x);
    y -= x * (a / b);
}
ll solve(ll A, ll B){ //Ax % P2 == B, solve x
    ll a = A, b = P2, g, x, y;
    exgcd(a, b, g, x, y);
    if(B % g) return -1;
    x *= B / g, y *= B / g;
    ll t = b / g;
    x = (x % t + t) % t;
    return x;
}

int main(){

    BSGS::init();
    read(K);
    for(int i = 1; i <= K; i++) read(b[i]);
    read(n), read(m);
    C = calcC();
    m = BSGS::log(m);
    ll ans = solve(C, m);
    if(ans == -1) puts("-1");
    else write(qpow(G, ans)), enter;

    return 0;
}
```












