# hdu 3507 Print Article（dp斜率优化） - HJ - CSDN博客
2018年08月07日 16:28:07[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：26
个人分类：[====ACM====																[--------动态规划---------																[DP优化																[---------OJ---------																[HDU](https://blog.csdn.net/feizaoSYUACM/article/category/6308041)](https://blog.csdn.net/feizaoSYUACM/article/category/6308067)](https://blog.csdn.net/feizaoSYUACM/article/category/7915204)](https://blog.csdn.net/feizaoSYUACM/article/category/7238093)](https://blog.csdn.net/feizaoSYUACM/article/category/6308070)
[http://acm.hdu.edu.cn/showproblem.php?pid=3507](http://acm.hdu.edu.cn/showproblem.php?pid=3507)
dp斜率优化入门题，解释在代码中。
```
#include<iostream>
#include<algorithm>
#include<cstdio>
using namespace std;
const int maxn=5e5+10;
int dp[maxn],sum[maxn];
int q[maxn];
int head,tail,n,m;
//dp[i] = min{ dp[j] + (sum[i] - sum[j]) ^ 2 };
int getdp(int i,int j)
{
    return dp[j]+m+(sum[i]-sum[j])*(sum[i]-sum[j]); 
} 
int getup(int j ,int k)
{
    return (dp[j]+sum[j]*sum[j])-(dp[k]+sum[k]*sum[k]);
}
int getdown(int j,int k)
{
    return 2*(sum[j]-sum[k]);
}
int main()
{
    while(~scanf("%d%d",&n,&m))
    {
        for(int i=1;i<=n;i++)
          cin>>sum[i];
        sum[0]=dp[0]=0;
        for(int i=1;i<=n;i++)
          sum[i]+=sum[i-1];
        head=tail=0;
        q[tail++]=0;
        for(int i=1;i<=n;i++)
        {
            //注意顺序，此处把斜率改成相乘
            //如果g(a,b)<2*sum,说明b比a优，删除队首a，直到g(a,b)>=2*sum[i]为止
            //这样队列维护了最优点的选择，即队首元素 
            while(head+1<tail&&getup(q[head+1],q[head])<=sum[i]*getdown(q[head+1],q[head]))
              head++;
            dp[i]=getdp(i,q[head]); 
            //用一个单调队列保存这些将要插入的点放在队尾，在插入之前看当前队尾元素j
            //如果满足g(k,j)>g(j,i)条件，就将j删除，直到不满足为止 
            while(head+1<tail&&getup(i,q[tail-1])*getdown(q[tail-1],q[tail-2])<=getup(q[tail-1],q[tail-2])*getdown(i,q[tail-1]))
              tail--;
            //此时将i插入 队列满足从头到尾的g(a,b),g(b,c)...一直递增 
            q[tail++]=i;
        }
        printf("%d\n",dp[n]);
    }
    return 0; 
}
```
