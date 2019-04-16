# poj3070 Fibonacci 矩阵快速幂 - 紫芝的博客 - CSDN博客





2018年05月09日 21:49:37[紫芝](https://me.csdn.net/qq_40507857)阅读数：33








Fibonacci
|Time Limit: 1000MS||Memory Limit: 65536K|
|----|----|----|
|Total Submissions: 18084||Accepted: 12572|

Description

In the Fibonacci integer sequence, F0 = 0, F1 = 1, and Fn = Fn − 1 + Fn − 2 for n ≥ 2. For example, the first ten terms of the Fibonacci sequence are:

0, 1, 1, 2, 3, 5, 8, 13, 21, 34, …

An alternative formula for the Fibonacci sequence is

![](http://poj.org/images/3070_1.png).

Given an integer n, your goal is to compute the last 4 digits of Fn.

Input

The input test file will contain multiple test cases. Each test case consists of a single line containing n (where 0 ≤ n ≤ 1,000,000,000). The end-of-file is denoted by a single line containing the number −1.

Output

For each test case, print the last four digits of Fn. If the last four digits of Fn are all zeros, print ‘0’; otherwise, omit any leading zeros (i.e., print Fn mod 10000).

Sample Input

```
0
9
999999999
1000000000
-1
```

Sample Output

```
0
34
626
6875
```

Hint

As a reminder, matrix multiplication is associative, and the product of two 2 × 2 matrices is given by

![](http://poj.org/images/3070_2.png).

Also, note that raising any 2 × 2 matrix to the 0th power gives the identity matrix:

![](http://poj.org/images/3070_3.gif).

Source

[Stanford Local 2006](http://poj.org/searchproblem?field=source&key=Stanford+Local+2006)





矩阵快速幂的知识

[点击打开链接](https://blog.csdn.net/qq_40507857/article/details/80259935)



```cpp
#include<iostream>
using namespace std;
typedef long long  ll;
const int MOD=1e4;
#define mod(x) ((x)%MOD)
struct mat{
int m[2][2];
}unit;
mat operator *(mat a,mat b)
{
    mat ret;
    ll x;
    for(int i=0;i<2;i++)
        for(int j=0;j<2;j++)
    {
        x=0;
        for(int k=0;k<2;k++)
            x+=mod( (ll) (a.m[i][k]*b.m[k][j]) );
            ret.m[i][j]=mod(x);
    }
    return ret;
}
void init_unit()
{
    for(int i=0;i<2;i++)
        unit.m[i][i]=1;//单位矩阵，主对角线上元素为1，其余元素为0
    return;
}
mat pow_mat(mat a,ll n)
{
    mat ret=unit;
    while(n){
        if(n&1) ret=ret*a;
        a=a*a;
        n>>=1;
    }
    return ret;
}
int main()
{
	ios::sync_with_stdio(false);
    ll n;
    init_unit();
    while(cin>>n&&n!=-1){
        mat a;
        a.m[0][0]=a.m[0][1]=a.m[1][0]=1;
        a.m[1][1]=0;
        a=pow_mat(a,n);
        cout<<a.m[0][1]<<endl;
    }
	return 0;
}
```





