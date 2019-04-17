# dp练习（5）——最长严格上升子序列 - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)







### **1576 最长严格上升子序列**







 时间限制: 1 s



 空间限制: 256000 KB



 题目等级 : 黄金 Gold




[题解](http://codevs.cn/wiki/solution/?problem_id=1576)












题目描述 Description


给一个数组a1, a2 ... an，找到最长的上升降子序列ab1<ab2< .. <abk，其中b1<b2<..bk。

输出长度即可。




输入描述 Input Description


第一行，一个整数N。

第二行 ，N个整数（N < = 5000）




输出描述 Output Description


输出K的极大值，即最长不下降子序列的长度




样例输入 Sample Input


5

9 3 6 2 7




样例输出 Sample Output


3




数据范围及提示 Data Size & Hint


【样例解释】

最长不下降子序列为3,6,7

```
#include <bits/stdc++.h>
using namespace std;

int main()
{
    int n;
    cin >> n;
    int a[n];
    int dp[n];
    memset(dp,0, sizeof(dp));
    for(int i=0;i < n ;i++)
        cin >> a[i];
    int ans = 0;
    for(int i=1;i < n;i++)
    {
        for(int j=i-1;j >= 0;j--)
        {
            if(a[j] < a[i])
                dp[i] = max(dp[i],dp[j]+1);
        }
        ans = max(ans,dp[i]);
    }

    cout << ans+1 << endl;
    return 0;
}
```

复杂度n^2的做法
















