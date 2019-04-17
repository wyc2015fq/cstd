# 【剑指offer】题43：n个骰子的点数 - xiaxzhou的博客 - CSDN博客





2017年07月05日 12:02:15[xiaxzhou](https://me.csdn.net/xiaxzhou)阅读数：136








动态规划 

使用dp[k][i]表示k个骰子时，点数和为i的组合数 

则转移状态方程： 


$dp[k][i]=\sum_{j=1}^6{dp[k-1][i-j]}$
```cpp
void func(int n)
{
    if (n < 1)
    {
        return;
    }
    vector<int> vec(n*6+1);
    vector<vector<int>> dp(n+1,vec);

    for (auto i = 1; i <= 6;i++)
    {
        dp[1][i] = 1;
    }
    for (auto k = 1; k < n; ++k)
    {
        for (auto i = k; i < 6*k+1; ++i)
        {
            for (auto j = 1; j <= 6; j++)
            {
                dp[k + 1][i + j] += dp[k][i];
            }
        }
    }
    int sum = accumulate(dp[n].begin(), dp[n].end(),0);
    for (auto i = 0; i < 6 * n + 1;++i)
    {
        if (dp[n][i]!=0)
        {
            cout << i << " " << (double)dp[n][i] / (double)sum<<endl;
        }
    }
}
```




