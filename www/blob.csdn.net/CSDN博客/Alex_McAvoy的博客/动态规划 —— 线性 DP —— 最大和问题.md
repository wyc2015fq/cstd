# 动态规划 ——  线性 DP —— 最大和问题 - Alex_McAvoy的博客 - CSDN博客





2019年03月14日 14:57:01[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：65








# 【最长子序列和】

问题定义：对于给定序列 a1,a2,a3……an 寻找它的某个连续子段，使得其和最大

模板：

```cpp
for(int i=2;i<=n;i++)
{
    if(dp[i-1]>=0)
        dp[i]=dp[i-1]+a[i];
    else
        dp[i]=a[i];
}
```

# 【最大子矩阵和】

问题定义：给定一个 m 行 n 列的整数矩阵 A，试求 A 的一个子矩阵，使其各元素之和为最大。

模板：

```cpp
for(int i=0; i<m; i++)//枚举行  
{  
    for(int k=0; k<n;k++)  
        b[k]=0;  
    for(int j=i;j<m;j++)//枚举初始行i,结束行j  
    {  
        for(int k=0; k<n; k++)  
        {  
            b[k] += a[j][k];//b[k]为纵向列之和  
            int maxx = MaxSum(n,b);  

            if(maxx>sum)   
                sum = max;  
        }  
    }  
}
```

# 【数字三角形】

数字三角形问题一般为：给出一个数字三角形，从左上角走到第 i 行第 j 列，求最大和为多少

用 F[i][j] 表示第 i 行第 j 列的最大和，则有状态转移方程：F[i][j]=a[i][j]+max(F[i−1][j],F[i−1][j−1])

模板：

```cpp
for (int i=n-1;i>=1;--i)
    for (int j=1;j<=i;++j)
        a[i][j]+=max(a[i+1][j],a[i+1][j+1]);
printf("%d\n",a[1][1]);
```



