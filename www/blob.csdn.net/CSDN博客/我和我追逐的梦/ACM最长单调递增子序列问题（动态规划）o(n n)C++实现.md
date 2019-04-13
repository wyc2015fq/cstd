
# ACM最长单调递增子序列问题（动态规划）o(n-n)C++实现 - 我和我追逐的梦~~~ - CSDN博客


置顶2011年11月03日 14:57:03[一只鸟的天空](https://me.csdn.net/heyongluoyao8)阅读数：3190标签：[c++																](https://so.csdn.net/so/search/s.do?q=c++&t=blog)[存储																](https://so.csdn.net/so/search/s.do?q=存储&t=blog)[system																](https://so.csdn.net/so/search/s.do?q=system&t=blog)[测试																](https://so.csdn.net/so/search/s.do?q=测试&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=system&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=存储&t=blog)个人分类：[C、C++开发																](https://blog.csdn.net/heyongluoyao8/article/category/908444)[算法设计与分析																](https://blog.csdn.net/heyongluoyao8/article/category/961150)[
							](https://blog.csdn.net/heyongluoyao8/article/category/908444)
[
				](https://so.csdn.net/so/search/s.do?q=存储&t=blog)
[
			](https://so.csdn.net/so/search/s.do?q=存储&t=blog)
[
		](https://so.csdn.net/so/search/s.do?q=c++&t=blog)

最长单调递增子序列问题（动态规划）o(![](http://hi.csdn.net/attachment/201111/3/0_1320303414Zg61.gif))
// 最长单调递增子序列.cpp : Defines the entry point for the console application.
//
\#include "stdafx.h"
\#include<iostream>
\#define N 100
using namespace std;
//递归打印最长递增子序列
void print_it(char *str,int *pre,int last)
{
if(last==-1)
return;
else
{
print_it(str,pre,pre[last]);
cout<<str[last];
}
}
int _tmain(int argc, _TCHAR* argv[])
{
//存储原字符串
char str[N];
//len[j]存储到str[j]为止的最长递增子序列长度
int len[N];
//pre[j]存储到str[j]为止的最长递增子序列的前一个字符的位置
int pre[N];
int cases;
cout<<"请输入案例个数："<<endl;
cin>>cases;
while(cases--)
{
cout<<"请输入字符串："<<endl;
cin>>str;
//初始化各变量
int i;
int length = strlen(str);
for(i=0;i<length;i++)
{
len[i] = 1;
pre[i] = -1;
}
int j;
int max = 0;
for(j=1;j<length;j++)
{
for(i=0;i<j;i++)
{
max = 0;
if(str[j]>str[i])
{
max = len[i]+1;
if(max>len[j])
{
len[j] = max;
pre[j] = i;
}
}
}
}
//找最大的len[i]
//last为最长的递增子序列的最后一个字符的位置
max = 0;
int last;
for(i=0;i<length;i++)
{
if(len[i]>max)
{
max = len[i];
last = i;
}
}
cout<<"最长递增子序列的长度为："<<max<<endl;
cout<<"最长递增子序列为："<<endl;
print_it(str,pre,last);
cout<<endl;
}
system("pause");
return 0;
}
-----------------------------------------------程序测试----------------------------------------
请输入案例个数：
2
请输入字符串：
abceabf
最长递增子序列的长度为：5
最长递增子序列为：
abcef
请输入字符串：
abcabcdef
最长递增子序列的长度为：6
最长递增子序列为：
abcdef
请按任意键继续. . .

