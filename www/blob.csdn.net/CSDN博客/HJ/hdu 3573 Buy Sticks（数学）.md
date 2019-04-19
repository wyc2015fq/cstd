# hdu 3573 Buy Sticks（数学） - HJ - CSDN博客
2017年09月08日 18:24:16[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：186
个人分类：[====ACM====																[----------数学----------																[容斥原理																[---------OJ---------																[HDU](https://blog.csdn.net/feizaoSYUACM/article/category/6308041)](https://blog.csdn.net/feizaoSYUACM/article/category/6308067)](https://blog.csdn.net/feizaoSYUACM/article/category/6355772)](https://blog.csdn.net/feizaoSYUACM/article/category/6130298)](https://blog.csdn.net/feizaoSYUACM/article/category/6308070)
[http://acm.hdu.edu.cn/showproblem.php?pid=3573](http://acm.hdu.edu.cn/showproblem.php?pid=3573)
考虑每一个75cm的绳子可以剪成28+20 * 2或者32+20 * 2 
如果20cm长绳子需要很多的话，剩下的就剪成20*3 
否则按照28 * 2或者32 * 2来剪即可。
```cpp
#include<iostream>
#include<cstdio>
using namespace std;
int main()
{
    int t,tt=1,a,b,c;
    cin>>t;
    while(t--)
    {
        cin>>a>>b>>c;
        int p=(a+1)/2,ans;
        if(p>b+c)
          ans=b+c+(a-(b+c)*2+2)/3; 
        else
          ans=(b+c-p+1)/2+p;
        printf("Case %d: %d\n",tt++,ans);
    }
    return 0;
}
```
