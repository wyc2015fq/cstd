# 【巧模拟】#75 A. Chips - CD's Coding - CSDN博客





2014年10月29日 17:51:49[糖果天王](https://me.csdn.net/okcd00)阅读数：618
所属专栏：[CF-A Terminator](https://blog.csdn.net/column/details/cf-amaster.html)


















There are *n* walruses sitting in a circle. All of them are numbered in the clockwise order: the walrus number 2 sits
 to the left of the walrus number 1, the walrus number 3 sits
 to the left of the walrus number 2, ..., the walrus number 1 sits
 to the left of the walrus number *n*.


The presenter has *m* chips. The presenter stands in the middle of the circle and starts giving the chips to the walruses starting from walrus
 number 1 and moving clockwise. The walrus number *i* gets *i* chips.
 If the presenter can't give the current walrus the required number of chips, then the presenter takes the remaining chips and the process ends. Determine by the given *n* and *m* how
 many chips the presenter will get in the end.




Input


The first line contains two integers *n* and *m* (1 ≤ *n* ≤ 50, 1 ≤ *m* ≤ 104)
 — the number of walruses and the number of chips correspondingly.




Output


Print the number of chips the presenter ended up with.




Sample test(s)




input
4 11




output
0




input
17 107




output
2




input
3 8




output
1






Note


In the first sample the presenter gives one chip to the walrus number 1, two chips to the walrus number 2,
 three chips to the walrus number 3, four chips to the walrus number 4,
 then again one chip to the walrus number 1. After that the presenter runs out of chips. He can't give anything to the walrus number 2 and
 the process finishes.


In the third sample the presenter gives one chip to the walrus number 1, two chips to the walrus number 2,
 three chips to the walrus number 3, then again one chip to the walrus number 1.
 The presenter has one chip left and he can't give two chips to the walrus number2, that's why the presenter takes the last chip.













有n个小盆友坐成一圈等着发薯条吃，编号为1-n，每个人发的薯条数为他们的编号，问这么一直发下去最后到某个地方发不下去了的时候剩几根薯条。

因为我们一圈一圈发，而每一圈的薯条数是一定的，故可以直接除以一整圈的薯条数，直接拿余数来模拟（因为这些都被之前的整圈发完了，肯定在最后一圈的时候停下）

代码如下：



```cpp
#include <cstdio>
#include <string>
#include <cstring> 
#include <iostream>
#include <algorithm>
using namespace std;
typedef long long ll;
// http://codeforces.com/contest/92
// Chips
int main()
{
	int n,m;	cin>>n>>m;
	int sum=(1+n)*n/2;
	m=m%sum;
	for(int i=1;i<=n;i++)
	{
		if(m>=i) m-=i;
		else break;
	}
	cout<<m;
	return 0;
}
```











