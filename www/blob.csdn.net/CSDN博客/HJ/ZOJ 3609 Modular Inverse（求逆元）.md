# ZOJ  3609 Modular Inverse（求逆元） - HJ - CSDN博客
2017年08月23日 19:21:50[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：198
个人分类：[====ACM====																[排列组合																[---------OJ---------																[杂题](https://blog.csdn.net/feizaoSYUACM/article/category/6684121)](https://blog.csdn.net/feizaoSYUACM/article/category/6308067)](https://blog.csdn.net/feizaoSYUACM/article/category/6308090)](https://blog.csdn.net/feizaoSYUACM/article/category/6308070)
[http://acm.zju.edu.cn/onlinejudge/showProblem.do?problemCode=3609](http://acm.zju.edu.cn/onlinejudge/showProblem.do?problemCode=3609)
解题思路： 
公式转换一下：
```
(a*x-1)%m==0
```
代码如下：
```cpp
#include<iostream>
using namespace std;
int gcd(int a,int b)
{
    return a%b==0?b:gcd(b,a%b);
}
int main()
{
    int t;
    cin>>t;
    while(t--)
    {
        int a,b,k=1;
        cin>>a>>b;
        if(gcd(a,b)!=1)
        {
            cout<<"Not Exist"<<endl;
            continue;
        }
        while((a*k-1)%b!=0)
          k++;
        cout<<k<<endl;
    }
    return 0;
}
```
