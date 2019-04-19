# hdu 4291 A Short problem（矩阵快速幂） - HJ - CSDN博客
2017年09月08日 17:44:16[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：186
个人分类：[====ACM====																[----------数学----------																[矩阵计算																[---------OJ---------																[HDU](https://blog.csdn.net/feizaoSYUACM/article/category/6308041)](https://blog.csdn.net/feizaoSYUACM/article/category/6308067)](https://blog.csdn.net/feizaoSYUACM/article/category/6308049)](https://blog.csdn.net/feizaoSYUACM/article/category/6130298)](https://blog.csdn.net/feizaoSYUACM/article/category/6308070)
[http://acm.hdu.edu.cn/showproblem.php?pid=4291](http://acm.hdu.edu.cn/showproblem.php?pid=4291)
因为是二阶线性递推式，因此必然存在循环。通过两次暴力模拟得到周期 
mod1=1000000007; 
mod2=222222224； 
mod3=183120；
再用矩阵快速幂即可。
```cpp
#include<iostream>
using namespace std;
typedef long long LL;
struct met{
    LL a[2][2];
};
met multi(met x,met y,LL mod)     //矩阵乘法 
{
    met z;
    for(int i=0;i<2;i++)
      for(int j=0;j<2;j++)
      {
          LL ans=0;
          for(int k=0;k<2;k++)
            ans=(ans+x.a[i][k]*y.a[k][j])%mod;
          z.a[i][j]=ans%mod; 
      }
    return z;
}
LL quickmult(LL n,LL mod)         //矩阵快速幂 
{
    met x,y;
    y.a[0][0]=1,y.a[0][1]=y.a[1][0]=y.a[1][1]=0;      //[1,0]矩阵初始化 
    x.a[0][0]=3,x.a[0][1]=1,x.a[1][0]=1,x.a[1][1]=0;        //构造矩阵初始化 
    while(n)
    {       
        if(n%2)
          y=multi(y,x,mod);
        x=multi(x,x,mod);
        n/=2;
    }
    return y.a[0][0];
}
int main()
{
    LL n,mod1=1000000007,mod2=222222224,mod3=183120;    
    while(cin>>n)
    {
        if(n>=2) n=quickmult(n-1,mod3);       //第二层的循环节 
        if(n>=2) n=quickmult(n-1,mod2);        //第一层的循环节 
        if(n>=2) n=quickmult(n-1,mod1);          //最外面的循环节 
        cout<<n<<endl;
    }
    return 0;
}
```
