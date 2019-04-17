# dp练习（2）——老鼠的旅行 - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)





**1267 老鼠的旅行（来源：codevs）**

```
#include "bits/stdc++.h"
using namespace std;

long long dp[100][100];
int Map[100][100];

int main()
{
    memset(dp,0,sizeof(dp));
    memset(Map,0, sizeof(Map));

    int n, m;
    cin >> n >> m;
    int k;
    cin >> k;
    for(int i=1 ;i <= k;i++)
    {
        int x,y;
        cin >> x >> y;
        Map[x][y] = 1;
    }

    for(int i=1;i <= n;i++)
    {
        if(Map[i][1] == 0) dp[i][1] = 1;
        else break;
    }
    for(int i=1;i <= m;i++)
    {
        if(Map[1][i] == 0) dp[1][i] = 1;
        else break;
    }
    for(int i=2;i <= n;i++)
    {
        for(int j=2;j <= m;j++)
        {
            if(Map[i][j] == 1) dp[i][j] = 0;
            else dp[i][j] = dp[i-1][j] + dp[i][j-1];
        }
    }
    cout << dp[n][m] <<endl;

    return 0;
}
```

相同练习











