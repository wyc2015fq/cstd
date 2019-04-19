# hdu 1588  Gauss Fibonacci （二分+矩阵快速幂） - HJ - CSDN博客
2016年07月17日 22:43:12[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：365
Description 
Without expecting, Angel replied quickly.She says: “I’v heard that you’r a very clever boy. So if you wanna me be your GF, you should solve the problem called GF~. ”  
How good an opportunity that Gardon can not give up! The “Problem GF” told by Angel is actually “Gauss Fibonacci”.  
As we know ,Gauss is the famous mathematician who worked out the sum from 1 to 100 very quickly, and Fibonacci is the crazy man who invented some numbers. 
Arithmetic progression:  
g(i)=k*i+b;  
We assume k and b are both non-nagetive integers. 
Fibonacci Numbers:  
f(0)=0  
f(1)=1  
f(n)=f(n-1)+f(n-2) (n>=2) 
The Gauss Fibonacci problem is described as follows:  
Given k,b,n ,calculate the sum of every f(g(i)) for （0大于等于i小于n）  
The answer may be very large, so you should divide this answer by M and just output the remainder instead.
Input 
The input contains serveral lines. For each line there are four non-nagetive integers: k,b,n,M  
Each of them will not exceed 1,000,000,000. 
Output 
For each line input, out the value described above.
Sample Input 
2 1 4 100 
2 0 4 100 
Sample Output 
21 
12 
题目翻译：先告诉你Fibonacci的定义，然后给出一个一次函数g(i)=k*i+b，要你求出从0到n-1的函数值的和，其中每个i都是相对应的Fibonacci的值。
解析：我们知道，如果按数来打表的话，long long再怎么强大，也只能求出Fibonacci的前90项而已，但是题目给出的n最大是1,000,000,000。因此，暴力打表肯定是行不通的。
那么求Fibonacci数的一种特殊的方式，是用矩阵来算。（这个可以百度）因此这个题目就需要转换到矩阵幂的思路上来了。求矩阵的幂，因为n的数据过大，所以又要用到快速幂，另外，求从0到n的和的话，也是n过大的原因，就又要用到二分法求等比数列。
具体实现请看代码：
```
#include<iostream>
using namespace std;
struct matrix                 //因为矩阵需要用数组保存数据，调用函数的时候一般需要用到结构体
{
    long long m[3][3];
};
matrix E,B;
long long k,b,n,mod;        //定义给出的变量，方便调用函数使用
void Init()                 //矩阵的分块
{
    for(int i=1;i<=2;i++)
      for(int j=1;j<=2;j++)
        E.m[i][j]=(i==j);
    B.m[1][1]=B.m[1][2]=B.m[2][1]=1;
    B.m[2][2]=0;
}
matrix multi(matrix a,matrix b)    //矩阵内相乘求和
{
    matrix c;
    for(int i=1;i<=2;i++)
      for(int j=1;j<=2;j++)
      {
          c.m[i][j]=0;
          for(int q=1;q<=2;q++)
            c.m[i][j]=(c.m[i][j]+a.m[i][q]*b.m[q][j])%mod;
      }
    return c;
}
matrix quickmulti(matrix a,int k)    //矩阵快速幂
{
    matrix ans=E;
    while(k)
    {
        if(k&1)
        {
            k--;
            ans=multi(ans,a);
        }
        else
        {
            k>>=1;
            a=multi(a,a);
        }
    }
    return ans;
}
matrix add(matrix a,matrix b)     //矩阵求和
{
    matrix c;
    for(int i=1;i<=2;i++)
      for(int j=1;j<=2;j++)
        c.m[i][j]=(a.m[i][j]+b.m[i][j])%mod;
    return c;
}
matrix sum(matrix mat,long long n)    //二分等比矩阵列求和
{
    matrix ans;
    if(n==0)
    {
        for(int i=1;i<=2;i++)
          for(int j=1;j<=2;j++)
            ans.m[i][j]=0;
        return ans;
    }
    else if(n==1)
      return mat;
    else
    {
        matrix res,t;
        t=sum(mat,n/2);
        res=add(t,multi(quickmulti(mat,n/2),t));
        if(n&1)
          res=add(res,quickmulti(mat,n));
        return res;
    }
}
int main()
{
    while(cin>>k>>b>>n>>mod)
    {
        Init();
        matrix p1=quickmulti(B,b);
        matrix p2=quickmulti(B,k);
        matrix ans=multi(p1,add(E,sum(p2,n-1)));
        cout<<ans.m[1][2]<<endl;
    }
    return 0;
}
```
补刀：函数调用比较多，没办法，因为矩阵计算就是这么麻烦。
