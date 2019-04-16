# 2604 Queuing（递推+矩阵快速幂） - PeterBishop - CSDN博客





2018年09月15日 23:28:20[PeterBishop0](https://me.csdn.net/qq_40061421)阅读数：25
个人分类：[HDOJ](https://blog.csdn.net/qq_40061421/article/category/7502192)









**分析： 题目要求不含101 和111 的串**

**设f[n]为长度为n串符合条件的个数**

**则很明显在长度为（n-1）并且符合条件的串后面加上一个0一定符合**

**如果在长度为n-1的串后面加上一个1的话我们得考虑n-1的串结尾的元素**

**如果是00的话我们可以看做是长度为n-3的串加上100 **

**如果是10的话我们可以看做长度为n-4的串加上1100**

**因此f[n]=f[n-1]+f[n-3]+f[n-4];**

### [矩阵快速幂看这里！](https://blog.csdn.net/qq_40061421/article/details/82625338)

```cpp
#include <iostream>
#include <cstdio>
using namespace std;
int n,M;
struct matrax
{
    int m[4][4];
};
matrax A={
  1,0,1,1,
  1,0,0,0,
  0,1,0,0,
  0,0,1,0
};
matrax E;
void init()
{
   for(int i=0;i<4;i++)
    for(int j=0;j<4;j++)
     E.m[i][j]=(i==j);
}
matrax multi(matrax a,matrax b)
{
    matrax c;
    for(int i=0;i<4;i++){
        for(int j=0;j<4;j++){
            c.m[i][j]=0;
            for(int k=0;k<4;k++)
                c.m[i][j]+=a.m[i][k]*b.m[k][j]%M;
        c.m[i][j]%=M;
        }
    }
    return c;
}
matrax power(matrax A,int k)
{
    matrax ans=E,p=A;
    while(k){
        if(k&1){
            ans=multi(ans,p);
            k--;
        }
        k>>=1;
        p=multi(p,p);
    }
    return ans;
}
int main()
{
    init();
    int a[4]={1,2,4,6};
    while(cin>>n>>M){
        matrax ans=power(A,n-3);
        int x=0;
        for(int i=0;i<4;i++)
            x+=(ans.m[0][i]*a[4-i-1])%M;
        /*for(int i=0;i<4;i++){
            for(int j=0;j<4;j++)
                cout<<ans.m[i][j]<<" ";
            cout<<endl;
        }*/
        cout<<x%M<<endl;
    }
    return 0;
}
```





