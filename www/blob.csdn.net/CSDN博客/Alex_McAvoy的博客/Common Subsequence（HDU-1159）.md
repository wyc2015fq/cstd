# Common Subsequence（HDU-1159） - Alex_McAvoy的博客 - CSDN博客





2018年03月26日 20:37:04[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：71
个人分类：[HDU																[动态规划——线性 DP](https://blog.csdn.net/u011815404/article/category/7813170)](https://blog.csdn.net/u011815404/article/category/7923257)








# Problem Description

    A subsequence of a given sequence is the given sequence with some elements (possible none) left out. Given a sequence X = <x1, x2, ..., xm>,another sequence Z = <z1, z2, ..., zk> is a subsequence of X if there exists a strictly increasing sequence <i1, i2, ..., ik> of indices of X such that for all j = 1,2,...,k, xij = zj. 

    For example, Z = <a, b, f, c> is a subsequence of X = <a, b, c, f, b, c> with index sequence <1, 2, 4, 6>. Given two sequences X and Y the problem is to find the length of the maximum-length common subsequence of X and Y. 

    The program input is from a text file. Each data set in the file contains two strings representing the given sequences. The sequences are separated by any number of white spaces. The input data are correct. For each set of data the program prints on the standard output the length of the maximum-length common subsequence from the beginning of a separate line. 

# Sample Input

**abcfbc abfcab**

**programming contest abcd mnp**

# Sample Output

**420**

————————————————————————————————————————————

思路：LCS问题经典模版

# Source Program

```cpp
#include<iostream>
#include<cstring>
#include<string>
using namespace std;
int dp[1001][1001];
int main()
{
    string str1,str2;
    int len1,len2;
    int i,j;

    while(cin>>str1>>str2)
    {
        len1=str1.length();
        len2=str2.length();

        for(i=1;i<=len1;i++)
        {
            for(j=1;j<=len2;j++)
            {
                if(str1[i-1]==str2[j-1])
                    dp[i][j]=dp[i-1][j-1]+1;
                else
                    dp[i][j]=max(dp[i][j-1],dp[i-1][j]);


            }
        }
        cout<<dp[len1][len2]<<endl;
    }
    return 0;
}
```






