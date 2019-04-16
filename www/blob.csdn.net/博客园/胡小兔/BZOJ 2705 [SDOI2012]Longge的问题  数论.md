# BZOJ 2705 [SDOI2012]Longge的问题 | 数论 - 胡小兔 - 博客园







# [BZOJ 2705 [SDOI2012]Longge的问题 | 数论](https://www.cnblogs.com/RabbitHu/p/BZOJ2705.html)





# [BZOJ 2705](http://www.lydsy.com/JudgeOnline/problem.php?id=2705) [SDOI2012]Longge的问题

> 
给定一个整数N，你需要求出∑gcd(i, N)(1<=i <=N)。


这道题在51nod上见过，当时自己做了一遍，给大哥讲了两遍，现在还记得……

首先，gcd(i, n)一定是n的一个因数，设这个因数为d。

d做了多少次贡献呢？

当d做贡献的时候，i、d一定满足：d是i的因数且gcd(i/d, n/d)=1。

则d做贡献的次数就是n/d的欧拉函数。

```
#include <cmath>
#include <cstdio>
#include <cstring>
#include <algorithm>
#define space putchar(' ')
#define enter putchar('\n')
using namespace std;
typedef long long ll;
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

ll n, ans;
ll phi(ll n){
    ll res = n;
    for(ll i = 2; i * i <= n; i++)
        if(n % i == 0){
            res = res / i * (i - 1);
            while(n % i == 0) n /= i;
        }
    if(n > 1) res = res / n * (n - 1);
    return res;
}

int main(){

    read(n);
    for(ll i = 1; i * i <= n; i++)
        if(n % i == 0){
            ans += phi(i) * n / i;
            if(i * i < n) ans += phi(n / i) * i;
        }
    write(ans), enter;

    return 0;
}
```












