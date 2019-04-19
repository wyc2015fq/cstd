# hdu  3336 Count the string（next数组+dp） - HJ - CSDN博客
2017年01月19日 01:42:02[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：190
个人分类：[====ACM====																[---------OJ---------																[HDU																[--------字符串---------																[KMP](https://blog.csdn.net/feizaoSYUACM/article/category/7238008)](https://blog.csdn.net/feizaoSYUACM/article/category/7238090)](https://blog.csdn.net/feizaoSYUACM/article/category/6308041)](https://blog.csdn.net/feizaoSYUACM/article/category/6308067)](https://blog.csdn.net/feizaoSYUACM/article/category/6308070)
Count the string
Problem Description 
It is well known that AekdyCoin is good at string problems as well as number theory problems. When given a string s, we can write down all the non-empty prefixes of this string. For example: 
s: “abab” 
The prefixes are: “a”, “ab”, “aba”, “abab” 
For each prefix, we can count the times it matches in s. So we can see that prefix “a” matches twice, “ab” matches twice too, “aba” matches once, and “abab” matches once. Now you are asked to calculate the sum of the match times for all the prefixes. For “abab”, it is 2 + 2 + 1 + 1 = 6. 
The answer may be very large, so output the answer mod 10007.
Input 
The first line is a single integer T, indicating the number of test cases. 
For each case, the first line is an integer n (1 <= n <= 200000), which is the length of string s. A line follows giving the string s. The characters in the strings are all lower-case letters.
Output 
For each case, output only one number: the sum of the match times for all the prefixes of s mod 10007.
Sample Input
1 
4 
abab
Sample Output
6
Author 
foreverlin@HNU
题目大意： 
 给一个字符串，输出包括从1到len长 字符串前缀的总个数
解法：
先计算next数组，设dp[i]表示子串b[0~i]共含有以b[i]为结尾的前缀的数目，则以b[i]结尾的前缀数就是自己本身加上以b[p[i]]结尾的前缀数。
```cpp
#include<iostream>
using namespace std;
#define mod 10007
char s[200005];
int n,t,d[200005],nxt[200005];
void getNext()
{
    int i,j=-1,ans=1;
    d[0]=1;
    nxt[0]=-1;
    for(i=1;i<n;i++)
    {
        while(j>=0&&s[i]!=s[j+1])
          j=nxt[j];
        nxt[i]=(s[i]==s[j+1])?j++:j; 
        if(j>=0)  d[i]=(d[j]+1)%mod;    //如果nxt数组j不为负 dp当前状态d[i]决策累加d[j]的值 
        else  d[i]=1;            //否则加一即可 
        ans=(ans+d[i])%mod;
    }
    cout<<ans<<endl;
}
int main()
{
    cin>>t;
    while(t--)
    {
        cin>>n>>s;
        getNext();
    }
    return 0;
}
```
