# POJ1185 炮兵阵地 状压DP - 紫芝的博客 - CSDN博客





2018年07月29日 09:57:24[紫芝](https://me.csdn.net/qq_40507857)阅读数：43








# 炮兵阵地
|**Time Limit:** 2000MS||**Memory Limit:** 65536K|
|----|----|----|
|**Total Submissions:** 31819||**Accepted:** 12295|

Description

司令部的将军们打算在N*M的网格地图上部署他们的炮兵部队。一个N*M的地图由N行M列组成，地图的每一格可能是山地（用"H" 表示），也可能是平原（用"P"表示），如下图。在每一格平原地形上最多可以布置一支炮兵部队（山地上不能够部署炮兵部队）；一支炮兵部队在地图上的攻击范围如图中黑色区域所示： 

![](http://poj.org/images/1185_1.jpg)



如果在地图中的灰色所标识的平原上部署一支炮兵部队，则图中的黑色的网格表示它能够攻击到的区域：沿横向左右各两格，沿纵向上下各两格。图上其它白色网格均攻击不到。从图上可见炮兵的攻击范围不受地形的影响。 

现在，将军们规划如何部署炮兵部队，在防止误伤的前提下（保证任何两支炮兵部队之间不能互相攻击，即任何一支炮兵部队都不在其他支炮兵部队的攻击范围内），在整个地图区域内最多能够摆放多少我军的炮兵部队。 

Input

第一行包含两个由空格分割开的正整数，分别表示N和M； 

接下来的N行，每一行含有连续的M个字符('P'或者'H')，中间没有空格。按顺序表示地图中每一行的数据。N <= 100；M <= 10。

Output

仅一行，包含一个整数K，表示最多能摆放的炮兵部队的数量。

Sample Input

5 4
PHPP
PPHH
PPPP
PHPP
PHHP
Sample Output

6
Source

[Noi 01](http://poj.org/searchproblem?field=source&key=Noi+01)

## 分析：

动态规划本来就很抽象，状态的设定和状态的转移都不好把握，而状态压缩的动态规划解决的就是那种状态很多，不容易用一般的方法表示的动态规划问题，这个就更加的难于把握了。难点在于以下几个方面：状态怎么压缩？压缩后怎么表示？怎么转移？是否具有最优子结构？是否满足后效性？涉及到一些位运算的操作，虽然比较抽象，但本质还是动态规划。找准动态规划几个方面的问题，深刻理解动态规划的原理，开动脑筋思考问题。这才是掌握动态规划的关键。

状态压缩动态规划最关键的要处理的问题就是位运算的操作，容易出错，状态的设计也直接决定了程序的效率，或者代码长短。状态转移方程一定要仔细推敲，不可一带而过，要思考为什么这么做，掌握一个套路，遇见这类问题能快速的识别出问题的本质，找出状态转移方程和DP的边界条件。

【题目大意】一个方格组成的矩阵，每个方格可以放大炮用0表示，不可以放大炮用1表示（原题用字母），

让放最多的大炮，大炮与大炮间不会互相攻击。

【解析】可以发现，对于每一行放大炮的状态，只与它上面一行和上上一行的状态有关，每一行用状态压缩的表示方法，0表示不放大炮，1表示放大炮，同样的，先要满足硬件条件，即有的地方不能放大炮，然后就是每一行中不能有两个1的距离小于2（保证横着不互相攻击），这些要预先处理一下。然后就是状态表示和转移的问题了，因为是和前两行的状态有关，所以要开个三维的数组来表示状态，当前行的状态可由前两行的状态转移而来。即如果当前行的状态符合前两行的约束条件（不和前两行的大炮互相攻击），则当前行的最大值就是上一个状态的值加上当前状态中1的个数（当前行放大炮的个数） 

【状态表示】dp[i][j][k] 表示第i行状态为k，第i-1状态为j时的最大炮兵个数。 

【状态转移方程】**dp[i][k][t] =max(dp[i][k][t],dp[i-1][j][k]+num[t]); **num[t]为t状态中1的个数 

【DP边界条件】dp[1][1][i] =num[i] 状态i能够满足第一行的硬件条件（注意：这里的i指的是第i个状态，不是一个二进制数，开一个数组保存二进制状态） 

【代码】

```cpp
//#include<bits/stdc++.h>
#include<cstdio>
#include<cstring>
#include<algorithm>
using namespace std;
int N,M;
char map[110][20],num[110],top;
int stk[70],cur[110];
int dp[110][70][70];

//判断该状态是否合法，即不存在相邻的1之间的距离小于3
inline  bool ok(int x)
{
    if(x&(x<<1))    return 0;
    if(x&(x<<2))    return 0;
    return 1;
}
//找到所有可能的合法状态，最多60种
inline void jinit()
{
    top=0;
    int i,total=1<<N;
    for(i=0;i<total;i++)
        if(ok(i))   stk[++top]=i;
}

//判断状态x是否与第k行匹配
inline bool fit(int x,int k)
{
    if(cur[k]&x)    return 0;
    return 1;
}

//统计一个整型数x的二进制中1的个数，用于初始化
inline int jcount(int x)
{
    int cnt=0;
    while(x){
        cnt++;
        x&=(x-1);
    }
    return cnt;
}

int main()
{
    while(scanf("%d%d",&M,&N)!=EOF){
        if(N==0&&M==0)  break;
        jinit();
        for(int i=1;i<=M;++i)
            scanf("%s",map[i]+1);
        for(int i=1;i<=M;i++)
        for(int j=1;j<=N;j++){
            cur[i]=0;
            for(j=1;j<=N;j++)
            {
                if(map[i][j]=='H')
                cur[i]+=(1<<(j-1));
            }
        }
        memset(dp,-1,sizeof(dp));
        //初始化第一行状态
        for(int i=1;i<=top;i++)
        {
            num[i]=jcount(stk[i]);
            if(fit(stk[i],1))
                dp[1][1][i]=num[i];
        }
        int i,t,j,k;
        for(i=2;i<=M;i++)
        {
            for(t=1;t<=top;t++)
            {
                if(!fit(stk[t],i))  continue;
                for(j=1;j<=top;j++)
                {
                    if(stk[t]&stk[j])   continue;
                    for(k=1;k<=top;k++)
                    {
                    if(stk[t]&stk[k])   continue;
                    if(dp[i-1][j][k]==-1)   continue;
            dp[i][k][t]=max(dp[i][k][t],dp[i-1][j][k]+num[t]);
                    }
                }
            }
        }
        int ans=0;
        for(i=1;i<=M;++i)
            for(j=1;j<=top;++j)
            for(k=1;k<=top;++k)
            ans=max(ans,dp[i][j][k]);
        printf("%d\n",ans);
    }
    return 0;
}
```





