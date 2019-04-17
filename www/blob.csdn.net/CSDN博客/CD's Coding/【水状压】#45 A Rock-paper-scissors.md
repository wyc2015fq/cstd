# 【水状压】#45 A. Rock-paper-scissors - CD's Coding - CSDN博客





2014年07月30日 14:32:15[糖果天王](https://me.csdn.net/okcd00)阅读数：815标签：[codeforces](https://so.csdn.net/so/search/s.do?q=codeforces&t=blog)
个人分类：[CF-A题终结者](https://blog.csdn.net/okcd00/article/category/2156945)

所属专栏：[CF-A Terminator](https://blog.csdn.net/column/details/cf-amaster.html)


















Uncle Fyodor, Matroskin the Cat and Sharic the Dog live their simple but happy lives in Prostokvashino. Sometimes they receive parcels from Uncle Fyodor’s parents and sometimes from anonymous benefactors, in which case it is hard to determine to which one of
 them the package has been sent. A photographic rifle is obviously for Sharic who loves hunting and fish is for Matroskin, but for whom was a new video game console meant? Every one of the three friends claimed that the present is for him and nearly quarreled.
 Uncle Fyodor had an idea how to solve the problem justly: they should suppose that the console was sent to all three of them and play it in turns. Everybody got relieved but then yet another burning problem popped up — who will play first? This time Matroskin
 came up with a brilliant solution, suggesting the most fair way to find it out: play rock-paper-scissors together. The rules of the game are very simple. On the count of three every player shows a combination with his hand (or paw). The combination corresponds
 to one of three things: a rock, scissors or paper. Some of the gestures win over some other ones according to well-known rules: the rock breaks the scissors, the scissors cut the paper, and the paper gets wrapped over the stone. Usually there are two players.
 Yet there are three friends, that’s why they decided to choose the winner like that: If someone shows the gesture that wins over the other two players, then that player wins. Otherwise, another game round is required. Write a program that will determine the
 winner by the gestures they have shown.




Input


The first input line contains the name of the gesture that Uncle Fyodor showed, the second line shows which gesture Matroskin showed and the third line shows Sharic’s gesture.




Output


Print "F" (without quotes) if Uncle Fyodor wins. Print "M" if Matroskin wins and "S" if Sharic wins. If it is impossible to find the winner, print "?".




Sample test(s)




input
rock
rock
rock




output
?




input
paper
rock
rock




output
F




input
scissors
rock
rock




output
?




input
scissors
paper
rock




output
?















三个人玩剪刀石头布问谁赢或者都没赢（输出？）

我们要是一个一个判断的话得判断27中情况简直不高兴……要是更多怎么破……

三个人就用三进制来表示就不可能出现重复，然后对于需要的情况进行列举是谁赢了即可：



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

#define findx(x) (tf==x?"F":tm==x?"M":"S")
using namespace std;

int gt(string s)
{
	if(s=="rock")		return 1;
	if(s=="scissors")	return 3;
	if(s=="paper")		return 9;
	return 0;
}

int main()
{
	string f,m,s;
	cin>>f>>m>>s;
	int tf=gt(f),tm=gt(m),ts=gt(s);
	int sum=tf+tm+ts;
	
	if(sum==7)		 cout<< findx(1) <<endl;
	else if(sum==21) cout<< findx(3) <<endl;
	else if(sum==11) cout<< findx(9) <<endl;
	else cout<<"?"<<endl;
	return 0;
}
```














