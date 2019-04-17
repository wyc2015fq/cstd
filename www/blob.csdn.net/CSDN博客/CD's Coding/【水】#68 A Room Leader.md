# 【水】#68 A. Room Leader - CD's Coding - CSDN博客





2014年10月20日 17:53:00[糖果天王](https://me.csdn.net/okcd00)阅读数：526标签：[Codeforces																[Room Leader																[Beta Round #68](https://so.csdn.net/so/search/s.do?q=Beta Round #68&t=blog)
个人分类：[CF-A题终结者](https://blog.csdn.net/okcd00/article/category/2156945)

所属专栏：[CF-A Terminator](https://blog.csdn.net/column/details/cf-amaster.html)](https://so.csdn.net/so/search/s.do?q=Room Leader&t=blog)













Let us remind you part of the rules of Codeforces. The given rules slightly simplified, use the problem statement as a formal document.


In the beginning of the round the contestants are divided into rooms. Each room contains exactly *n* participants. During the contest the participants
 are suggested to solve five problems, *A*, *B*, *C*, *D* and *E*.
 For each of these problem, depending on when the given problem was solved and whether it was solved at all, the participants receive some points. Besides, a contestant can perform hacks on other contestants. For each successful hack a contestant earns 100 points,
 for each unsuccessful hack a contestant loses 50points. The number of points for every contestant is represented by the sum of points he has received
 from all his problems, including hacks.


You are suggested to determine the leader for some room; the leader is a participant who has maximum points.




Input


The first line contains an integer *n*, which is the number of contestants in the room (1 ≤ *n* ≤ 50).
 The next *n* lines contain the participants of a given room. The *i*-th
 line has the format of "*handle**i**plus**i**minus**i**a**i**b**i**c**i**d**i**e**i*"
 — it is the handle of a contestant, the number of successful hacks, the number of unsuccessful hacks and the number of points he has received from problems *A*, *B*,*C*, *D*, *E* correspondingly.
 The handle of each participant consists of Latin letters, digits and underscores and has the length from 1to 20 characters.
 There are the following limitations imposed upon the numbers:
- 0 ≤ *plus**i*, *minus**i* ≤ 50;
- 150 ≤ *a**i* ≤ 500 or *a**i* = 0,
 if problem *A* is not solved;
- 300 ≤ *b**i* ≤ 1000 or *b**i* = 0,
 if problem *B* is not solved;
- 450 ≤ *c**i* ≤ 1500 or *c**i* = 0,
 if problem *C* is not solved;
- 600 ≤ *d**i* ≤ 2000 or *d**i* = 0,
 if problem *D* is not solved;
- 750 ≤ *e**i* ≤ 2500 or *e**i* = 0,
 if problem *E* is not solved.


All the numbers are integer. All the participants have different handles. It is guaranteed that there is exactly one leader in the room (i.e. there are no two participants with the maximal number of points).




Output


Print on the single line the handle of the room leader.




Sample test(s)




input
5
Petr 3 1 490 920 1000 1200 0
tourist 2 0 490 950 1100 1400 0
Egor 7 0 480 900 950 0 1000
c00lH4x0R 0 10 150 0 0 0 0
some_participant 2 1 450 720 900 0 0




output
tourist





Note


The number of points that each participant from the example earns, are as follows:
- Petr — 3860
- tourist — 4140
- Egor — 4030
- c00lH4x0R —  - 350
- some_participant — 2220


Thus, the leader of the room is tourist.










这又是一道读入的时候更新最大值类型的问题……真的不知道该划分到哪个类型……直接就写个【水】不好意思哦……



每次读入的时候就把这个人的分数算出来，如果是最大的就把最大值更新一下即可。

Code：



```cpp
#include <map>
#include <stack>
#include <queue>
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
	int cases=0;
	scanf("%d",&cases);
	int max=-2999;
	string win="";
	for(int _case=1;_case<=cases;_case++)
	{
		string name; 
		int p,m,a,b,c,d,e,score;
		cin>>name>>p>>m>>a>>b>>c>>d>>e;
		score=p*100-m*50+a+b+c+d+e;
		if (score>max) max=score,win=name;
	}
	cout<<win;
	return 0;
}
```](https://so.csdn.net/so/search/s.do?q=Codeforces&t=blog)




