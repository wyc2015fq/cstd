# hdu  3535  AreYouBusy（多条件分类01背包） - HJ - CSDN博客
2017年08月02日 16:20:29[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：224
Problem Description 
Happy New Term! 
As having become a junior, xiaoA recognizes that there is not much time for her to AC problems, because there are some other things for her to do, which makes her nearly mad. 
What’s more, her boss tells her that for some sets of duties, she must choose at least one job to do, but for some sets of things, she can only choose at most one to do, which is meaningless to the boss. And for others, she can do of her will. We just define the things that she can choose as “jobs”. A job takes time , and gives xiaoA some points of happiness (which means that she is always willing to do the jobs).So can you choose the best sets of them to give her the maximum points of happiness and also to be a good junior(which means that she should follow the boss’s advice)?
Input 
There are several test cases, each test case begins with two integers n and T (0<=n,T<=100) , n sets of jobs for you to choose and T minutes for her to do them. Follows are n sets of description, each of which starts with two integers m and s (0< m<=100), there are m jobs in this set , and the set type is s, (0 stands for the sets that should choose at least 1 job to do, 1 for the sets that should choose at most 1 , and 2 for the one you can choose freely).then m pairs of integers ci,gi follows (0<=ci,gi<=100), means the ith job cost ci minutes to finish and gi points of happiness can be gained by finishing it. One job can be done only once.
Output 
One line for each test case contains the maximum points of happiness we can choose from all jobs .if she can’t finish what her boss want, just output -1 .
Sample Input
3 3 
2 1 
2 5 
3 8 
2 0 
1 0 
2 1 
3 2 
4 3 
2 1 
1 1
3 4 
2 1 
2 5 
3 8 
2 0 
1 1 
2 8 
3 2 
4 4 
2 1 
1 1
1 1 
1 0 
2 1
5 3 
2 0 
1 0 
2 1 
2 0 
2 2 
1 1 
2 0 
3 2 
2 1 
2 1 
1 5 
2 8 
3 2 
3 8 
4 9 
5 10
Sample Output
5 
13 
-1 
-1
题目大意： 
    给你n个工作集合，你有T的时间去做它们。其中每个工作集合有m件事可以做，它们是s类的工作集合（s=0,1,2） 
    s=0说明这m件事中最少得做一件; 
    s=1说明这m件事中最多只能做一件; 
    s=2说明这m件事你可以做也可以不做; 
    并且每件事情需要用ci的时间，但能获得gi的快乐值。求在T的时间内你能获得的最大快乐值。
解题思路： 
设dp[i][j] 表示处理完前面i个工作集所花时间不超过j的最大快乐值  
则分类讨论：
①当s==0 的时候，表示这个集合内的m件事情至少做一件，也就分两种情况比较：
```
1）这个集合中m件事情已经选择其中k件做了（k>=1）;
  dp[i][j]=max(dp[i][j],dp[i][j-cost]+val);
2）这个集合中m件事情还没有做过一件；
  dp[i][j]=max(dp[i][j],dp[i-1][j-cost]+val);
```
②当s==1的时候，表示这个集合内的m件事情最多做一件，如果现在还有挑选的余地，就已经说明这个集合中的m件事情还没有做过一件;
```
dp[i][j]=max(d[i][j],dp[i-1][j-cost]+val);
```
③当s==2的时候，表示这个集合内的m件事情可以不做任何一件，也可以做任何多件；
```
dp[i][j]=max(dp[i][j],dp[i][j-cost]+val);
```
另外还有一个疑问需要解答： 
在情况①中的1),2)的先后顺序不可以改变，一定要先更新这个集合中m件事情已经选择其中k件做过了(k>=1)dp[i][j]，后更新这个集合中m件事情还没有做过一件的dp[i][j]
为什么？
因为如果情况①先更新2)，后更新1)的话，2)的值会被覆盖，也就得不到特殊情况的结果（也就是若剩下的时间已经不够了 那么就需要输出-1的情况）
```cpp
#include<iostream>
#include<cstring>
using namespace std;
#define INF 0x3f3f3f3f
int dp[105][105];               //dp[i][j] 表示处理完前面i个工作集所花时间不超过j的最大快乐值 
int main()
{
    int n,t,cost,val,m,s;
    while(cin>>n>>t)
    {
        memset(dp,0,sizeof(dp));
        for(int i=1;i<=n;i++)
        {
            cin>>m>>s;
            for(int j=0;j<=t;j++)
              dp[i][j]=s==0?-INF:dp[i-1][j]; //如果s==0 则至少需要选择一个的01背包 若剩下的时间已经不够了 那么就需要输出-1 因此初始化-INF 
            for(int k=0;k<m;k++)
            {
                cin>>cost>>val;
                for(int j=t;j>=cost;j--)         //都选择逆序枚举 
                {
                    if(s==0)                 //至少选择一个的01背包 
                      dp[i][j]=max(max(dp[i][j],dp[i][j-cost]+val),dp[i-1][j-cost]+val);
                    else if(s==1)            //最多选择一个的01背包 
                      dp[i][j]=max(dp[i][j],dp[i-1][j-cost]+val);
                    else                      //选择任意多个的01背包 
                      dp[i][j]=max(dp[i][j],dp[i][j-cost]+val);
                }
            }
        }
        int ans=max(dp[n][t],-1);
        cout<<ans<<endl;
    }
    return 0;
}
```
