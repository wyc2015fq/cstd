# hdu  2710   Max Factor - HJ - CSDN博客
2016年08月02日 18:56:04[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：246
个人分类：[====ACM====																[---------OJ---------](https://blog.csdn.net/feizaoSYUACM/article/category/6308067)](https://blog.csdn.net/feizaoSYUACM/article/category/6308070)
hdu  2710   Max Factor
Problem Description 
To improve the organization of his farm, Farmer John labels each of his N (1 <= N <= 5,000) cows with a distinct serial number in the range 1..20,000. Unfortunately, he is unaware that the cows interpret some serial numbers as better than others. In particular, a cow whose serial number has the highest prime factor enjoys the highest social standing among all the other cows.
(Recall that a prime number is just a number that has no divisors except for 1 and itself. The number 7 is prime while the number 6, being divisible by 2 and 3, is not).
Given a set of N (1 <= N <= 5,000) serial numbers in the range 1..20,000, determine the one that has the largest prime factor.
Input 
* Line 1: A single integer, N
- Lines 2..N+1: The serial numbers to be tested, one per line
Output 
* Line 1: The integer with the largest prime factor. If there are more than one, output the one that appears earliest in the input file.
Sample Input 
4 
36 
38 
40 
42
Sample Output 
38
解析：用到唯一分解定理，注意在比较素因子大小的时候绑定赋值。
```cpp
#include<iostream>
using namespace std;
int main()
{
    int n,s[5005],t;
    while(cin>>n)
    {
      for(int i=0;i<n;i++)
        cin>>s[i];
      int max=1,count=1;
      for(int i=0;i<n;i++)
      {
          t=s[i];
            for(int j=2;j<=t;j++)
          {
              if(t%j==0)
              {
                  while(t%j==0)
                    t=t/j;
                  if(max<j)
                  {
                        max=j;
                      count=s[i];
                  }
              }
          }
      }
      cout<<count<<endl;
    }
    return 0;
}
```
