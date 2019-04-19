# hdu 1220 Cube（规律+公式） - HJ - CSDN博客
2017年08月09日 14:12:35[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：197
[http://acm.hdu.edu.cn/showproblem.php?pid=1220](http://acm.hdu.edu.cn/showproblem.php?pid=1220)
题目大意： 
给你一个n*n*n的正方体，切割成单位体积的小正方体，求公共顶点数不超过2的小正方体的对数有多少。
解题思路： 
总共有C（2，n*n*n）对小正方体，排除公共点数超过2（也就是公共点个数为4）的正方体对数，考虑有n*n阶，每一阶有一排n个正方体，那么就会有n-1对公共点个数为4的正方体对数，另外每个正方体会与之相邻的三个正方体公共点个数都为4，因此还要算三次。
即总公式为  ans = n^3 * (n^3-1)/2 - 3*n*n*(n-1)
```cpp
#include<iostream>
using namespace std;
int main()
{
    int n;
    while(cin>>n)
    {
        long long ans=n*n*n*(n*n*n-1)/2-3*n*n*(n-1);
        cout<<ans<<endl; 
    }
    return 0;
}
```
