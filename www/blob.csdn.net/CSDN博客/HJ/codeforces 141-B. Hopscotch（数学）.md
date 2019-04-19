# codeforces 141-B. Hopscotch（数学） - HJ - CSDN博客
2017年08月30日 17:46:16[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：246
个人分类：[====ACM====																[----------数学----------																[容斥原理																[---------OJ---------																[codeforce](https://blog.csdn.net/feizaoSYUACM/article/category/6353637)](https://blog.csdn.net/feizaoSYUACM/article/category/6308067)](https://blog.csdn.net/feizaoSYUACM/article/category/6355772)](https://blog.csdn.net/feizaoSYUACM/article/category/6130298)](https://blog.csdn.net/feizaoSYUACM/article/category/6308070)
所属专栏：[codeforces](https://blog.csdn.net/column/details/17151.html)
[http://codeforces.com/problemset/problem/141/B](http://codeforces.com/problemset/problem/141/B)
题目大意： 
除了第一个格子之外，以上的格子都按如图所示的方式摆放，给出坐标，问这个坐标在第几个格子内？
解题思路： 
按照纵坐标奇偶来筛选，排除在格子线上的情况枚举一下就行了。
```cpp
#include<iostream>
#include<cmath>
using namespace std;
int main()
{
    int a,x,y;
    while(cin>>a>>x>>y)
    {
        int k=x,q;
        x=fabs(x);
        if(x<a)            //在宽度为2a的盒子内 
        {
            int p=y/a;
            if(p*a==y)             //纵坐标为整数直接跳出 
            {
                cout<<-1<<endl;
                continue;
            }
            if(p==0)             //对于y属于[0,a]特判 
            {
                if(x*2<a)
                  q=1;
                else
                  q=-1;
                cout<<q<<endl;
                continue;
            }
            if(p%2==0)               //纵坐标往下取整为偶数 
            {
                if(k==0)
                {
                    cout<<-1<<endl;
                    continue;
                }
                if(k<0)
                  q=p/2*3;
                else
                  q=p/2*3+1;
            }
            else
            {
                if(x*2<a)
                  q=(p+1)/2*3-1;
                else
                  q=-1;
            }
        }
        else
          q=-1;
        cout<<q<<endl;
    }
    return 0;
}
```
