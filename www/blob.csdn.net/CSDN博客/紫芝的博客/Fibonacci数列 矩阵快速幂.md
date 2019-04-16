# Fibonacci数列 矩阵快速幂 - 紫芝的博客 - CSDN博客





2018年05月11日 12:11:27[紫芝](https://me.csdn.net/qq_40507857)阅读数：128








## [点击打开链接](http://sustoj.com/problem.php?cid=1018&pid=0)

## 问题 A: 一道简单的Fibonacci

时间限制: 1 Sec  内存限制: 32 MB

## 题目描述

有如下数列：F(0) = 7, F(1) = 11, F(n) = F(n - 1) + F(n - 2)  (n >= 2)。

## 输入

输入由一系列的行构成，每一行包含一个正数 n (n < 1000000)。

## 输出

如果 F(n) 能被 3 整除，则输出 “Yes”，否则输出 “No”。

## 样例输入

```
0
1
2
3
4
5
```

## 样例输出

```
No
No
Yes
No
No
No
```

## ![](https://img-blog.csdn.net/20180515195641555)

![](https://img-blog.csdn.net/20180515195801276)

## [点击打开链接](http://sustoj.com/problem.php?cid=1018&pid=1)

##  Fibonacci变形1

时间限制: 1 Sec  内存限制: 32 MB

提交: 47  解决: 4

[[提交](http://sustoj.com/submitpage.php?cid=1018&pid=1&langmask=0)][[状态](http://sustoj.com/problemstatus.php?id=1681)][[讨论版](http://sustoj.com/bbs.php?pid=1681&cid=1018)]

## 题目描述

有如下数列：F(1) = 1, F(2) = 1, F(n) = ( a * F(n - 1) + b * F(n - 2) ) mod 7。现给定 a, b, n ，求 F(n)。

## 输入

输入由多组数据构成，每组数据一行，分别为 a, b, n (1 <= a, b <= 1000, 1 <= n <= 100000000) 。当 a，b，n 均为 0 时停止输入。

## 输出

对于每组数据，输出 F(n)。每个输出占一行。

## 样例输入

```
1 1 3
1 2 10
0 0 0
```

## 样例输出

```
2
5
```



```cpp
#include<bits/stdc++.h>
using namespace std;
typedef long long  ll;
const int MOD=7;
#define mod(x) ((x)%MOD)
struct mat{
ll m[2][2];
}unit;
int a,b,n;
mat operator *(mat a,mat b)
{
    mat ret;
    ll x;
    for(int i=0;i<2;i++)
        for(int j=0;j<2;j++)
    {
        x=0;
        for(int k=0;k<2;k++)
            x+=mod( (a.m[i][k]*b.m[k][j]) );
            ret.m[i][j]=mod(x);
    }
    return ret;
}
void init_unit()
{
    unit.m[0][0]=mod(a+b);   unit.m[0][1]=1;
    unit.m[1][0]=1;     unit.m[1][1]=1;
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
    while(scanf("%d%d%d",&a,&b,&n)!=EOF,a+b+n){
        init_unit();
        mat x;
        x.m[0][0]=a;    x.m[0][1]=1;
        x.m[1][0]=b;    x.m[1][1]=0;
        if(n==1||n==2)
            cout<<1<<endl;
        else{
            x=pow_mat(x,n-2);
        cout<<x.m[1][0]<<endl;
        }
        //输出矩阵
        for(int i=0;i<2;i++){
            for(int j=0;j<2;j++)
            {
                if(j+1==2)  cout<<x.m[i][j]<<endl;
                else cout<<x.m[i][j]<<" ";
            }
        }

    }
    return 0;
}
```





