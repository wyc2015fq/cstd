# 【String】#65 A. Way Too Long Words - CD's Coding - CSDN博客





2014年10月10日 10:56:24[糖果天王](https://me.csdn.net/okcd00)阅读数：713
所属专栏：[CF-A Terminator](https://blog.csdn.net/column/details/cf-amaster.html)


















Sometimes some words like "localization" or "internationalization"
 are so long that writing them many times in one text is quite tiresome.


Let's consider a word too long, if its length is strictly more than 10 characters.
 All too long words should be replaced with a special abbreviation.


This abbreviation is made like this: we write down the first and the last letter of a word and between them we write the number of letters between the first and the last letters. That number is in decimal system and doesn't contain any leading zeroes.


Thus, "localization" will be spelt as "l10n", and "internationalization»
 will be spelt as "i18n".


You are suggested to automatize the process of changing the words with abbreviations. At that all too long words should be replaced by the abbreviation and the words that are not too long should not undergo any changes.




Input


The first line contains an integer *n* (1 ≤ *n* ≤ 100).
 Each of the following *n* lines contains one word. All the words consist of lowercase Latin letters and possess the lengths of from 1 to 100 characters.




Output


Print *n* lines. The *i*-th line should contain the result
 of replacing of the *i*-th word from the input data.




Sample test(s)




input
4
word
localization
internationalization
pneumonoultramicroscopicsilicovolcanoconiosis




output
word
l10n
i18n
p43s















这题我曾经做过一次，当时似乎写了还是有20几行的，现在会用string之后觉得真的相当容易呢。

string是有着 str.length()这样的函数可以调用的，所以我们可以很方便的获取字符串的长度，以及str[0]和str[str.length()-1]是头尾两个字符，那么这道题不就迎刃而解了吗~

需要注意的是，string是C++的库文件，cstring是C语言的哦，他们里面包含的函数是不同的呢~



```cpp
#include <cstdio>
#include <string>
#include <cstring> 
#include <iostream>
using namespace std;

int main()
{
	int n;	cin>>n;
	for(int i=0;i<n;i++)
	{
		string s;	cin>>s;
		if(s.length()>10)cout<<s[0]<<s.length()-2<<s[s.length()-1]<<endl;
		else cout<<s<<endl;
	}
	return 0;
}
```











