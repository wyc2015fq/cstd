# codeforces 118-D. Caesar's Legions（dp计数） - HJ - CSDN博客
2017年07月13日 10:14:11[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：271
所属专栏：[codeforces](https://blog.csdn.net/column/details/17151.html)
Gaius Julius Caesar, a famous general, loved to line up his soldiers. Overall the army had n1 footmen and n2 horsemen. Caesar thought that an arrangement is not beautiful if somewhere in the line there are strictly more that k1 footmen standing successively one after another, or there are strictly more than k2 horsemen standing successively one after another. Find the number of beautiful arrangements of the soldiers.
Note that all n1 + n2 warriors should be present at each arrangement. All footmen are considered indistinguishable among themselves. Similarly, all horsemen are considered indistinguishable among themselves. 
Input
The only line contains four space-separated integers n1, n2, k1, k2 (1 ≤ n1, n2 ≤ 100, 1 ≤ k1, k2 ≤ 10) which represent how many footmen and horsemen there are and the largest acceptable number of footmen and horsemen standing in succession, correspondingly. 
Output
Print the number of beautiful arrangements of the army modulo 100000000 (108). That is, print the number of such ways to line up the soldiers, that no more than k1 footmen stand successively, and no more than k2 horsemen stand successively. 
Examples 
Input
2 1 1 10
Output
1
Input
2 3 1 2
Output
5
Input
2 4 1 1
Output
0
Note
Let’s mark a footman as 1, and a horseman as 2.
In the first sample the only beautiful line-up is: 121
In the second sample 5 beautiful line-ups exist: 12122, 12212, 21212, 21221, 22121
题目大意： 
要求n个人和匹马排成一列，其中必须满足两个条件： 
1）连续站在一起的人不超过k1； 
2）连续站在一起的马不超过k2；
同时满足这两个条件的站队方法共有多少种？
解题思路： dp
```
四维dp： dp[i][j][p][q]   (0<i<n,0<=j<=m,0<=p<=1,0<q<=k1/k2)  
  其中   p=0表示男生  p=1表示女生   
         k1表示男生最多能连续站在一起的数目
         k2表示女生最多能连续站在一起的数目  
  总方程：
  ①当p=0时  表示前i个人有j个男生并且最后一个人是男生且最后连续站了q位男生的方案数
  ②当p=1时  表示前i个人有j个男生并且最后一个人是女生且最后连续站了q位女生的方案数
  初始化：当i=1时候
    dp[1][1][0][1]=1      表示前1个人有1个男生并且最后一个人是男生且最后连续站了1位男生的方案数 
    dp[1][0][1][1]=1      表示前1个人有0个男生并且最后一个人是女生且最后连续站了1位女生的方案数 
  状态转移： 
  ①当p=0时  最后一个人站的是男生 
    a) 当q=1时  最后连续站了1位男生   
      dp[i][j][0][1] = ∑dp[i-1][j-1][1][s] (0<s<=k2)      //枚举前面连续站了s位女生的方案数并累加所有和 
    b) 当q>1时  最后连续站了q位男生 
      dp[i][j][0][q] = dp[i-1][j-1][0][q-1]        //递推前面连续站了q-1位男生的方案数 
  ②当p=1时
    a) 当q=1时  最后连续站了1位女生 
      dp[i][j][1][1] = ∑dp[i-1][j][0][s]  (0<s<=k1)      //枚举前面连续站了s位男生的方案数并累加所有和 
    b) 当q>1时  最后连续站了q为女生
      dp[i][j][1][q] = dp[i-1][j][1][q-1]          //递推前面连续站了q-1位女生的方案数 
  最后的总方案数 ans = ∑dp[i][j][0][s1] + ∑dp[i][j][1][s2] (0<s1<=k1 , 0<s2<=k2)
```
```cpp
#include<iostream>
#include<cstring>
using namespace std;
#define mod 100000000
int dp[205][205][5][15];     
int main()
{
    int n,m,k1,k2;
    while(cin>>n>>m>>k1>>k2)
    {
        memset(dp,0,sizeof(dp));
        dp[1][1][0][1]=1;
        dp[1][0][1][1]=1;
        for(int i=2;i<=n+m;i++)
          for(int j=0;j<=n;j++)
            for(int p=0;p<=1;p++)
            {
                if((p&&j==n+1)||(!p&&i-j==m+1))
                  continue;
                for(int q=1;q<=(p?k2:k1);q++)
                {
                    if(q==1)
                      for(int s=1;s<=(p?k1:k2);s++)
                        dp[i][j][p][1]=(dp[i][j][p][1]+dp[i-1][(p?j:j-1)][!p][s])%mod;
                    else
                      dp[i][j][p][q]=dp[i-1][(p?j:j-1)][p][q-1];
                }
            }
        long long ans=0;
        for(int p=0;p<=1;p++)
          for(int q=1;q<=(p?k2:k1);q++)
            ans=(ans+dp[n+m][n][p][q])%mod;
        cout<<ans<<endl;
    }
    return 0;
}
```
