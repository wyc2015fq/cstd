# 动态规划 —— 线性 DP —— 序列问题 - Alex_McAvoy的博客 - CSDN博客





2019年03月14日 14:55:43[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：84








# 【基本概念】
- 子序列： 一个序列 A＝a1,a2,……an 中任意删除若干项，剩余的序列叫做 A 的一个子序列。也可以认为是从序列 A 按原顺序保留任意若干项得到的序列。（例如：对序列｛1,3,5,4,2,6,8,7｝来说，序列｛3,4,8,7｝是它的一个子序列。）
- 公共子序列 ：如果序列 C 既是序列 A 的子序列，也是序列 B 的子序列，则称它为序列 A 和序列 B 的公共子序列。（例如：对序列｛1,3,5,4,2,6,8,7｝和序列｛1,4,8,6,7,5｝来说，序列｛1,8,7｝是它们的一个公共子序列）
- 最长公共子序列：A 和 B 的公共子序列中长度最长的（包含元素最多的）序列叫做 A 和 B 的公共子序列。（ 最长公共子序列不唯一）
- 对于一个长度为 n 的序列，它一共有 2^n 个子序列，有 (2^n – 1) 个非空子序列。
- 子序列不是子集，它和原始序列的元素顺序是相关的。
- 空序列是任何两个序列的公共子序列。
- 角标为 0 时，认为子序列是空序列。

# 【LIS问题】

LIS 问题（Longest Increasing Subsequence），最长上升子序列，其一般为求最长下降子序列或是最长上升子序列。

用 DP[i] 表示 A[i] 为结尾的最长上升子序列的长度，则有状态转移方程：![dp[i]={max}\:(dp[j]+1),0\leqslant j < i,a[j]<a[i]](https://private.codecogs.com/gif.latex?dp%5Bi%5D%3D%7Bmax%7D%5C%3A%28dp%5Bj%5D&plus;1%29%2C0%5Cleqslant%20j%20%3C%20i%2Ca%5Bj%5D%3Ca%5Bi%5D)

模板：

```cpp
for(int i=2;i<=n+1;i++)
{
    int num=0;
    for(int j=i-1;j>=1;j--)
        if(a[i]>a[j])
            num=max(num,dp[j]);

    dp[i]=num+1;
}
```

# 【LCS 问题】

LCS 问题（Longest Common Subsequence），求序列的最长公共子序列。

F[i][j] 表示前缀子串 A[1~i] 与 B[1~j] 的最长公共子序列的长度，则有状态转移方程： 

![F[i][j]=max(F[i-1][j],F[i][j-1],F[i-1][j-1]+1(if\:A[i]=B[j]))](https://private.codecogs.com/gif.latex?F%5Bi%5D%5Bj%5D%3Dmax%28F%5Bi-1%5D%5Bj%5D%2CF%5Bi%5D%5Bj-1%5D%2CF%5Bi-1%5D%5Bj-1%5D&plus;1%28if%5C%3AA%5Bi%5D%3DB%5Bj%5D%29%29)

即：

![](https://img-blog.csdn.net/20180820145909820?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

模板：

```cpp
char s[MAX],t[MAX];  
scanf("%s%s",s,t);  
int x=strlen(s),y=strlen(t);  
for(i=0;i<x;i++)  
{  
    for(j=0;j<y;j++)  
    {
        if(s[i]==t[j])  
            dp[i][j]=dp[i-1][j-1]+1;  
        else  
            dp[i][j]=max(dp[i][j-1],dp[i-1][j]);  
    }  
    printf("%d\n",dp[i][j]);              
}
```

# 【LCIS 问题】

LCIS 问题（Longest Common Increasing Subsequence），求序列的最长公共上升子序列。

用 F[i][j] 表示 A[1~j] 与 B[1~j] 可以构成的以 B[j] 为结尾的 LCIS 的长度，易得状态转移方程：

![\left\{\begin{matrix} dp[i][j] = dp[i-1][j],(a[i]!= b[j]) \\ dp[i][j] = max(dp[i-1][k]+1) ,(1 <= k <= j-1,a[i] =b[j], b[j] > b[k]) \end{matrix}\right.](https://private.codecogs.com/gif.latex?%5Cleft%5C%7B%5Cbegin%7Bmatrix%7D%20dp%5Bi%5D%5Bj%5D%20%3D%20dp%5Bi-1%5D%5Bj%5D%2C%28a%5Bi%5D%21%3D%20b%5Bj%5D%29%20%5C%5C%20dp%5Bi%5D%5Bj%5D%20%3D%20max%28dp%5Bi-1%5D%5Bk%5D&plus;1%29%20%2C%281%20%3C%3D%20k%20%3C%3D%20j-1%2Ca%5Bi%5D%20%3Db%5Bj%5D%2C%20b%5Bj%5D%20%3E%20b%5Bk%5D%29%20%5Cend%7Bmatrix%7D%5Cright.)

因此对于决策集合中的元素只增多不减少的情景，就可以维护一个变量来记录决策集合的当前消息，只需要两重循环即可求解。

模板：

```cpp
for (int i=1;i<=n;++i)
{
    int val=0;//val是决策集合S(i,j)中f[i-1][k]的最大值 
    for(int j=1;j<=m;++j)
    {
        //原来的k循环+判断+状态转移 

        if (a[i]==b[j]) 
            f[i][j]=val+1;
        else 
            f[i][j]=f[i-1][j];

        if (b[j]<a[i]) 
            val=max(val,f[i-1][j]);

        //j即将增大为j+1,检查j能否进入新的决策集合 
    }
}
```



