# dp练习（3）——棋盘问题 - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)







设有一个n*m的棋盘（2≤n≤50，2≤m≤50），如下图，在棋盘上有一个中国象棋马。

规定：

1)马只能走日字

2)马只能向右跳

问给定起点x1,y1和终点x2,y2，求出马从x1,y1出发到x2,y2的合法路径条数。
![](http://codevs.cn/media/image/problem/1219.jpg)



输入描述 Input Description


第一行2个整数n和m

第二行4个整数x1,y1,x2,y2




输出描述 Output Description


输出方案数




样例输入 Sample Input


30 30

1 15 3 15




样例输出 Sample Output


2




数据范围及提示 Data Size & Hint


2<=n,m<=50



```
#include "bits/stdc++.h"
using namespace std;

long long dp[100][100];
//int Map[100][100];

int main()
{
    int n,m;
    scanf("%d%d",&n,&m);
    int x1,y1,x2,y2;
    scanf("%d%d%d%d",&x1,&y1,&x2,&y2);
    memset(dp,0, sizeof(dp));
//    memset(Map,0,sizeof(Map));

    dp[y1][x1] = 1;

    if(x2 <= x1) cout << 0 << endl;

    for(int i=x1;i < x2;i++)
    {
        for(int j=1;j <= n;j++)
        {
            if(dp[j][i] != 0)
            {
                if(j-2 >= 1 && i+1 <= x2) dp[j-2][i+1] += dp[j][i];
                if(j-1 >= 1 && i+2 <= x2) dp[j-1][i+2] += dp[j][i];
                if(j+1 <= n && i+2 <= x2) dp[j+1][i+2] += dp[j][i];
                if(j+2 <= n && i+1 <= x2) dp[j+2][i+1] += dp[j][i];
            }
        }
    }
//
//    for(int i=1;i <= n;i++)
//    {
//        for(int j=1;j <= m;j++)
//        {
//            cout << dp[i][j] << " ";
//        }
//        cout << endl;
//    }

    cout << dp[y2][x2] << endl;

    return 0;
}
```

嘻嘻，智商低就要多刷题













