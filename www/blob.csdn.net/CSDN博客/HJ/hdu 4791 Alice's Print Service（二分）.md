# hdu 4791 Alice's Print Service（二分） - HJ - CSDN博客
2017年10月30日 21:05:29[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：205
[http://acm.hdu.edu.cn/showproblem.php?pid=4791](http://acm.hdu.edu.cn/showproblem.php?pid=4791)
题目大意： 
  打印张数越多价格区间越优惠（也可能相同）。问给定张数为n的时候，打印多少张最省钱。问最后花多少钱。
解题思路： 
逆向求出每个区间的总价格，二分查找最后花钱最少的区间。
```cpp
#include<cstdio>
#include<cstring>
#include<algorithm>
#define M 100005
#define LL long long
using namespace std;
struct node
{
    LL s,p;
}a[M];
LL price[M];
int main()
{
    int T,n,m;
    LL ans,ans1,ans2,total;
    scanf("%d",&T);
    while(T--)
    {
        scanf("%d%d",&n,&m);
        for(int i=0;i<n;i++)
            scanf("%lld%lld",&a[i].s,&a[i].p);
        price[n-1]=a[n-1].p*a[n-1].s;
        for(int i=n-2;i>=0;i--)           //注意逆向更新 
            price[i]=min(a[i].p*a[i].s,price[i+1]); 
        for(int i=0;i<m;i++)
        {
            scanf("%lld",&total);
            int l,r,mid;
            l=0,r=n-1;
            while(l<r)
            {
                mid=(l+r+1)/2;
                if(total>=a[mid].s)
                  l=mid;
                else
                  r=mid-1;
            }
            ans1=total*a[l].p;
            if(l==n-1)
              ans2=ans1;
            else
              ans2=price[l+1];
            ans=min(ans1,ans2);
            printf("%lld\n",ans);
        } 
    } 
    return 0;
}
```
