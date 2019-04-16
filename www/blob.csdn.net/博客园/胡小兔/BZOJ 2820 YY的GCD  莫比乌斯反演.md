# BZOJ 2820 YY的GCD | 莫比乌斯反演 - 胡小兔 - 博客园







# [BZOJ 2820 YY的GCD | 莫比乌斯反演](https://www.cnblogs.com/RabbitHu/p/BZOJ2820.html)





```
#include <cstdio>
#include <cstring>
#include <algorithm>
#include <queue>
using namespace std;
typedef long long ll;
#define space putchar(' ')
#define enter putchar('\n')
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

const int N = 10000000;
int T, n, m, prime[N + 5], tot, mu[N + 5];
bool notprime[N + 5];
ll ans, sum[N + 5];

void getmu(){
    mu[1] = 1;
    for(int i = 2; i <= N; i++){
    if(!notprime[i]) prime[++tot] = i, mu[i] = -1;
    for(int j = 1; j <= tot && prime[j] * i <= N; j++){
        notprime[i * prime[j]] = 1;
        if(i % prime[j]) mu[i * prime[j]] = -mu[i];
        else{
        mu[i * prime[j]] = 0;
        break;
        }
    }
    }
    for(int i = 1; i <= tot; i++)
    for(int j = prime[i]; j <= N; j += prime[i])
        sum[j] += mu[j / prime[i]];
    for(int i = 2; i <= N; i++)
    sum[i] += sum[i - 1];
}

int main(){
    getmu();
    read(T);
    while(T--){
    read(n), read(m), ans = 0;
    if(n > m) swap(n, m);
    for(int i = 1, last; i <= n; i = last + 1){
        last = min(n / (n / i), m / (m / i));
        ans += (ll)(n / i) * (m / i) * (sum[last] - sum[i - 1]);
    }
    write(ans), enter;
    }
    return 0;
}
```












