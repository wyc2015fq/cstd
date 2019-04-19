# POJ 3070 Fibonacci（矩阵快速幂） - HJ - CSDN博客
2016年07月13日 09:25:28[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：305
个人分类：[====ACM====																[----------数学----------																[矩阵计算																[---------OJ---------																[POJ](https://blog.csdn.net/feizaoSYUACM/article/category/6308043)](https://blog.csdn.net/feizaoSYUACM/article/category/6308067)](https://blog.csdn.net/feizaoSYUACM/article/category/6308049)](https://blog.csdn.net/feizaoSYUACM/article/category/6130298)](https://blog.csdn.net/feizaoSYUACM/article/category/6308070)
Description 
In the Fibonacci integer sequence, F0 = 0, F1 = 1, and Fn = Fn − 1 + Fn − 2 for n ≥ 2. For example, the first ten terms of the Fibonacci sequence are:
0, 1, 1, 2, 3, 5, 8, 13, 21, 34, …
An alternative formula for the Fibonacci sequence is 
| Fn+1       Fn|         | 1   1 |(n) 
| Fn        Fn-1|    =   |1    0  | 
Given an integer n, your goal is to compute the last 4 digits of Fn.
Input 
The input test file will contain multiple test cases. Each test case consists of a single line containing n (where 0 ≤ n ≤ 1,000,000,000). The end-of-file is denoted by a single line containing the number −1.
Output 
For each test case, print the last four digits of Fn. If the last four digits of Fn are all zeros, print ‘0’; otherwise, omit any leading zeros (i.e., print Fn mod 10000).
Sample Input 
0 
9 
999999999 
1000000000 
-1
Sample Output 
0 
34 
626 
6875
解析：根据常识，Fibonacci的递推公式为F（0）=0，F（1）=1，F（n）=F（n-1）+F（n-2）（n>=2） 
求解的方法也有多种：
1、递归实现
其中递归结束条件是f[1]=1，f[2]=1。
```
int fib(int n)
{     
  if(n<1)
    return -1;
  if(n==1 || n==2)
    return 1;
  return fib(n-1)+fib(n-2);
}
```
2、数组实现
```
int fib(int n) 
{
  if(n<1)
    return -1;
  if(n<3)
    return 1;
  int *a=new int[n];
  a[0]=a[1]=1;
  for(int i=2;i<n;i++)
    a[i]=a[i-1]+a[i-2];
  int m=a[n-1];
  delete a;         //释放内存空间
  return m;
}
```
3、二分矩阵法实现 
首先，需要了解矩阵的乘法运算
```
比如一个2阶矩阵A，四个数分别是a11、a12、a21、a22
那么A*A的结果形成新的四个数b11、b12、b21、b22，则这四个数的结果分别是
b11==a11*a11+a12*a21
b12==a11*a12+a12*22
b21==a21*a11+a22*21
b22==a21*a12+a22*a22
```
那么如果有n个矩阵A相乘，这个n比较小的话还好
问：A为单位矩阵，A^19  = ？ 
A^156 = ？ 
已知：
19(10) = 10011 (2) 
156(10) = 10011100(2)
比如矩阵A, A的n次方A^n , 
如果幂很大，即n很大的时候，可做如下判断：
```
当n为偶数时，A^n= A^(n/2 + n/2) = A^(n/2) * A^(n/2)，A*A*A*A*A*A  =>  (A*A)*(A*A)*(A*A)
当n为奇数时，A^n= A^(n/2 + n/2 + 1) = A^(n/2) * A^(n/2) * A，
```
因此， 
A^19  =（A^16）*（A^2）*（A^1） 
A^156 而156(10)=10011100(2) 
10011100中的最右端开始计算到最左端
```
A^156 = (A^4)*(A^8)*(A^16)*(A^128)
```
这样一来，除了多一个矩阵乘法运算复杂了一点之外，跟快速幂取模就十分相似了，请看代码：
```
#include<iostream>
using namespace std;
int m=10000;
void Multiply(int c[2][2],int b[2][2],int a[2][2],int m)   //矩阵乘法运算
{
    int d[4];
    d[0]=a[0][0]*b[0][0]+a[0][1]*b[1][0];
    d[1]=a[0][0]*b[0][1]+a[0][1]*b[1][1];
    d[2]=a[1][0]*b[0][0]+a[1][1]*b[1][0];
    d[3]=a[1][0]*b[0][1]+a[1][1]*b[1][1];
    c[0][0]=d[0]%m;
    c[0][1]=d[1]%m;
    c[1][0]=d[2]%m;
    c[1][1]=d[3]%m;
}
int Matrix(int n,int m)
{
    int s;
    if(n==0) 
      s=0;
    else if(n==1||n==2) 
      s=1;
    else if(n>2)
    {
        int a[2][2]={{1,1},{1,0}},b[2][2]={{1,0},{0,1}};   //定义矩阵b为单位矩阵
        n=n-2;
        while(n>0)    
        {
            if(n&1)
              Multiply(b,b,a,m);
            Multiply(a,a,a,m);
            n>>=1;                    //while循环内的模板跟快速幂取模雷同
        }  
        s=(b[0][0]+b[0][1])%m;       //取矩阵第一行的两个数之和取模
    } 
    return s;
}
int main()
{
    int n;
    while(cin>>n)
    {
        int sum;
        if(n==-1) break;
        sum=Matrix(n,m);
        cout<<sum<<endl;
    }
    return 0;
}
```
总结：不管是快速幂取模，还是矩阵快速幂，都离不开二分法，二分法可以使得指数从n直线降到lgn，特别是遇上指数n很大的时候，使用二分法运算起来效率非常快。
