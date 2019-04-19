# codeforces 1016-C. Vasya And The Mushrooms（dp+前缀和） - HJ - CSDN博客
2018年08月08日 13:42:28[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：129
所属专栏：[codeforces](https://blog.csdn.net/column/details/17151.html)
[http://codeforces.com/problemset/problem/1016/C](http://codeforces.com/problemset/problem/1016/C)
题目大意：
从一个2*n格子的左上角出发，要求每个点都有且必须只能访问一次，并且每次访问一个点都会得到此点上的数与基数的乘积，问最后最大的乘积和为多少？
解题思路：
其实自己模拟一下就很容易发现，只有两种走法：
① 按照 ‘下左右上’ 四次一个循环，重复地走； 
② 按照’一直往左再拐回一直往右’ ，一次性走完；
那么如何才能得到最大值呢？
可以设dp[i]表示区间[1,i]按照第①种走法，区间[i,n]按照第②种走法，然后提前计算出前缀和以及预处理前i个数和后i个数的带基数的答案和，顺序走一遍即可。
```
#include<iostream>
#include<cstring>
#include<algorithm>
#include<cstdio>
using namespace std;
const int maxn=3e5+10;
typedef long long ll;
ll s[2][maxn];      //s[i][j] 表示第j行 
ll n,ans=0,cnt=0;
ll sum[2][maxn];   //sum[i][j] 表示第i行前k个数的前缀和 
ll sum1[2][2][maxn];   
/*
   sum1[i][j][k] 
     当j等于0的时候表示第i行前k个数按照 0*s[i][1]+1*s[i][1]+2*s[i][2]+...方式的和 
     当j等于1的时候表示第i行后[k,n]内的数按照 0*s[i][n]+1*s[i][n-1]+2*s[i][n-2]+...方式的和 
*/ 
ll p[maxn];  //p[i]表示按照正常模拟的方式前i列数的和 
int main()
{
    while(cin>>n)
    {
        for(int i=0;i<2;i++)
          for(int j=1;j<=n;j++)  
            cin>>s[i][j];
        for(int i=0;i<2;i++)
          for(int j=0;j<2;j++)
            sum1[i][j][0]=0;
        p[0]=sum[0][0]=sum[1][0]=0;
        for(int i=0;i<2;i++)
          for(int j=1;j<=n;j++)
            sum[i][j]=sum[i][j-1]+s[i][j];
        for(int i=0;i<2;i++)
        {
            for(int j=1;j<=n;j++)
              sum1[i][0][j]=sum1[i][0][j-1]+(ll)(s[i][j]*(j-1));
            for(int j=n;j>=1;j--)
              sum1[i][1][j]=sum1[i][1][j+1]+(ll)(s[i][j]*(n-j));
        }
        for(int i=1,k=0,blag=0;i<=n;i++,k+=2,blag=1-blag)
          p[i]=p[i-1]+(ll)(k*s[blag][i])+(ll)((k+1)*(s[!blag][i]));
        ll ans=p[n];
        for(int i=1,blag=0;i<=n;i++,blag=1-blag)
        {
            ll p1=(ll)((i-1)*(sum[blag][n]-sum[blag][i-1])),p2=sum1[blag][0][n]-sum1[blag][0][i-1];
            ll p3=(ll)((n+i-1)*(sum[!blag][n]-sum[!blag][i-1])),p4=sum1[!blag][1][i];
            ll res=p1+p2+p3+p4; 
            ans=max(ans,(ll)(p[i-1]+res));
        }
        cout<<ans<<endl;
    }
    return 0;
}
```
