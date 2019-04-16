# BZOJ 4569 [Scoi2016]萌萌哒 | ST表 并查集 - 胡小兔 - 博客园







# [BZOJ 4569 [Scoi2016]萌萌哒 | ST表 并查集](https://www.cnblogs.com/RabbitHu/p/BZOJ4569.html)





## 传送门

[BZOJ 4569](http://www.lydsy.com/JudgeOnline/problem.php?id=4569)

## 题解

ST表和并查集是我认为最优雅（其实是最好写……）的两个数据结构。

然鹅！他俩加一起的这道题，我却……没有做出来……

咳咳。

正解是这样的：

类似ST表有\(\log n\)层一样，我们开\(\log n\)个并查集。当已知\([l_1, r_1]\)和\([l_2, r_2]\)相同的时候，设\(j = \lfloor \log (r_1 - l_1 + 1) \rfloor\)，把\(l_1, l_2\)在\(j\)这层的并查集中合并，把\(r_1 - 2^j + 1, r_2 - 2^j + 1\)也在\(j\)这层并查集中合并。

最后是要下放的。下方时，从大到小枚举\(j\)，在\(j - 1\)这层并查集中合并\(i, fa[i][j]\)以及\(i + 2^{j - 1}, fa[i][j] + 2 ^ {j - 1}\)。

最后统计有多少不同的\(fa[i][0]\)即可，设有\(x\)个，则答案是\(9 * 10^{x - 1}\)，因为第一位不能是0。

我犯了个低级失误，就是最后统计有多少不同的\(fa[i][0]\)时，我打的真的是\(fa[i][0]\)，其实这里一定要`findfa(i, 0)`。。。

```
#include <cstdio>
#include <cmath>
#include <cstring>
#include <algorithm>
#include <ctime>
#include <cstdlib>
using namespace std;
typedef unsigned long long ll;
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
const int N = 100005, P = 1000000007;
int n, m, lg[N], fa[N][20];
bool vis[N];
ll ans = 9;
void init(){
    for(int i = 1, j = 0; i <= n; i++)
        lg[i] = i == (1 << (j + 1)) ? ++j : j;
    for(int j = 0; (1 << j) <= n; j++)
        for(int i = 1; i + (1 << j) - 1 <= n; i++)
            fa[i][j] = i;
}
int findfa(int u, int j){
    return fa[u][j] == u ? u : fa[u][j] = findfa(fa[u][j], j);
}
void merge(int u, int v, int j){
    if(findfa(u, j) != findfa(v, j))
        fa[fa[v][j]][j] = fa[u][j];
}
void merge(int l1, int r1, int l2, int r2){
    int j = lg[r1 - l1 + 1];
    merge(l1, l2, j);
    merge(r1 - (1 << j) + 1, r2 - (1 << j) + 1, j);
}
int main(){
    read(n), read(m);
    init();
    while(m--){
        int l1, r1, l2, r2;
        read(l1), read(r1), read(l2), read(r2);
        merge(l1, r1, l2, r2);
    }
    for(int j = lg[n]; j; j--)
        for(int i = 1; i + (1 << j) - 1 <= n; i++){
            merge(i, fa[i][j], j - 1);
            merge(i + (1 << (j - 1)), fa[i][j] + (1 << (j - 1)), j - 1);
        }
    for(int i = 1; i <= n; i++)
        vis[findfa(i, 0)] = 1;
    for(int i = 1, fir = 1; i <= n; i++)
        if(vis[i]){
            if(fir) fir = 0;
            else ans = ans * 10 % P;
        }
    write(ans), enter;
    return 0;
}
```












