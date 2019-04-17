# 【水字符】#55 A. Word - CD's Coding - CSDN博客





2014年08月08日 21:23:10[糖果天王](https://me.csdn.net/okcd00)阅读数：740
所属专栏：[CF-A Terminator](https://blog.csdn.net/column/details/cf-amaster.html)


















Vasya is very upset that many people on the Net mix uppercase and lowercase letters in one word. That's why he decided to invent an extension for his favorite browser that would change the letters' register in every word so that it either only consisted of
 lowercase letters or, vice versa, only of uppercase ones. At that as little as possible letters should be changed in the word. For example, the word HoUse must
 be replaced with house, and the word ViP — with VIP.
 If a word contains an equal number of uppercase and lowercase letters, you should replace all the letters with lowercase ones. For example, maTRIx should be
 replaced by matrix. Your task is to use the given method on one given word.




Input


The first line contains a word *s* — it consists of uppercase and lowercase Latin letters and possesses the length from 1 to 100.




Output


Print the corrected word *s*. If the given word *s* has
 strictly more uppercase letters, make the word written in the uppercase register, otherwise - in the lowercase one.




Sample test(s)




input
HoUse




output
house




input
ViP




output
VIP




input
maTRIx




output
matrix















如果大写字符多就把字符串全大写，小写字符多或者大小写一样多就把字符串全都变小写

for一次来遍历数数，

再for一次输出即可。

Code：



```cpp
#include <cstdio>
#include <memory>
#include <string>
#include <cstring>
#include <iostream>
#include <algorithm>
using namespace std;

int main()
{
	int u=0,l=0;
	string s;	cin>>s;
	for(int i=0;i<s.length();i++)
		if(isupper(s[i])) u++;
		else l++;
	for(int i=0;i<s.length();i++)
		printf( "%c",((u>l) ? toupper(s[i]):tolower(s[i]) ) );
	return 0;
}
```











