# BZOJ 2242 [SDOI2011]计算器 | BSGS - 胡小兔 - 博客园







# [BZOJ 2242 [SDOI2011]计算器 | BSGS](https://www.cnblogs.com/RabbitHu/p/BZOJ2242.html)





insert的时候忘了取模了……

```
#include <cstdio>
#include <cmath>
#include <cstring>
#include <algorithm>
#include <queue>
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
const int P = 999979, N = 100005;
ll T, K, A, B, C;
ll adj[P], nxt[N], num[N], val[N], stk[N], top;
ll qpow(ll a, ll x, ll p){
    ll ret = 1;
    while(x){
        if(x & 1) ret = ret * a % p;
        a = a * a % p;
        x >>= 1;
    }
    return ret;
}
ll gcd(ll a, ll b){
    return b ? gcd(b, a % b) : a;
}
void exgcd(ll a, ll b, ll &x, ll &y){
    if(!b) return (void)(x = 1, y = 0);
    exgcd(b, a % b, y, x);
    y -= a / b * x;
}
ll inv(ll a, ll p){
    ll x, y;
    exgcd(a, p, x, y);
    return (x % p + p) % p;
}
void clear(){
    while(top) adj[stk[top--]] = 0;
}
void insert(ll x, ll y){
    stk[++top] = x % P;
    nxt[top] = adj[stk[top]];
    adj[stk[top]] = top;
    num[top] = x, val[top] = y;
}
ll find(ll x){
    for(int i = adj[x % P]; i; i = nxt[i])
        if(num[i] == x) return val[i];
    return -1;
}
bool force(){
    A %= C, B %= C;
    ll sum = 1 % C;
    for(ll i = 0; i <= 30; i++, sum = sum * A % C)
        if(sum == B) return write(i), enter, 1;
    return 0;
}
void bsgs(){
    clear();
    ll D = 1, sum = 1 % C, cnt = 0, n = sqrt(C), m = ceil(C / n);
    for(ll g = gcd(A, C); g > 1; g = gcd(A, C)){
        if(B % g) return (void)puts("Orz, I cannot find x!");
        C /= g, B /= g, D = D * A / g % C, cnt++;
    }
    B = B * inv(D, C) % C;
    for(ll i = 1; i <= m; i++)
        sum = sum * A % C, insert(sum * B % C, i);
    for(ll i = 1, tot = 1; i <= n; i++){
        ll y = find(tot = tot * sum % C);
        if(y != -1) return (void)(write(i * m - y + cnt), enter);
    }
    puts("Orz, I cannot find x!");
}
int main(){
    read(T), read(K);
    while(T--){
        read(A), read(B), read(C);
        if(K == 1) write(qpow(A, B, C)), enter;
        else if(K == 2){
            ll x, y, g = gcd(A, C);
            if(B % g){
                puts("Orz, I cannot find x!");
                continue;
            }
            exgcd(A, C, x, y);
            x *= B / g;
            write((x % (C / g) + (C / g)) % C/g), enter;
        }
        else bsgs();
    }
    return 0;
}
```












