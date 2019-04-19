# hdu  1176  免费馅饼（dp） - HJ - CSDN博客
2017年02月01日 20:36:01[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：185
个人分类：[====ACM====																[---------OJ---------																[HDU																[--------动态规划---------																[基础DP](https://blog.csdn.net/feizaoSYUACM/article/category/7238027)](https://blog.csdn.net/feizaoSYUACM/article/category/7238093)](https://blog.csdn.net/feizaoSYUACM/article/category/6308041)](https://blog.csdn.net/feizaoSYUACM/article/category/6308067)](https://blog.csdn.net/feizaoSYUACM/article/category/6308070)
Problem Description 
都说天上不会掉馅饼，但有一天gameboy正走在回家的小径上，忽然天上掉下大把大把的馅饼。说来gameboy的人品实在是太好了，这馅饼别处都不掉，就掉落在他身旁的10米范围内。馅饼如果掉在了地上当然就不能吃了，所以gameboy马上卸下身上的背包去接。但由于小径两侧都不能站人，所以他只能在小径上接。由于gameboy平时老呆在房间里玩游戏，虽然在游戏中是个身手敏捷的高手，但在现实中运动神经特别迟钝，每秒种只有在移动不超过一米的范围内接住坠落的馅饼。现在给这条小径如图标上坐标： 
![这里写图片描述](http://acm.hdu.edu.cn/data/images/1176_1.jpg)
为了使问题简化，假设在接下来的一段时间里，馅饼都掉落在0-10这11个位置。开始时gameboy站在5这个位置，因此在第一秒，他只能接到4,5,6这三个位置中其中一个位置上的馅饼。问gameboy最多可能接到多少个馅饼？（假设他的背包可以容纳无穷多个馅饼）
Input 
输入数据有多组。每组数据的第一行为以正整数n(0< n<100000)，表示有n个馅饼掉在这条小径上。在结下来的n行中，每行有两个整数x,T(0< T<100000),表示在第T秒有一个馅饼掉在x点上。同一秒钟在同一点上可能掉下多个馅饼。n=0时输入结束。
Output 
每一组输入数据对应一行输出。输出一个整数m，表示gameboy最多可能接到m个馅饼。 
提示：本题的输入数据量比较大，建议用scanf读入，用cin可能会超时。
Sample Input
6 
5 1 
4 1 
6 1 
7 2 
7 2 
8 3 
0
Sample Output
4
Author 
lwg
解题思路：
d[i][j]表示第i秒有第j个位置之前得到的最大馅饼个数
则可以选择逆序遍历，（因为总是从第0秒第5个位置开始的），状态转移方程为：
```
d[i][j]=max(d[i+1][j+1],d[i-1][j+1],d[i][j+1])
```
本来应该都是有三种决策没错，但是要考虑到两个特殊点（第0个位置和第10个位置）
所以特殊处理这两个点：
```
d[0][j]=max(d[0][j+1],d[1][j+1]);
d[10][j]=max(d[10][j+1],d[9][j+1]);
```
逆序之后最后输出d[5][0]即可
代码如下：
```cpp
#include<iostream>
#include<cstring> 
using namespace std;
int Max(int a,int b,int c)    //三个数取最大值 
{
    if(a<b) a=b;
    if(a<c) a=c;
    return a;
}
int dp[11][100005];
int main()
{
    int n;
    while(cin>>n)
    {
        if(n==0)
          break;
        memset(dp,0,sizeof(dp));
        int time=0,a,b;
        while(n--) 
        {
            cin>>a>>b;
            dp[a][b]++;
            time=b>time?b:time;      //求出最后一个馅饼掉落的时间 
        }
        for(int j=time-1;j>=0;j--)             //逆序遍历 
        {
            dp[0][j]+=max(dp[0][j+1],dp[1][j+1]);     //如果在第1个节点，则只能留在原地或者往右 
            dp[10][j]+=max(dp[10][j+1],dp[9][j+1]);   //如果在第10个节点，则只能留在原地或者往左 
            for(int i=1;i<10;i++)
              dp[i][j]+=Max(dp[i-1][j+1],dp[i][j+1],dp[i+1][j+1]);     //否则既可以留在原地也可以任意左右移动 
        }
        cout<<dp[5][0]<<endl;    
    }
    return 0;
}
```
