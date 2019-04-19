# codeforces 259-B. Little Elephant and Magic Square（数学） - HJ - CSDN博客
2017年08月30日 17:32:00[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：317
所属专栏：[codeforces](https://blog.csdn.net/column/details/17151.html)
[http://codeforces.com/problemset/problem/259/B](http://codeforces.com/problemset/problem/259/B)
九宫格里正中间的数等于任意旁边每一对数之和的一半。
```cpp
#include<iostream>
using namespace std;
int main()
{
    int n;
    cin>>n;
    while(n--)
    {
        int a[5][5];
        for(int i=0;i<3;i++)
          for(int j=0;j<3;j++)
            cin>>a[i][j];
        a[1][1]=(a[0][1]+a[2][1])/2;
        a[0][0]=3*a[1][1]-(a[0][1]+a[0][2]);
        a[2][2]=3*a[1][1]-(a[0][2]+a[1][2]);
        for(int i=0;i<3;i++)
        {
            for(int j=0;j<2;j++)
              cout<<a[i][j]<<" ";
            cout<<a[i][2]<<endl;
        }
   }
    return 0;
}
```
