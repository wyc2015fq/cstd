# CSU 1946 A Rational Sequence （递归） - HJ - CSDN博客
2017年08月30日 17:18:36[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：198
个人分类：[====ACM====																[---------OJ---------																[杂题](https://blog.csdn.net/feizaoSYUACM/article/category/6684121)](https://blog.csdn.net/feizaoSYUACM/article/category/6308067)](https://blog.csdn.net/feizaoSYUACM/article/category/6308070)
[http://acm.csu.edu.cn/csuoj/problemset/problem?pid=1946](http://acm.csu.edu.cn/csuoj/problemset/problem?pid=1946)
把n转换成 二进制表示，然后递归一遍就行了。
```cpp
#include<iostream>
using namespace std;
#define maxn 2147483647
int main()
{
    long long ans=1,n;
    for(int i=1;ans<maxn;i++)
      ans*=2;
    int t,x;
    cin>>t;
    while(t--)
    {
        char a[50],b[50];
        cin>>n;
        long long k=ans,p=1,q=1;
        int cnt=0;
        while(k&&n<k)
          k/=2;
        n-=k;
        k/=2;
        while(k)
        {
            if(n>=k)
            {
                n-=k;
                p=p+q;
            }
            else
              q=p+q;
            k/=2;
        }
        cout<<p<<"/"<<q<<endl;
    }
    return 0;
}
```
