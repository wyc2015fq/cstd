# dp练习（4）——过河卒 - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)







### **1010 过河卒**

2002年NOIP全国联赛普及组





 时间限制: 1 s



 空间限制: 128000 KB



 题目等级 : 黄金 Gold




[题解](http://codevs.cn/wiki/solution/?problem_id=1010)












题目描述 Description


　如图，A 点有一个过河卒，需要走到目标 B 点。卒行走规则：可以向下、或者向右。同时在棋盘上的任一点有一个对方的马（如上图的C点），该马所在的点和所有跳跃一步可达的点称为对方马的控制点。例如上图 C 点上的马可以控制 9 个点（图中的P1，P2 … P8 和 C）。卒不能通过对方马的控制点。


　　棋盘用坐标表示，A 点（0，0）、B 点（n,m）(n,m 为不超过 20 的整数，并由键盘输入)，同样马的位置坐标是需要给出的（约定: C不等于A，同时C不等于B）。现在要求你计算出卒从 A 点能够到达 B 点的路径的条数。

1<=n,m<=15
![](http://codevs.cn/media/image/problem/1010.png)



输入描述 Input Description


　键盘输入
　　　B点的坐标（n,m）以及对方马的坐标（X,Y）{不用判错}




输出描述 Output Description


　　屏幕输出
　　　　一个整数（路径的条数）。




样例输入 Sample Input


　6 6 3 2




样例输出 Sample Output


17




数据范围及提示 Data Size & Hint


如描述









```
#include "bits/stdc++.h"
using namespace std;

long long dp[100][100];
int Map[100][100];

int main()
{
    int n,m;
    scanf("%d%d",&n,&m);
    int x,y;
    scanf("%d%d",&x,&y);
    memset(dp,0, sizeof(dp));
    memset(Map,0,sizeof(Map));

    Map[x][y] = 1;
    Map[x-2][y-1] = 1;
    Map[x-1][y-2] = 1;
    Map[x+1][y-2] = 1;
    Map[x+2][y-1] = 1;
    Map[x+2][y+1] = 1;
    Map[x+1][y+2] = 1;
    Map[x-1][y+2] = 1;
    Map[x-2][y+1] = 1;

    for(int i=0;i <= n;i++)
    {
        if(Map[i][0] == 0) dp[i][0] = 1;
        else break;
    }
    for(int i=0;i <= m;i++)
    {
        if(Map[0][i] == 0) dp[0][i] = 1;
        else break;
    }
    for(int i=1;i <= n;i++)
    {
        for(int j=1;j <= m;j++)
        {
            if(Map[i][j] == 1) dp[i][j] = 0;
            else dp[i][j] = dp[i-1][j] + dp[i][j-1];
        }
    }

    cout << dp[n][m] <<endl;




    return 0;
}
```

emmm和练习1的一样，这次很快就写出来了。












