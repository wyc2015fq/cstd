# hdu 4345 Permutation（dp） - HJ - CSDN博客
2017年09月08日 18:00:07[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：264
个人分类：[====ACM====																[---------OJ---------																[HDU																[--------动态规划---------																[基础DP](https://blog.csdn.net/feizaoSYUACM/article/category/7238027)](https://blog.csdn.net/feizaoSYUACM/article/category/7238093)](https://blog.csdn.net/feizaoSYUACM/article/category/6308041)](https://blog.csdn.net/feizaoSYUACM/article/category/6308067)](https://blog.csdn.net/feizaoSYUACM/article/category/6308070)
[http://acm.hdu.edu.cn/showproblem.php?pid=4345](http://acm.hdu.edu.cn/showproblem.php?pid=4345)
dp[i][j] 表示取到前i个素数 和小于等于j的方法数 
```cpp
#include<iostream>
#include<cstring>
using namespace std;
#define N 1001
long long k,vis[1005],prime[1005],dp[1005][1005];
void Init()                     //素数打表 
{
    k=1;
    memset(vis,0,sizeof(vis));
    for(int i=2;i<N;i++)
      if(vis[i]==0)
      {
          prime[k++]=i;
          for(int j=i*i;j<N;j+=i)
            vis[j]=1;
      }
    k--;             //prime[]保存N以内的k个素数 
}
int main()
{
    Init(); 
    for(int i=0;i<N;i++)           //dp[i][j] 表示取到前i个素数 和小于等于j的方法数 
      dp[0][i]=1;                      //全部取1的情况只有一种 因此初始化为1 
    for(int i=1;i<=k;i++)          //N以内有k个素数 
      for(int j=0;j<N;j++)
      {
          dp[i][j]=dp[i-1][j];         //首先继承前面一个的方法数 
          for(int p=prime[i];p<=j;p*=prime[i])         //再判断对于第i个素数是否存在新的方法数 
            dp[i][j]+=dp[i-1][j-p];
      }
    int n;
    while(cin>>n)
      cout<<dp[k][n]<<endl;
    return 0;
}
```
