# （多校）hdu  6033  Add More Zero（数学） - HJ - CSDN博客
2017年08月01日 16:59:15[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：228
There is a youngster known for amateur propositions concerning several mathematical hard problems.
Nowadays, he is preparing a thought-provoking problem on a specific type of supercomputer which has ability to support calculations of integers between 0 and (2m−1) (inclusive).
As a young man born with ten fingers, he loves the powers of 10 so much, which results in his eccentricity that he always ranges integers he would like to use from 1 to 10k (inclusive).
For the sake of processing, all integers he would use possibly in this interesting problem ought to be as computable as this supercomputer could.
Given the positive integer m, your task is to determine maximum possible integer k 
    that is suitable for the specific supercomputer.  
Input 
    The input contains multiple test cases. Each test case in one line contains only one positive integer m, satisfying 1≤m≤105 
    . 
Output 
    For each test case, output ” Case #x: y” in one line (without quotes), where x indicates the case number starting from 1 and y 
    denotes the answer of corresponding case. 
Sample Input
```
1
64
```
Sample Output
```
Case #1: 0
Case #2: 19
```
题目大意： 
求出最大的k 使10^k>=2^m-1
解题思路：
直接取对数，用公式 k>=m*(log2/log10)
```cpp
#include<iostream>
#include<cstdio>
using namespace std;
int main()
{
    long long n,k,ans,tt=0;
    while(cin>>n>>k)
    {
        if(n>=k)
          ans=k;
        else
        {
            k-=n;
            int p=k/(n-1),q=k%(n-1);
            if(p%2)
              ans=q==0?n-1:q;
            else
              ans=q==0?n:q;
        }
        printf("Case #%lld: %lld\n",++tt,ans);
    }
    return 0;
}
```
