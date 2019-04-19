# POJ   1159  Palindrome（最长公共子序列） - HJ - CSDN博客
2016年08月11日 13:17:28[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：199
Description
A palindrome is a symmetrical string, that is, a string read identically from left to right as well as from right to left. You are to write a program which, given a string, determines the minimal number of characters to be inserted into the string in order to obtain a palindrome. 
As an example, by inserting 2 characters, the string “Ab3bd” can be transformed into a palindrome (“dAb3bAd” or “Adb3bdA”). However, inserting fewer than 2 characters does not produce a palindrome.  
Input
Your program is to read from standard input. The first line contains one integer: the length of the input string N, 3 <= N <= 5000. The second line contains one string with length N. The string is formed from uppercase letters from ‘A’ to ‘Z’, lowercase letters from ‘a’ to ‘z’ and digits from ‘0’ to ‘9’. Uppercase and lowercase letters are to be considered distinct. 
Output
Your program is to write to standard output. The first line contains one integer, which is the desired minimal number.
Sample Input 
5 
Ab3bd
Sample Output 
2
解析：设输入的为A串，A串的反串为B串，其实只要求出A串跟B串的最长公共子序列，那么再用A串的长度减去这个子序列长度，得到的即是需要改动最少字符数是的A串成为回文串，即此时A串跟B串相等。
代码如下：
```cpp
#include<iostream>
#include<algorithm> 
using namespace std;
int main()
{
    char a[5001],b[5001];
    int c[2][5001];
    int n,j=0;
    cin>>n>>a;
    for(int i=n-1;i>=0;i--)
      b[j++]=a[i];         //逆序赋值 
    for(int i=0;i<n;i++)
      c[0][i]=0;
    for(int j=0;j<n;j++)    
    {
        for(int i=0;i<n;i++)   
        {
            if(a[i]==b[j])           //首尾字符逐对逐对地比较，用dp方法求出a跟b的最长公共子序列 
              c[1][i+1]=c[0][i]+1;
            else
              c[1][i+1]=max(c[1][i],c[0][i+1]);
        }
        for(int i=1;i<=n;i++)
          c[0][i]=c[1][i];
    }
    cout<<n-c[1][n]<<endl;    //输出不属于最长公共子序列部分的字符个数 
    return 0;
}
```
