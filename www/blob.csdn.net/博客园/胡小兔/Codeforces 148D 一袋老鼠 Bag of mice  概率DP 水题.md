# Codeforces 148D 一袋老鼠 Bag of mice | 概率DP 水题 - 胡小兔 - 博客园







# [Codeforces 148D 一袋老鼠 Bag of mice | 概率DP 水题](https://www.cnblogs.com/RabbitHu/p/CF148D.html)





除非特别忙，我接下来会尽可能翻译我做的每道CF题的题面！

# [Codeforces 148D](http://codeforces.com/contest/148/problem/D) 一袋老鼠 Bag of mice | 概率DP 水题

## 题面

胡小兔和司公子都认为对方是垃圾。

为了决出谁才是垃圾，大哥拿来了一袋老鼠，其中有w只白老鼠和b只黑老鼠。胡小兔先抓，先抓到白老鼠的人赢。

每次学姐抓完老鼠之后，总会有另外一只老鼠从袋子里自己跑出来（这只老鼠不算任何人抓的），而胡小兔抓老鼠时则不会发生这样的事。

每次袋子里的每只老鼠被抓到的概率相等，当有一只老鼠跑出来的时候，每只老鼠跑出来的几率也相等。抓出来和跑出来的老鼠都不会回到袋子中。

如果袋子里没有老鼠了，并且没有人抓到过白老鼠，则学姐赢。

给出w和b，求胡小兔赢的概率。

数据范围：w, b <= 1000。

## 题解

这道题还是很水的，dp[i][j][0/1]表示“还剩i黑j白，该胡小兔/学姐抓，胡小兔赢的概率”。写出转移方程即可：

$ dp[i][j][0] = \frac{i}{i + j} dp[i - 1][j][1] + \frac{j}{i + j} $

$ dp[i][j][1] = \frac{i}{i + j} * \frac{i - 1}{i + j - 1} dp[i - 2][j] + \frac{i}{i + j} * \frac{j}{i + j - 1} dp[i - 1][j - 1] $ (前面是抓黑跑黑，后面是抓黑跑白）

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
bool read(T &x){
    char c;
    bool op = 0;
    while(c = getchar(), c < '0' || c > '9')
    if(c == '-') op = 1;
    else if(c == EOF) return 0;
    x = c - '0';
    while(c = getchar(), c >= '0' && c <= '9')
    x = x * 10 + c - '0';
    if(op) x = -x;
    return 1;
}
template <class T>
void write(T x){
    if(x < 0) putchar('-'), x = -x;
    if(x >= 10) write(x / 10);
    putchar('0' + x % 10);
}
const int N = 1003;
int w, b;
double dp[N][N][2];
int main(){
    read(w), read(b);
    for(int i = 0; i <= b; i++)
    for(int j = 0; j <= w; j++){
        if(i + j) dp[i][j][0] += (double)j / (i + j);
        if(i) dp[i][j][0] += (double)i / (i + j) * dp[i - 1][j][1];
        if(i > 1) dp[i][j][1] += (double)i / (i + j) * (i - 1) / (i + j - 1) * dp[i - 2][j][0];
        if(i && j) dp[i][j][1] += (double) i / (i + j) * j / (i + j - 1) * dp[i - 1][j - 1][0];
        //printf("dp[%d][%d][0] = %lf, dp[%d][%d][1] = %lf\n", i, j, dp[i][j][0], i, j, dp[i][j][1]);
    }
    printf("%.11lf\n", dp[b][w][0]);
    return 0;
}
```












