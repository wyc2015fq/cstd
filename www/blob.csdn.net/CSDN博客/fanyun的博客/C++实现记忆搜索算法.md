# C++实现记忆搜索算法 - fanyun的博客 - CSDN博客
2018年08月26日 14:28:25[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：210
1、记忆搜索算法：
       给n＊n地图，老鼠初始位置在(0,0)，它每次行走要么横着走要么竖着走，每次最多可以走出k个单位长度，且落脚点的权值必须比上一个落脚点的权值大，求最终可以获得的最大权值 
2、思路
     每次最多m步，记录四个方向的下一次走法可以获得的权值总和，只能直走。m种情况选最大。
3、代码实现
```cpp
#include<iostream>
#include<cstring>
#include<cmath>
#include<cstdio>
#include<cstdlib>
#include<algorithm>
#include<stack>
#include<queue>
using namespace std;
const int N = 109;
int dp[N][N], a[N][N];
int px[] = {0, 0, -1, 1};
int py[] = {1, -1, 0, 0};
int dfs(int n, int m, int x, int y)
{
    if(!dp[x][y])
    {
        int ans = 0;
        for(int i=1; i<=m; i++)//最多m步
        {
            int temp = 0;//记录四个方向的下一次走法可以获得的权值总和
            for(int j=0; j<4; j++)//四个方向
            {
                int tx = x + px[j]*i;//因为只能直走
                int ty = y + py[j]*i;
                if(tx>=1 && tx<=n && ty>=1 && ty<=n && a[tx][ty]>a[x][y])
                    temp = max(temp, dfs(n, m, tx, ty));
            }
            ans = max(ans, temp);//m种情况选最大
        }
        dp[x][y] = ans + a[x][y];//记录
    }
    return dp[x][y];
}
int main()
{
    int n, m;
    while(~scanf("%d%d", &n, &m) && n!=-1 && m!=-1)
    {
        memset(dp, 0, sizeof(a));
        for(int i=1; i<=n; i++)
            for(int j=1; j<=n; j++)
                scanf("%d", &a[i][j]);
        printf("%d\n", dfs(n, m, 1, 1));
    }
    return 0;
}
```
