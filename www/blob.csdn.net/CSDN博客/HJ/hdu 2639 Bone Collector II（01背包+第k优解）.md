# hdu 2639 Bone Collector II（01背包+第k优解） - HJ - CSDN博客
2017年08月02日 14:42:54[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：197
Problem Description 
The title of this problem is familiar,isn’t it?yeah,if you had took part in the “Rookie Cup” competition,you must have seem this title.If you haven’t seen it before,it doesn’t matter,I will give you a link:
Here is the link:[http://acm.hdu.edu.cn/showproblem.php?pid=2602](http://acm.hdu.edu.cn/showproblem.php?pid=2602)
Today we are not desiring the maximum value of bones,but the K-th maximum value of the bones.NOTICE that,we considerate two ways that get the same value of bones are the same.That means,it will be a strictly decreasing sequence from the 1st maximum , 2nd maximum .. to the K-th maximum.
If the total number of different values is less than K,just ouput 0.
Input 
The first line contain a integer T , the number of cases. 
Followed by T cases , each case three lines , the first line contain two integer N , V, K(N <= 100 , V <= 1000 , K <= 30)representing the number of bones and the volume of his bag and the K we need. And the second line contain N integers representing the value of each bone. The third line contain N integers representing the volume of each bone.
Output 
One integer per line representing the K-th maximum of the total value (this number will be less than 231).
Sample Input
3 
5 10 2 
1 2 3 4 5 
5 4 3 2 1 
5 10 12 
1 2 3 4 5 
5 4 3 2 1 
5 10 16 
1 2 3 4 5 
5 4 3 2 1
Sample Output
12 
2 
0
题目大意： 
就是求01背包的第k优解
解题思路： 
首先分析如何求第k优解？ 
分析分析思路，因为每次遇到一个物品，都只有两种决策：要么取，要么不取。 
也就是说，相当于全年级有两个班，要求年级排名第k，那么只要知道这两个班的班排名前k就行了。
因此用A[]表示选择第i个物品的从大到小的排列，B[]表示不选择第i个物品的从大到小的排列。
然后每次循环找出两个数组里前k个，也就是在2*k个数中不重复地找出前k个数，记录到dp[i][j]中。
其中dp[i][j]表示体积为i的时候第j优解。
```cpp
#include<iostream>
#include<cstring>
using namespace std;
int dp[1005][35],A[35],B[35],val[1005],vol[1005];
int main()
{
    int t,n,v,k,a,b,c,kk;
    cin>>t;
    while(t--)
    {
        cin>>n>>v>>k;
        for(int i=0;i<n;i++)
          cin>>val[i];
        for(int i=0;i<n;i++)
          cin>>vol[i];
        memset(dp,0,sizeof(dp));
        for(int i=0;i<n;i++)
          for(int j=v;j>=vol[i];j--)
          {
              for(kk=1;kk<=k;kk++)
              {
                  A[kk]=dp[j-vol[i]][kk]+val[i];   //A[] 记录选择第i个物品的从大到小的排列 
                  B[kk]=dp[j][kk];                 //B[] 记录不选择第i个物品的从大到小的排列 
              }
              A[kk]=B[kk]=-1;
              a=b=c=1;
              while(c<=k&&(A[a]!=-1||B[b]!=-1))
              {
                  dp[j][c]=A[a]>B[b]?A[a++]:B[b++];        //d[i][j] 记录体积为 i 的时候的第 j 优解 
                  if(dp[j][c]!=dp[j][c-1])          //去重操作 
                    c++;
              }
          }
        cout<<dp[v][k]<<endl;
    }
    return 0;
}
```
