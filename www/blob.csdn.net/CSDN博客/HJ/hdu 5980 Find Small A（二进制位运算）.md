# hdu  5980 Find Small A（二进制位运算） - HJ - CSDN博客
2017年10月27日 13:36:36[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：204
[http://acm.hdu.edu.cn/showproblem.php?pid=5980](http://acm.hdu.edu.cn/showproblem.php?pid=5980)
题目大意： 
给出n个数，判断每个数化成二进制每四位中有多少个“a”?
解题思路： 
把每个数循环整除256，判断多少个余数为97即可。
```cpp
#include<iostream>
using namespace std;
int main()
{
    int n;
    while(cin>>n)
    {
        long long x,ans=0;
        for(int i=0;i<n;i++)
        {
            cin>>x;
            while(x)
            {
                if(x%(1<<8)==97)
                  ans++;
                x>>=8;
            } 
        }
        cout<<ans<<endl;
    }
    return 0;
}
```
