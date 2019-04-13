
# ACM最长公共子序列问题（动态规划）C++实现 - 我和我追逐的梦~~~ - CSDN博客


置顶2011年11月02日 23:49:31[一只鸟的天空](https://me.csdn.net/heyongluoyao8)阅读数：3874


// 最长公共子序列问题.cpp : Defines the entry point for the console application.
//动态规划问题
对于X=x1x2...xm ， Y=y1y2...yn的最长公共子序列Z=z1z2...zk
1)、如果xm=yn，那么zk=xm=yn，且Zk-1是Xm-1和Yn-1的一个LCS
2)、如果xm！=yn，那么zk！=xm蕴含Z是Xm-1和Y的一个LCS
3)、如果xm！=yn，那么zk！=yn蕴含Z是X和Yn-1的一个LCS
最长公共子序列的长度
![](http://hi.csdn.net/attachment/201111/3/0_1320288836dMV1.gif)
\#include "stdafx.h"
\#include<iostream>
\#define N 1000
using namespace std;
//str1存储字符串1，str2存储字符串2
char str1[N],str2[N];
//lcs存储最长公共子序列
char lcs[N];
//c[i][j]存储str1[1...i]与str2[1...j]的最长公共子序列的长度
int c[N][N];
//flag[i][j]标记是那种子问题
//flag[i][j]==0为str1[i]==str2[j]
//flag[i][j]==1为c[i-1][j]>=s[i][j-1]
//flag[i][j]==-1为c[i-1][j]<s[i][j-1]
int flag[N][N];
//
int getLCSlength(const char *s1, const char *s2)
{
int i;
int len1 = strlen(s1);
int len2 = strlen(s2);
for(i=1;i<=len1;i++)
c[i][0] = 0;
for(i=0;i<=len2;i++)
c[0][i] = 0;
int j;
for(i=1;i<=len1;i++)
for(j=1;j<=len2;j++)
{
if(s1[i-1]==s2[j-1])
{
c[i][j] = c[i-1][j-1] +1;
flag[i][j] = 0;
}
else if(c[i-1][j]>=c[i][j-1])
{
c[i][j] = c[i-1][j];
flag[i][j] = 1;
}
else
{
c[i][j] = c[i][j-1];
flag[i][j] = -1;
}
}
return c[len1][len2];
}
char* getLCS(const char *s1, const char *s2,int len,char *lcs)
{
int i = strlen(s1);
int j = strlen(s2);
while(i&&j)
{
if(flag[i][j]==0)
{
lcs[--len] = s1[i-1];
i--;
j--;
}
else if(flag[i][j]==1)
i--;
else
j--;
}
return lcs;
}
int _tmain(int argc, _TCHAR* argv[])
{
int cases;
cout<<"请输入案例的个数："<<endl;
cin>>cases;
while(cases--)
{
int i;
cout<<"请输入字符串1："<<endl;
cin>>str1;
cout<<"请输入字符串2："<<endl;
cin>>str2;
int lcsLen = getLCSlength(str1,str2);
cout<<"最长公共子序列长度："<<lcsLen<<endl;
char *p = getLCS(str1,str2,lcsLen,lcs);
cout<<"最长公共子序列为："<<endl;
for(i=0;i<lcsLen;i++)
cout<<lcs[i];
cout<<endl;
}
system("pause");
return 0;
}

请输入案例的个数：
1
请输入字符串1：
abcbdab
请输入字符串2：
bdcaba
最长公共子序列长度：4
最长公共子序列为：
bcba
请按任意键继续. . .

