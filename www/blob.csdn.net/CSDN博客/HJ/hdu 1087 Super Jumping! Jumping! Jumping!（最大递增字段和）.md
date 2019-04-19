# hdu  1087  Super Jumping! Jumping! Jumping!（最大递增字段和） - HJ - CSDN博客
2017年02月02日 18:38:08[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：207
个人分类：[====ACM====																[---------OJ---------																[HDU																[--------动态规划---------																[基础DP](https://blog.csdn.net/feizaoSYUACM/article/category/7238027)](https://blog.csdn.net/feizaoSYUACM/article/category/7238093)](https://blog.csdn.net/feizaoSYUACM/article/category/6308041)](https://blog.csdn.net/feizaoSYUACM/article/category/6308067)](https://blog.csdn.net/feizaoSYUACM/article/category/6308070)
Problem Description 
Nowadays, a kind of chess game called “Super Jumping! Jumping! Jumping!” is very popular in HDU. Maybe you are a good boy, and know little about this game, so I introduce it to you now.
![这里写图片描述](https://odzkskevi.qnssl.com/b69a36fbfe92ef60e13d861294653386?v=1485429229)
The game can be played by two or more than two players. It consists of a chessboard（棋盘）and some chessmen（棋子）, and all chessmen are marked by a positive integer or “start” or “end”. The player starts from start-point and must jumps into end-point finally. In the course of jumping, the player will visit the chessmen in the path, but everyone must jumps from one chessman to another absolutely bigger (you can assume start-point is a minimum and end-point is a maximum.). And all players cannot go backwards. One jumping can go from a chessman to next, also can go across many chessmen, and even you can straightly get to end-point from start-point. Of course you get zero point in this situation. A player is a winner if and only if he can get a bigger score according to his jumping solution. Note that your score comes from the sum of value on the chessmen in you jumping path. 
Your task is to output the maximum value according to the given chessmen list.
Input 
Input contains multiple test cases. Each test case is described in a line as follow: 
N value_1 value_2 …value_N 
It is guarantied that N is not more than 1000 and all value_i are in the range of 32-int. 
A test case starting with 0 terminates the input and this test case is not to be processed.
Output 
For each case, print the maximum according to rules, and one line one case.
Sample Input
3 1 3 2 
4 1 2 3 4 
4 3 3 2 1 
0
Sample Output
4 
10 
3
题目大意：
简单来说，就是求最大递增子段和（不一定要连续）
解题思路：
dp可怼
```cpp
#include<bits/stdc++.h>
using namespace std;
#define INF 0x3f3f3f3f
int d[10005],s[10005];
int main()
{
    int n,ans;
    while(cin>>n&&n)
    {
        for(int i=1;i<=n;i++)
          cin>>s[i];
        memset(d,0,sizeof(d));
        for(int i=1;i<=n;i++)
        {
            ans=-INF;
            for(int j=0;j<i;j++)
              if(s[i]>s[j])
                ans=max(ans,d[j]);   //第i个数字之前的最大递增字段和 
            d[i]=ans+s[i];
        }
        ans=-INF;
        for(int i=0;i<=n;i++)
          ans=max(ans,d[i]);
        cout<<ans<<endl;
    }
    return 0;
}
```
