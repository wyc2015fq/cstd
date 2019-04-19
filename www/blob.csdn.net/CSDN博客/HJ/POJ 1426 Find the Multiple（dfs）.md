# POJ  1426  Find the Multiple（dfs） - HJ - CSDN博客
2017年07月02日 17:11:18[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：227
个人分类：[====ACM====																[-----------搜索-----------																[DFS																[---------OJ---------																[POJ](https://blog.csdn.net/feizaoSYUACM/article/category/6308043)](https://blog.csdn.net/feizaoSYUACM/article/category/6308067)](https://blog.csdn.net/feizaoSYUACM/article/category/6317663)](https://blog.csdn.net/feizaoSYUACM/article/category/6307975)](https://blog.csdn.net/feizaoSYUACM/article/category/6308070)
Given a positive integer n, write a program to find out a nonzero multiple m of n whose decimal representation contains only the digits 0 and 1. You may assume that n is not greater than 200 and there is a corresponding m containing no more than 100 decimal digits. 
Input 
    The input file may contain multiple test cases. Each line contains a value of n (1 <= n <= 200). A line containing a zero terminates the input. 
Output 
    For each value of n in the input print a line containing the corresponding value of m. The decimal representation of m must not contain more than 100 digits. If there are multiple solutions for a given value of n, any one of them is acceptable. 
Sample Input 
    2 
    6 
    19 
    0
Sample Output 
    10 
    100100100100100100
题目大意：输出任意一个只由0和1组成且能整除n的十进制数。
方法：dfs
```cpp
#include<iostream>
#include<cstdio>
using namespace std;
int n,blag;
void dfs(unsigned long long t,int k)
{
    if(blag)
      return ;
    if(t%n==0)
    {
        cout<<t<<endl;
        blag=1;
        return ;
    }
    if(k==19)
      return ;
    dfs(t*10,k+1);    //因为只由0和1组成，因此只需要搜索两次 
    dfs(t*10+1,k+1);
}
int main()
{
    while(cin>>n&&n)
    {
        blag=0;
        dfs(1,0);
    }
    return 0;
}
```
