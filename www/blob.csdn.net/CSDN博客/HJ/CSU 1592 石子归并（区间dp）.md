# CSU  1592  石子归并（区间dp） - HJ - CSDN博客
2017年01月31日 03:16:04[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：319
个人分类：[====ACM====																[---------OJ---------																[杂题																[--------动态规划---------																[区间DP](https://blog.csdn.net/feizaoSYUACM/article/category/7238029)](https://blog.csdn.net/feizaoSYUACM/article/category/7238093)](https://blog.csdn.net/feizaoSYUACM/article/category/6684121)](https://blog.csdn.net/feizaoSYUACM/article/category/6308067)](https://blog.csdn.net/feizaoSYUACM/article/category/6308070)
现在有n堆石子，第i堆有ai个石子。现在要把这些石子合并成一堆，每次只能合并相邻两个，每次合并的代价是两堆石子的总石子数。求合并所有石子的最小代价。
Input
第一行包含一个整数T（T<=50），表示数据组数。 
每组数据第一行包含一个整数n（2<=n<=100），表示石子的堆数。 
第二行包含n个正整数ai（ai<=100），表示每堆石子的石子数。
Output
每组数据仅一行，表示最小合并代价。
Sample Input
2 
4 
1 2 3 4 
5 
3 5 2 1 4
Sample Output
19 
33
解题思路：
设d[i][j]为合并第i-j堆石子所需要的最小花费
状态转移方程为：dp[i][j]=max(dp[i]k]+dp[k+1][j]+sum[i][j])，i<=k<=j。
```cpp
#include<iostream>  
#include<cstdio>  
#define maxn 110  
#define INF 0x7fffffff  
using namespace std;   
int n,a[maxn],dp[maxn][maxn],sum[maxn];  
int main(){   
    int t;
    cin>>t;  
    while(t--)
    {  
        cin>>n;  
        for(int i=1;i<=n;i++)  
            cin>>a[i];  
        sum[0]=0;  
        for(int i=1;i<=n;i++)  
          sum[i]=sum[i-1]+a[i];  
        for(int i=1;i<=n;i++) 
          for(int j=1;j<=n;j++) 
             dp[i][j]=INF;//初始化  
        for(int i=1;i<=n;i++) 
          dp[i][0]=0;//初始化  
        for(int j=1;j<n;j++) 
          for(int i=1;i+j<=n;i++)    //先枚举长度j  
            for(int k=0;k<j;k++)  
              dp[i][j]=min(dp[i][j],dp[i][k]+dp[i+k+1][j-k-1]+sum[i+j]-sum[i-1]);  
        cout<<dp[1][n-1]<<endl;  
    }  
    return 0;  
}
```
