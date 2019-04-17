# 【水模拟】#54 A. Chat room - CD's Coding - CSDN博客





2014年08月08日 21:09:09[糖果天王](https://me.csdn.net/okcd00)阅读数：1413
所属专栏：[CF-A Terminator](https://blog.csdn.net/column/details/cf-amaster.html)













Vasya has recently learned to type and log on to the Internet. He immediately entered a chat room and decided to say hello to everybody. Vasya typed the word *s*.
 It is considered that Vasya managed to say hello if several letters can be deleted from the typed word so that it resulted in the word "hello". For example,
 if Vasya types the word "ahhellllloou", it will be considered that he said hello, and if he types "hlelo",
 it will be considered that Vasya got misunderstood and he didn't manage to say hello. Determine whether Vasya managed to say hello by the given word *s*.




Input


The first and only line contains the word *s*, which Vasya typed. This word consisits of small Latin letters, its length is no less that 1 and no more than
 100 letters.




Output


If Vasya managed to say hello, print "YES", otherwise print "NO".




Sample test(s)




input
ahhellllloou




output
YES




input
hlelo




output
NO






这道题，看完一遍题开始写，写完直接交了，一次都没有编译过……然后就A了

抱歉……你不能怪我轻视你，真的是你太水……

给一个字符串，问里面有没有hello这个子序列

那么……模拟即可

Code：



```cpp
#include <cstdio>
#include <memory>
#include <cstring>
#include <iostream>
#include <algorithm>
using namespace std;


int main()
{
	int flag=0;
	string s;	cin>>s;
	for(int i=0;i<s.length();i++)
	{
		if(flag==0 && s[i]=='h')flag=1;
		else if(flag==1 && s[i]=='e')flag=2;
		else if(flag==2 && s[i]=='l')flag=3;
		else if(flag==3 && s[i]=='l')flag=4;
		else if(flag==4 && s[i]=='o')flag=5;
	}
	if(flag==5)cout<<"YES"<<endl;
	else cout<<"NO"<<endl;
	return 0;
}
```











