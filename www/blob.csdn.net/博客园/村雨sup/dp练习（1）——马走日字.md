# dp练习（1）——马走日字 - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)





## 3328: 马走日字
时间限制: 1 Sec  内存限制: 128 MB
提交: 35  解决: 5
[[提交](http://202.4.155.100/JudgeOnline/submitpage.php?id=3328)][[状态](http://202.4.155.100/JudgeOnline/problemstatus.php?id=3328)][[讨论版](http://202.4.155.100/JudgeOnline/bbs.php?pid=3328)]
## 题目描述


一次外出旅游，你路上遇到了一个骑着马的强盗，你很害怕，你需要找一条路跑回家B(n,m).地图类似一个矩阵，你在左上角A(0,0)，你每次只可以走一步，可以选择向下走，也可以选择向右走。但是强盗所在的位置和强盗的马一次所能跳到的位置是不可以走过去的（强盗的马的移动方法与象棋中的马相同.请计算出从A点能够走到B点的所有路径条数。





![](http://202.4.155.100/JudgeOnline/upload/image/20180106/20180106163200_18195.jpg)


## 输入

首先输入一个整数t，代表有t组测试数据。
每组测试数据为四个整数，即B点的坐标（n，m）和强盗的坐标（x，y）。
1 < n,m < 20.

## 输出

输出一个整数即路径的条数。

## 样例输入
2
6 6 3 2
8 8 3 5
## 样例输出
17
1393
## 提示



## 来源




```
#include "bits/stdc++.h"
using namespace std;

long long dp[100][100];
int Map[100][100];

int main()
{
    int T;
    cin >> T;
    while(T--)
    {
        memset(dp,0, sizeof(dp));
        memset(Map,0, sizeof(Map));
        int n,m,x,y;
        cin >> n >> m >> x >> y;
        Map[x][y] = 1;
        Map[x-2][y-1] = 1;
        Map[x-1][y-2] = 1;

        Map[x+1][y+2] = 1;
        Map[x+2][y+1] = 1;

        Map[x-2][y+1] = 1;
        Map[x-1][y+2] = 1;

        Map[x+1][y-2] = 1;
        Map[x+2][y-1] = 1;

        for(int i=0;i <= m;i++)
        {
            if(Map[0][i] == 0)
                dp[0][i] = 1;
            else
                break;
        }
        for(int i=0;i <= n;i++)
        {
            if(Map[i][0] == 0)
                dp[i][0] = 1;
            else
                break;
        }

        for(int i=1;i <= n;i++)
        {
            for(int j=1;j <= m;j++)
            {
                if(Map[i][j] == 1)
                    dp[i][j] = 0;
                else
                    dp[i][j] = dp[i-1][j] + dp[i][j-1];
            }
        }

        cout << dp[n][m] <<endl;

    }
    return 0;
}
```

下次再写一遍












