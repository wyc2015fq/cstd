# BZOJ 3864 Hero meet devil 超详细超好懂题解 - 胡小兔 - 博客园







# [BZOJ 3864 Hero meet devil 超详细超好懂题解](https://www.cnblogs.com/RabbitHu/p/BZOJ3864.html)





## 题目链接

[BZOJ 3864](https://www.lydsy.com/JudgeOnline/problem.php?id=3864)

## 题意简述

设字符集为ATCG，给出一个长为\(n(n \le 15)\)的字符串\(A\)，问有多少长度为\(m(m \le 1000)\)的字符串\(B\)与\(A\)的最长公共子序列为\(i\)，对所有\(0 \le i \le n\)输出答案。

## 题解

传说中的计算机理论科科科科科学家cls的DP套DP。

因为看别人写的题解我都看不懂……所以我在这篇题解中，换一种方式讲解，从暴力一点点优化得到DP套DP，应该更容易理解。

暴力怎么写呢？显然是枚举所有可能的字符串\(B\)，然后对每一个都用经典的DP，求出与\(A\)的LCS。写个伪代码：

```
dfs(cur)
    if(cur > m)
        for(i: 1 -> m)
            for(j: 1 -> n)
                f[i][j] = max(f[i - 1][j], f[i][j - 1])
                if(b[i] == a[j]) f[i][j] = max(f[i][j], f[i - 1][j - 1] + 1)
            ans[f[m][n]]++
        return;
    for(c in {A, T, C, G})
        b[cur] = c
        dfs(cur + 1)
```

考虑略微更改一下暴力的顺序，从把字符串枚举完再DP求LCS，变成一边枚举一边DP，并把\(f[cur]\)传入到递归函数中。

```
dfs(cur, f[])
    if(cur > m)
        ans[f[n]]++
        return;
    for(c in {A, T, C, G})
        for(i : 1 -> n)
            newf[i] = max(f[i], newf[i - 1])
            if(c == a[i]) newf[i] = max(newf[i], f[i - 1] + 1)
        dfs(cur + 1, newf)
```

往函数里传一个数组显然非常菜，考虑状压这个\(f\)数组。显然，一行f数组的每一位\(f[i]\)要么比\(f[i - 1]\)多1，要么和\(f[i - 1]\)相同。那么用一个长为\(n\)的二进制数状压这个\(f\)数组的差分即可。伪代码（**\(cnt1(s)\)表示二进制数\(s\)中1的个数，此时就等于\(f[n]\)**）：

```
dfs(cur, s)
    if(cur > m)
        ans[cnt1(s)]++
        return;
    for(c in {A, T, C, G})
        for(i : 1 -> n)
            f[i] = f[i - 1] + (s >> (i - 1) & 1)
        for(i : 1 -> n)
            newf[i] = max(f[i], newf[i - 1])
            if(c == a[i]) newf[i] = max(newf[i], f[i - 1] + 1)
        for(i: 1 -> n)
            t |= (f[i] - f[i - 1]) << (i - 1)
        dfs(cur + 1, t)
```

\(s\)显然有很多重复的，每层DFS都这样算一遍非常浪费，因为这段代码中\(s\)对应的\(t\)只和\(c\)有关，不如预处理出每个\(s\)在\(B[cur] == c\)时能转移到哪个状态\(t\)（预处理方法就和上面这段代码中的那部分一样）。设这个状态\(t\)为\(trans[s][c]\)。

```
dfs(cur, s)
    if(cur > m)
        ans[cnt1(s)]++
        return;
    for(c in {A, T, C, G})
        dfs(cur + 1, trans[s][c])
```

这个DFS都变成这样了，忍不住考虑能不能把它变成DP。用\(dp[i][s]\)表示字符串\(B\)长为\(i\)，对应的数组\(f\)状压后为\(s\)的方案数。

```
dp[0][0] = 1
for(i : 1 -> m)
    for(s: 1 -> (1 << n) - 1)
        for(c in {A, T, C, G})
            dp[i][trans[s][c]] += dp[i - 1][s]
for(s: 1 -> (1 << n) - 1)
    ans[cnt1(s)] += dp[m][s]
```

至此你就从DFS暴力一步步优化出了这道题的DP套DP解法！

## AC代码

```
#include <cstdio>
#include <cmath>
#include <cstring>
#include <algorithm>
#include <iostream>
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

const int N = 15, M = 1005, P = 1000000007;
int T, n, m, id[128], a[N];
int bcnt[1<<N], trans[1<<N][4], f[2][1<<N];
char str[N];

void init_trans(){
    static int pre[N], cur[N];
    for(int s = 0; s < (1 << n); s++){
    if(s) bcnt[s] = bcnt[s >> 1] + (s & 1);
    pre[0] = s & 1;
    for(int i = 1; i < n; i++)
        pre[i] = pre[i - 1] + (s >> i & 1);
    for(int c = 0; c < 4; c++){
        int t = 0;
        cur[0] = pre[0];
        if(c == a[0]) cur[0] = 1;
        t |= cur[0];
        for(int i = 1; i < n; i++){
        cur[i] = max(cur[i - 1], pre[i]);
        if(c == a[i]) cur[i] = max(cur[i], pre[i - 1] + 1);
        t |= (cur[i] - cur[i - 1]) << i;
        }
        trans[s][c] = t;
    }
    }
}
void inc(int &x, int y){
    x += y;
    if(x >= P) x -= P;
}
void calc_f(){
    int pre = 1, cur = 0;
    memset(f[1], 0, sizeof(f[1]));
    f[1][0] = 1;
    for(int i = 0; i < m; i++){
    for(int s = 0; s < (1 << n); s++)
        f[cur][s] = 0;
    for(int s = 0; s < (1 << n); s++)
        if(f[pre][s]){
        for(int c = 0; c < 4; c++)
            inc(f[cur][trans[s][c]], f[pre][s]);
        }
    swap(pre, cur);
    }
    static int ans[N + 1];
    for(int i = 0; i <= n; i++) ans[i] = 0;
    for(int s = 0; s < (1 << n); s++)
    inc(ans[bcnt[s]], f[pre][s]);
    for(int i = 0; i <= n; i++)
    write(ans[i]), enter;
}

int main(){

    id['A'] = 0, id['T'] = 1, id['C'] = 2, id['G'] = 3;
    read(T);
    while(T--){
    scanf("%s%d", str, &m);
    n = strlen(str);
    for(int i = 0; i < n; i++)
        a[i] = id[int(str[i])];
    init_trans();
    calc_f();
    }

    return 0;
}
```












