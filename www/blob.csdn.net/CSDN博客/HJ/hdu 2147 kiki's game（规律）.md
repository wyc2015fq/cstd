# hdu  2147  kiki's game（规律） - HJ - CSDN博客
2017年09月05日 17:52:50[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：144
个人分类：[====ACM====																[----------数学----------																[---------OJ---------																[HDU																[简单博弈](https://blog.csdn.net/feizaoSYUACM/article/category/7150812)](https://blog.csdn.net/feizaoSYUACM/article/category/6308041)](https://blog.csdn.net/feizaoSYUACM/article/category/6308067)](https://blog.csdn.net/feizaoSYUACM/article/category/6130298)](https://blog.csdn.net/feizaoSYUACM/article/category/6308070)
[http://acm.hdu.edu.cn/showproblem.php?pid=2147](http://acm.hdu.edu.cn/showproblem.php?pid=2147)
必胜点：n或者m存在至少一个偶数即可。
```cpp
#include<iostream>
using namespace std;
int main()
{
    int n,m;
    while(cin>>n>>m&&(n||m))
    {
        if(n%2==0||m%2==0)
          cout<<"Wonderful!"<<endl;
        else
          cout<<"What a pity!"<<endl;
    }
    return 0;
}
```
