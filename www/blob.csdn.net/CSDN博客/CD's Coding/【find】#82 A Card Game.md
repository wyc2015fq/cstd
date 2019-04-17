# 【find】#82 A. Card Game - CD's Coding - CSDN博客





2014年11月29日 20:44:40[糖果天王](https://me.csdn.net/okcd00)阅读数：693标签：[Card Game																[find																[Codeforces																[Beta Round #82 Div.](https://so.csdn.net/so/search/s.do?q=Beta Round #82 Div.&t=blog)
个人分类：[CF-A题终结者](https://blog.csdn.net/okcd00/article/category/2156945)

所属专栏：[CF-A Terminator](https://blog.csdn.net/column/details/cf-amaster.html)](https://so.csdn.net/so/search/s.do?q=Codeforces&t=blog)













There is a card game called "Durak", which means "Fool" in Russian. The game is quite popular in the countries that used to form USSR. The problem does not state all the game's rules explicitly — you can find them later yourselves if you want.


To play durak you need a pack of 36 cards. Each card has a suit ("S",
 "H", "D" and "C")
 and a rank (in the increasing order "6", "7", "8",
 "9", "T", "J",
 "Q", "K" and "A").
 At the beginning of the game one suit is arbitrarily chosen as trump.


The players move like that: one player puts one or several of his cards on the table and the other one should beat each of them with his cards.


A card beats another one if both cards have similar suits and the first card has a higher rank then the second one. Besides, a trump card can beat any non-trump card whatever the cards’ ranks are. In all other cases you can not beat the second card with the
 first one.


You are given the trump suit and two different cards. Determine whether the first one beats the second one or not.




Input


The first line contains the tramp suit. It is "S", "H", "D"
 or "C".


The second line contains the description of the two different cards. Each card is described by one word consisting of two symbols. The first symbol stands for the rank ("6",
 "7", "8", "9",
 "T", "J", "Q",
 "K" and "A"), and the second one stands for the suit ("S",
 "H", "D" and "C").




Output


Print "YES" (without the quotes) if the first cards beats the second one. Otherwise, print "NO"
 (also without the quotes).




Sample test(s)




input
H
QH 9S




output
YES




input
S
8D 6D




output
YES



input
C
7H AS




output
NO














两张牌比大小的游戏。

第一个字母是牌面点数，第二个是花色（Spade黑桃、Heart红桃、Diamond方片、Clover梅花（其实是四叶草啦））

先给出的那个字母是王牌花色，如果两张牌一个是王牌另一个不是，那么王牌花色获胜，如果都是或者都不是比点数大小。

这里我用了find函数 find(begin,end,key)可以返回找到的值在数组内的指针，指针之间可以相减获得距离。



```cpp
#include <cstdio>
#include <string>
#include <vector>
#include <cstring> 
#include <iostream>
#include <algorithm>
using namespace std;
typedef long long ll;
// http://codeforces.com/contest/106
// Card Game
char tramp[9]={'6','7','8','9','T','J','Q','K','A'};

int main()
{
	string trp;	cin>>trp;
	string a,b;	cin>>a>>b;
	if(a[1]==trp[0] && b[1]!=trp[0]) cout<<"YES";
	else if(a[1]!=trp[0] && b[1]==trp[0]) cout<<"NO";
	else 
	{
		int dist=find(tramp,tramp+9,a[0])-find(tramp,tramp+9,b[0]);
		if(dist>0) cout<<"YES";
		else cout<<"NO";
	}
	return 0;
}
```](https://so.csdn.net/so/search/s.do?q=find&t=blog)](https://so.csdn.net/so/search/s.do?q=Card Game&t=blog)




