# codeforces 466-C. Number of Ways（前缀和+尺取） - HJ - CSDN博客
2017年08月30日 17:40:12[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：294
所属专栏：[codeforces](https://blog.csdn.net/column/details/17151.html)
[http://codeforces.com/problemset/problem/466/C](http://codeforces.com/problemset/problem/466/C)
题目大意： 
给出一组数，你需要将它划分成和相等的三部分。问你最多有多少种不同的划分方式？
解题思路： 
用前缀和保存数据，之后在划分的时候需要用尺取，因为你需要排除2*k的部分在k的部分前的情况，例如数据：
```
3  
4 -2 4
```
```
#include<iostream>
using namespace std;
#define maxn 500005
long long s[maxn],sum[maxn],a[maxn],b[maxn];
int main()
{
    int n;
    while(cin>>n)
    {
        long long ans=0;
        sum[0]=0;
        for(int i=1;i<=n;i++)
        {
            cin>>s[i];
            sum[i]=sum[i-1]+s[i];
        }
        long long k=sum[n]/3;
        if(sum[n]%3==0)
        {
            int t1=0,t2=0;
            for(int i=1;i<n;i++)
            {
                if(sum[i]==k&&i<n-1)
                  a[t1++]=i;
                if(sum[i]==2*k&&i<n)
                  b[t2++]=i;
            }
            int j=0;
            for(int i=0;i<t1;i++)
            {
                while(b[j]<=a[i]&&j<t2)
                  j++;
                ans+=t2-j;  
            }
        }
        else
          ans=0;
        cout<<ans<<endl;
    }
    return 0;
}
```
