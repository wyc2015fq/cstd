# 【水模拟】#71 A. Bus Game - CD's Coding - CSDN博客





2014年10月27日 15:06:39[糖果天王](https://me.csdn.net/okcd00)阅读数：694标签：[Bus Game																[Codeforces																[模拟																[Beta Round #71](https://so.csdn.net/so/search/s.do?q=Beta Round #71&t=blog)
个人分类：[CF-A题终结者](https://blog.csdn.net/okcd00/article/category/2156945)

所属专栏：[CF-A Terminator](https://blog.csdn.net/column/details/cf-amaster.html)](https://so.csdn.net/so/search/s.do?q=模拟&t=blog)













After Fox Ciel won an onsite round of a programming contest, she took a bus to return to her castle. The fee of the bus was 220 yen. She met Rabbit Hanako in the bus. They decided to play the following game because they got bored in the bus.
- 
Initially, there is a pile that contains *x* 100-yen coins and *y* 10-yen
 coins.
- 
They take turns alternatively. Ciel takes the first turn.
- 
In each turn, they must take exactly 220 yen from the pile. In Ciel's turn, if there are multiple ways to take 220 yen, she will choose the way that contains the maximal number of 100-yen coins. In Hanako's turn, if there are multiple ways to take 220 yen,
 she will choose the way that contains the maximal number of 10-yen coins.
- 
If Ciel or Hanako can't take exactly 220 yen from the pile, she loses.




Determine the winner of the game.




Input


The first line contains two integers *x* (0 ≤ *x* ≤ 106)
 and *y* (0 ≤ *y* ≤ 106),
 separated by a single space.




Output


If Ciel wins, print "Ciel". Otherwise, print "Hanako".




Sample test(s)




input
2 2




output
Ciel




input
3 22




output
Hanako






Note


In the first turn (Ciel's turn), she will choose 2 100-yen coins and 2 10-yen coins. In the second turn (Hanako's turn), she will choose 1 100-yen coin and 12 10-yen coins. In the third turn (Ciel's turn), she can't pay exactly 220 yen, so Ciel will lose.










原先看到这个题说谁赢谁输还以为是博弈……意思是有x枚100元硬币、y枚10元硬币，两个人轮流取钱每次取220元，Ciel（希尔）每次尽可能多的取100元硬币，Hanako（花子）则每次尽可能多的取10元硬币（意味不明……），然后问谁第一次没法取了另一个人就赢了。



那么我们就模拟他们取硬币的过程吧。

希尔（Ciel）的话，100元不小于2个的情况下就取2个加上2个10元的，不足2个就取光剩下的10元来凑

Hanako反之，同理。

自然，当总钱数不足220或者10元硬币不足2个的时候失败。

代码如下：



```cpp
#include <cstdio>
#include <string>
#include <cstring> 
#include <iostream>
#include <algorithm>
using namespace std;
typedef long long ll;
// http://codeforces.com/contest/79
// Bus Game
int main()
{
	int x,y,turn=1;	//1-Ciel 2-Hanako
	cin>>x>>y;
	while(turn)
	{
		if(y<2 || 100*x+10*y<220) break;
		if(turn==1)
		{
			if(x>=2)x-=2,y-=2;
			else if(x==1)x-=1,y-=12;
			else if(x==0)y-=22;
		}
		else if(turn==2)
		{
			if(y>=22)y-=22;
			else if(y>=12)y-=12,x-=1;
			else if(y>=2)y-=2,x-=2;
		}
		turn = 3-turn;
	}
	if(turn==1)printf("Hanako");
	else printf("Ciel");
	return 0;
}
```](https://so.csdn.net/so/search/s.do?q=Codeforces&t=blog)](https://so.csdn.net/so/search/s.do?q=Bus Game&t=blog)




