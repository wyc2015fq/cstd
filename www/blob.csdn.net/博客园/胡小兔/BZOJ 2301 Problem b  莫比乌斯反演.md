# BZOJ 2301 Problem b | 莫比乌斯反演 - 胡小兔 - 博客园







# [BZOJ 2301 Problem b | 莫比乌斯反演](https://www.cnblogs.com/RabbitHu/p/BZOJ2301.html)





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

const int N = 50000;
int T, a, b, c, d, k, mu[N + 5], prime[N + 5], sum[N + 5], tot;
bool notprime[N + 5];

void getmu(){
    mu[1] = sum[1] = 1;
    for(int i = 2; i <= N; i++){
    if(!notprime[i]) mu[i] = -1, prime[++tot] = i;
    for(int j = 1; j <= tot && i * prime[j] <= N; j++){
        notprime[i * prime[j]] = 1;
        if(i % prime[j]) mu[i * prime[j]] = -mu[i];
        else{
        mu[i * prime[j]] = 0;
        break;
        }
    }
    sum[i] = sum[i - 1] + mu[i];
    }
}
ll calc(int a, int b){
    ll ret = 0;
    a /= k, b /= k;
    if(a > b) swap(a, b);
    for(int i = 1, last; i <= a; i = last + 1){
    last = min(a / (a / i), b / (b / i));
    ret += (ll)(a / i) * (b / i) * (sum[last] - sum[i - 1]);
    }
    return ret;
}
int main(){
    getmu();
    read(T);
    while(T--){
    read(a), read(b), read(c), read(d), read(k);
    write(calc(b, d) - calc(a - 1, d) - calc(b, c - 1) + calc(a - 1, c - 1)), enter;
    }
    return 0;
}
```












