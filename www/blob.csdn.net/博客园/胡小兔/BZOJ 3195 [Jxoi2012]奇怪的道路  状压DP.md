# BZOJ 3195 [Jxoi2012]奇怪的道路 | 状压DP - 胡小兔 - 博客园







# [BZOJ 3195 [Jxoi2012]奇怪的道路 | 状压DP](https://www.cnblogs.com/RabbitHu/p/BZOJ3195.html)





## 传送门

[BZOJ 3195](http://www.lydsy.com/JudgeOnline/problem.php?id=3195)

## 题解

这是一道画风正常的状压DP题。

可以想到，\(dp[i][j][k]\)表示到第\(i\)个点、已经连了\(j\)条边，当前\([i - K, i]\)区间内的点的度数的奇偶性状态是\(k\)（用二进制表示，度数为奇则对应位是1，反之为0）的方案数。

然后对每个状态枚举\(i\)要和区间\([i - K, i - 1]\)中的谁连边，如果\(i\)和\(i - p\)连边的话，新的状态\(k'\)就是 k ^ (1 << p) ^ 1（二进制第\(x\)位表示点\(i - x\)，第\(0\)位表示点\(i\)）。

但是这样写WA了！因为会出现重复，例如让3号点先和1号点连再和2号点连，与先和2号点连再和1号点连，是同一种方案，却被算了两次。

为了解决这个问题，我们再加一维，强制让先连的边“短一点”（终点离\(i\)近一点）。

现在，\(dp[i][j][k][l]\)表示：到第\(i\)个点、已经连了\(j\)条边，当前\([i - K, i]\)区间内的点的度数的奇偶性状态是\(k\)， **当前要尝试连\(i\)和\(i - l\)**。从小到大枚举\(l\)就不会重复了。

代码很好写。

```
#include <cstdio>
#include <cstring>
#include <cmath>
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
const int N = 32, MAXK = 9, P = 1000000007;
int n, m, K, dp[N][N][1 << MAXK][MAXK];
int main(){
    read(n), read(m), read(K);
    dp[1][0][0][1] = 1;
    for(int i = 1; i <= n; i++)
        for(int j = 0; j <= m; j++)
            for(int k = 0; k < (1 << (K + 1)); k++){
                for(int l = 1; l <= K; l++){
                    //printf("dp[%d][%d][%d][%d] = %d\n", i, j, k, l, dp[i][j][k][l]);
                    (dp[i][j][k][l + 1] += dp[i][j][k][l]) %= P;
                    if(j < m && i - l > 0)
                        (dp[i][j + 1][k ^ (1 << l) ^ 1][l] += dp[i][j][k][l]) %= P;
                }
                if(!(k >> K & 1))
                    (dp[i + 1][j][k << 1][1] += dp[i][j][k][K]) %= P;
            }
    write(dp[n][m][0][K]), enter;
    return 0;
}
```












