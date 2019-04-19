# hdu 3496  Watch The Movie（带限制的01背包） - HJ - CSDN博客
2017年08月02日 13:03:04[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：251
Problem Description 
New semester is coming, and DuoDuo has to go to school tomorrow. She decides to have fun tonight and will be very busy after tonight. She like watch cartoon very much. So she wants her uncle to buy some movies and watch with her tonight. Her grandfather gave them L minutes to watch the cartoon. After that they have to go to sleep. 
DuoDuo list N piece of movies from 1 to N. All of them are her favorite, and she wants her uncle buy for her. She give a value Vi (Vi > 0) of the N piece of movies. The higher value a movie gets shows that DuoDuo likes it more. Each movie has a time Ti to play over. If a movie DuoDuo choice to watch she won’t stop until it goes to end. 
But there is a strange problem, the shop just sell M piece of movies (not less or more then), It is difficult for her uncle to make the decision. How to select M piece of movies from N piece of DVDs that DuoDuo want to get the highest value and the time they cost not more then L. 
How clever you are! Please help DuoDuo’s uncle.
Input 
The first line of the input file contains a single integer t (1 ≤ t ≤ 10), the number of test cases, followed by input data for each test case: 
The first line is: N(N <= 100),M(M<=N),L(L <= 1000) 
N: the number of DVD that DuoDuo want buy. 
M: the number of DVD that the shop can sale. 
L: the longest time that her grandfather allowed to watch. 
The second line to N+1 line, each line contain two numbers. The first number is the time of the ith DVD, and the second number is the value of ith DVD that DuoDuo rated.
Output 
Contain one number. (It is less then 2^31.) 
The total value that DuoDuo can get tonight. 
If DuoDuo can’t watch all of the movies that her uncle had bought for her, please output 0.
Sample Input
1 
3 2 10 
11 100 
1 2 
9 1
Sample Output
3
题目大意： 
有n场电影可以看，每场电影有一定的时长，也有一定的价值。但是只能在规定时长t内，看最多不超过m场电影。问如何选择，才能使得价值最大？
解题思路： 
看成01背包，只需要中间考虑最多只能看m场电影的情况，开一个循环遍历。 
注意，价值有可能为负数，因此初始值应该赋为负无穷大。
```cpp
#include<iostream>
using namespace std;
#define INF 0x3f3f3f3f
int dp[110][1005];          //dp[i][j]表示从n件物品中拿出i件还剩下体积j的最大价值 
int main()
{
    int t,n,m,l,time,val;
    cin>>t;
    while(t--)
    {
        cin>>n>>m>>l;
        for(int i=0;i<=m;i++)
          for(int j=0;j<=l;j++)
            dp[i][j]=i==0?0:-INF;
        for(int i=1;i<=n;i++)
        {
            cin>>time>>val;
            for(int j=m;j>=1;j--)
              for(int k=l;k>=time;k--)
                dp[j][k]=max(dp[j][k],dp[j-1][k-time]+val);
        }
        int ans=dp[m][l]<0?0:dp[m][l];
        cout<<ans<<endl;
    }
    return 0;
}
```
