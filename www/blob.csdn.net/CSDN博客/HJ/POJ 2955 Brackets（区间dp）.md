# POJ 2955 Brackets（区间dp） - HJ - CSDN博客
2017年03月14日 23:18:46[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：275
个人分类：[====ACM====																[---------OJ---------																[POJ																[区间DP																[--------动态规划---------](https://blog.csdn.net/feizaoSYUACM/article/category/7238093)](https://blog.csdn.net/feizaoSYUACM/article/category/7238029)](https://blog.csdn.net/feizaoSYUACM/article/category/6308043)](https://blog.csdn.net/feizaoSYUACM/article/category/6308067)](https://blog.csdn.net/feizaoSYUACM/article/category/6308070)
Description
We give the following inductive definition of a “regular brackets” sequence:
```
the empty sequence is a regular brackets sequence,
if s is a regular brackets sequence, then (s) and [s] are regular brackets sequences, and
if a and b are regular brackets sequences, then ab is a regular brackets sequence.
no other sequence is a regular brackets sequence
```
For instance, all of the following character sequences are regular brackets sequences:
```
(), [], (()), ()[], ()[()]
```
while the following character sequences are not:
```
(, ], )(, ([)], ([(]
```
Given a brackets sequence of characters a1a2 … an, your goal is to find the length of the longest regular brackets sequence that is a subsequence of s. That is, you wish to find the largest m such that for indices i1, i2, …, im where 1 ≤ i1 < i2 < … < im ≤ n, ai1ai2 … aim is a regular brackets sequence.
Given the initial sequence ([([]])], the longest regular brackets subsequence is [([])].
Input
The input test file will contain multiple test cases. Each input test case consists of a single line containing only the characters (, ), [, and ]; each input test will have length between 1 and 100, inclusive. The end-of-file is marked by a line containing the word “end” and should not be processed.
Output
For each input case, the program should print the length of the longest possible regular brackets subsequence on a single line.
Sample Input
((())) 
()()() 
([]]) 
)[)( 
([][][) 
end
Sample Output
6 
6 
4 
0 
6
题目大意：
求出满足括号匹配的最长个数
解题思路：
设d[i][j]表示[i,j]满足括号匹配的最长个数，则有
(1)当s[i]与s[j]匹配：  
```
d[i][j]=d[i-1][j-1]+2;
```
(2)当s[i]与s[j]不匹配：
```
d[i][j]=max(d[i][k]+d[k+1][j] (k在i+1与j之间）
```
```cpp
#include<iostream>
#include<cstdio>
#include<cstring>
using namespace std;
char s[105];
int d[105][105];
int main()
{
    int n;
    cin>>n;
    getchar(); 
    while(n--)
    {
        gets(s);
        memset(d,0,sizeof(d));
        int len=strlen(s);
        for(int k=1;k<len;k++)
        {
            for(int i=0,j=k;j<len;i++,j++)
            {
                if(s[i]=='('&&s[j]==')'||s[i]=='['&&s[j]==']')
                   d[i][j]=d[i+1][j-1]+2;
                for(int x=i;x<j;x++)
                  d[i][j]=max(d[i][j],d[i][x]+d[x+1][j]);
            }
        }
        cout<<len-d[0][len-1]<<endl;
    }
    return 0;
}
```
