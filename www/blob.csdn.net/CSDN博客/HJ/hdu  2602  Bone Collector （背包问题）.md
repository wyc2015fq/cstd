# hdu   2602   Bone Collector  （背包问题） - HJ - CSDN博客
2016年08月11日 13:43:42[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：204
个人分类：[====ACM====																[---------OJ---------																[HDU																[--------动态规划---------																[背包问题](https://blog.csdn.net/feizaoSYUACM/article/category/7238225)](https://blog.csdn.net/feizaoSYUACM/article/category/7238093)](https://blog.csdn.net/feizaoSYUACM/article/category/6308041)](https://blog.csdn.net/feizaoSYUACM/article/category/6308067)](https://blog.csdn.net/feizaoSYUACM/article/category/6308070)
Problem Description 
Many years ago , in Teddy’s hometown there was a man who was called “Bone Collector”. This man like to collect varies of bones , such as dog’s , cow’s , also he went to the grave … 
The bone collector had a big bag with a volume of V ,and along his trip of collecting there are a lot of bones , obviously , different bone has different value and different volume, now given the each bone’s value along his trip , can you calculate out the maximum of the total value the bone collector can get ?
Input 
The first line contain a integer T , the number of cases. 
Followed by T cases , each case three lines , the first line contain two integer N , V, (N <= 1000 , V <= 1000 )representing the number of bones and the volume of his bag. And the second line contain N integers representing the value of each bone. The third line contain N integers representing the volume of each bone.
Output 
One integer per line representing the maximum of the total value (this number will be less than 231).
Sample Input 
1 
5 10 
1 2 3 4 5 
5 4 3 2 1
Sample Output 
14
背包入门题！
看代码的时候，一定要理解状态跟状态转移方程。
```cpp
#include<iostream>
using namespace std;
int v[1005],w[1005];
int dp[1005][1005];
int max(int a,int b)
{
    return a>b?a:b;
}
int main()
{
    int t,m,n;
    cin>>t;
    while(t--)
    {
        cin>>n>>m;
        memset(dp,0,sizeof(dp));
        for(int i=0;i<n;i++)
          cin>>v[i];
        for(int i=0;i<n;i++)
          cin>>w[i];
        for(int i=n-1;i>=0;i--)
          for(int j=0;j<=m;j++)
          {
            dp[i][j]=(i==n-1?0:dp[i+1][j]);    
            if(j>=w[i])          //决策条件，剩余背包容量不小于当前可选择物品体积
              dp[i][j]=max(dp[i][j],dp[i+1][j-w[i]]+v[i]);   //每次两种决策：选择当前物品换来价值牺牲体积；或者不选择当前物品，毫无损失。
          }
          cout<<dp[0][m]<<endl;    //输出最优解
    }
    return 0;
}
```
01背包还可以转换成一维的，但是一定要注意逆序！！！
```cpp
#include<iostream>
#include<algorithm> 
using namespace std;
int v[1005],w[1005];
int dp[1005];
int main()
{
    int t,m,n;
    cin>>t;
    while(t--)
    {
        cin>>n>>m;
        memset(dp,0,sizeof(dp));
        for(int i=0;i<n;i++)
          cin>>v[i];
        for(int i=0;i<n;i++)
          cin>>w[i];
        for(int i=0;i<n;i++)
          for(int j=m;j>=w[i];j--)         //注意01背包用一维数组开的时候第二重循环一定要逆序！！！ 
              dp[j]=max(dp[j],dp[j-w[i]]+v[i]);       //每次两种决策：选择当前物品换来价值牺牲体积；或者不选择当前物品，毫无损失。
          cout<<dp[m]<<endl;                  //输出最优解
    }
    return 0;
}
```
