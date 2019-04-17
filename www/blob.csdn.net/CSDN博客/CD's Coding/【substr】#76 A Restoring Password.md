# 【substr】#76 A. Restoring Password - CD's Coding - CSDN博客





2014年11月15日 21:36:43[糖果天王](https://me.csdn.net/okcd00)阅读数：668标签：[Codeforces																[Beta Round #76																[Div. 2 Only																[Restoring Password																[substr](https://so.csdn.net/so/search/s.do?q=substr&t=blog)
个人分类：[CF-A题终结者](https://blog.csdn.net/okcd00/article/category/2156945)

所属专栏：[CF-A Terminator](https://blog.csdn.net/column/details/cf-amaster.html)](https://so.csdn.net/so/search/s.do?q=Restoring Password&t=blog)













Igor K. always used to trust his favorite Kashpirovsky Antivirus. That is why he didn't hesitate to download the link one of his groupmates sent him via QIP Infinium. The link was said to contain "some real funny stuff about swine influenza". The antivirus
 had no objections and Igor K. run the flash application he had downloaded. Immediately his QIP Infinium said: "invalid login/password".


Igor K. entered the ISQ from his additional account and looked at the info of his main one. His name and surname changed to "H1N1" and "Infected" correspondingly, and the "Additional Information" field contained a strange-looking binary code 80 characters
 in length, consisting of zeroes and ones. "I've been hacked" — thought Igor K. and run the Internet Exploiter browser to quickly type his favourite search engine's address.


Soon he learned that it really was a virus that changed ISQ users' passwords. Fortunately, he soon found out that the binary code was actually the encrypted password where each group of 10 characters
 stood for one decimal digit. Accordingly, the original password consisted of 8 decimal digits.


Help Igor K. restore his ISQ account by the encrypted password and encryption specification.




Input


The input data contains 11 lines. The first line represents the binary code 80 characters
 in length. That is the code written in Igor K.'s ISQ account's info. Next 10 lines contain pairwise distinct binary codes 10 characters
 in length, corresponding to numbers 0, 1, ..., 9.




Output


Print one line containing 8 characters — The password to Igor K.'s ISQ account. It is guaranteed that the solution exists.




Sample test(s)




input
01001100100101100000010110001001011001000101100110010110100001011010100101101100
0100110000
0100110010
0101100000
0101100010
0101100100
0101100110
0101101000
0101101010
0101101100
0101101110




output
12345678




input
10101101111001000010100100011010101101110010110111011000100011011110010110001000
1001000010
1101111001
1001000110
1010110111
0010110111
1101001101
1011000001
1110010101
1011011000
0110001000




output
30234919















题目名就告诉你们这个题目叫做【存密码】

给一个长字符串，然后给你0-9的表示方法，问刚刚那个字符串是怎样一个8位数

我们就用str[i] i=0~9 来存储每个数字代表的串咯~

然后怎么匹配呢，我们可以用substr这个函数 str.substr(pos,len) 是代表从str字符串的pos开始，获得一个长度为len的字符串，这是一个字符串问题常用的很方便快捷的函数。

那么来看看源码？



```cpp
#include <cstdio>
#include <string>
#include <cstdlib>
#include <cstring> 
#include <iostream>
#include <algorithm>
using namespace std;
typedef long long ll;
// http://codeforces.com/contest/94
// Restoring Password
int main()
{
	string acinfo;	cin>>acinfo;
	string s[10];
	for(int i=0;i<10;i++)	cin>>s[i];
	for(int i=0;i<8;i++)
	{
		string tmp=acinfo.substr(i*10,10);
		//cout<<tmp<<endl;
		for(int i=0;i<10;i++)
		{
			if(tmp==s[i])
			{
				cout<<i;
				break;
			}
		} 
	}
	
	return 0;
}
```](https://so.csdn.net/so/search/s.do?q=Div. 2 Only&t=blog)](https://so.csdn.net/so/search/s.do?q=Beta Round #76&t=blog)](https://so.csdn.net/so/search/s.do?q=Codeforces&t=blog)




