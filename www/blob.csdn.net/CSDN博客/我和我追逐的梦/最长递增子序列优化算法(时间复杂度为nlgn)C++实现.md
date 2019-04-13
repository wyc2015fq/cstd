
# 最长递增子序列优化算法(时间复杂度为nlgn)C++实现 - 我和我追逐的梦~~~ - CSDN博客


置顶2011年11月03日 16:17:37[一只鸟的天空](https://me.csdn.net/heyongluoyao8)阅读数：3458


最长递增子序列优化算法(时间复杂度为nlgn)
// 最长递增子序列优化算法.cpp : Defines the entry point for the console application.
//对于j位置的字符，我们需要寻找此位置之前的最大的len[i](i<j),然而len[i]是无序的
//如果len[i]有序，则可通过二分查找快速找到
//于是我们定义一个s[]数组，s[len[i]]存储str[i]，表示长度为len[i]的递增子序列尾元素为str[i]。
//对于str的位置为j的元素，在s[]数组中进行二分查找，如果str[j]>s[mid]，则查找后半部分的s[],直到下界超过上界，
//如果str[j]<=s[mid]，则查找前半部分的s[],直到上界低于下界。
//如果下界大于当前最大长度maxLen，则更新maxLen
//时间复杂度为nlgn
\#include "stdafx.h"
\#include<iostream>
\#define N 100
using namespace std;
int _tmain(int argc, _TCHAR* argv[])
{
//存储原字符串
char str[N];
//b[j]存储以j为长度的递增子序列的结尾元素
char b[N];
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
b[0] = '0'; //b[0]为最小，假设输入的字符全部是字母
b[1] = str[0];//以1为长度的递增子序列的结尾元素都是str[0]
int first,mid,end;//分别为二分查找的首，中，尾位置
int maxLen = 1; //为目前递增子序列最大长度
for(i=1;i<length;i++)
{
first = 0, end = maxLen;
while(first<=end)
{
mid = (first+end)/2;
if(b[mid]<str[i])
first = mid +1;
else
end = mid -1;
}
b[first] = str[i];
if(first>maxLen) maxLen++;
}
cout<<"最长递增子序列的长度为："<<maxLen<<endl;
cout<<"最长递增子序列为："<<endl;
for(i=1;i<=maxLen;i++)
cout<<b[i];
cout<<endl;
}
system("pause");
return 0;
}
-------------------------------------------------程序测试---------------------------------------------------------
请输入案例个数：
2
请输入字符串：
abaceabf
最长递增子序列的长度为：5
最长递增子序列为：
abcef
请输入字符串：
abcabcdef
最长递增子序列的长度为：6
最长递增子序列为：
abcdef
请按任意键继续. . .

