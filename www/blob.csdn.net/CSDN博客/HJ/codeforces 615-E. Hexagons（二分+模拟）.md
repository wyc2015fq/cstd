# codeforces 615-E. Hexagons（二分+模拟） - HJ - CSDN博客
2018年08月08日 11:14:02[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：35
个人分类：[====ACM====																[-----------搜索-----------																[二分&三分																[---------OJ---------																[codeforce](https://blog.csdn.net/feizaoSYUACM/article/category/6353637)](https://blog.csdn.net/feizaoSYUACM/article/category/6308067)](https://blog.csdn.net/feizaoSYUACM/article/category/7238038)](https://blog.csdn.net/feizaoSYUACM/article/category/6307975)](https://blog.csdn.net/feizaoSYUACM/article/category/6308070)
所属专栏：[codeforces](https://blog.csdn.net/column/details/17151.html)
[http://codeforces.com/problemset/problem/615/E](http://codeforces.com/problemset/problem/615/E)
题目大意： 略
解题思路：
首先二分判断n所在的层数，然后根据层次k算出六个点的坐标；
接下来将n减去k-1层内所有步数，对k整除和取膜，模拟判断六种情况，算出坐标即可
```
#include<iostream>
#include<cmath>
using namespace std;
typedef long long ll;
ll sum(ll x)
{
    return (ll)(3*x*(x+1));
}
ll find(ll x)
{
    ll l=0,r=sqrt(x+0.5);
    while(l<r)
    {
        ll mid=(l+r)/2;
        if(sum(mid)<x)
          l=mid+1;
        else
          r=mid;
    }
    return l;
}
int main()
{
    ll n;
    while(cin>>n)
    {
        if(n==0)
        {
            cout<<"0 0\n";
            continue;
        }
        ll k=find(n),ans=n-sum(k-1);
        ll x,y;
        ll p=ans/k,q=ans%k;
        n=k;
        if(p==0)
          x=(ll)(2*n-q),y=(ll)(2*q);
        else if(p==1)
          x=(ll)(n-2*q),y=(ll)(2*n);
        else if(p==2)
          x=(ll)(-n-q),y=(ll)(2*(n-q));
        else if(p==3)
          x=(ll)(-2*n+q),y=(ll)(-2*q);
        else if(p==4)
          x=(ll)(-n+2*q),y=(ll)(-2*n);
        else if(p==5)
          x=n+q,y=(ll)(-2*(n-q));
        else if(p==6)
          x=(ll)(2*k),y=0;
        cout<<x<<" "<<y<<endl;
    }
    return 0;
}
```
