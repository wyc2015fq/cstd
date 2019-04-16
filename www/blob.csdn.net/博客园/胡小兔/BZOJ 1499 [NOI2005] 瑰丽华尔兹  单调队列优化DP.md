# BZOJ 1499 [NOI2005] 瑰丽华尔兹 | 单调队列优化DP - 胡小兔 - 博客园







# [BZOJ 1499 [NOI2005] 瑰丽华尔兹 | 单调队列优化DP](https://www.cnblogs.com/RabbitHu/p/BZOJ1499.html)





# [BZOJ 1499](http://www.lydsy.com/JudgeOnline/problem.php?id=1499) 瑰丽华尔兹 | 单调队列优化DP

## 题意

> 
有一块\(n \times m\)的矩形地面，上面有一些障碍（用'#'表示），其余的是空地（用'.'表示）。每时每刻，地面都会向某个方向倾斜，但不同时刻倾斜方向可能不同：具体来说，当时间处于区间\([l_i, r_i]\)内时，向\(d_i\)方向倾斜（输入保证给出的所有区间是连续的）。

海上钢琴师1900的钢琴在这块地面上滑动，每时每刻，可以选择让钢琴向着地面倾斜的方向滑动一格，或是待在原地不动。当然，钢琴不能滑到障碍上，也不能滑出矩形地面之外。问最多能滑动多少路程（进行一次“向某方向滑动一格”这个操作视作增加1单位的路程）。

数据范围：\(n, m, k \le 200\).


## 题解

首先，这道题是个DP。用\(f[i][x][y]\)表示第i个时间区间结束时（即当前时间是该区间的右端点）以位置\((x, y)\)为终点能走过的最长路程。

四种倾斜方向也对应着四种不同的状态转移方程。以下移为例，状态转移方程是：
\[f[i][x][y] = \max_{j = s}^{x} \{f[i - 1][j][y] + x - j\}\]

其中，\(s\) = max(x - 时间区间长度, 上一个障碍物的位置 + 1)。

如果直接暴力这样做，复杂度是\(O(n^4)\)的（\(n, m, k\)同阶），显然会TLE。怎么优化呢？

我们发现，所谓对\(f[i - 1][j][y] + x - j\)取max，其实只要对\(f[i - 1][j][y] - j\)取max即可。假如\(j\)的起始点\(s\)不变，那么只需维护一个变量记录\(f[i - 1][j][y] - j\)的最大值、固定从这个最大值转移；但是现在\(s\)随着要求的\(x\)的变化而变化，怎么办呢？注意到\(s\)随着\(x\)的增加也是递增的，可以用一个带有取max功能的队列——单调队列来维护。

所以这道题状态转移部分的伪代码大概是这样的：

将所有距离当前求的\(x\)超过时间区间长度的\(j\)从队首中弹出；

将队尾所有值小于\(f[i - 1][x][y]\)的元素弹出，并将\(f[i-1][x][y] - x\)放到队尾。

（至此所有\(f[i][x][y]\)可以由之转移而来的值已经全在队列中了，且队首的是值最大的、也就是最优的那个。）

用队首元素求出\(f[i][x][y] = f[i - 1][que.front()][y]\) + x。

以上是以“向下倾斜”为例写出的过程，其他三个方向也类似。

以下的内容可以不看。

> 
学姐说：“我这个代码没超过200行……”


于是我决定往死里压行。想压行，就一定要摒弃复制粘贴四次、每段代码稍有不同来处理四个不同倾斜方向的写法！一定要写一个函数！

于是写了下面这样的代码……

```
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
const int N = 205, _INF = 0xc0c0c0c0;
const int dx[] = {0, -1, 1, 0, 0}, dy[] = {0, 0, 0, -1, 1};
int ans, n, m, q, sx, sy, f[2][N][N], len, que[N], pos[N];
char mp[N][N];
void solve(int i, int x, int y, int dir){
    int cnt = 1, ql = 1, qr = 0;
    while(x >= 1 && y >= 1 && x <= n && y <= m){
        if(mp[x][y] == 'x') ql = 1, qr = 0;
        while(ql <= qr && cnt - pos[ql] > len) ql++;
        while(ql <= qr && que[qr] + cnt <= f[i ^ 1][x][y]) qr--;
        que[++qr] = f[i ^ 1][x][y] - cnt, pos[qr] = cnt;
        f[i][x][y] = que[ql] + cnt;
        ans = max(ans, f[i][x][y]);
        cnt++, x += dx[dir], y += dy[dir];
    }
}
int main(){
    read(n), read(m), read(sx), read(sy), read(q);
    for(int i = 1; i <= n; i++)
        scanf("%s", mp[i] + 1);
    memset(f, _INF, sizeof(f));
    f[0][sx][sy] = 0;
    for(int i = 1, st, ed, dir; i <= q; i++){
        read(st), read(ed), read(dir), len = ed - st + 1;
        if(dir == 1) for(int j = 1; j <= m; j++) solve(i & 1, n, j, dir);
        if(dir == 2) for(int j = 1; j <= m; j++) solve(i & 1, 1, j, dir);
        if(dir == 3) for(int j = 1; j <= n; j++) solve(i & 1, j, m, dir);
        if(dir == 4) for(int j = 1; j <= n; j++) solve(i & 1, j, 1, dir);
    }
    write(ans), enter;
    return 0;
}
```












