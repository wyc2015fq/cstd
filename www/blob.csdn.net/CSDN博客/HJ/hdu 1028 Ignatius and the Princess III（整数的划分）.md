# hdu  1028  Ignatius and the Princess III（整数的划分） - HJ - CSDN博客
2017年03月07日 23:59:38[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：285
个人分类：[====ACM====																[---------OJ---------																[HDU																[--------动态规划---------																[基础DP](https://blog.csdn.net/feizaoSYUACM/article/category/7238027)](https://blog.csdn.net/feizaoSYUACM/article/category/7238093)](https://blog.csdn.net/feizaoSYUACM/article/category/6308041)](https://blog.csdn.net/feizaoSYUACM/article/category/6308067)](https://blog.csdn.net/feizaoSYUACM/article/category/6308070)
Problem Description 
“Well, it seems the first problem is too easy. I will let you know how foolish you are later.” feng5166 says.
“The second problem is, given an positive integer N, we define an equation like this: 
  N=a[1]+a[2]+a[3]+…+a[m]; 
  a[i]>0,1<=m<=N; 
My question is how many different equations you can find for a given N. 
For example, assume N is 4, we can find: 
  4 = 4; 
  4 = 3 + 1; 
  4 = 2 + 2; 
  4 = 2 + 1 + 1; 
  4 = 1 + 1 + 1 + 1; 
so the result is 5 when N is 4. Note that “4 = 3 + 1” and “4 = 1 + 3” is the same in this problem. Now, you do it!”
Input 
The input contains several test cases. Each test case contains a positive integer N(1<=N<=120) which is mentioned above. The input is terminated by the end of file.
Output 
For each test case, you have to output a line contains an integer P which indicate the different equations you have found.
Sample Input
4 
10 
20
Sample Output
5 
42 
627
1.递归法：
根据n和m的关系，考虑以下几种情况：
（1）当n=1时，不论m的值为多少（m>0)，只有一种划分即{1};
(2) 当m=1时，不论n的值为多少，只有一种划分即n个1，{1,1,1,…,1};
(3) 当n=m时，根据划分中是否包含n，可以分为两种情况：
```
(a). 划分中包含n的情况，只有一个即{n}；
 (b). 划分中不包含n的情况，这时划分中最大的数字也一定比n小，即n的所有(n-1)划分。 因此 f(n,n) =1 + f(n,n-1);
```
(4) 当n< m时，由于划分中不可能出现负数，因此就相当于f(n,n);
(5) 但n>m时，根据划分中是否包含最大值m，可以分为两种情况：
```
(a). 划分中包含m的情况，即{m, {x1,x2,...xi}}, 其中{x1,x2,... xi} 的和为n-m，因此这种情况下为f(n-m,m)
(b). 划分中不包含m的情况，则划分中所有值都比m小，即n的(m-1)划分，个数为f(n,m-1);因此 f(n, m) = f(n-m, m)+f(n,m-1);
```
综上所述：
```
f(n,m)=1;           (n=1 or m=1)
f(n,m)=f(n, n);                (n<m)
f(n,m)=1+ f(n, m-1);           (n=m)
f(n,m)=f(n-m,m)+f(n,m-1);      (n>m)
```
2.动态规划
状态转移方程：
当 n == 1 || k == 1 时，f(n, k) == 1， n为1，那么只能为1； 而k为1，那么只能划分成n个1. 
当 n < k 时，f(n, k) == f(n, n)，因为n的划分中不可能出现比n大的数，所以可以将最大值从k降到n； 
当 n >= k 时，f(n, k) = f(n-k, k) + f(n, k-1)， 前半部分是划分中存在最大值k，所以可以在(n-k)中继续以最大值为k来划分，而后半部分则是划分中最大值不是k，那么其结果和以(k-1)为最大值的划分是一样的。在初始化中可以将f(0, k)初始化为1，及对应f(n, n)可能出现的情况。
综合来说，使用递归更加容易理解，不过使用动态规划时间消耗更小。
```cpp
#include<iostream>
using namespace std;
long long d[125][125];
void dp()
{
    for(int i=1;i<=120;i++)
      d[i][1]=d[1][i]=d[0][i]=1;
    for(int i=2;i<=120;i++)
      for(int j=2;j<=120;j++)
      {
          if(i<j)
            d[i][j]=d[i][i];
          else
            d[i][j]=d[i-j][j]+d[i][j-1];
      }
}
int main()
{
    int n;
    dp();
    while(cin>>n)
      cout<<d[n][n]<<endl;
    return 0;
}
```
