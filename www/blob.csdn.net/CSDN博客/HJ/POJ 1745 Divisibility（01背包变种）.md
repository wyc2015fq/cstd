# POJ 1745 Divisibility（01背包变种） - HJ - CSDN博客
2017年08月02日 13:14:41[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：466
Description 
Consider an arbitrary sequence of integers. One can place + or - operators between integers in the sequence, thus deriving different arithmetical expressions that evaluate to different values. Let us, for example, take the sequence: 17, 5, -21, 15. There are eight possible expressions: 17 + 5 + -21 + 15 = 16 
17 + 5 + -21 - 15 = -14 
17 + 5 - -21 + 15 = 58 
17 + 5 - -21 - 15 = 28 
17 - 5 + -21 + 15 = 6 
17 - 5 + -21 - 15 = -24 
17 - 5 - -21 + 15 = 48 
17 - 5 - -21 - 15 = 18 
We call the sequence of integers divisible by K if + or - operators can be placed between integers in the sequence in such way that resulting value is divisible by K. In the above example, the sequence is divisible by 7 (17+5+-21-15=-14) but is not divisible by 5.
You are to write a program that will determine divisibility of sequence of integers.
Input 
The first line of the input file contains two integers, N and K (1 <= N <= 10000, 2 <= K <= 100) separated by a space. 
The second line contains a sequence of N integers separated by spaces. Each integer is not greater than 10000 by it’s absolute value.
Output 
Write to the output file the word “Divisible” if given sequence of integers is divisible by K or “Not divisible” if it’s not.
Sample Input
4 7 
17 5 -21 15
Sample Output
Divisible
题目大意： 
给出n个数，要求n个数之间任意选择加减，是否存在一个最后的结果能被k整除？
解题思路： 
设 dp[i][j] 表示前i个数任意选择加减，得到的值除k的余数:
1)如果余数为j   dp[i][j]=1 
2)如果余数不为j  dp[i][j]=0
最后只要判断dp[n-1][0]的值是否为1即可
```
#include<iostream>
#include<cmath>
#include<cstring>
using namespace std;
int s[10005],dp[10005][105];           //dp[i][j]表示前i个数字运算之后余数是否为j  值为1表示余数为j 值为0表示余数不为j 
int main()
{
    int n,k;
    while(cin>>n>>k)
    {
        for(int i=0;i<n;i++)
        {
            cin>>s[i];
            s[i]=abs(s[i])%k;
        }
        memset(dp,0,sizeof(dp));
        dp[0][s[0]]=1;
        for(int i=1;i<n;i++)
          for(int j=k-1;j>=0;j--)
            if(dp[i-1][j])
            {
                 dp[i][(j+s[i])%k]=1;
                 dp[i][((j-s[i])%k+k)%k]=1;
            }
        if(dp[n-1][0])
          cout<<"Divisible"<<endl;
        else
          cout<<"Not divisible"<<endl;
    }
    return 0;
}
```
