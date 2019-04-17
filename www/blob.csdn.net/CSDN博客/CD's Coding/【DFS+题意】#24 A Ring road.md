# 【DFS+题意】#24 A. Ring road - CD's Coding - CSDN博客





2014年05月31日 17:33:11[糖果天王](https://me.csdn.net/okcd00)阅读数：592标签：[codeforces																[水题](https://so.csdn.net/so/search/s.do?q=水题&t=blog)](https://so.csdn.net/so/search/s.do?q=codeforces&t=blog)
个人分类：[CF-A题终结者](https://blog.csdn.net/okcd00/article/category/2156945)

所属专栏：[CF-A Terminator](https://blog.csdn.net/column/details/cf-amaster.html)

















Nowadays the one-way traffic is introduced all over the world in order to improve driving safety and reduce traffic jams. The government of Berland decided to keep up with new trends. Formerly all *n* cities
 of Berland were connected by *n* two-way roads in the ring, i. e. each city was connected directly to exactly two other cities, and from each city it was
 possible to get to any other city. Government of Berland introduced one-way traffic on all *n* roads, but it soon became clear that it's impossible to get
 from some of the cities to some others. Now for each road is known in which direction the traffic is directed at it, and the cost of redirecting the traffic. What is the smallest amount of money the government should spend on the redirecting of roads so that
 from every city you can get to any other?




Input


The first line contains integer *n* (3 ≤ *n* ≤ 100) —
 amount of cities (and roads) in Berland. Next *n* lines contain description of roads. Each road is described by three integers *a**i*, *b**i*, *c**i* (1 ≤ *a**i*, *b**i* ≤ *n*, *a**i* ≠ *b**i*, 1 ≤ *c**i* ≤ 100)
 — road is directed from city *a**i*to
 city *b**i*, redirecting
 the traffic costs *c**i*.




Output


Output single integer — the smallest amount of money the government should spend on the redirecting of roads so that from every city you can get to any other.




Sample test(s)




input
3
1 3 1
1 2 1
3 2 1




output
1




input
3
1 3 1
1 2 5
3 2 1




output
2




input
6
1 5 4
5 3 8
2 4 15
1 6 16
2 3 23
4 6 42




output
39




input
4
1 2 9
2 3 8
3 4 7
4 1 5




output
0












我还是想问，这个不是建立联系表么……为啥这道题要算作是dfs啊……还是说这个就是dfs的意思么……



这道题是要求形成一个单向环（最初吓了我一跳，还以为是图论题，吾辈最怕图论了……），后来发现……只需要一条路找下去，不行的就花钱逆回来，然后看看正着便宜还是倒着便宜就行了。






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

int  n=0,pos=0,ans1=0,ans2=0;
bool vis	[111];
int  chain	[111];
int  cost	[111][111];

#define Max(a,b) ((a)>(b)?(a):(b))
#define Min(a,b) ((a)<(b)?(a):(b))

void dfs(int now)// make a linked chain of nodes
{
	vis[now]=true;
	chain[pos++]=now;
	for(int k=1;k<=n;k++)// find next
	{
		if(vis[k]==false && (cost[k][now]>0 || cost[now][k]>0))
		dfs(k);
	}
}

int main()
{
	memset(cost,0,sizeof(cost));
	memset(vis,false,sizeof(vis));
	int total_cost=0;
	scanf("%d",&n);
	for(int i=0;i<n;i++)
	{
		int _start=0,_end=0;
		scanf("%d%d",&_start,&_end);
		scanf("%d",&cost[_start][_end]);
		total_cost+=cost[_start][_end];
	}
	dfs(1);// start from 1,make a chain until 1 ends
	chain[pos]=1;// when end, we get back to 1
	for(int j=0;j<pos;j++)		ans1+= cost[chain[j]][chain[j+1]];
	for(int j=pos;j>=1;j--)		ans2+= cost[chain[j]][chain[j-1]];
	//cout<<total_cost<<endl<<ans1<<endl<<ans2<<endl;
	printf("%d",Min(ans1,ans2));
	return 0;
}
```









