# codeforces-762A k-th divisor（数学题） - HJ - CSDN博客
2017年01月26日 00:50:18[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：861
所属专栏：[codeforces](https://blog.csdn.net/column/details/17151.html)
You are given two integers n and k. Find k-th smallest divisor of n, or report that it doesn’t exist.
Divisor of n is any such natural number, that n can be divided by it without remainder. 
Input
The first line contains two integers n and k (1 ≤ n ≤ 1015, 1 ≤ k ≤ 109). 
Output
If n has less than k divisors, output -1.
Otherwise, output the k-th smallest divisor of n. 
Examples 
Input
4 2
Output
2
Input
5 3
Output
-1
Input
12 5
Output
6
Note
In the first example, number 4 has three divisors: 1, 2 and 4. The second one is 2.
In the second example, number 5 has only two divisors: 1 and 5. The third divisor doesn’t exist, so the answer is -1.
题目大意： 
  给出一个数n，求出这个数的第k个因子，如果不存在输出-1.
解题思路： 
    因子都是成对存在，因此只要求出sqrt（n）之内的因子。需要考虑两点： 
    （1）k在前sqrt（n）中和在后sqrt( n）是不一样处理的； 
    （2）n是否为平方数； 
详细请看代码：
```cpp
#include<iostream>
#include<cmath>
using namespace std;
long long n,k;
int main()
{
    while(cin>>n>>k)
    {
        long long blag=0,s=sqrt(n),q;
        long long i,t=0,ans=-1,p=0;     
        if(s*s==n) blag=1;    //如果n为平方数 
        for(i=1;i*i<n;i++)
        {           
            if(n%i==0)
            {
                t++;
                if(k==t)     
                  ans=i;    //如果k在sqrt(n)前半部分，就可以提前求出来 
            }
        }
        if(ans==-1)
        {
            if(blag&&k==t+1)   //否则判断是否刚好在平方根那个位置 
              ans=sqrt(n);
            else
            {
                if(blag)
                  q=2*t+1;    //扩大sqrt(n)后半部分 
                else 
                  q=2*t;
                if(k<=q&&k>t)
                {
                    k=q-k+1;     //k从后边开始查找 
                    for(int i=1;i*i<n;i++)
                    {
                        if(n%i==0)
                          p++;
                        if(k==p)
                        {
                            ans=n/i;    //因子都是成对的，前后具有对称性 
                            break;
                        }
                    }
                }
            }
        }
        cout<<ans<<endl;
    }
    return 0;
}
```
