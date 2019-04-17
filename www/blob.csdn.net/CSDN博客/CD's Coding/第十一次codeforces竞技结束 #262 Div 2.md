# 第十一次codeforces竞技结束 #262 Div 2 - CD's Coding - CSDN博客





2014年09月07日 20:38:16[糖果天王](https://me.csdn.net/okcd00)阅读数：530








似乎就是这场没打好，然后262-263都没更新……抱歉…… 262当场只出了2题……

趁着现在#265赛前来写一下题解^_^











Vasya has *n* pairs of socks. In the morning of each day Vasya has to put on a pair of socks before he goes to school. When he comes home in the evening, Vasya
 takes off the used socks and throws them away. Every *m*-th day (at days with numbers *m*, 2*m*, 3*m*, ...)
 mom buys a pair of socks to Vasya. She does it late in the evening, so that Vasya cannot put on a new pair of socks before the next day. How many consecutive days pass until Vasya runs out of socks?




Input


The single line contains two integers *n* and *m*(1 ≤ *n* ≤ 100; 2 ≤ *m* ≤ 100),
 separated by a space.




Output


Print a single integer — the answer to the problem.




Sample test(s)




input
2 2




output
3




input
9 3




output
13






Note


In the first sample Vasya spends the first two days wearing the socks that he had initially. Then on day three he puts on the socks that were bought on day two.


In the second sample Vasya spends the first nine days wearing the socks that he had initially. Then he spends three days wearing the socks that were bought on the third, sixth and ninth days. Than he spends another day wearing the socks that were bought on
 the twelfth day.





这熊孩子原先有n双袜子，每天消耗一双，每m天他的麻麻给他买一双，问——他能坚持多少天……

数据才100，当然是模拟啦^_^

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
	int n,m;	cin>>n>>m;
	int day=0;
	while(n)
	{
		day++, n--;
		if(day!=0 && day%m==0) n++;	
	}
	cout<< day <<endl;
	return 0;
}
```









Little Dima misbehaved during a math lesson a lot and the nasty teacher Mr. Pickles gave him the following problem as a punishment.


Find all integer solutions *x*(0 < *x* < 109) of
 the equation:
*x* = *b*·*s*(*x*)*a* + *c*, 

where *a*, *b*, *c* are
 some predetermined constant values and function *s*(*x*) determines the sum of all digits in the decimal representation of number *x*.


The teacher gives this problem to Dima for each lesson. He changes only the parameters of the equation: *a*, *b*, *c*.
 Dima got sick of getting bad marks and he asks you to help him solve this challenging problem.




Input


The first line contains three space-separated integers: *a*, *b*, *c*(1 ≤ *a* ≤ 5; 1 ≤ *b* ≤ 10000;  - 10000 ≤ *c* ≤ 10000).




Output


Print integer *n* — the number of the solutions that you've found. Next print *n* integers
 in the increasing order — the solutions of the given equation. Print only integer solutions that are larger than zero and strictly less than 109.




Sample test(s)




input
3 2 8




output
3
10 2008 13726 



input
1 2 -18




output
0




input
2 2 -1




output
4
1 31 337 967 














*x* = *b*·*s*(*x*)*a* + *c*, 


有这么一个式子，问你给你a,b,c了之后，有多少满足条件的x存在

这个很巧妙的哦~千万别想着枚举x，再怎么简直都活不过去……1e9还得了。

仔细想想哦~，s(x)是不是很少呀？

这题阻碍大家AC的几个不能算坑的坑如下：（这题简直成为hacker涨分的利器了，各种B题Hack一片的）

1. 1e9是九九八十一不是八九七十二哦~

2. 大家好我叫longlong我又坑到人了

3. 对0次方以及0的n次方的处理

### Code:



```cpp
#include <cmath> 
#include <queue>
#include <vector>
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
ll a,b,c;
ll vpow[88][8];
priority_queue< int,vector<int>,greater<int> > pq;

int s(ll x)
{
	ll tmp=x;
	ll ret=0;
	while(tmp)
	{
		ret+=tmp%10;
		tmp/=10;
	} 
	return ret;
}

void init()
{
	while(!pq.empty())pq.pop();
	for(int i=1;i<=81;i++)
	{
		vpow[i][1]=i;
		for(int j=2;j<=5;j++)
			vpow[i][j]=vpow[i][j-1]*i;
	}
	
}

void solve()
{
	ll temp;
	for(int i=1;i<=81;i++)
	{
		temp=(ll)vpow[i][a]*(ll)b+(ll)c;
		if(s(temp)==i && temp>0 && temp<(int)1e9) 
		{
			int tmp=vpow[i][a]*b+c;
			pq.push(tmp);
		}
	}
	if(pq.empty())	printf("0\n");
	else
	{
		printf("%d\n",pq.size());
		while(!pq.empty())
		{
			int a=pq.top();
			printf("%d ",a);
			pq.pop(); 
		}
	}
}

int main()
{
	cin>>a>>b>>c; 
	init();
	solve();
	return 0;
}
```











