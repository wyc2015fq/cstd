# 【水字符操作】#46 A. Sleuth - CD's Coding - CSDN博客





2014年07月30日 14:48:42[糖果天王](https://me.csdn.net/okcd00)阅读数：506
所属专栏：[CF-A Terminator](https://blog.csdn.net/column/details/cf-amaster.html)


















Vasya plays the sleuth with his friends. The rules of the game are as follows: those who play for the first time, that is Vasya is the sleuth, he should investigate a "crime" and find out what is happening. He can ask any questions whatsoever that can be answered
 with "Yes" or "No". All the rest agree beforehand to answer the questions like that: if the question’s last letter is a vowel, they answer "Yes" and if the last letter is a consonant, they answer "No". Of course, the sleuth knows nothing about it and his task
 is to understand that.


Unfortunately, Vasya is not very smart. After 5 hours of endless stupid questions everybody except Vasya got bored. That’s why Vasya’s friends ask you to write a program that would give answers instead of them.


The English alphabet vowels are: A, E, I, O, U, Y


The English alphabet consonants are: B, C, D, F, G, H, J, K, L, M, N, P, Q, R, S, T, V, W, X, Z




Input


The single line contains a question represented by a non-empty line consisting of large and small Latin letters, spaces and a question mark. The line length does not exceed 100. It is guaranteed that the question mark occurs exactly once in the line — as the
 last symbol and that the line contains at least one letter.




Output


Print answer for the question in a single line: YES if the answer is "Yes", NO if
 the answer is "No".


Remember that in the reply to the question the last letter, not the last character counts. I. e. the spaces and the question mark do not count as letters.




Sample test(s)




input
Is it a melon?




output
NO




input
Is it an apple?




output
YES




input
  Is     it a banana ?




output
YES




input
Is   it an apple  and a  banana   simultaneouSLY?




output
YES












这道题是说给你一个字符串，问你结尾的那个字符是不是元音或者半元音（A、E、I、O、U、Y）



所以我们每次读入一行（getline），然后从后往前数找到第一个是字母的字符（isupper()||islower()），然后看看它是不是这六个之一，为了省去大小写的问题，我们直接使用toupper把它变成大写，方便比对。

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
char vwl[]={'A','E','I','O','U','Y'};
int main()
{
	int index; 
	string str;
	getline(cin,str);
	for(int i=str.length()-1;i>=0;i--)
	{
		if(isupper(str[i])||islower(str[i]))	
		{
			index=i;
			break;
		}
	}
	for(int i=0;i<6;i++) 
	if(toupper(str[index])==vwl[i])
	{
		cout<<"YES"<<endl;
		return 0;
	}
	cout<<"NO"<<endl;
	return 0;
}
```














