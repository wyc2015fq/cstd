# hdu 4407 Sum（容斥原理+数论） - HJ - CSDN博客
2017年09月08日 17:30:09[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：243
[http://acm.hdu.edu.cn/showproblem.php?pid=4407](http://acm.hdu.edu.cn/showproblem.php?pid=4407)
首先求出1~N之内与P互质的数的和，然后保存每次改变的操作。如遇到询问，dfs搜索一下更新ans值即可。
在求1~N之内与P互质的数的和的时候，反着先求与P不互质的数的和，再用总和减去要方便很多。而求与P不互质的数可以用容斥原理来实现，再用公式求和来进行更新即可。
处理起来颇为复杂，不失为一道好题。
```cpp
#include<iostream>
#include<cstdio>
#include<map>
using namespace std;
#define maxn 400005
typedef long long LL;
int prime[maxn][15]={0},vis[maxn]={0};
LL ans;
map<int,int>s;
map<int,int>::iterator it;
int gcd(int a,int b)         //求最大公约数 
{
    return a%b?gcd(b,a%b):b;
}
void EulerPrime()           //求欧拉函数值和每个数的素因子 
{
    for(int i=2;i<maxn;i++)
      if(!vis[i])
      {
          prime[i][++prime[i][0]]=i;
          for(int j=2;j*i<maxn;j++)
          {
              vis[i*j]=1;
              prime[i*j][++prime[i*j][0]]=i;
          }
      } 
}
void dfs(int id,int num,int cnt,int m,int n,int k)   //dfs的容斥原理 
{
    if(cnt==m)
    {
        int k=n/num;
        LL res=(LL)num*k*(k+1)/2;
        ans+=m%2?-res:res; 
    }
    else if(id<=prime[k][0]&&num<=n)
    {
        dfs(id+1,num,cnt,m,n,k);
        dfs(id+1,num*prime[k][id],cnt+1,m,n,k);
    }
}
LL get(int n,int k)     //求和 
{
    if(n<=0)
      return 0;
    ans=(LL)n*(n+1)/2;
    for(int i=1;i<=prime[k][0];i++)
      dfs(1,1,0,i,n,k);
    return ans;
}
int main()
{
    int t,n,m,k,x,y,c;
    scanf("%d",&t);
    EulerPrime();
    while(t--)
    {
        scanf("%d%d",&n,&m);
        s.clear();
        while(m--)
        {
            scanf("%d",&k);
            if(k==1)
            {
                scanf("%d%d%d",&x,&y,&c);
                LL res=get(y,c)-get(x-1,c);
                for(it=s.begin();it!=s.end();it++)
                {
                    if((*it).first>=x&&(*it).first<=y)
                    {
                        if(gcd(c,(*it).first)==1)
                          res-=(*it).first;
                        if(gcd(c,(*it).second)==1)
                          res+=(*it).second;
                    }
                }
                printf("%lld\n",res);
            }
            else
            {
                scanf("%d%d",&x,&y);
                s[x]=y;
            }
        } 
    }
    return 0;
}
```
