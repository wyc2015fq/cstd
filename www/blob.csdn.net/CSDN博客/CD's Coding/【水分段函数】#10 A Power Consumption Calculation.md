# 【水分段函数】#10 A.Power Consumption Calculation - CD's Coding - CSDN博客





2014年04月15日 17:13:36[糖果天王](https://me.csdn.net/okcd00)阅读数：801
所属专栏：[CF-A Terminator](https://blog.csdn.net/column/details/cf-amaster.html)


















Tom is interested in power consumption of his favourite laptop. His laptop has three modes. In normal mode laptop consumes *P*1 watt
 per minute. *T*1 minutes
 after Tom moved the mouse or touched the keyboard for the last time, a screensaver starts and power consumption changes to *P*2 watt
 per minute. Finally, after *T*2 minutes
 from the start of the screensaver, laptop switches to the "sleep" mode and consumes *P*3 watt
 per minute. If Tom moves the mouse or touches the keyboard when the laptop is in the second or in the third mode, it switches to the first (normal) mode. Tom's work with the laptop can be divided into *n* time
 periods [*l*1, *r*1], [*l*2, *r*2], ..., [*l**n*, *r**n*].
 During each interval Tom continuously moves the mouse and presses buttons on the keyboard. Between the periods Tom stays away from the laptop. Find out the total amount of power consumed by the laptop during the period [*l*1, *r**n*].




Input


The first line contains 6 integer numbers *n*, *P*1, *P*2, *P*3, *T*1, *T*2 (1 ≤ *n* ≤ 100, 0 ≤ *P*1, *P*2, *P*3 ≤ 100, 1 ≤ *T*1, *T*2 ≤ 60).
 The following *n* lines contain description of Tom's work. Each *i*-th
 of these lines contains two space-separated integers *l**i* and *r**i* (0 ≤ *l**i* < *r**i* ≤ 1440, *r**i* < *l**i* + 1 for *i* < *n*),
 which stand for the start and the end of the *i*-th period of work.




Output


Output the answer to the problem.




Sample test(s)




input
1 3 2 1 5 10
0 10




output
30



input
2 8 4 2 5 10
20 30
50 100




output
570














这道题叫“功耗计算”，大概意思呢是告诉你这么件事——

当你afk时，电脑的耗电大概是这么回事：




然而期间只要你动了一下电脑，立马从零开始计时。问你这个Tom在这么几个阶段里耗了多少电。

嘛~ 大概意思就是个水题啦~^_^  函数分定义域计算的问题似乎是初中……还是小学来着的……那个什么出租车开车算路费的问题吧~




Python：



```python
# input
list = raw_input().split()
n,p1,p2,t1,t2,t3 = map(int , list)

# solve
ans = 0
pre = -1

while n > 0:
    n -= 1
    list = raw_input().split()
    start,end = map(int , list)
    ans += (end-start)*p1
    
    if pre != -1:
       x = start-pre
       if x > t1:
          ans += t1*p1
          x -= t1
          if x > t2:
             ans += t2*p2
             x -= t2
             ans += x*p3
          else:
             ans += x*p2
       else:
          ans += x*p1 
    pre = end

print ans
```











C++：



```cpp
#include <iostream>
#include <cstdio>
#include <algorithm>
using namespace std;


int main()
{
    int n,p1,p2,p3,t1,t2 ;
    int l,r,l2,r2;
	scanf("%d%d%d%d%d%d",&n,&p1,&p2,&p3,&t1,&t2);
	scanf("%d%d", &l, &r);
	int sum = (r - l) * p1;
	while(--n)
	{
		scanf("%d%d",&l2,&r2);
        int mid = l2 - r ;
	    int p1pow = t1 * p1, p2pow = t2 * p2;
	    if (mid <= t1)
			sum += mid * p1;
	    else
	    {
		 sum += p1pow;
		 mid -= t1;
		 if (mid <= t2)
			sum += mid * p2;
		 else
			sum += p2pow + (mid - t2) * p3;
        }
	    sum += (r2 - l2) * p1;
	    l = l2;
	    r = r2;
	}
	 printf("%d",sum);	
	 return 0;
}
```











