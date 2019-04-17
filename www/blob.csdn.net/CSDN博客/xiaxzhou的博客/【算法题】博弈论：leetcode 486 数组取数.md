# 【算法题】博弈论：leetcode 486 数组取数 - xiaxzhou的博客 - CSDN博客





2017年09月15日 11:28:55[xiaxzhou](https://me.csdn.net/xiaxzhou)阅读数：578








[参见](http://www.cnblogs.com/liujinhong/p/6477367.html)

【题目】：

给定一个正整数数组，选手1从数组的头部或者尾部选择一个数，选手2从剩下部分的头部或尾部选择一个数，循环往复，直到该数组中的数都被取完。选手1、2都足够聪明。判断选手1取的数的和值是否大于选手2.

【思路】：

$dp[i][j]$表示原数组中从i到j的这么多数中，按照游戏规则，某个玩家所能获得的最大分数。 

> 


$dp[i][j]=max(sum[i][j]-dp[i+1][j], sum[i][j]-dp[i][j-1]) $


注： $dp[i][j]=max(sum[i+1][j]-dp[i+1][j]+nums[i], sum[i][j-1]-dp[i][j-1]+nums[j]) $

```cpp
#include <vector>
#include <cstdio>
#include <numeric>
#include <iostream>
#include <algorithm>

using namespace std;

bool PredictTheWinner(vector<int>& nums)
{
    int n = nums.size();
    vector<vector<int>> dp(n, vector<int>(n));
    vector<int> sum(n);

    for (int i = 0; i < n; i++)
        dp[i][i] = nums[i];

    sum[0] = nums[0];

    for (int i = 0; i < n - 1; i++)
        sum[i + 1] = sum[i] + nums[i + 1];//累积分布和

    //三角
    for (int i = 1; i < n; i++)
        for (int j = 0; i + j < n; j++)
            dp[j][i + j] = max(sum[i + j] - sum[j] + nums[j] - dp[j + 1][i + j], sum[i + j] - sum[j] + nums[j] - dp[j][i + j - 1]);

    return 2 * dp[0][n - 1] >= sum[n - 1];
}

int main()
{
    vector<int>  vec{1,2,3,4,5,6};
    cout<<PredictTheWinner(vec)<<endl;

    return 0;
}
```



