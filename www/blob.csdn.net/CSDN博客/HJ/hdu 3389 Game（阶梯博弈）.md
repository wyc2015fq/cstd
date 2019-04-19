# hdu 3389 Game（阶梯博弈） - HJ - CSDN博客
2017年09月05日 21:29:06[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：233
[http://acm.hdu.edu.cn/showproblem.php?pid=3389](http://acm.hdu.edu.cn/showproblem.php?pid=3389)
1 3 4号盒子是最终状态，所有出始状态都会经过若干步操作变成这个最终状态。
如果仔细分析，会发现：n % 6 == 0 或 2 或 5的盒子，经过奇数步转移到1 3 4中去，其他的则须经过偶数步才能转移过去。
易证：偶数步的不用管，对于奇数步的操作只需转换成Nim博弈，利用异或运算判断是否必胜即可。
```cpp
#include<iostream>
#include<cstdio>
using namespace std;
int main()
{
    int t,tt=1,n,x;
    cin>>t;
    while(t--)
    {
        cin>>n;
        int ans=0;
        for(int i=1;i<=n;i++)
        {
            cin>>x;
            if(i%6==0||i%6==2||i%6==5)
              ans^=x;
        }
        printf("Case %d: %s\n",tt++,ans?"Alice":"Bob");
    }
    return 0;
}
```
