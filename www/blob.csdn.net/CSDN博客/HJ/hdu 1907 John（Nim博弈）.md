# hdu  1907 John（Nim博弈） - HJ - CSDN博客
2017年08月23日 19:34:01[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：188
[http://acm.hdu.edu.cn/showproblem.php?pid=1907](http://acm.hdu.edu.cn/showproblem.php?pid=1907)
解题思路： 
注意尼姆博弈是n堆石子谁先取完谁先赢，这个是谁取到最后一颗糖谁算输，因此结论应该倒过来。另外需要特判所有糖果种类的个数都为1的情况（分奇偶即可）
```cpp
#include<iostream>
using namespace std;
int main()
{
    int n,t,x;
    cin>>t;
    while(t--)
    {
        cin>>n;
        int blag=0,ans=0;
        for(int i=0;i<n;i++)
        {
            cin>>x;
            if(x!=1)
              blag=1;
            ans^=x;
        }
        if(blag==0)
        {
            if(n%2)
              cout<<"Brother"<<endl;
            else
              cout<<"John"<<endl;
        }
        else
        {
            if(ans)
              cout<<"John"<<endl;
            else
              cout<<"Brother"<<endl;
        }
    }
    return 0;
}
```
