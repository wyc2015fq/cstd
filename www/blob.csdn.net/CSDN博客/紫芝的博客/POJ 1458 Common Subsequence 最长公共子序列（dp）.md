# POJ 1458 Common Subsequence  最长公共子序列（dp） - 紫芝的博客 - CSDN博客





2018年07月25日 09:35:36[紫芝](https://me.csdn.net/qq_40507857)阅读数：20
个人分类：[动态规划](https://blog.csdn.net/qq_40507857/article/category/7565832)









## [Common Subsequence](https://cn.vjudge.net/problem/POJ-1458)

## [POJ - 1458](https://cn.vjudge.net/problem/17083/origin)

A subsequence of a given sequence is the given sequence with some elements (possible none) left out. Given a sequence X = < x1, x2, ..., xm > another sequence Z = < z1, z2, ..., zk > is a subsequence of X if there exists a strictly increasing sequence < i1, i2, ..., ik > of indices of X such that for all j = 1,2,...,k, x ij= zj. For example, Z = < a, b, f, c > is a subsequence of X = < a, b, c, f, b, c > with index sequence < 1, 2, 4, 6 >. Given two sequences X and Y the problem is to find the length of the maximum-length common subsequence of X and Y.

Input

The program input is from the std input. Each data set in the input contains two strings representing the given sequences. The sequences are separated by any number of white spaces. The input data are correct.

Output

For each set of data the program prints on the standard output the length of the maximum-length common subsequence from the beginning of a separate line.

Sample Input

abcfbc         abfcab
programming    contest 
abcd           mnp
Sample Output

4
2
0
```cpp
#include<stdio.h>
#include<iostream>
#include<string.h>
#include<queue>
#include<cstdio>
#include<string>
#include<math.h>
#include<algorithm>
#include<map>
#include<set>
#include<stack>
#define mod 998244353
#define INF 0x3f3f3f3f
#define eps 1e-6
using namespace std;
typedef long long ll;
using namespace std;
const int MAXX=1000+5;
int dp[MAXX][MAXX];
char a[MAXX],b[MAXX];
int main(){
    while(scanf("%s%s",a+1,b+1)!=EOF){
        memset(dp,0,sizeof dp);
        int la=strlen(a+1);
        int lb=strlen(b+1);
        for(int i=1;i<=la;i++){
            for(int j=1;j<=lb;j++){
                if(a[i]==b[j])
                    dp[i][j]=dp[i-1][j-1]+1;
                else
                    dp[i][j]=max(dp[i-1][j],dp[i][j-1]);
            }
        }
        printf("%d\n",dp[la][lb]);
    }
}
```

## 反恐精英渣渣辉

时间限制: 1 Sec  内存限制: 128 MB

## 题目描述

渣渣辉从小的梦想就是当一名警察，做一名出色的反恐精英。

但是事与愿违，他现在只能天天在家里打 CF (codeforce) 了。

我们知道，CF上经常有比赛，渣渣辉这么热爱CF，当然一次也没有落下。

比赛规则是这样的： 

        1.有 N 种子弹型号，编号分别为 1，2 ... N。

        2.有 N 种恐怖分子类型，编号也分别为1，2 ... N。

        3.敌人有 M 个，子弹也会有 M 个。

        4.只有一种子弹打中同种编号类型的敌人时，得分才会加 1 分。

        5.子弹已经按照输入顺序装填，恐怖分子也按照输入顺序出现。

你能帮助渣渣辉打到最高分吗？

（渣渣辉射击百发百中，他可以放空枪，也可以选择不射击某名恐怖分子）

## 输入

输入有多组测试样例。

对于每组测试样例：

第一行输入为两个正整数 N 和 M，表示有 N 种类型的敌人和子弹，M 个敌人和子弹。( 1 <= N <= 100，1 <= M <= 1000） 

第二行输入为 M 个数字，分别代表按照顺序出现的恐怖分子类型。

第三行输入为 M 个数字，分别代表按照顺序装填的子弹类型。

## 输出

对于每组测试样例，输出占一行，输出结果为渣渣辉可以得到的最高分数。

## 样例输入

```
3 6
1 2 2 3 1 1
2 2 1 2 1 3
```

## 样例输出
`4`
分析：动态规划基础题目，求子弹序列和敌人序列的最长上升子序列即可

```cpp
//最长公共子序列
#include<bits/stdc++.h>
using namespace std;
const int maxn=1005;
int n,m;//n为无用参数，m为序列长度
int a[maxn],b[maxn];
int dp[maxn][maxn];
int main()
{
    while(scanf("%d%d",&n,&m)!=EOF){
        memset(dp,0,sizeof(dp));
        for(int i=1;i<=m;i++)
            scanf("%d",&a[i]);

        for(int j=1;j<=m;j++)
            scanf("%d",&b[j]);

       for(int i=1;i<=m;i++)
        for(int j=1;j<=m;j++)
        if(a[i]==b[j])
        dp[i][j]=dp[i-1][j-1]+1;
       else dp[i][j]=max(dp[i-1][j],dp[i][j-1]);

       printf("%d\n",dp[m][m]);
    }
    return 0;
}
```





