# POJ 3233 Matrix Power Serie （矩阵快速幂） - HJ - CSDN博客
2016年07月17日 22:15:30[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：284
个人分类：[====ACM====																[----------数学----------																[矩阵计算																[---------OJ---------																[POJ](https://blog.csdn.net/feizaoSYUACM/article/category/6308043)](https://blog.csdn.net/feizaoSYUACM/article/category/6308067)](https://blog.csdn.net/feizaoSYUACM/article/category/6308049)](https://blog.csdn.net/feizaoSYUACM/article/category/6130298)](https://blog.csdn.net/feizaoSYUACM/article/category/6308070)
Description
Given a n × n matrix A and a positive integer k, find the sum S = A + A2 + A3 + … + Ak.
Input
The input contains exactly one test case. The first line of input contains three positive integers n (n ≤ 30), k (k ≤ 109) and m (m < 104). Then follow n lines each containing n nonnegative integers below 32,768, giving A’s elements in row-major order.
Output
Output the elements of S modulo m in the same way as A is given.
Sample Input
2 2 4 
0 1 
1 1 
Sample Output
1 2 
2 3
解析：给出矩阵A，求等比前k+1项数的和。那么从数据来看，k最大取10的9次方，一个一个来算，不用想肯定超时。因此，需要用到快速幂。类比数的快速幂，矩阵的快速幂可以依葫芦画瓢，把两个数相乘改成两个矩阵相乘，调用一个函数来实现矩阵相加，再调用一个函数来实现矩阵相乘就OK了，本题没有什么特殊的情况，最后输出矩阵就可以了，具体实现如下：
```
#include <cstdio>  
#include <iostream>  
#include <cstring>  
using namespace std;    
typedef struct node{       //用结构体实现函数通用，这比调用数组返回值什么的要方便的多
    int matrix[32][32];  
}Matrix;  
Matrix a,sa,unit;        //定义结构体变量，实际上就是一个数组 
int n,k,mod;             //相关变量提前定义，方便各个函数使用
Matrix Add(Matrix a,Matrix b)     //两个矩阵相加
{  
    Matrix c;  
    for (int i=0; i<n; i++)  
      for (int j=0; j<n; j++)  
      {  
          c.matrix[i][j]=a.matrix[i][j]+b.matrix[i][j];  
          c.matrix[i][j]%=mod;  
      }  
    return c;  
}  
Matrix Mul(Matrix a,Matrix b)     //两个矩阵相乘，注意返回值也要是一个矩阵
{  
    Matrix c;  
    for (int i=0; i<n; i++)  
      for (int j=0; j<n; j++)  
      {  
          c.matrix[i][j]=0;  
          for (k=0; k<n; k++)  
             c.matrix[i][j]+=a.matrix[i][k]*b.matrix[k][j];  
          c.matrix[i][j]%=mod;  
      }  
    return c;  
}  
Matrix Cal(int exp)      //矩阵快速幂，跟数的快速幂类似
{  
    Matrix p,q;  
    p=a;  
    q=unit;  
    while (exp!=1)  
    {  
        if (exp&1)  
        {  
            exp--;  
            q=Mul(p,q);  
        }  
        else  
        {  
            exp>>=1;  
            p=Mul(p,p);  
        }  
    }  
    return Mul(p,q);  
}  
Matrix MatrixSum(int k)    //等比矩阵列求和，涵盖以上所有函数
{  
    if (k==1) return a;  
    Matrix tmp,tnow;  
    tmp=MatrixSum(k/2);  
    if (k&1)      
    {  
        tnow=Cal(k/2+1);  
        tmp=Add(tmp,Mul(tmp,tnow));  
        tmp=Add(tnow,tmp);  
    }     
    else        
    {  
        tnow=Cal(k/2);  
        tmp=Add(tmp,Mul(tmp,tnow));  
    }  
    return tmp;  
}  
int main()  
{  
    cin>>n>>k>>mod;  
    for (int i=0; i<n; i++)  
       for (int j=0; j<n; j++)  
       {  
           cin>>a.matrix[i][j];  
           a.matrix[i][j]%=mod;  
           unit.matrix[i][j]=(i==j);  
       }  
    sa=MatrixSum(k);     //调用一个总功能实现的函数，避免麻烦
    for (int i=0; i<n; i++)  
    {  
        for (int j=0; j<n; j++)  
             cout<<sa.matrix[i][j]<<" ";  
        cout<<endl;  
    }  
    return 0;  
}
```
