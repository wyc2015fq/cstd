# CSU  1597: 薛XX后代的IQ （矩阵运算） - HJ - CSDN博客
2017年01月31日 03:20:14[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：316
个人分类：[====ACM====																[----------数学----------																[矩阵计算																[---------OJ---------																[杂题](https://blog.csdn.net/feizaoSYUACM/article/category/6684121)](https://blog.csdn.net/feizaoSYUACM/article/category/6308067)](https://blog.csdn.net/feizaoSYUACM/article/category/6308049)](https://blog.csdn.net/feizaoSYUACM/article/category/6130298)](https://blog.csdn.net/feizaoSYUACM/article/category/6308070)
薛XX的低IQ是个令人头疼的问题，他的队友深受其害。幸运的是，薛XX非常有钱，所以他买了一些可以提高他的后代的IQ的药。这种药有三个属性，A，B和P。当薛XX使用这种药的时候，他的基因会发生变化，所以他的儿子的IQ也会跟着变化。假设薛XX的父亲的IQ为X，薛XX自己的IQ为Y，那么薛XX的儿子的IQ为(A*X+B*Y) mod P。薛XX的孙子的IQ依次类推。 
现在给定X和Y，还有药的属性A、B和P，现在他想知道他的N代子孙的IQ（儿子是第一代，孙子是第二代）。
Input
第一行包含一个整数T（T<=100），表示数据组数 
每组数据只有一行，包含六个整数X,Y,A,B,P,N（1 ≤ X, Y ≤ 300,1 ≤ A, B ≤ 30, 1≤ P ≤ 300 , 1 ≤ N < 1000000000），含义如题目所述
Output
对于每组数据，输出答案
Sample Input
4 
180 80 1 1 190 1 
189 83 2 2 190 1 
189 83 1 1 190 2  
172 73 23 19 273 9999
Sample Output
70 
164 
165 
233
解题思路： 
当分析暴力会超时之后，马上要想到，给出关系式，利用矩阵计算，快速幂提高时间效率。
```
#include<iostream>
#include<cstdio>
#include<cstring>
using namespace std;
typedef long long ll;
const int maxn=2;
int a,x,b,y,c,mod;
ll n;
struct Matrix
{
    int a[maxn][maxn];
    Matrix(){memset(a,0,sizeof(a));}
    Matrix operator* (const Matrix &p)
    {
        Matrix res;
        for(int i=0;i<maxn;i++)
        {
            for(int j=0;j<maxn;j++)
            {
                for(int k=0;k<maxn;k++)
                {
                    res.a[i][j]+=(a[i][k]*p.a[k][j]%mod);
                }
                res.a[i][j]%=mod;
            }
        }
        return res;
    }
}ans,base;
Matrix quick_pow(Matrix base,ll n)
{
    Matrix res;
    for(int i=0;i<maxn;i++)
    {
        res.a[i][i]=1;
    }
    while(n)
    {
        if(n&1) res=res*base;
        base=base*base;
        n>>=1;
    }
    return res;
}
void init_matrix()
{
    ans.a[0][0]=y;
    ans.a[0][1]=x;
    ans.a[1][0]=0;
    ans.a[1][1]=0;
    base.a[0][0]=b;
    base.a[0][1]=1;
    base.a[1][0]=a;
    base.a[1][1]=0;
}
int main()
{
    int t;
    scanf("%d",&t);
    while(t--)
    {
        scanf("%d%d%d%d%d%lld",&x,&y,&a,&b,&mod,&n);
        init_matrix();
        ans=ans*quick_pow(base,n);
        printf("%d\n",ans.a[0][0]);
    }
    return 0;
}
```
