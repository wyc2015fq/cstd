# BZOJ 2337 XOR和路径 | 高斯消元 期望 位运算 - 胡小兔 - 博客园







# [BZOJ 2337 XOR和路径 | 高斯消元 期望 位运算](https://www.cnblogs.com/RabbitHu/p/BZOJ2337.html)





# [BZOJ 2337](http://www.lydsy.com/JudgeOnline/problem.php?id=2337) XOR和路径

![](https://images2017.cnblogs.com/blog/1129536/201712/1129536-20171207154400816-1589756921.png)

## 题解

这道题和[游走](http://www.cnblogs.com/RabbitHu/p/BZOJ3143.html)那道题很像，但又不是完全相同。

因为异或，所以我们考虑拆位，分别考虑每一位；

设x[u]是从点u**出发**、到达点n时这一位异或和是1的概率。

对于所有这一位是1的边，若一个端点是u、另一个是v，则x[u] += (1 - x[v]) / deg[u]，反之亦然；

对于这一位是0的边，x[u] += x[v] / deg[u]，反之亦然。

然后得到好多方程，高斯消元即可。

```
#include <cstdio>
#include <cmath>
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

const int N = 105, M = 10005;
int n, m, u[M], v[M], w[M], deg[N];
double f[N][N], ans;
void build(int p){
    memset(f, 0, sizeof(f));
    for(int i = 1; i < n; i++) f[i][i] = deg[i];
    for(int e = 1; e <= m; e++){
    if(w[e] & (1 << p)){
        f[u[e]][v[e]] += 1, f[u[e]][n + 1] += 1;
        if(u[e] != v[e]) f[v[e]][u[e]] += 1, f[v[e]][n + 1] += 1;
    }
    else{
        f[u[e]][v[e]] += -1;
        if(u[e] != v[e]) f[v[e]][u[e]] += -1;
    }
    }
    for(int i = 1; i < n; i++) f[n][i] = 0;
    f[n][n] = 1, f[n][n + 1] = 0;
}
void Gauss(){
    for(int i = 1; i <= n; i++){
    int l = i;
    for(int j = i + 1; j <= n; j++)
        if(fabs(f[j][i]) > fabs(f[l][i])) l = j;
    if(i != l)
        for(int j = i; j <= n + 1; j++)
        swap(f[i][j], f[l][j]);
    for(int j = n + 1; j >= i; j--)
        f[i][j] /= f[i][i];
    for(int j = i + 1; j <= n; j++)
        for(int k = n + 1; k >= i; k--)
        f[j][k] -= f[j][i] * f[i][k];
    }
    for(int i = n; i; i--)
    for(int j = 1; j < i; j++)
        f[j][n + 1] -= f[j][i] * f[i][n + 1];
}
int main(){
    read(n), read(m);
    for(int i = 1; i <= m; i++){
    read(u[i]), read(v[i]), read(w[i]);
    deg[u[i]]++;
    if(u[i] != v[i]) deg[v[i]]++;
    }
    for(int i = 0; i < 31; i++){
    build(i);
    Gauss();
    ans += f[1][n + 1] * (1 << i);
    }
    printf("%.3lf\n", ans);
    return 0;
}
```












