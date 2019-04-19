# ZOJ  3600   Taxi Fare（模拟） - HJ - CSDN博客
2017年08月23日 19:07:08[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：198
个人分类：[====ACM====																[---------OJ---------																[杂题](https://blog.csdn.net/feizaoSYUACM/article/category/6684121)](https://blog.csdn.net/feizaoSYUACM/article/category/6308067)](https://blog.csdn.net/feizaoSYUACM/article/category/6308070)
[http://acm.zju.edu.cn/onlinejudge/showProblem.do?problemCode=3600](http://acm.zju.edu.cn/onlinejudge/showProblem.do?problemCode=3600)
题目大意： 
杭州有两种出租车的价格，一种是三公里之内10元，3~10公里每公里2元，超出10公里的部分每公里3元，每等5分钟需要2元，但是需要额外付1元。另一种是三公里之内11元，3~10公里每公里2.5元，超出10公里的部分每公里3.75元，每等4分钟需要2.5元。问第一种比第二种便宜多少钱？
解题思路： 
按照题意模拟一遍，注意精度问题。
```cpp
#include<iostream>
#include<cstdio>
using namespace std;
int main()
{
    int tt,d,t;
    cin>>tt;
    while(tt--)
    {
        cin>>d>>t;
        double ans1=t*0.4+11,ans2=t*0.625+11;
        if(d>3)
        {
            if(d<=10)
            {
                ans1+=2*(d-3);
                ans2+=2.5*(d-3);
            }
            else
            {
                ans1+=14+3*(d-10);
                ans2+=17.5+3.75*(d-10);
            }
        }
        int ans=(int)(ans2+0.5)-(int)(ans1+0.5);
        cout<<ans<<endl;
    }
    return 0;
}
```
