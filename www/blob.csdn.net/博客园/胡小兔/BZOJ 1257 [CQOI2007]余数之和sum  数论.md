# BZOJ 1257 [CQOI2007]余数之和sum | 数论 - 胡小兔 - 博客园







# [BZOJ 1257 [CQOI2007]余数之和sum | 数论](https://www.cnblogs.com/RabbitHu/p/BZOJ1257.html)





# [BZOJ 1257](http://www.lydsy.com/JudgeOnline/problem.php?id=1257) [CQOI2007]余数之和sum

> 
给出正整数n和k，计算j(n, k)=k mod 1 + k mod 2 + k mod 3 + … + k mod n的值，其中k mod i表示k除以i的余数。例如j(5, 3)=3 mod 1 + 3 mod 2 + 3 mod 3 + 3 mod 4 + 3 mod 5=0+1+0+3+3=7


\(\sum_{i=1}^{n} k \mod i = \sum_{i=1}^{n} k - \lfloor \frac ki \rfloor i\)

\(\lfloor \frac ki \rfloor\)的取值很少，在\(\sqrt k\)级别。

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

ll n, k, ans;

int main(){
    read(n), read(k), ans = n * k;
    for(ll l = 1, t, r; l <= min(n, k); l = r + 1){
        t = k / l, r = min(n, k / t);
        ans -= t * (l + r) * (r - l + 1) / 2;
    }
    write(ans), enter;
    return 0;
}
```












