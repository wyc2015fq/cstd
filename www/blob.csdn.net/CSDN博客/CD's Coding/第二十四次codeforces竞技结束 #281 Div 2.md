# 第二十四次codeforces竞技结束 #281 Div 2 - CD's Coding - CSDN博客





2014年12月04日 03:04:12[糖果天王](https://me.csdn.net/okcd00)阅读数：1040










[](http://codeforces.com/blog/entry/14985)

Codeforces Round #281 (Div. 2) Editorial



By [albertg](http://codeforces.com/profile/albertg), 93
 minutes ago, translation, ![In English](http://codeforces.com/static/images/flags/24/gb.png), ![](http://codeforces.com/static/images/icons/paperclip-16x16.png)





[493A - Vasya and Football](http://codeforces.com/contest/493/problem/A)


We need 2 arrays — for the first and second team, in which we must save "status" of the player — is he "clear", yellow carded or sent off. Then while inputing we must output the players name if he wasn't sent off, and after the event he must be sent off.

[493B - Vasya and Wrestling](http://codeforces.com/contest/493/problem/B)


We need to vectors in which we will save points of first and second wrestlers, and two int-s, where we will save who made the last technique and what is the sum of all the numbers in the input. If the sum is not zero, we know the answer. Else we pass by the
 vectors, checking are there respective elements which are not equal. If yes — then we know the answer, else everything depends on who made the last technique.

[493C - Vasya and Basketball](http://codeforces.com/contest/493/problem/C)


We need an array of pairs — in each pair we save the distance and the number of team. Then we sort the array. Then we assume that all the throws bring 3 points. Then we pass by the array and one of our numbers we decrease on 1 (which one — it depends on the
 second element of array). Then we compare it with our answer. In the end — we print our answer.

[493D - Vasya and Chess](http://codeforces.com/contest/493/problem/D)


If n is odd, then black can win white doing all the moves symetric by the central line. Else white can win putting his queen on (1,2) (which is the lexicographicly smallest place) and play symetricly — never using the first row.

[493E - Vasya and Polynomial](http://codeforces.com/contest/493/problem/E)


Let's discuss 2 case. 1) t!=1 и 2) t=1.


1) If our function is not constant (n>=1) than a is greater all the coefficients, so the only polynom can be the number b — in the a-ary counting system. We must only check that one and constant function.


2)if t=1 must be careful: in case 1 1 1: the answer is inf, in case 1 1 n: the answer is 0 in case 1 а а^x(x-integer, x>0): the answer is 1 in the other cases P(1) is greater than other coefficients.



![](http://codeforces.com/static/images/icons/paperclip-16x16.png)Tutorial of [Codeforces
 Round #282 (Div. 2)](http://codeforces.com/contest/495)













Vasya has started watching football games. He has learned that for some fouls the players receive yellow cards, and for some fouls they receive red cards. A player who receives the second yellow card automatically receives a red card.


Vasya is watching a recorded football match now and makes notes of all the fouls that he would give a card for. Help Vasya determine all the moments in time when players would be given red cards if Vasya were the judge. For each player, Vasya wants to know
 only the first moment of time when he would receive a red card from Vasya.




Input


The first line contains the name of the team playing at home. The second line contains the name of the team playing away. Both lines are not empty. The lengths of both lines do not exceed 20. Each line contains only of large English letters. The names of the
 teams are distinct.


Next follows number *n* (1 ≤ *n* ≤ 90) — the number of
 fouls.


Each of the following *n* lines contains information about a foul in the following form:
- 
first goes number *t* (1 ≤ *t* ≤ 90) — the minute when
 the foul occurs;
- 
then goes letter "h" or letter "a" — if the letter is "h",
 then the card was given to a home team player, otherwise the card was given to an away team player;
- 
then goes the player's number *m* (1 ≤ *m* ≤ 99);
- 
then goes letter "y" or letter "r" — if the letter is "y",
 that means that the yellow card was given, otherwise the red card was given.


The players from different teams can have the same number. The players within one team have distinct numbers. The fouls go chronologically, no two fouls happened at the same minute.




Output


For each event when a player received his first red card in a chronological order print a string containing the following information:
- 
The name of the team to which the player belongs;
- 
the player's number in his team;
- 
the minute when he received the card.


If no player received a card, then you do not need to print anything.


It is possible case that the program will not print anything to the output (if there were no red cards).




Sample test(s)




input
MC
CSKA
9
28 a 3 y
62 h 25 y
66 h 42 y
70 h 25 y
77 a 4 y
79 a 25 y
82 h 42 r
89 h 16 y
90 a 13 r




output
MC 25 70
MC 42 82
CSKA 13 90















else if(c=="y" && hc[no]!=-1) 这里的-1手残写成了0……


这题是足球犯规给牌的记录题目，如果有红牌了之后这个人你就不用管它了（用-1标记就好，以及一个人不能拿两次红牌哦，输出第一次即可），如果给了一张黄牌，记录一下拿过牌了，下一次拿黄牌的时候记得标记改成-1然后输出~

### Code：



```cpp
#include <cmath> 
#include <cctype>
#include <cstdio>
#include <string>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <algorithm>
using namespace std;
int hc[100]={0};
int ac[100]={0};
#define Max(a,b) ((a)>(b)?(a):(b))
#define Min(a,b) ((a)<(b)?(a):(b))

bool cmp(const int a, const int b)
{
	return a > b;
}

int main()
{
	string h,a;	cin>>h>>a;
	int n;	cin>>n;
	for(int i=0;i<n;i++)
	{
		int time,no;
		string w,c;
		cin>>time>>w>>no>>c;
		if(w=="a")
		{
			if(c=="r" && ac[no]!=-1)
			{
				ac[no]=-1;
				cout<<a<<" "<<no<<" "<<time<<endl;
			}
			else if(c=="y" && ac[no]==0)
			{
				ac[no]=time;
			}
			else if(c=="y" && ac[no]!=-1)
			{
				ac[no]=-1;
				cout<<a<<" "<<no<<" "<<time<<endl;
			}
		}
		else
		{
			if(c=="r" && hc[no]!=-1)
			{
				hc[no]=-1;
				cout<<h<<" "<<no<<" "<<time<<endl;
			}
			else if(c=="y" && hc[no]==0)
			{
				hc[no]=time;
			}
			else if(c=="y" && hc[no]!=-1)
			{
				hc[no]=-1;
				cout<<h<<" "<<no<<" "<<time<<endl;
			}
		}
	}
	
	return 0;
}
```









Vasya has become interested in wrestling. In wrestling wrestlers use techniques for which they are awarded points by judges. The wrestler who gets the most points wins.


When the numbers of points of both wrestlers are equal, the wrestler whose sequence of points is lexicographically greater, wins.


If the sequences of the awarded points coincide, the wrestler who performed the last technique wins. Your task is to determine which wrestler won.




Input


The first line contains number *n* — the number of techniques that the wrestlers have used (1 ≤ *n* ≤ 2·105).


The following *n* lines contain integer numbers *a**i* (|*a**i*| ≤ 109, *a**i* ≠ 0).
 If *a**i* is positive,
 that means that the first wrestler performed the technique that was awarded with *a**i* points.
 And if *a**i* is
 negative, that means that the second wrestler performed the technique that was awarded with ( - *a**i*) points.


The techniques are given in chronological order.




Output


If the first wrestler wins, print string "first", otherwise print "second"




Sample test(s)




input
5
1
2
-3
-4
3




output
second




input
3
-1
-2
3




output
first




input
2
4
-4




output
second






Note


Sequence *x*  =  *x*1*x*2... *x*|*x*| is lexicographically
 larger than sequence *y*  =  *y*1*y*2... *y*|*y*|,
 if either |*x*|  >  |*y*| and *x*1  =  *y*1,  *x*2  =  *y*2, ...
 ,  *x*|*y*|  =  *y*|*y*|,
 or there is such number *r* (*r*  <  |*x*|, *r*  <  |*y*|),
 that *x*1  =  *y*1,  *x*2  =  *y*2,  ...
 ,  *x**r*  =  *y**r* and *x**r*  +  1  >  *y**r*  +  1.


We use notation |*a*| to denote length of sequence *a*.


















25




**Time**: 15 ms, **memory**: 1556 KB


Verdict: WRONG_ANSWER



Input

6
-1
-2
-3
1
2
3




Output

second



Answer

first




Checker comment

wrong answer 1st words differ - expected: 'first', found: 'second'











WA 在了上述这个数据上…… 为啥为啥为啥为啥……

然后我在解题报告中看到了这句——then we know the answer, else everything depends on who made the last technique.

好吧……我错了，错在这种地方好不甘心呀……

### Code：



```cpp
#include <cmath> 
#include <cctype>
#include <cstdio>
#include <string>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <algorithm>
using namespace std;
typedef long long ll;
#define Max(a,b) ((a)>(b)?(a):(b))
#define Min(a,b) ((a)<(b)?(a):(b))

bool cmp(const int a, const int b)
{
	return a > b;
}

int main()
{
	int n=0;	cin>>n;
	ll sum1=0,sum2=0;
	int num1[200048]={0},v1=0;
	int num2[200048]={0},v2=0;
	int last=0;
	for(int i=0;i<n;i++)
	{
		int tmp;	scanf("%d",&tmp);
		if(tmp>0) 	sum1+=tmp,num1[v1++]=tmp;
		else 		sum2+=-tmp,num2[v2++]=-tmp;
		if(i==n-1)last=tmp;
	}
	if(sum1>sum2)cout<<"first";
	else if(sum2>sum1)cout<<"second";
	else 
	{
		if(v1>v2)
		{
			for(int i=0;i<v2;i++)
			{
				if(num1[i]==num2[i])continue;
				else if(num1[i]>num2[i])
				{
					cout<<"first"; 
					return 0;
				}
				else
				{
					cout<<"second"; 
					return 0;
				}
			}
			cout<<"first";
			return 0;
		}
		else
		{
			for(int i=0;i<v1;i++)
			{	
				if(num1[i]==num2[i])continue;
				else if(num1[i]>num2[i])
				{
					cout<<"first"; 
					return 0;
				}
				else
				{
					cout<<"second"; 
					return 0;
				}
			}
			if(v1==v2 && last<0)cout<<"second";
			else if(v1==v2) cout<<"first";
			else cout<<"second";
			return 0;
		}
	}
	return 0;
}
```









Vasya follows a basketball game and marks the distances from which each team makes a throw. He knows that each successful throw has value of either 2 or 3 points. A throw is worth 2 points if the distance it was made from doesn't exceed some value of*d* meters,
 and a throw is worth 3 points if the distance is larger than *d* meters, where *d* is
 some non-negative integer.


Vasya would like the advantage of the points scored by the first team (the points of the first team minus the points of the second team) to be maximum. For that he can mentally choose the value of *d*.
 Help him to do that.




Input


The first line contains integer *n* (1 ≤ *n* ≤ 2·105)
 — the number of throws of the first team. Then follow *n* integer numbers — the distances of throws *a**i* (1 ≤ *a**i* ≤ 2·109).


Then follows number *m* (1 ≤ *m* ≤ 2·105)
 — the number of the throws of the second team. Then follow *m* integer numbers — the distances of throws of *b**i* (1 ≤ *b**i* ≤ 2·109).




Output


Print two numbers in the format a:b — the score that is possible considering the problem conditions where the result of subtraction *a* - *b* is
 maximum. If there are several such scores, find the one in which number *a* is maximum.




Sample test(s)




input
3
1 2 3
2
5 6




output
9:6




input
5
6 7 8 9 10
5
1 2 3 4 5




output
15:10












这道题是说有两个队伍各在距离篮筐这么多米的距离投进了球，让你来制定三分线在哪里，以至于一队比二队的净胜分最大。



需要注意的地方挺多的，错了的孩子可以好好琢磨一下这组数据：




Test: #10, time: 46 ms., memory: 3132 KB, exit code: 0, checker exit code: 1, verdict: WRONG_ANSWER



Input
4
2 5 3 2
4
1 5 6 2



Output
12:12



Answer
12:11



Checker Log
wrong answer 1st words differ - expected: '12:11', found: '12:12'






这是一个十分需要细心的题目……主要是当判断到两队相同米数时很容易出错，需要知道的是，当三分线向外移动时，两个队伍分数是【同时】降低的。

### Code：



```cpp
#include<bits/stdc++.h>
using namespace std;
#define ll long long
const int maxn=200010;
int a[maxn],b[maxn],n,m;
ll x,y,A,B;
int main()
{
    cin>>n;
    for(int i=0;i<n;i++) cin>>a[i];
    cin>>m;
    for(int i=0;i<m;i++) cin>>b[i];
    sort(a,a+n);   sort(b,b+m);
    int j=m-1;
    A=n;    B=m;
    for(int i=n-1;i>=0;i--)
    {
        while(j>=0 && a[i]<=b[j]) j--,y++;
        x++;
        if(x-y>=A-B) A=x,B=y;
    }
    if(A<B) A=B=0;
    cout<<(ll)n*2+A<<":"<<(ll)m*2+B<<endl;
    return 0;
}
```









Vasya decided to learn to play chess. Classic chess doesn't seem interesting to him, so he plays his own sort of chess.


The queen is the piece that captures all squares on its vertical, horizontal and diagonal lines. If the cell is located on the same vertical, horizontal or diagonal line with queen, and the cell contains a piece of the enemy color, the queen is able to move
 to this square. After that the enemy's piece is removed from the board. The queen cannot move to a cell containing an enemy piece if there is some other piece between it and the queen.


There is an *n* × *n* chessboard. We'll denote a cell on the intersection of the *r*-th
 row and *c*-th column as (*r*, *c*). The square (1, 1) contains
 the white queen and the square (1, *n*) contains the black queen. All other squares contain green pawns that don't belong to anyone.


The players move in turns. The player that moves first plays for the white queen, his opponent plays for the black queen.


On each move the player has to capture some piece with his queen (that is, move to a square that contains either a green pawn or the enemy queen). The player loses if either he cannot capture any piece during his move or the opponent took his queen during the
 previous move.


Help Vasya determine who wins if both players play with an optimal strategy on the board *n* × *n*.




Input


The input contains a single number *n* (2 ≤ *n* ≤ 109)
 — the size of the board.




Output


On the first line print the answer to problem — string "white" or string "black",
 depending on who wins if the both players play optimally.


If the answer is "white", then you should also print two integers *r* and *c* representing
 the cell (*r*, *c*), where the first player should make his first move to win. If there are multiple such cells, print the one with the minimum *r*.
 If there are still multiple squares, print the one with the minimum *c*.




Sample test(s)




input
2




output
white
1 2




input
3




output
black






Note


In the first sample test the white queen can capture the black queen at the first move, so the white player wins.


In the second test from the statement if the white queen captures the green pawn located on the central vertical line, then it will be captured by the black queen during the next move. So the only move for the white player is to capture the green pawn located
 at (2, 1).


Similarly, the black queen doesn't have any other options but to capture the green pawn located at (2, 3), otherwise if it goes to the middle vertical line, it will
 be captured by the white queen.


During the next move the same thing happens — neither the white, nor the black queen has other options rather than to capture green pawns situated above them. Thus, the white queen ends up on square (3, 1),
 and the black queen ends up on square(3, 3).


In this situation the white queen has to capture any of the green pawns located on the middle vertical line, after that it will be captured by the black queen. Thus, the player who plays for the black queen wins.










这道题1A的时候我只想说……你是在逗我嘛……



博弈，nxn的棋盘，白皇后在(1,1),黑皇后在(1,n)，皇后每次只能走相邻的八方向格子（如果相邻的格子被Remove了可以走到这个方向的第一个还健在的格子里），如果走到对方皇后所在格子内或者让对方皇后没路可走则获胜。 很轻易的可以知道，2white，3black，4只要向右一格就变成了3的黑白互换，5同理向右一格变成4所以不这么走即可black胜，依此类推看奇偶性即可。

### Code：



```cpp
#include <cmath> 
#include <cctype>
#include <cstdio>
#include <string>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <algorithm>
using namespace std;

#define Max(a,b) ((a)>(b)?(a):(b))
#define Min(a,b) ((a)<(b)?(a):(b))

bool cmp(const int a, const int b)
{
	return a > b;
}

int main()
{
	int n;	cin>>n;
	if(n%2)cout<<"black"<<endl;
	else cout<<"white"<<endl<<"1 2"<<endl;
	return 0;
}
```














