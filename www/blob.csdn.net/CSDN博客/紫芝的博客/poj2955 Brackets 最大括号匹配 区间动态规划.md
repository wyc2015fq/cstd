# poj2955 Brackets 最大括号匹配 区间动态规划 - 紫芝的博客 - CSDN博客





2018年07月29日 00:21:51[紫芝](https://me.csdn.net/qq_40507857)阅读数：67








# **Brackets**
|**Time Limit:** 1000MS||**Memory Limit:** 65536K|
|----|----|----|
|**Total Submissions:** 11633||**Accepted:** 6145|

**Description**

We give the following inductive definition of a “regular brackets” sequence:
- the empty sequence is a regular brackets sequence,
- if *s* is a regular brackets sequence, then (*s*) and [*s*] are regular brackets sequences, and
- if *a* and *b* are regular brackets sequences, then *ab* is a regular brackets sequence.
- no other sequence is a regular brackets sequence

For instance, all of the following character sequences are regular brackets sequences:

> 
`(), [], (()), ()[], ()[()]`


while the following character sequences are not:

> 
`(, ], )(, ([)], ([(]`


Given a brackets sequence of characters *a*1*a*2 … *an*, your goal is to find the length of the longest regular brackets sequence that is a subsequence of *s*. That is, you wish to find the largest *m*such that for indices *i*1, *i*2, …, *im* where 1 ≤ *i*1 < *i*2 < … < *im* ≤ *n*, *ai*1*ai*2 … *aim* is a regular brackets sequence.

Given the initial sequence `([([]])]`, the longest regular brackets subsequence is `[([])]`.

**Input**

The input test file will contain multiple test cases. Each input test case consists of a single line containing only the characters `(`, `)`, `[`, and `]`; each input test will have length between 1 and 100, inclusive. The end-of-file is marked by a line containing the word “end” and should not be processed.

**Output**

For each input case, the program should print the length of the longest possible regular brackets subsequence on a single line.

**Sample Input**

```
((()))
()()()
([]])
)[)(
([][][)
end
```

**Sample Output**

```
6
6
4
0
6
```

**Source**

[Stanford Local 2004](http://poj.org/searchproblem?field=source&key=Stanford+Local+2004)

•题目大意：给出一个括号序列，求出其中括号最大匹配数量

•**第一步：确定状态**

•f[i][j] 表示ai……aj的串中，有多少个已经匹配的括号

•**第二步：确定状态转移方程**

•如果ai与aj是匹配的

•f[i][j] = f[i + 1][j - 1] + 2

•否则f[i][j] = max(f[i][j],f[i][k] + f[k + 1][j])

•（相当于是将i到j分成[xxxxx]xxxxx两部分）

•边界 f[i][i] = 0

```cpp
//#include<bits/stdc++.h>
#include<cstdio>
#include<cstring>
#include<algorithm>
using namespace std;
const int maxn=150;
int dp[maxn][maxn];//表示字符串s的第i..j字符需要最少括号数
char a[maxn];
bool cmp(int n,int m)
{
    if((n=='('&&m==')')||(n=='['&&m==']'))
        return 1;
    return 0;
}
int main()
{
    //while(scanf("%s",a)!=EOF){
    while(scanf("%s",a+1)!=EOF&&a[1]!='e'){//下标从1开始
        memset(dp,0,sizeof(dp));
        int n=strlen(a+1);
        for(int len=2;len<=n;len++)
        {
            for(int i=1;i<=n;i++)
            {
                int j=i+len-1;
                if(j>n) break;

                if(cmp(a[i],a[j]))//如果ai与aj是匹配的
                {
                    if(i+1>j-1) dp[i][j]=2;//中间长度为0
                    else dp[i][j]=dp[i+1][j-1]+2;
                }
                for(int k=i;k<j;k++)//中断点
                {
                    dp[i][j]=max(dp[i][j],dp[i][k]+dp[k+1][j]);
                }
            }
        }
        printf("%d\n",dp[1][n]);
    }
    return 0;
}
```





