# POJ  2739 Sum of Consecutive Prime Numbers（尺取） - HJ - CSDN博客
2017年03月21日 23:39:36[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：291
Description 
Some positive integers can be represented by a sum of one or more consecutive prime numbers. How many such representations does a given positive integer have? For example, the integer 53 has two representations 5 + 7 + 11 + 13 + 17 and 53. The integer 41 has three representations 2+3+5+7+11+13, 11+13+17, and 41. The integer 3 has only one representation, which is 3. The integer 20 has no such representations. Note that summands must be consecutive prime 
numbers, so neither 7 + 13 nor 3 + 5 + 5 + 7 is a valid representation for the integer 20. 
Your mission is to write a program that reports the number of representations for the given positive integer.
Input 
The input is a sequence of positive integers each in a separate line. The integers are between 2 and 10 000, inclusive. The end of the input is indicated by a zero.
Output 
The output should be composed of lines each corresponding to an input line except the last zero. An output line includes the number of representations for the input integer as the sum of one or more consecutive prime numbers. No other characters should be inserted in the output.
Sample Input
2 
3 
17 
41 
20 
666 
12 
53 
0
Sample Output
1 
1 
2 
3 
0 
0 
1 
2
题目大意：
给你一个数，询问有多少个连续质数序列和等于该数例如53=5 + 7 + 11 + 13 + 17 
解题方法： 
筛法求质数，然后直接twopoint就可以了，统计可以相等的个数。用尺取法解决。
```cpp
#include<iostream>
#include<cstring>
using namespace std;
int p[100010],n;
void Init()
{
    int k=0;
    memset(p,0,sizeof(p));
    for(int i=2;i<=10020;i++)
    {
        if(p[i]==0)
          for(int j=i*i;j<=10020;j+=i)
            p[j]=1;
    }
    for(int i=2;i<=10020;i++)
      if(p[i]==0)
        p[k++]=i;
    p[k]='\0';
}
int main()
{
    Init();
    while(cin>>n&&n)
    {
        int l=0,r=0,ans=0,m=0,sum=0;
        for(;p[m]!='\0';m++)
          if(p[m]>n)
            break;
        while(l<m)
        {
            while(r<m&&sum<n)
              sum+=p[r++];
            if(sum<n)
              break;
            if(sum==n)
              ans++;
            sum-=p[l++];
        }
        cout<<ans<<endl;
    }
    return 0;
}
```
