# Codeforces-804 B. Minimum number of steps（贪心+规律） - HJ - CSDN博客
2017年05月05日 14:54:03[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：445
所属专栏：[codeforces](https://blog.csdn.net/column/details/17151.html)
We have a string of letters ‘a’ and ‘b’. We want to perform some operations on it. On each step we choose one of substrings “ab” in the string and replace it with the string “bba”. If we have no “ab” as a substring, our job is done. Print the minimum number of steps we should perform to make our job done modulo 109 + 7.
The string “ab” appears as a substring if there is a letter ‘b’ right after the letter ‘a’ somewhere in the string. 
Input
The first line contains the initial string consisting of letters ‘a’ and ‘b’ only with length from 1 to 106. 
Output
Print the minimum number of steps modulo 109 + 7. 
Examples 
Input
ab
Output
1
Input
aab
Output
3
Note
The first example: “ab”  →  “bba”.
The second example: “aab”  →  “abba”  →  “bbaba”  →  “bbbbaa”.
解题思路：
可以这样想，如果题目要求是把“ab”转换成“ba”的话，就相当于把所有的b移到前面去，把a移到后面去。但是现在在转换的过程中增添了一个‘b’，因此在逆序的时候，每次遇到一个“b”就把累积的的变量加1，如果遇到”a”，就把累积变量累加的到总和里，并且把累积变量乘以2，因为每次转换都会把a移动到后面，而前面留下了两个b，对前面的a造成影响。
```
#include<iostream>
#include<cstring>
using namespace std;
#define mod 1000000007
char a[1000005];
int main()
{
    while(cin>>a)
    {
        int len=strlen(a),ans=0,sum=0;
        for(int i=len-1;i>=0;i--)
        {
            if(a[i]=='b')
              sum=(sum+1)%mod;
            else
            {
                ans=(ans+sum)%mod;
                sum=(sum*2)%mod;
            }
        }
        cout<<ans<<endl;
    }
    return 0;
}
```
