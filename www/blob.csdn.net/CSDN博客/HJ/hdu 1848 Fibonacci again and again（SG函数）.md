# hdu 1848 Fibonacci again and again（SG函数） - HJ - CSDN博客
2017年09月05日 18:16:32[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：161
个人分类：[====ACM====																[----------数学----------																[---------OJ---------																[HDU																[简单博弈](https://blog.csdn.net/feizaoSYUACM/article/category/7150812)](https://blog.csdn.net/feizaoSYUACM/article/category/6308041)](https://blog.csdn.net/feizaoSYUACM/article/category/6308067)](https://blog.csdn.net/feizaoSYUACM/article/category/6130298)](https://blog.csdn.net/feizaoSYUACM/article/category/6308070)
[http://acm.hdu.edu.cn/showproblem.php?pid=1848](http://acm.hdu.edu.cn/showproblem.php?pid=1848)
SG函数打表。
```cpp
#include<iostream>
#include<cstring>
using namespace std;
int sg[1005],h[1005],f[25];
void get_sg()
{
    memset(sg,0,sizeof(sg));
    for(int i=0;i<=1001;i++)
    {
        memset(h,0,sizeof(h));
        for(int j=1;j<=20;j++)
          if(i>=f[j])
            h[sg[i-f[j]]]=1;
        for(int j=0;j<=1001;j++)
          if(!h[j])
          {
              sg[i]=j;
              break;
          }
    }
}
int main()
{
    int n,m,p;
    f[1]=1,f[2]=2;
    for(int i=3;i<=20;i++)
      f[i]=f[i-1]+f[i-2];
    get_sg();
    while(cin>>n>>m>>p&&(n||m||p))
    {
        int ans=sg[n]^sg[m]^sg[p];
        if(ans)
          cout<<"Fibo"<<endl;
        else
          cout<<"Nacci"<<endl;
    }
    return 0;
}
```
