# BZOJ 2226 [Spoj 5971] LCMSum 最大公约数之和 | 数论 - 胡小兔 - 博客园







# [BZOJ 2226 [Spoj 5971] LCMSum 最大公约数之和 | 数论](https://www.cnblogs.com/RabbitHu/p/BZOJ2226.html)





# [BZOJ 2226](http://www.lydsy.com/JudgeOnline/problem.php?id=2226) [Spoj 5971] LCMSum

这道题和[上一道题](http://www.cnblogs.com/RabbitHu/p/BZOJ2705.html)十分类似。

\[\begin{align*}
\sum_{i = 1}^{n}\operatorname{LCM}(i, n) &= \sum_{i = 1}^{n}\frac{i \times n}{\operatorname{gcd}(i, n)}\\
&= n \times \sum_{i = 1}^{n}\frac{i}{\operatorname{gcd}(i, n)}
\end{align*}\]

设\(d = \operatorname{gcd}(i, n)\)，则\(d | n\)且\(\operatorname{gcd}(\frac{i}{d}, \frac{n}{d}) = 1\)。

则每个\(n\)的因数\(d\)的贡献是小于等于\(d\)的所有数（\(\frac{i}{d}\)）之和。而这个值等于\(\frac{\phi(d) * d}{2}\)。

所以答案就是:

\[\sum_{d | n}\frac{\phi(d) * d}{2}\]

注意这道题卡常卡得非常难受，所以能预处理的都预处理吧。
![](https://images2017.cnblogs.com/blog/1129536/201712/1129536-20171217143209202-2005240393.png)

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

const int N = 1000000;
int T, n, lst[N + 5], cnt;
bool notprime[N + 5];
ll ans, phi[N + 5];
void init(){
    phi[1] = 1;
    for(int i = 2; i <= N; i++){
        if(!notprime[i]) lst[++cnt] = i, phi[i] = i - 1;
        for(int j = 1; j <= cnt && lst[j] * i <= N; j++){
            notprime[lst[j] * i] = 1;
            if(i % lst[j] == 0){
                phi[lst[j] * i] = lst[j] * phi[i];
                break;
            }
            phi[i * lst[j]] = phi[i] * (lst[j] - 1);
        }
    }
    for(int i = 2; i <= N; i++)
        phi[i] = phi[i] * i / 2;
}

int main(){
    
    init();
    read(T);
    while(T--){
        read(n);
        ans = 0;
        for(int i = 1; i * i <= n; i++)
            if(n % i == 0){
                ans += phi[i];
                if(i * i < n) ans += phi[n / i];
            }
        write(ans * n), enter;
    }
    
    return 0;
}
```












