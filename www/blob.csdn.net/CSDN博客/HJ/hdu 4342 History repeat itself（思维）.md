# hdu  4342 History repeat itself（思维） - HJ - CSDN博客
2017年09月08日 18:13:49[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：191
个人分类：[====ACM====																[----------数学----------																[容斥原理																[---------OJ---------																[HDU](https://blog.csdn.net/feizaoSYUACM/article/category/6308041)](https://blog.csdn.net/feizaoSYUACM/article/category/6308067)](https://blog.csdn.net/feizaoSYUACM/article/category/6355772)](https://blog.csdn.net/feizaoSYUACM/article/category/6130298)](https://blog.csdn.net/feizaoSYUACM/article/category/6308070)
[http://acm.hdu.edu.cn/showproblem.php?pid=4342](http://acm.hdu.edu.cn/showproblem.php?pid=4342)
先统计n前面有多少个平方数，然后确定第n个非平方数对应的数值，再用求和公式进行计算。
```cpp
#include<iostream>
#include<cmath>
using namespace std;
long long a,n,res,ans;
int main()
{
    int t;
    cin>>t;
    while(t--)
    {
        cin>>a;
        n=sqrt(a+sqrt(a));     //统计n前面有多少个平方数 
        res=a+n;                    //确定第n个非平方数对应的数值 
        ans=n*(n+1)*(2*n+1)/3-3*(n+1)*n/2+n+(n+a-n*n+1)*n;    //计算和       
        cout<<res<<" "<<ans<<endl;
    } 
    return 0;
}
```
