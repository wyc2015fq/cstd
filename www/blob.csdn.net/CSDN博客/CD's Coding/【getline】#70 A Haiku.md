# 【getline】#70 A. Haiku - CD's Coding - CSDN博客





2014年10月27日 12:26:25[糖果天王](https://me.csdn.net/okcd00)阅读数：630标签：[Codeforces																[Beta Round #70																[Haiku](https://so.csdn.net/so/search/s.do?q=Haiku&t=blog)
个人分类：[CF-A题终结者](https://blog.csdn.net/okcd00/article/category/2156945)

所属专栏：[CF-A Terminator](https://blog.csdn.net/column/details/cf-amaster.html)](https://so.csdn.net/so/search/s.do?q=Beta Round #70&t=blog)













Haiku is a genre of Japanese traditional poetry.


A haiku poem consists of 17 syllables split into three phrases, containing 5, 7 and 5 syllables correspondingly (the first phrase should contain exactly 5 syllables, the second phrase should contain exactly 7 syllables, and the third phrase should contain exactly
 5 syllables). A haiku masterpiece contains a description of a moment in those three phrases. Every word is important in a small poem, which is why haiku are rich with symbols. Each word has a special meaning, a special role. The main principle of haiku is
 to say much using a few words.


To simplify the matter, in the given problem we will consider that the number of syllable in the phrase is equal to the number of vowel letters there. Only the following letters are regarded as vowel letters: "a",
 "e", "i", "o"
 and "u".


Three phases from a certain poem are given. Determine whether it is haiku or not.




Input


The input data consists of three lines. The length of each line is between 1 and 100,
 inclusive. The *i*-th line contains the *i*-th
 phrase of the poem. Each phrase consists of one or more words, which are separated by one or more spaces. A word is a non-empty sequence of lowercase Latin letters. Leading and/or trailing spaces in phrases are allowed. Every phrase has at least one non-space
 character. See the example for clarification.




Output


Print "YES" (without the quotes) if the poem is a haiku. Otherwise, print "NO"
 (also without the quotes).




Sample test(s)




input
on  codeforces 
beta round is running
   a rustling of keys 




output
YES



input
how many gallons
of edo s rain did you drink
                                cuckoo




output
NO














给三句话问这是不是俳句~

啊啦，做题目还可以学知识，妙哉妙哉~

俳句是[日本](http://baike.baidu.com/view/1554.htm)的一种古典短诗，由“五-七-五”，共十七字音组成，要求严格，受“季语”的限制。


不过这里我们的题目被简化了，问三句里的元音字母是否是5/7/5的%……（嫌弃脸）

好吧，遍历一遍数数字拿分走人

这里因为每行中间是有空格的，而string的cin每次都会遇到空格结束，我们想一下读入一行怎么办呢？

getline()函数—— getline(cin,s)就是在控制台录入单行赋值给s，这里cin是可以换成fin啥的哦，因为有的时候我们还需要文件读入呢~



```cpp
#include <cstdio>
#include <string>
#include <cstring> 
#include <iostream>
#include <algorithm>
using namespace std;
typedef long long ll;
// http://codeforces.com/contest/78 
// Haiku

int judge(string s,int n)
{
	int cnt=7-n%2*2;
	for(int i=0;i<s.length();i++)
	{
		if(s[i]=='a'||s[i]=='e'||s[i]=='i'||s[i]=='o'||s[i]=='u')
		cnt--;
	}
	if(!cnt)return n;
	else return 0;
}

int main()
{
	string s;
	int flag=0; 
	for(int i=1;i<=3;i++)
	{ getline(cin,s); flag+=judge(s,i);}
	if(flag==6)cout<<"YES";else cout<<"NO";
	return 0;
}
```](https://so.csdn.net/so/search/s.do?q=Codeforces&t=blog)




