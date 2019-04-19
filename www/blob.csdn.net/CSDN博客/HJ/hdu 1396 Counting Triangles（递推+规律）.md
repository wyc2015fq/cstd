# hdu  1396 Counting Triangles（递推+规律） - HJ - CSDN博客
2017年08月09日 14:17:28[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：347
[http://acm.hdu.edu.cn/showproblem.php?pid=1396](http://acm.hdu.edu.cn/showproblem.php?pid=1396)
前一个的三角形数+新增的两层大小的三角形数+新增的三层以上的三角形数+倒着的三角数目（奇数与偶数不同，注意区分）
```cpp
#include<iostream>
using namespace std;
long long a[505];
int main()
{
    a[1]=1;
    for(int i=2;i<=501;i++)
    {
        a[i]=a[i-1]+i*(i-1)/2+2*i-1;
        int k=i-3,sum=0;
        while(k>0)
        {
            sum+=k;
            k-=2;
        }
        a[i]+=sum;
    }
    int n;
    while(cin>>n)
      cout<<a[n]<<endl;
    return 0;
}
```
