# dp练习（8）——数的划分 - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)







### **1039 数的划分**

2001年NOIP全国联赛提高组





 时间限制: 1 s



 空间限制: 128000 KB



 题目等级 : 黄金 Gold




[题解](http://codevs.cn/wiki/solution/?problem_id=1039)












题目描述 Description


将整数n分成k份，且每份不能为空，任意两种划分方案不能相同(不考虑顺序)。
例如：n=7，k=3，下面三种划分方案被认为是相同的。
1 1 5

1 5 1

5 1 1
问有多少种不同的分法。




输入描述 Input Description


输入：n，k (6<n<=200，2<=k<=6)




输出描述 Output Description



输出：一个整数，即不同的分法。




样例输入 Sample Input


 7 3




样例输出 Sample Output


4




数据范围及提示 Data Size & Hint


 {四种分法为：1，1，5;1，2，4;1，3，3;2，2，3;}






贴出代码：


```
#include<bits/stdc++.h>
using namespace std;

int main()
{
    int n,k;
    cin >> n >> k;
    int dp[505][505];
    for(int i=1;i <= n;i++)
    {
        dp[i][1] = 1;
        dp[i][i] = 1;
    }
    for(int i=1;i <= n;i++)
    {
        for(int j=1;j < i;j++)
        {
            dp[i][j] = dp[i-1][j-1] + dp[i-j][j];
        }
    }
    cout << dp[n][k] << endl;

    return 0;
}
```

代码解释：

首先分析第一段：

```
for(int i=1;i <= n;i++)
    {
        dp[i][1] = 1;
        dp[i][i] = 1;
    }
```

dp[第几个数][划分几次]   无论什么数划分1次的结果都是只有一种情况，无论什么数划分和他位数相同的数的结果只有一种情况。

然后分析第二段：

```
for(int i=1;i <= n;i++)
    {
        for(int j=1;j < i;j++)
        {
            dp[i][j] = dp[i-1][j-1] + dp[i-j][j];
        }
    }
```

这个我一开始也思考不明白，感觉和传统的dp有点不同，结合了各个博客的解释稍微理解了点。

主要思想：分为有1存在和没有1存在两种情况

有1存在：dp[i-1][j-1] 表示的是将i分为最小值为1的方案的总个数，例如，6(=7-1)分成2(=3-1)份可以分为{1,5}{2,4}{3,3},则7可以分为{1,5,1}{2,4,1}{3,3,1}【共3种】

没有1存在：dp[i-j][j-1]]表示的是将i分为不包含1的方案的总个数，例如，4(=7-3)分成3份可以分为{1,1,2}，则7可以分为{1+1,1+1,2+1}->{2,2,3}【共1种】**注意下划线部分**



由此构成了如下的递推式：
 dp[i][j] = dp[i-1][j-1] + dp[i-j][j];











