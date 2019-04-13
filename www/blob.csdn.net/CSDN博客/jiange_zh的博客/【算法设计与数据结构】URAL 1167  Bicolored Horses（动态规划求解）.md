
# 【算法设计与数据结构】URAL 1167. Bicolored Horses（动态规划求解） - jiange_zh的博客 - CSDN博客


2015年11月13日 11:34:00[jiange_zh](https://me.csdn.net/jiange_zh)阅读数：503



## 题目链接：
> [http://acm.timus.ru/problem.aspx?space=1&num=1167](http://acm.timus.ru/problem.aspx?space=1&num=1167)

## 题目大意：
有N匹马，分为黑马和白马，要把他们安排到K个马槽中，最后所有马槽都不能是空的。我们将N匹马排成一列，要求按顺序安排马槽，比如前P1匹马安排在第一个槽，接下来的P2匹马安排在第二个槽。对于某一个马槽，设有i匹黑马和j匹白马，求使得所有马槽的i*j之和最小的方案，输出最小值。
## 思路：
前i个马槽放j匹马，可转移为：前i-1个马槽放m匹马(i-1 <= m < j)，第j个槽放j-m匹马，遍历所有的m，取最优值。
设dp[i][j]表示前i个马槽放j匹马的最优值，black[k]、white[k]分别表示前k匹马中的黑马和白马数量。
则状态转移方程为：
```python
for
```
```python
(int m = i-
```
```python
1
```
```python
; m < j; m++)
    dp[i][j]=
```
```python
min
```
```python
(dp[i-
```
```python
1
```
```python
][m]+(
```
```python
black
```
```python
[j]-
```
```python
black
```
```python
[m])*(white[j]-white[m]), dp[i][j]);
```
## 算法步骤：
### 1）   初始化边界条件：
```python
//先全部置为不合法
```
```python
for
```
```python
(
```
```python
int
```
```python
i =
```
```python
0
```
```python
; i <= k; i++)
```
```python
for
```
```python
(
```
```python
int
```
```python
j =
```
```python
0
```
```python
; j <= n; j++) 
        dp[i][j] = INF;
```
```python
//前i个马槽放i匹马，即一匹马一个马槽时的情况
```
```python
for
```
```python
(
```
```python
int
```
```python
i =
```
```python
0
```
```python
; i < MAXN; i++)
    dp[i][i]=
```
```python
0
```
```python
;
```
### 2）   根据状态转移方程动态规划求解：
```python
for
```
```python
(
```
```python
int
```
```python
i =
```
```python
1
```
```python
; i <= k; i++)
```
```python
for
```
```python
(
```
```python
int
```
```python
j = i; j <= n-(k-i); j++)
```
```python
//至少留下(k-i)匹马给剩下的k-i个马槽
```
```python
for
```
```python
(
```
```python
int
```
```python
m = i-
```
```python
1
```
```python
; m < j; m++)
```
```python
//前i-1个马槽放m匹马，第i个马槽放j-m匹马，于是需要算出m到j之间的黑马和白马数量
```
```python
dp[i][j]=
```
```python
min
```
```python
(dp[i-
```
```python
1
```
```python
][m]+(black[j]-black[m])*(white[j]-white[m]), dp[i][j]);
cout<<dp[k][n]<<endl;
```
## 源程序：
```python
#include<iostream>
```
```python
#include<iomanip>
```
```python
#include<cmath>
```
```python
#include<string>
```
```python
#include<cstring>
```
```python
using
```
```python
namespace
```
```python
std
```
```python
;
```
```python
const
```
```python
int
```
```python
INF =
```
```python
99999999
```
```python
;
```
```python
const
```
```python
int
```
```python
MAXN =
```
```python
505
```
```python
;
```
```python
bool
```
```python
h[MAXN];
```
```python
int
```
```python
dp[MAXN][MAXN];
```
```python
int
```
```python
black[MAXN],white[MAXN];
```
```python
int
```
```python
main()
{
```
```python
memset
```
```python
(white,
```
```python
0
```
```python
,
```
```python
sizeof
```
```python
(white));
```
```python
memset
```
```python
(black,
```
```python
0
```
```python
,
```
```python
sizeof
```
```python
(black));
```
```python
int
```
```python
n, k;
```
```python
cin
```
```python
>>n>>k;
```
```python
for
```
```python
(
```
```python
int
```
```python
i =
```
```python
1
```
```python
; i <= n; i++)
    {
```
```python
cin
```
```python
>>h[i];  
        black[i] = black[i-
```
```python
1
```
```python
] + h[i];
```
```python
//前i匹马中，黑马的数量
```
```python
white[i] = white[i-
```
```python
1
```
```python
] + !h[i];
    }
```
```python
for
```
```python
(
```
```python
int
```
```python
i =
```
```python
0
```
```python
; i <= k; i++)
```
```python
for
```
```python
(
```
```python
int
```
```python
j =
```
```python
0
```
```python
; j <= n; j++) 
            dp[i][j] = INF;
```
```python
for
```
```python
(
```
```python
int
```
```python
i =
```
```python
0
```
```python
; i < MAXN; i++)
         dp[i][i]=
```
```python
0
```
```python
;
```
```python
//前i个马槽放j匹马，可转移为：前i-1个马槽放m匹马(i-1<=m<j)，第j个槽放j-m匹马，遍历所有的m，取最优值
```
```python
for
```
```python
(
```
```python
int
```
```python
i =
```
```python
1
```
```python
; i <= k; i++)
```
```python
for
```
```python
(
```
```python
int
```
```python
j = i; j <= n-(k-i); j++)
```
```python
//至少留下(k-i)匹马给剩下的k-i个马槽
```
```python
for
```
```python
(
```
```python
int
```
```python
m = i-
```
```python
1
```
```python
; m < j; m++)
```
```python
//前i-1个马槽放m匹马，第i个马槽放j-m匹马，于是需要算出m到j之间的黑马和白马数量
```
```python
dp[i][j]=min(dp[i-
```
```python
1
```
```python
][m]+(black[j]-black[m])*(white[j]-white[m]), dp[i][j]);
```
```python
cout
```
```python
<<dp[k][n]<<endl;
```
```python
return
```
```python
0
```
```python
;
}
```

