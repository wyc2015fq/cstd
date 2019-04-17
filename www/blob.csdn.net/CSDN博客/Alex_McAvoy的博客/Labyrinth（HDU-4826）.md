# Labyrinth（HDU-4826） - Alex_McAvoy的博客 - CSDN博客





2019年01月14日 21:04:13[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：57
个人分类：[HDU																[动态规划——线性 DP](https://blog.csdn.net/u011815404/article/category/7813170)](https://blog.csdn.net/u011815404/article/category/7923257)








# Problem Description

度度熊是一只喜欢探险的熊，一次偶然落进了一个m*n矩阵的迷宫，该迷宫只能从矩阵左上角第一个方格开始走，只有走到右上角的第一个格子才算走出迷宫，每一次只能走一格，且只能向上向下向右走以前没有走过的格子，每一个格子中都有一些金币（或正或负，有可能遇到强盗拦路抢劫， 度度熊身上金币可以为负，需要给强盗写欠条），度度熊刚开始时身上金币数为0，问度度熊走出迷宫时候身上最多有多少金币？ 

# **Input**

输入的第一行是一个整数T（T < 200），表示共有T组数据。 

每组数据的第一行输入两个正整数m，n（m<=100，n<=100）。接下来的m行，每行n个整数，分别代表相应格子中能得到金币的数量，每个整数都大于等于-100且小于等于100。

# Output

对于每组数据，首先需要输出单独一行”Case #?:”，其中问号处应填入当前的数据组数，组数从1开始计算。 

每组测试数据输出一行，输出一个整数，代表根据最优的打法，你走到右上角时可以获得的最大金币数目。

# Sample Input

**23 41 -1 1 02 -2 4 23 5 1 -902 21 11 1**

# Sample Output

**Case #1:18Case #2:4**

————————————————————————————————————————————————

思路：一开始写的 dfs，然后超时，于是想到了用 dp，设 dp[i][j][0~3] 前两维表示坐标，第三维表示从三个方向进入 (i,j) 的最优值。对于第一列的 (i,j) 只能从其上方或下方得来，对于其他行列，先枚举每一列，在每一列中的每一行的 (i,j) 其能从左、上、下三个方向得来，最后比较 (1,m) 的从上方、下方、左方得来的最大值即可

# Source Program

```cpp
#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<string>
#include<cstring>
#include<cmath>
#include<ctime>
#include<algorithm>
#include<utility>
#include<stack>
#include<queue>
#include<vector>
#include<set>
#include<map>
#define PI acos(-1.0)
#define E 1e-6
#define MOD 1000000007
#define INF 0x3f3f3f3f
#define N 1001
#define LL long long
using namespace std;
int dp[N][N][3];//第三维的0、1、2分别代表从上、下、左三个方向进入
int G[N][N];
int max(int a,int b,int c){
    int maxx=max(a,b);
    maxx=max(maxx,c);
    return maxx;
}
int main()
{
    int t;
    int Case=1;
    scanf("%d",&t);
    while(t--){
        int n,m;
        scanf("%d%d",&n,&m);
        for(int i=1;i<=n;i++)
            for(int j=1;j<=m;j++)
                scanf("%d",&G[i][j]);

        memset(dp,-INF,sizeof(dp));
        //初始值等于左上角的值
        dp[1][1][0]=G[1][1];
        dp[1][1][1]=G[1][1];
        dp[1][1][2]=G[1][1];

        for(int i=2;i<=n;i++)//第一列的最优值，只能从上向下走
            dp[i][1][0]=dp[i-1][1][0]+G[i][1];

        for(int j=2;j<=m;j++){//枚举除第一列外的每一列，其可从左边进入、上边进入、下边进入
            for(int i=1;i<=n;i++)//对于(i,j)左边的一列，从其上方或下方或左方进入的最大值
                dp[i][j][2]=max(dp[i][j-1][0],dp[i][j-1][1],dp[i][j-1][2])+G[i][j];

            for(int i=2;i<=n;i++)//对于(i,j)上边的一行，从其上方或左方进入的最大值
                dp[i][j][0]=max(dp[i-1][j][0],dp[i-1][j][2])+G[i][j];

            for(int i=n-1;i>=1;i--)//对于(i,j)下边的一行，从其下方或左方进入的最大值
                dp[i][j][1]=max(dp[i+1][j][1],dp[i+1][j][2])+G[i][j];

        }

        int maxx=max(dp[1][m][0],dp[1][m][1],dp[1][m][2]);//对于(1,m)从其上方、下方、左方进入的最大值
        printf("Case #%d:\n%d\n", Case++,maxx);
    }
    return 0;
}
```






