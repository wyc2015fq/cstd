# hdu 1023 Train Problem II（Catlan数） - HJ - CSDN博客
2017年09月08日 18:45:04[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：217
个人分类：[====ACM====																[----------数学----------																[排列组合																[---------OJ---------																[HDU](https://blog.csdn.net/feizaoSYUACM/article/category/6308041)](https://blog.csdn.net/feizaoSYUACM/article/category/6308067)](https://blog.csdn.net/feizaoSYUACM/article/category/6308090)](https://blog.csdn.net/feizaoSYUACM/article/category/6130298)](https://blog.csdn.net/feizaoSYUACM/article/category/6308070)
[http://acm.hdu.edu.cn/showproblem.php?pid=1023](http://acm.hdu.edu.cn/showproblem.php?pid=1023)
Catlan数计算模板（适用于数据较大的时候使用）
```cpp
#include<iostream>
using namespace std;
int a[105][1005];
void ktl()         
{
    a[2][0]=1,a[2][1]=2;
    a[1][0]=1,a[1][1]=1;
    int len=1;
    for(int i=3;i<101;i++)        //  递归公式： h(n) = h(n-1)*(4*n-2)/(n+1) 
    {
        int yu=0;                  //保存每一位的进位数 
        for(int j=1;j<=len;j++)                 //计算 s(n)=h(n-1)*（4*n-2) 
        {
            int t=a[i-1][j]*(4*i-2)+yu;
            yu=t/10;
            a[i][j]=t%10;
        }
        while(yu)
        {
            a[i][++len]=yu%10;
            yu/=10;
        }
        for(int j=len;j>=1;j--)           //计算  s(n)/(n+1) 
        {
            int t=a[i][j]+yu*10;
            a[i][j]=t/(i+1);
            yu=t%(i+1);
        }
        while(!a[i][len])        //去掉前导0 计算数据长度 
          len--;
        a[i][0]=len;
    }
}
int main()
{
    ktl();
    int n;
    while(cin>>n)
    {
        for(int i=a[n][0];i>0;i--)       //注意保存下来的数据是逆序的，因此要倒着输出 
          cout<<a[n][i];
        cout<<endl;
    }
    return 0;
}
```
