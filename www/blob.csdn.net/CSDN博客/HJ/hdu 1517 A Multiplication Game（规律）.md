# hdu 1517 A Multiplication Game（规律） - HJ - CSDN博客
2017年09月05日 18:02:42[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：147
个人分类：[====ACM====																[----------数学----------																[---------OJ---------																[HDU																[简单博弈](https://blog.csdn.net/feizaoSYUACM/article/category/7150812)](https://blog.csdn.net/feizaoSYUACM/article/category/6308041)](https://blog.csdn.net/feizaoSYUACM/article/category/6308067)](https://blog.csdn.net/feizaoSYUACM/article/category/6130298)](https://blog.csdn.net/feizaoSYUACM/article/category/6308070)
[http://acm.hdu.edu.cn/showproblem.php?pid=1517](http://acm.hdu.edu.cn/showproblem.php?pid=1517)
必胜点：[0,9]、[18,162]、… 
必败点：[10,17]、…
实际上先存好一些数：2,9,9*2,9*2*9，9*2*9*2,…
然后判断在哪个区间内即可。
```cpp
#include<iostream>
using namespace std;
#define maxn 4294967295
long long d[10005]; 
int main()
{
    int blag=0;
    d[0]=0;
    d[1]=9;
    for(int i=2;;i++)
    {
        if(blag==0)
        {
            d[i]=d[i-1]*2;
            blag=1;
        }
        else
        {
            d[i]=d[i-1]*9;
            blag=0;
        }
        if(d[i]>maxn)
          break;
    }
    long long n;
    while(cin>>n)
    {
        for(int i=1;;i++)
        {
            if(n>d[i-1]&&n<=d[i])
            {
                if(i%2)
                  cout<<"Stan wins."<<endl;
                else
                  cout<<"Ollie wins."<<endl;
                break; 
            }
        }
    }
    return 0;
}
```
