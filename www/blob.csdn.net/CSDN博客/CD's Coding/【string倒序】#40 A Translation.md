# 【string倒序】#40 A. Translation - CD's Coding - CSDN博客





2014年07月28日 14:46:11[糖果天王](https://me.csdn.net/okcd00)阅读数：990
所属专栏：[CF-A Terminator](https://blog.csdn.net/column/details/cf-amaster.html)


















The translation from the Berland language into the Birland language is not an easy task. Those languages are very similar: a berlandish word differs from a birlandish word with the same meaning a little: it is spelled (and pronounced) reversely. For example,
 a Berlandish word code corresponds to a Birlandish word edoc.
 However, it's easy to make a mistake during the «translation». Vasya translated word *s* from Berlandish into Birlandish as *t*.
 Help him: find out if he translated the word correctly.




Input


The first line contains word *s*, the second line contains word *t*.
 The words consist of lowercase Latin letters. The input data do not consist unnecessary spaces. The words are not empty and their lengths do not exceed 100 symbols.




Output


If the word *t* is a word *s*, written reversely, print YES,
 otherwise print NO.




Sample test(s)




input
code
edoc




output
YES




input
abb
aba




output
NO




input
code
code




output
NO


















这题就是问输入的两个字符串是否为倒序。

判断1）长度是否相等 2）a第i位和b的第a.length()-i位是否一致即可

Code：



```cpp
#include <queue>
#include <cmath> 
#include <memory> 
#include <cctype>
#include <cstdio>
#include <string>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <algorithm>
using namespace std;

int main()
{
	string a,b;
	cin>>a>>b;
	if(a.length()==b.length())
	{
		int i=0,flag=0;
		for(i=0;i<a.length();i++)
		{
			if(a[i]!=b[a.length()-(i+1)])
			{
				flag=1;
				break;
			}
		}
		if(!flag)
		{
			cout<<"YES"<<endl;
			return 0;	
		}	
	}	
	cout<<"NO"<<endl;
	return 0;
}
```










﻿﻿



