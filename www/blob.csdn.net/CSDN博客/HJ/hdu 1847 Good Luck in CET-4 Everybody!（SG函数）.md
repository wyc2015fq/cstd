# hdu  1847  Good Luck in CET-4 Everybody!（SG函数） - HJ - CSDN博客
2017年09月05日 17:54:25[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：196
个人分类：[====ACM====																[----------数学----------																[---------OJ---------																[HDU																[简单博弈](https://blog.csdn.net/feizaoSYUACM/article/category/7150812)](https://blog.csdn.net/feizaoSYUACM/article/category/6308041)](https://blog.csdn.net/feizaoSYUACM/article/category/6308067)](https://blog.csdn.net/feizaoSYUACM/article/category/6130298)](https://blog.csdn.net/feizaoSYUACM/article/category/6308070)
[http://acm.hdu.edu.cn/showproblem.php?pid=1847](http://acm.hdu.edu.cn/showproblem.php?pid=1847)
根据要求SG函数打表，SG函数练手题。
```cpp
#include<iostream>
#include<cstring>
using namespace std;
#define maxn 2005
int n,pos=1,sg[maxn],f[maxn];
bool h[maxn];
void get_sg()
{
    memset(sg,0,sizeof(sg));
    for(int i=0;i<maxn;i++)
    {
        memset(h,0,sizeof(h));
        for(int j=1;f[j]<=i;j++)
          h[sg[i-f[j]]]=1;
        for(int j=0;j<maxn;j++)
          if(!h[j])
          {
              sg[i]=j;
              break;
          }
    }
}
int main()
{
    for(int i=1;i<maxn;i*=2)
      f[pos++]=i;
    get_sg();
    while(cin>>n)
    {
        if(sg[n])
          cout<<"Kiki"<<endl;
        else
          cout<<"Cici"<<endl; 
    }
    return 0;
}
```
