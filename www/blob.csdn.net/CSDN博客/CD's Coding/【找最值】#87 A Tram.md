# 【找最值】#87 A. Tram - CD's Coding - CSDN博客





2015年01月30日 01:07:18[糖果天王](https://me.csdn.net/okcd00)阅读数：640标签：[Codeforces																[Beta Round #87																[Div. 2 Only																[Tram](https://so.csdn.net/so/search/s.do?q=Tram&t=blog)
个人分类：[CF-A题终结者](https://blog.csdn.net/okcd00/article/category/2156945)

所属专栏：[CF-A Terminator](https://blog.csdn.net/column/details/cf-amaster.html)](https://so.csdn.net/so/search/s.do?q=Div. 2 Only&t=blog)








Linear Kingdom has exactly one tram line. It has *n* stops, numbered from 1 to *n* in
 the order of tram's movement. At the *i*-th stop *a**i*passengers
 exit the tram, while *b**i* passengers
 enter it. The tram is empty before it arrives at the first stop. Also, when the tram arrives at the last stop, all passengers exit so that it becomes empty.


Your task is to calculate the tram's minimum capacity such that the number of people inside the tram at any time never exceeds this capacity. Note that at each stop all exiting passengers exit before any
 entering passenger enters the tram.




Input


The first line contains a single number *n* (2 ≤ *n* ≤ 1000)
 — the number of the tram's stops.


Then *n* lines follow, each contains two integers *a**i* and *b**i* (0 ≤ *a**i*, *b**i* ≤ 1000)
 — the number of passengers that exits the tram at the *i*-th stop, and the number of passengers that enter the tram at the *i*-th
 stop. The stops are given from the first to the last stop in the order of tram's movement.
- 
The number of people who exit at a given stop does not exceed the total number of people in the tram immediately before it arrives at the stop. More formally, ![](http://espresso.codeforces.com/eb1e20fd58cc4373d8f42eb7e743d3ea232d1a19.png).
 This particularly means that *a*1 = 0.
- 
At the last stop, all the passengers exit the tram and it becomes empty. More formally, ![](http://espresso.codeforces.com/8cbe43ecd252bf7d670f9a3956cbe50263d2f09b.png).
- 
No passenger will enter the train at the last stop. That is, *b**n* = 0.




Output


Print a single integer denoting the minimum possible capacity of the tram (0 is allowed).




Sample test(s)




input
4
0 3
2 5
4 2
4 0




output
6






Note


For the first example, a capacity of 6 is sufficient:
- 
At the first stop, the number of passengers inside the tram before arriving is 0. Then, 3 passengers enter the tram, and the number of passengers inside the tram becomes 3.
- 
At the second stop, 2 passengers exit the tram (1 passenger remains inside). Then, 5 passengers enter the tram. There are 6 passengers inside the tram now.
- 
At the third stop, 4 passengers exit the tram (2 passengers remain inside). Then, 2 passengers enter the tram. There are 4 passengers inside the tram now.
- 
Finally, all the remaining passengers inside the tram exit the tram at the last stop. There are no passenger inside the tram now, which is in line with the constraints.


Since the number of passengers inside the tram never exceeds 6, a capacity of 6 is sufficient. Furthermore it is not possible for the tram to have a capacity less than 6. Hence, 6 is the correct answer.








最近这些题目啊真是读起来神烦神烦的，那么长的题目可讨厌了，等一读完每次的感想都是——这么点事情你用这么多字闹哪样啊啊啊！

Tram就是有轨电车，可以理解为新干线，题目的意思就是告诉你有n个站，每个站上车几个人下车几个人（注意是先下后上），问：车上最多的时候有几个人…… 

用maxValue简称maxv(麦克斯韦23333)来记忆一下最高值即可




Code：



```cpp
#include <cstdio>
#include <string>
#include <cstring> 
#include <iostream>
#include <algorithm>
using namespace std;
typedef long long ll;
// http://codeforces.com/contest/116
// Tram
int main()
{
	int maxv=0,a=0,b=0,now=0,n=0;	cin>>n;
	for(int i=0;i<n;i++)
	{
		cin>>a>>b;
		now=now-a+b;
		maxv=max(maxv,now);
	}
	cout<<maxv;
	return 0;
}
```](https://so.csdn.net/so/search/s.do?q=Beta Round #87&t=blog)](https://so.csdn.net/so/search/s.do?q=Codeforces&t=blog)




