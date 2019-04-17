# 【水模拟】#50 A. Presents - CD's Coding - CSDN博客





2014年07月31日 09:25:54[糖果天王](https://me.csdn.net/okcd00)阅读数：602
所属专栏：[CF-A Terminator](https://blog.csdn.net/column/details/cf-amaster.html)


















The Hedgehog likes to give presents to his friend, but no less he likes to receive them.


Having received another present today, the Hedgehog suddenly understood that he has no place to put it as there was no room left on the special shelf in the cupboard. He will have to choose another shelf, but which one should he choose, how large should it
 be?


In order to get to know this, the Hedgehog asks you to write him a program that will count the estimated number of presents that he will receive during the following *N* days.
 Besides, he is guided by the principle:
- 
on each holiday day the Hedgehog will necessarily receive a present,
- 
he receives presents at least every *K* days (i.e., if he received a present on the *i*-th
 day, he will receive the next present no later than on the *i* + *K*-th day).


For the given *N* and *K*, as well as the list of holidays
 among the following *N* days count the minimal number of presents that could be given to the Hedgehog. The number of today's day is zero, and you should regard
 today's present as already given (i.e., you shouldn't count it in the answer).




Input


The first line contains integers *N* and *K* (1 ≤ *N* ≤ 365, 1 ≤ *K* ≤ *N*).


The second line contains a number *C* which represents the number of holidays (0 ≤ *C* ≤ *N*).
 Then in the same line follow *C* numbers ranging from 1 to *N* which
 are the numbers of holiday days. The numbers are given in the increasing order, without repeating numbers among them.




Output


Print a single number — the minimal number of presents the Hedgehog will receive over the following *N* days.




Sample test(s)




input
5 2
1 3




output
3



input
10 1
3 6 7 8




output
10











今天为第0天，接下来有N天，每两次收到礼物的日子之间不会超过k天（即隔得最远的话是N和N+K天都有礼物），期间共有C个假日，分别是后面的共C个数，每个假期必定有礼物。问接下来礼物数量最小值为多少。



那么就按日模拟即可——



```cpp
#include <cstdio>
#include <iostream>
#include <algorithm>
using namespace std;

int main()
{
	int n,k,c,days[366],idx=0;
	cin>>n>>k>>c;
	for(int i=0;i<c;i++)		cin>>days[i];
	int cnt=0,ans=0;
	for(int i=0;i<=n;i++)
	{
		if(i==days[idx] && i!=0 )//0没有特判 wa at 5 
		{
			cnt=0;
			ans++;
			//cout<<i<<endl;
			idx++;
		}
		if(cnt==k)
		{
			cnt=0;
			//cout<<i<<endl;
			ans++;
		}
		cnt++;
	}
	cout<<ans<<endl;
	
	return 0;
}
```











