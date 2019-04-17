# 第十四次codeforces竞技结束 #266 Div 2 - CD's Coding - CSDN博客





2014年09月29日 13:00:33[糖果天王](https://me.csdn.net/okcd00)阅读数：535








完败…… 掉了101Rank……

前一天是网赛，下午有网赛，当晚比赛的时候实在是僵尸一般的困死了……

哭……C还fst了……只剩一个A让我怎么活……

自然还是献上3题的解题报告——










Ann has recently started commuting by subway. We know that a one ride subway ticket costs *a* rubles. Besides, Ann found out that she can buy a special ticket
 for *m* rides (she can buy it several times). It costs*b* rubles.
 Ann did the math; she will need to use subway *n* times. Help Ann, tell her what is the minimum sum of money she will have to spend to make *n* rides?




Input


The single line contains four space-separated integers *n*, *m*, *a*, *b* (1 ≤ *n*, *m*, *a*, *b* ≤ 1000)
 — the number of rides Ann has planned, the number of rides covered by the *m* ride ticket, the price of a one ride ticket and the price of an *m* ride
 ticket.




Output


Print a single integer — the minimum sum in rubles that Ann will need to spend.




Sample test(s)




input
6 2 1 2




output
6




input
5 2 2 3




output
8






Note


In the first sample one of the optimal solutions is: each time buy a one ride ticket. There are other optimal solutions. For example, buy three *m* ride tickets.










这题呢是说有个人想要去n个地方旅游，有单次票一次去一个地方要花a元，也有联票一次去m个地方，花b元，问去n个地方都旅游完最少花多少钱。

之前我想的是可能要刚好去n个地方吧，然后给想复杂了，这题的题意是说旅游到了n个就行了，就是多了也成，那么——

对于m个地方，是单程还是联票划算，讨论一下，然后m的整数倍都这么来，最后剩下的几个地方再讨论下就可以了

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

int main()
{
	int n,m,a,b;
	cin>>n>>m>>a>>b;
	cout<<min(a*n,min((n/m+1)*b,((n+m)%m)*a+(n/m)*b))<<endl;
	return 0;
}
```











The start of the new academic year brought about the problem of accommodation students into dormitories. One of such dormitories has a *a* × *b* square
 meter wonder room. The caretaker wants to accommodate exactly *n* students there. But the law says that there must be at least 6 square meters per student
 in a room (that is, the room for *n* students must have the area of at least 6*n* square
 meters). The caretaker can enlarge any (possibly both) side of the room by an arbitrary positive integer of meters. Help him change the room so as all*n* students
 could live in it and the total area of the room was as small as possible.




Input


The first line contains three space-separated integers *n*, *a* and *b* (1 ≤ *n*, *a*, *b* ≤ 109)
 — the number of students and the sizes of the room.




Output


Print three integers *s*, *a*1 and *b*1(*a* ≤ *a*1; *b* ≤ *b*1) —
 the final area of the room and its sizes. If there are multiple optimal solutions, print any of them.




Sample test(s)




input
3 3 5




output
18
3 6




input
2 4 4




output
16
4 4












有一个aXb大小的房间，需要给n个人住（即需要至少6n的面积）问长宽各扩张成多长才行



这道题实际上是找i*j>=6n,i>a,j>b的i*j最小整数解。遍历肯定是NodoNodie。

先挑出两个长度之间较小的边记作a，另一个叫他b，先考虑a，很明显我们可以遍历a的范围是a~n/a(除不尽的话+1)，对于每个a我们来考虑对应这个a，b应该的值（当然如果刚好就是6n直接输出finish），每次刷新当前离6n最近的面积s记录下来。

啊过不去的可以试试这个: 我当时就会有一组数据过不去 4 7 30,是怎么回事呢，细节问题~ 比如那个+1,

代码如下

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

ll n, a, b, s, x, y, t;

int main()
{
    cin>>n>>a>>b;
    n*=6;  s = -1;
    if(a*b>=n) 
	{
        s = a*b;
        x = a; y = b;
    }
    else 
	{
    	if(a<b) 
		{
			for(ll i=a; i*i<=n; i++) 
			{
        	    t = max(n%i==0?n/i:n/i+1,b);
        	    if(i*t<s||s<0) {x=i; y=t; s=x*y;}
        	}
    	}
    	else 
		{
        	for(ll i=b; i*i<=n; i++) 
			{
        	    t = max(n%i==0?n/i:n/i+1,a);
        	    if(t*i<s||s<0) {y=i; x=t; s=x*y;}
        	}
      	}	
    }
    cout<<s<<endl<<x<<' '<<y<<endl;
	return 0;
}
```














You've got array *a*[1], *a*[2], ..., *a*[*n*], consisting of *n* integers.
 Count the number of ways to split all the elements of the array into three contiguous parts so that the sum of elements in each part is the same.


More formally, you need to find the number of such pairs of indices *i*, *j*(2 ≤ *i* ≤ *j* ≤ *n* - 1),
 that ![](http://espresso.codeforces.com/669a2f09a3b9e143f54b1f1d9fd6b7dddf403680.png).




Input


The first line contains integer *n*(1 ≤ *n* ≤ 5·105),
 showing how many numbers are in the array. The second line contains *n* integers *a*[1],*a*[2],
 ..., *a*[*n*](|*a*[*i*]| ≤  109) —
 the elements of array *a*.




Output


Print a single integer — the number of ways to split the array into three parts with the same sum.




Sample test(s)




input
5
1 2 3 0 3




output
2




input
4
0 1 -1 0




output
1




input
2
4 1




output
0















这题是问，有一串数字，我们把它分为三段，三段的和相等，有多少种分法。

首先连3个数字都没有的串串自然是边儿呆着去了

然后我们先算出个全和，然后1/3的点和2/3的点的值就知道了不是么~

然后找出所有这些点，最后……1/3的点在2/3的点左边就行咯（千万注意有的时候和为0的时候别让其中某一段没有分配到数字）~

然后为了加速用了下dp~

代码如下

### Code：



```cpp
#include <cstdio>
#include <vector>
#include <cstring>
#include <iostream>
using namespace std;
#define maxn 500010
typedef long long ll;
ll a[maxn], n, ls[maxn], rs[maxn], l[maxn], r[maxn], s;

int main()
{
	cin>>n; 
	for(int i=1; i<=n; i++) cin>>a[i], s+=a[i];
    if(s%3) 
	{
        cout<<0<<endl;
        return 0;
    }
    s/=3;
    for(int i=1; i<=n; i++) 
	{
        ls[i] = ls[i-1] + a[i];
        if(ls[i]==s) l[i] = l[i-1] + 1;
        else l[i] = l[i-1];
    }
    ll ans = 0;
    for(int i=n; i>0; i--) 
	{
        rs[i] = rs[i+1] + a[i];
        if(rs[i]==s&&i>2) ans += l[i-2];
    }
    cout<<ans<<endl;
	return 0;
}
```



































