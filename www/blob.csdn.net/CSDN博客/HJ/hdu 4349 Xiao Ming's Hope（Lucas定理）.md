# hdu 4349 Xiao Ming's Hope（Lucas定理） - HJ - CSDN博客
2017年09月08日 18:06:14[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：221
个人分类：[====ACM====																[----------数学----------																[初等数论																[---------OJ---------																[HDU](https://blog.csdn.net/feizaoSYUACM/article/category/6308041)](https://blog.csdn.net/feizaoSYUACM/article/category/6308067)](https://blog.csdn.net/feizaoSYUACM/article/category/6308034)](https://blog.csdn.net/feizaoSYUACM/article/category/6130298)](https://blog.csdn.net/feizaoSYUACM/article/category/6308070)
[http://acm.hdu.edu.cn/showproblem.php?pid=4349](http://acm.hdu.edu.cn/showproblem.php?pid=4349)
Lucas定理：A,B是非负整数，P是质数。A、B写成P进制 
A=a[n]a[n-1]…a[0] 
B=b[n]b[n-1]…b[0] 
则组合数C（A,B）与C(a[n],b[n]) * C(a[n-1],b[n-1]) * …*C(a[0],b[0])mod p同余 
即：Lucas(n,m,p)=C(n%p,m%p)*Lucas(n/p,m/p,p)
因此本题中直接把A化成2进制，计算有多少个1,然后求出2^ans即可。
```cpp
#include<iostream>
#include<cstdio>
using namespace std;
int main()
{
    int n,ans;
    while(cin>>n)
    {
        ans=0;
        while(n)
        {
            ans+=n%2;
            n/=2;
        }
        printf("%d\n",1<<ans);
    }
    return 0;
}
```
