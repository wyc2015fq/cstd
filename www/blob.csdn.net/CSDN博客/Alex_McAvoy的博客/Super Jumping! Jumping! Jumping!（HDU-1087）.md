# Super Jumping! Jumping! Jumping!（HDU-1087） - Alex_McAvoy的博客 - CSDN博客





2018年03月26日 20:40:59[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：60
个人分类：[HDU																[动态规划——线性 DP](https://blog.csdn.net/u011815404/article/category/7813170)](https://blog.csdn.net/u011815404/article/category/7923257)








# Problem Description

    Nowadays, a kind of chess game called “Super Jumping! Jumping! Jumping!” is very popular in HDU. Maybe you are a good boy, and know little about this game, so I introduce it to you now.

![](https://img-blog.csdn.net/20180326203814545)

    The game can be played by two or more than two players. It consists of a chessboard（棋盘）and some chessmen（棋子）, and all chessmen are marked by a positive integer or “start” or “end”. The player starts from start-point and must jumps into end-point finally. In the course of jumping, the player will visit the chessmen in the path, but everyone must jumps from one chessman to another absolutely bigger (you can assume start-point is a minimum and end-point is a maximum.). And all players cannot go backwards. One jumping can go from a chessman to next, also can go across many chessmen, and even you can straightly get to end-point from start-point. Of course you get zero point in this situation. A player is a winner if and only if he can get a bigger score according to his jumping solution. Note that your score comes from the sum of value on the chessmen in you jumping path.

    Your task is to output the maximum value according to the given chessmen list.

# Input

    Input contains multiple test cases. Each test case is described in a line as follow:  N value_1 value_2 …value_N 

    It is guarantied that N is not more than 1000 and all value_i are in the range of 32-int.

    A test case starting with 0 terminates the input and this test case is not to be processed.

# Output

    For each case, print the maximum according to rules, and one line one case.

# Sample Input

**3 1 3 24 1 2 3 44 3 3 2 10**

# Sample Output

**4103**

————————————————————————————————————————————

思路：

求最长上升子序列的和，可以先看前三项：

    第一项：dp[1] = num[1]

    第二项：若num[2]>num[1]，则dp[2] = dp[1] + num[2]，否则dp[2] = num[2]

    第三项：若num[3]>num[2]，则dp[3] = dp[2] + num[3]；

           若num[3]>num[1]，则dp[3] = dp[1] + num[3]；

           若num[3]≯num[2]且num[3]≯num[1]，则dp[3] = num[3]

综上：用之前的每一个数和当前的数比较，比当前数小的就加上dp（之前数），否则就等于当前数。

# Source Program

```cpp
#include<iostream>
#include<cstring>
using namespace std;
int main()
{
    int n;
    int num[1001],dp[1001];
    int max_num;
    int i,j;

    while(cin>>n && n)
    {
        /*清零工作*/
        max_num=-1;
        memset(dp,0,sizeof(dp));

        for(i=1;i<=n;i++)
            cin>>num[i];

        for(i=1;i<=n;i++)
        {
            for(j=0;j<=i;j++)//之前数比当前数小，就将当前数与之前数相加
                if(num[i]>num[j])
                    dp[i]=max(dp[i],dp[j]+num[i]);

            dp[i]=max(dp[i],num[i]);//若之前数不比当前数小，直接就是当前数

            max_num=max(max_num,dp[i]);//寻找最大和
        }
        cout<<max_num<<endl;
    }
    return 0;
}
```






