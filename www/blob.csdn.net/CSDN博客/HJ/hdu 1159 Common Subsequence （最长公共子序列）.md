# hdu  1159  Common Subsequence （最长公共子序列） - HJ - CSDN博客
2016年08月11日 13:06:30[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：192
个人分类：[====ACM====																[---------OJ---------																[HDU																[--------动态规划---------																[基础DP](https://blog.csdn.net/feizaoSYUACM/article/category/7238027)](https://blog.csdn.net/feizaoSYUACM/article/category/7238093)](https://blog.csdn.net/feizaoSYUACM/article/category/6308041)](https://blog.csdn.net/feizaoSYUACM/article/category/6308067)](https://blog.csdn.net/feizaoSYUACM/article/category/6308070)
Problem Description 
A subsequence of a given sequence is the given sequence with some elements (possible none) left out. Given a sequence X = 《x1, x2, …, xm》 another sequence Z = 《z1, z2, …, zk》 is a subsequence of X if there exists a strictly increasing sequence 《i1, i2, …, ik》 of indices of X such that for all j = 1,2,…,k, xij = zj. For example, Z = 《a, b, f, c》 is a subsequence of X = 《a, b, c, f, b, c》 with index sequence 《1, 2, 4, 6》. Given two sequences X and Y the problem is to find the length of the maximum-length common subsequence of X and Y.  
The program input is from a text file. Each data set in the file contains two strings representing the given sequences. The sequences are separated by any number of white spaces. The input data are correct. For each set of data the program prints on the standard output the length of the maximum-length common subsequence from the beginning of a separate line. 
Sample Input 
abcfbc abfcab 
programming contest  
abcd mnp
Sample Output 
4 
2 
0
经典题！
求两个串中的最长公共子序列，
```
d[i][j]表示串A的第i位跟串B的第j位之前共有的公共子序列的值
```
开两个循环，对串A串B中的每个字符进行逐个比较
如果存在a[i]==b[j]
```
d[i+1][j+1]=d[i][j]+1;
```
否则
```
d[i+1][j+1]=max(d[i][j+1],d[i+1][j]);
```
如果不懂可以自己好好模拟一下，代码如下：
```cpp
#include<iostream>
#include<cstdio>
#include<cstring>
#include<algorithm>
using namespace std;
char a[1005],b[1005];
int d[1005][1005];
int main()
{
    while(cin>>a>>b)            
    { 
        int len1,len2;       //先计算好两个子串的长度 
        len1=strlen(a);     
        len2=strlen(b);
        memset(d,0,sizeof(d));      //注意因为在输入的时候串是从下标0开始的，因此数组d在写状态跟转移方程的时候自动往后面加一位 
        for(int i=0;i<len1;i++)
          for(int j=0;j<len2;j++)
          {
              if(a[i]==b[j])            //如果两个串中存在相同字符 
                d[i+1][j+1]=d[i][j]+1;
              else                      //如果两个串中的字符不相同 
                d[i+1][j+1]=max(d[i][j+1],d[i+1][j]);      
          } 
        cout<<d[len1][len2]<<endl;
    }
    return 0;
}
```
