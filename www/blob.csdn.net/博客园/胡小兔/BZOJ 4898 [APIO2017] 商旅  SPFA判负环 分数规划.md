# BZOJ 4898 [APIO2017] 商旅 | SPFA判负环 分数规划 - 胡小兔 - 博客园







# [BZOJ 4898 [APIO2017] 商旅 | SPFA判负环 分数规划](https://www.cnblogs.com/RabbitHu/p/BZOJ4898.html)





# [BZOJ 4898](http://www.lydsy.com/JudgeOnline/problem.php?id=4898) [APIO2017] 商旅 | SPFA判负环 分数规划

更清真的题面链接：[https://files.cnblogs.com/files/winmt/merchant%28zh_CN%29.pdf](https://files.cnblogs.com/files/winmt/merchant%28zh_CN%29.pdf)

## 题解

……APIO2017那天我似乎在……北京一日游……

【更新】诶？我……我Rank1了？//虽然只有不几个人做这道题
![](https://images2017.cnblogs.com/blog/1129536/201712/1129536-20171215100447685-1519176597.png)

正经的题解：

二分答案，如果存在一种环路使得【总获利/总路程 > mid】，那么这个环路的【总(获利 - 路程 * mid)】一定大于0，换句话说，把边权换成【获利 - 路程 * mid】后，该图有正环。

正环可以用DFS版SPFA判，详见这篇论文——[SPFA算法的优化及应用](https://files.cnblogs.com/files/RabbitHu/SPFA%E7%AE%97%E6%B3%95%E7%9A%84%E4%BC%98%E5%8C%96%E5%8F%8A%E5%BA%94%E7%94%A8.pdf)，每对点对(u, v)的获利、最短路程都可以预处理出来。

那么这道题还是很简单的啦。

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

const int N = 105, MAXK = 1005,  INF = 0x3f3f3f3f;
int n, m, K, dis[N][N], val[N][N], buy[N][MAXK], sell[N][MAXK];
double l, r, mid, d[N];
bool done, ins[N];

void spfa(int u){
    if(done) return;
    ins[u] = 1;
    for(int v = 1; v <= n; v++){
        if(done) return;
        if(v != u && dis[u][v] < INF && d[u] + val[u][v] - mid * dis[u][v] > d[v]){
            d[v] = d[u] + val[u][v] - mid * dis[u][v];
            if(ins[v]) return (void)(done = 1);
            spfa(v);
        }
    }
    ins[u] = 0;
}
bool check(){
    done = 0;
    memset(ins, 0, sizeof(ins));
    memset(d, 0, sizeof(d));
    for(int i = 1; i <= n && !done; i++)
        spfa(i);
    return done;
}

int main(){

    read(n), read(m), read(K);
    for(int i = 1; i <= n; i++)
        for(int j = 1; j <= K; j++)
            read(buy[i][j]), read(sell[i][j]);
    for(int i = 1; i <= n; i++)
        for(int j = 1; j <= n; j++)
            dis[i][j] = i == j ? 0 : INF;
    for(int i = 1, u, v, w; i <= m; i++)
        read(u), read(v), read(w), dis[u][v] = w;
    for(int k = 1; k <= n; k++)
        for(int i = 1; i <= n; i++)
            for(int j = 1; j <= n; j++)
                dis[i][j] = min(dis[i][j], dis[i][k] + dis[k][j]);
    for(int i = 1; i <= n; i++)
        for(int j = 1; j <= n; j++)
            for(int k = 1; k <= K; k++)
                if(buy[i][k] != -1 && sell[j][k] != -1){
                    val[i][j] = max(val[i][j], sell[j][k] - buy[i][k]);
                    r = max(r, (double)val[i][j]);
                }
    int cnt = 0;
    while(++cnt <= 60){
        mid = (l + r) / 2;
        if(check()) l = mid;
        else r = mid;
    }
    printf("%lld\n", (ll)floor((l + r) / 2));
    
    return 0;
}
```












