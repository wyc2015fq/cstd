# codefores 538-C. Tourist's Notes（数学） - HJ - CSDN博客
2017年08月30日 17:36:28[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：281
所属专栏：[codeforces](https://blog.csdn.net/column/details/17151.html)
[http://codeforces.com/problemset/problem/538/C](http://codeforces.com/problemset/problem/538/C)
题目大意： 
在一个组数中给出一些下标固定的数，要求每个相邻的数之间差值为1，问这组数最大值可以是多少？
解题思路： 
考虑两个尽头的特殊情况，接着按照2*n-1的公式不断枚举，找出最大值。
```cpp
#include<iostream>
#include<cmath>
using namespace std;
int main()
{
    long long n,m,a,b,x,y;
    while(cin>>n>>m)
    {
        cin>>a>>b;
        long long blag=0,ans=a+b-1,i=1;
        for(;i<m;i++)
        {
            cin>>x>>y;
            if(blag)
              continue;
            if(x-a>=fabs(y-b))
            {
                long long p=x-a,q=fabs(y-b),res=p-q-1,j=1,tt=1;   //p表示长度  q表示差值   
                while(tt<=res)
                {
                    j++;
                    tt+=2;
                }   
                ans=max(ans,j-1+max(y,b));
            }
            else
              blag=1;
            a=x,b=y;
        }
        ans=max(ans,n-a+b);
        if(blag)
          cout<<"IMPOSSIBLE"<<endl;
        else
          cout<<ans<<endl;
    }
    return 0;
}
```
