# 【string比较】#85 A. Petya and Strings - CD's Coding - CSDN博客





2014年11月29日 21:32:51[糖果天王](https://me.csdn.net/okcd00)阅读数：954标签：[Codeforces																[Beta Round #85																[Div. 2 Only](https://so.csdn.net/so/search/s.do?q=Div. 2 Only&t=blog)
个人分类：[CF-A题终结者](https://blog.csdn.net/okcd00/article/category/2156945)

所属专栏：[CF-A Terminator](https://blog.csdn.net/column/details/cf-amaster.html)](https://so.csdn.net/so/search/s.do?q=Beta Round #85&t=blog)













Little Petya loves presents. His mum bought him two strings of the same size for his birthday. The strings consist of uppercase and lowercase Latin letters. Now Petya wants to compare those two strings lexicographically.
 The letters' case does not matter, that is an uppercase letter is considered equivalent to the corresponding lowercase letter. Help Petya perform the comparison.




Input


Each of the first two lines contains a bought string. The strings' lengths range from 1 to 100 inclusive.
 It is guaranteed that the strings are of the same length and also consist of uppercase and lowercase Latin letters.




Output


If the first string is less than the second one, print "-1". If the second string is less than the first one, print "1". If the strings are equal, print "0". Note that the letters' case is not taken into consideration when the strings are compared.




Sample test(s)




input
aaaa
aaaA




output
0




input
abs
Abz




output
-1




input
abcdefg
AbCdEfF




output
1






Note


If you want more formal information about the lexicographical order (also known as the "dictionary order" or "alphabetical order"),
 you can visit the following site:
- http://en.wikipedia.org/wiki/Lexicographical_order
- 







这题是说，无视大小写的情况下比较两个字符串字典序的大小



大于输出1，小于是-1，相等是0.

刚好C++的STL里有string的逻辑运算符重载，直接==、>、<就可以比较了 （注意，string的比较似乎是和大小写相关的所以我们要把它变成全大写或者全小写）




Code：



```cpp
#include <cstdio>
#include <string>
#include <cstring> 
#include <iostream>
#include <algorithm>
using namespace std;
typedef long long ll;
// http://codeforces.com/contest/112
// Petya and Strings
int main()
{
	string a,b;	cin>>a>>b;
	for(int i=0;i<a.length();i++)
	{
		a[i]=tolower(a[i]);
		b[i]=tolower(b[i]);
	}
	if(a==b)	cout<<0;
	else if(a>b)cout<<1;
	else		cout<<-1;
	return 0;
}
```](https://so.csdn.net/so/search/s.do?q=Codeforces&t=blog)




