# BZOJ 4380 [POI2015]Myjnie | DP - 胡小兔 - 博客园







# [BZOJ 4380 [POI2015]Myjnie | DP](https://www.cnblogs.com/RabbitHu/p/BZOJ4380.html)





## 链接

[BZOJ 4380](http://www.lydsy.com/JudgeOnline/problem.php?id=4380)

## 题面

有n家洗车店从左往右排成一排，每家店都有一个正整数价格p[i]。

有m个人要来消费，第i个人会驶过第a[i]个开始一直到第b[i]个洗车店，且会选择这些店中最便宜的一个进行一次消费。但是如果这个最便宜的价格大于c[i]，那么这个人就不洗车了。

请给每家店指定一个价格，使得所有人花的钱的总和最大。

Input

第一行包含两个正整数n,m(1<=n<=50，1<=m<=4000)。

接下来m行，每行包含三个正整数a[i],b[i],c[i](1%3C=a%5Bi%5D%3C=b%5Bi%5D%3C=n，1%3C=c%5Bi%5D%3C=500000)

Output

第一行输出一个正整数，即消费总额的最大值。

第二行输出n个正整数，依次表示每家洗车店的价格p[i]，要求1<=p[i]<=500000。

若有多组最优解，输出任意一组。

## 题解

这道题……是一个正常的（？）区间DP。

先将所有c[i]离散化。

f[i][j][k]表示区间[i, j]内的店、最低价格为k，能取到的最大价值。这个最大价值包括所有“完全包含在[i, j]”内的顾客提供的价值。

g[i][j][k]表示区间[i, j]内的店、最低价格>=k，能取到的最大价值。

转移的时候，枚举最低价格k所在的位置p，采取类似分治的思想，设完全包含在[i, j]内、横跨位置p、且c[i] >= k的顾客有cnt[p][k]个，那么有：
\[f[i][j][k] = max_{p = i}^{j} g[i][p - 1][k] + g[p + 1][j][k] + k * cnt[p][k]\]

这个cnt数组在每次枚举[i, j]的时候都预处理一下。

因为要输出方案，所以额外记录一下最优解都是在哪里取到的，最后dfs求出方案。

总复杂度是\(O(n^3m)\)。

```
#include <cstdio>
#include <cmath>
#include <cstring>
#include <algorithm>
using namespace std;
typedef long long ll;
#define enter putchar('\n')
#define space putchar(' ')
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

const int N = 52, M = 4002;
int n, m, a[M], b[M], c[M], cnt[N][M], ans[N], lst[M], idx, f[N][N][M], g[N][N][M], gk[N][N][M];
short fp[N][N][M];

void dfs(int l, int r, int k){
    int val = gk[l][r][k], pos = fp[l][r][val];
    ans[pos] = lst[val];
    if(pos > l) dfs(l, pos - 1, val);
    if(pos < r) dfs(pos + 1, r, val);
}

int main(){
    
    read(n), read(m);
    for(int i = 1; i <= m; i++) 
        read(a[i]), read(b[i]), read(c[i]), lst[i] = c[i];
    sort(lst + 1, lst + m + 1);
    idx = unique(lst + 1, lst + m + 1) - lst - 1;
    for(int i = 1; i <= m; i++)
        c[i] = lower_bound(lst + 1, lst + idx + 1, c[i]) - lst;
    for(int i = n; i; i--)
        for(int j = i; j <= n; j++){
            for(int k = 1; k <= idx; k++)
                for(int h = i; h <= j; h++)
                    cnt[h][k] = 0;
            for(int k = 1; k <= m; k++)
                for(int h = i; h <= j; h++)
                    if(a[k] >= i && a[k] <= h && b[k] >= h && b[k] <= j) 
                        cnt[h][c[k]]++;
            for(int h = i; h <= j; h++)
                for(int k = idx; k; k--)
                    cnt[h][k] += cnt[h][k + 1];
            for(int k = 1; k <= idx; k++)
                for(int p = i; p <= j; p++)
                    if(g[i][p - 1][k] + g[p + 1][j][k] + lst[k] * cnt[p][k] >= f[i][j][k])
                        fp[i][j][k] = p, f[i][j][k] = g[i][p - 1][k] + g[p + 1][j][k] + lst[k] * cnt[p][k];
            for(int k = idx; k; k--){
                if(g[i][j][k + 1] > f[i][j][k])
                    gk[i][j][k] = gk[i][j][k + 1], g[i][j][k] = g[i][j][k + 1];
                else
                    gk[i][j][k] = k, g[i][j][k] = f[i][j][k];
            }
        }
    write(g[1][n][1]), enter;
    dfs(1, n, 1);
    for(int i = 1; i <= n; i++)
        write(ans[i]), i == n ? enter: space;
    
    return 0;
}
```












