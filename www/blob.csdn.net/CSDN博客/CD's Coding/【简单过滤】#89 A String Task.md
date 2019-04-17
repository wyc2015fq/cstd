# 【简单过滤】#89 A. String Task - CD's Coding - CSDN博客





2015年02月22日 16:23:17[糖果天王](https://me.csdn.net/okcd00)阅读数：682

















Petya started to attend programming lessons. On the first lesson his task was to write a simple program. The program was supposed to do the following: in the given string, consisting if uppercase and lowercase Latin letters, it:
- 
deletes all the vowels,
- 
inserts a character "." before each consonant,
- 
replaces all uppercase consonants with corresponding lowercase ones.


Vowels are letters "A", "O", "Y", "E", "U", "I", and the rest are consonants. The program's input is exactly one string, it should return the output as a single string, resulting after the program's processing the initial string.


Help Petya cope with this easy task.




Input


The first line represents input string of Petya's program. This string only consists of uppercase and lowercase Latin letters and its length is from 1 to 100,
 inclusive.




Output


Print the resulting string. It is guaranteed that this string is not empty.




Sample test(s)




input
tour




output
.t.r




input
Codeforces




output
.c.d.f.r.c.s




input
aBAcAba




output
.b.c.b















题意是说给一个字符串，要求输出一个序列，只包含原字符串的非元音字母的小写形式，每个字母输出前要加一个点

首先处理大小写，我们直接把源字符串变成全小写的就好，tolower(char a)函数返回的就是当前字母的小写形式，然后我们判断一下这个字母是否元音字母，不是的话我们就输出点和这个字母即可

Code：



```cpp
#include <cstdio>
#include <string>
#include <cstring> 
#include <iostream>
#include <algorithm>
using namespace std;
typedef long long ll;
// http://codeforces.com/contest/118
// String Task

bool judge(char c)
{
	if(c=='a'||c=='e'||c=='i'||c=='o'||c=='u'||c=='y')return false;
	return true;
}

int main()
{
	string s;	cin>>s;
	for(int i=0;i<s.length();i++)
	{
		char now=tolower(s[i]);
		if(judge(now))cout<<'.'<<now;
	} 
	return 0;
}
```








