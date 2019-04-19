# codeforces 237-C. Primes on Interval（尺取） - HJ - CSDN博客
2017年08月31日 13:30:23[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：241
所属专栏：[codeforces](https://blog.csdn.net/column/details/17151.html)
[http://codeforces.com/problemset/problem/237/C](http://codeforces.com/problemset/problem/237/C)
先素数打表，然后顺着和倒着各自尺取一遍，取最大值即可。
```
#include<iostream>
#include<cstring>
using namespace std;
#define maxn 1000005
int vis[maxn],s[2][maxn],n,k;
void Init()
{
    memset(vis,0,sizeof(vis));
    vis[1]=1;
    for(int i=2;i<=maxn-5;i++)
      if(vis[i]==0)
        for(int j=i*2;j<=maxn-5;j+=i)
          vis[j]=1;
} 
int cq(int q)
{
    int l=0,r=0,res=0,ans=0;
    while(l<n&&r<n)
    {
        while(r<n&&res<k)
        {
            if(vis[s[q][r]]==0)
              res++;
            r++;
        }
        if(res>=k)
          ans=max(ans,r-l);
        if(vis[s[q][l]]==0)
          res--;
        l++;
    }
    if(ans==0)
      ans--;
    return ans;
} 
int main()
{
    Init();
    int a,b;
    while(cin>>a>>b>>k)
    {
        int cnt=0,e=0;
        for(int i=a;i<=b;i++)
          s[0][cnt++]=i;
        for(int i=cnt-1;i>=0;i--)
          s[1][e++]=s[0][i];
        n=cnt;
        int ans1=cq(0),ans2=cq(1),ans=max(ans1,ans2);
        cout<<ans<<endl;
    }
    return 0;
}
```
