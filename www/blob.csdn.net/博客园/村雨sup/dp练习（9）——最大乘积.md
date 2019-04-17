# dp练习（9）——最大乘积 - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)







### **1017 乘积最大**

2000年NOIP全国联赛普及组NOIP全国联赛提高组





 时间限制: 1 s



 空间限制: 128000 KB



 题目等级 : 黄金 Gold




[题解](http://codevs.cn/wiki/solution/?problem_id=1017)












题目描述 Description


今年是国际数学联盟确定的“2000——世界数学年”，又恰逢我国著名数学家华罗庚先生诞辰90周年。在华罗庚先生的家乡江苏金坛，组织了一场别开生面的数学智力竞赛的活动，你的一个好朋友XZ也有幸得以参加。活动中，主持人给所有参加活动的选手出了这样一道题目：



设有一个长度为N的数字串，要求选手使用K个乘号将它分成K+1个部分，找出一种分法，使得这K+1个部分的乘积能够为最大。



同时，为了帮助选手能够正确理解题意，主持人还举了如下的一个例子：



有一个数字串：312， 当N=3，K=1时会有以下两种分法：



1)  3*12=36

2)  31*2=62



   这时，符合题目要求的结果是：31*2=62



   现在，请你帮助你的好朋友XZ设计一个程序，求得正确的答案。




输入描述 Input Description


程序的输入共有两行：

   第一行共有2个自然数N，K（6≤N≤40，1≤K≤6）

   第二行是一个长度为N的数字串。




输出描述 Output Description


   结果显示在屏幕上，相对于输入，应输出所求得的最大乘积（一个自然数）。




样例输入 Sample Input


4  2

1231




样例输出 Sample Output


62




数据范围及提示 Data Size & Hint


本题由于比较老，数据实际也比较小，用long long 即可通过

```
#include<bits/stdc++.h>
using namespace std;

long long dp[1005][1005];
char s[1005];

long long SUM(int j,int i)
{
    long long sum = 0;
    for(int t=j-1;t < i;t++)
    {
        sum = sum * 10 + (s[t] - '0');
    }
    return sum;
}

int main()
{
    int n,k;
    cin >> n >> k;
    cin >> s;
    memset(dp,0, sizeof(dp));
    for(int i=1;i <= n;i++)
    {
        dp[i][0] = SUM(1,i);
//        cout << dp[i][0] << endl;
    }

    for(int h=1;h <= k;h++)
    {
        for(int i=1;i <= n;i++)
        {
            for(int j=1;j <= i;j++)
            {
                dp[i][h] = max(dp[i][h],dp[j][h-1] * SUM(j+1,i));
            }
        }
    }

//    for(int i=0;i < 5;i++)
//    {
//        for(int j=0;j < 5;j++)
//            cout << dp[i][j] << " ";
//        cout << endl;
//    }
    cout << dp[n][k] << endl;


    return 0;
}
```

隔了一天重写一遍，写的倒是很快，基本都是背出来的，不是自己想出来的，还是要加深理解，不要当一个模板战士。
















