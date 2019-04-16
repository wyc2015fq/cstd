# BZOJ 4671 异或图 | 线性基 容斥 DFS - 胡小兔 - 博客园







# [BZOJ 4671 异或图 | 线性基 容斥 DFS](https://www.cnblogs.com/RabbitHu/p/BZOJ4671.html)





## 题面

> 
Description

定义两个结点数相同的图 G1 与图 G2 的异或为一个新的图 G, 其中如果 (u, v) 在 G1 与

G2 中的出现次数之和为 1, 那么边 (u, v) 在 G 中, 否则这条边不在 G 中.

现在给定 s 个结点数相同的图 G1...s, 设 S = {G1, G2, . . . , Gs}, 请问 S 有多少个子集的异

或为一个连通图?

Input

第一行为一个整数s, 表图的个数.

接下来每一个二进制串, 第 i 行的二进制串为 gi, 其中 gi 是原图通过以下伪代码转化得

到的. 图的结点从 1 开始编号, 下面设结点数为 n.

Algorithm 1 Print a graph G = (V, E)

for i = 1 to n do

for j = i + 1 to n do

if G contains edge (i, j) then

print 1

else

print 0

end if

end for

end for

2 ≤ n ≤ 10,1 ≤ s ≤ 60.

Output

输出一行一个整数, 表示方案数

Sample Input

3

1

1

0

Sample Output

4


## 题解

容斥+线性基+DFS

（其中我最不会的居然是DFS! TAT）

要求异或图连通，不会。

考虑容斥。

将n个点分为x组，然后求连通性“至少”是当前分组情况的方案数——什么是“至少”呢？不同组的点之间一定不连通，相同组的点之间可以连通也可以不连通。

这样，最终答案就是：所有分成一组的方案数 - 所有分成两组的 + 所有分成三组的……

n个点分成任意大小组的所有方案都可以DFS枚举出来。

那么已知一种分组方案，如何求有多少图的集合的异或图（原题里提到的那个）满足“不同组的点之间一定不连通”呢？

把每对有要求限制的点对（即不同组之间的点对）用一个二进制位表示， 如果某个图中有这条边，则这一位是1，否则为0，就用一个二进制数表示了一个图。

现在要求的就是有多少个图的集合（即二进制数的集合）异或和是0，即在异或图上所有有限制的点对都不存在对应的边。

怎么求呢？用线性基，集合个数就是\(2^{图的数量 - 线性基大小}\)。

```
#include <cstdio>
#include <cstring>
#include <cmath>
#include <algorithm>
#include <iostream>
#include <vector>
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

const int N = 11, M = 61;
char s[N*N];
bool conn[M][N][N];
int n, m, bel[N];
ll fac[N], lb[M], ans;

void dfs(int u, int x){
    if(u > n){
    int sze = 0;
    for(int i = 1; i <= n; i++)
        for(int j = i + 1; j <= n; j++)
        if(bel[i] != bel[j]){
            ll val = 0;
            for(int k = 1; k <= m; k++)
            if(conn[k][i][j]) val |= 1LL << (k - 1);
            for(int k = 1; k <= sze; k++)
            if((val ^ lb[k]) < val) val ^= lb[k];
            if(val) lb[++sze] = val;
        }
    ans += fac[x] * (1LL << (m - sze));
    return;
    }
    for(int i = 1; i <= (x + 1); i++)
    bel[u] = i, dfs(u + 1, x + (i > x));
}

int main(){

    read(m);
    for(int k = 1; k <= m; k++){
    scanf("%s", s + 1);
    if(!n)
        for(int i = 1, len = strlen(s + 1); !n; i++)
        if(i * (i - 1) / 2 == len)
            n = i;
    for(int i = 1, p = 1; i <= n; i++)
        for(int j = i + 1; j <= n; j++)
        conn[k][i][j] = s[p++] - '0';
    }
    fac[1] = 1;
    for(int i = 2; i <= n; i++)
    fac[i] = fac[i - 1] * (1 - i);
    dfs(1, 0);
    write(ans), enter;
    
    return 0;
}
```












