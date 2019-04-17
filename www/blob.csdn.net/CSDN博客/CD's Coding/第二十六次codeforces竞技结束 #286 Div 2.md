# 第二十六次codeforces竞技结束 #286 Div 2 - CD's Coding - CSDN博客





2015年01月28日 17:53:54[糖果天王](https://me.csdn.net/okcd00)阅读数：498












|[A](http://codeforces.com/contest/505/problem/A)|[Mr. Kitayuta's Gift](http://codeforces.com/contest/505/problem/A)standard input/output1 s, 256 MB|![Submit](http://codeforces.com/static/images/icons/submit-22x22.png)![Add to favourites](http://codeforces.com/static/images/icons/star_gray_16.png)|![](http://codeforces.com/static/images/icons/user.png) x2396|
|----|----|----|----|
|[B](http://codeforces.com/contest/505/problem/B)|[Mr. Kitayuta's Colorful Graph](http://codeforces.com/contest/505/problem/B)standard input/output1 s, 256 MB|![Submit](http://codeforces.com/static/images/icons/submit-22x22.png)![Add to favourites](http://codeforces.com/static/images/icons/star_gray_16.png)|![](http://codeforces.com/static/images/icons/user.png) x1618|
|[C](http://codeforces.com/contest/505/problem/C)|[Mr. Kitayuta, the Treasure Hunter](http://codeforces.com/contest/505/problem/C)standard input/output1 s, 256 MB|![Submit](http://codeforces.com/static/images/icons/submit-22x22.png)![Add to favourites](http://codeforces.com/static/images/icons/star_gray_16.png)|![](http://codeforces.com/static/images/icons/user.png) x456|






Mr. Kitayuta has kindly given you a string *s* consisting of lowercase English letters. You are asked to insert exactly one lowercase English letter into *s* to
 make it a palindrome. A palindrome is a string that reads the same forward and backward. For example, "noon",
 "testset" and "a" are all palindromes, while "test"
 and "kitayuta" are not.


You can choose any lowercase English letter, and insert it to any position of *s*, possibly to the beginning or the end of *s*.
 You have to insert a letter even if the given string is already a palindrome.


If it is possible to insert one lowercase English letter into *s* so that the resulting string will be a palindrome, print the string after the insertion.
 Otherwise, print "NA" (without quotes, case-sensitive). In case there is more than one palindrome that can be obtained, you are allowed to print any of them.




Input


The only line of the input contains a string *s* (1 ≤ |*s*| ≤ 10).
 Each character in *s* is a lowercase English letter.




Output


If it is possible to turn *s* into a palindrome by inserting one lowercase English letter, print the resulting string in a single line. Otherwise, print "NA"
 (without quotes, case-sensitive). In case there is more than one solution, any of them will be accepted.




Sample test(s)




input
revive




output
reviver




input
ee




output
eye



input
kitayuta




output
NA






Note


For the first sample, insert 'r' to the end of "revive" to
 obtain a palindrome "reviver".


For the second sample, there is more than one solution. For example, "eve" will also be accepted.


For the third sample, it is not possible to turn "kitayuta" into a palindrome by just inserting one letter.





给一个字符串，必须向里头添加一个字符，问能不能构成一个回文串，能的话输出回文串，不能的话输出NA。

我们定义*号为通配符，尝试在0到s.length()的每一个地方放置，每次放置后判断当前字符串是不是回文串（我们定义*号和任何一个字母都称作“相同”），如果当前字符串为回文串，我们把*号变为它对应的那个字母，然后输出当前字符串即可。

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
	string s,ans,st="*";	cin>>s;
	int len=s.length();
	for(int i=0;i<=len;i++)
	{
		bool f=true;
		ans=s.substr(0,i)+st+s.substr(i,len-i);
		for(int j=0;j<=ans.length()/2;j++)
		{
			if(ans[j]==ans[ans.length()-1-j]) continue;
			else if(ans[j]=='*') {ans[j]=ans[ans.length()-1-j];continue;}
			else if(ans[ans.length()-1-j]=='*') {ans[ans.length()-1-j]=ans[j];continue;}
			else
			{
				f=false;
				break;
			}
		} 
		if(f) {cout<<ans;return 0;} 
	} 
	cout<< "NA";
	return 0;
}
```




Mr. Kitayuta has just bought an undirected graph consisting of *n* vertices and *m* edges.
 The vertices of the graph are numbered from 1 to *n*. Each edge, namely edge *i*,
 has a color *c**i*,
 connecting vertex *a**i* and *b**i*.


Mr. Kitayuta wants you to process the following *q* queries.


In the *i*-th query, he gives you two integers — *u**i* and *v**i*.


Find the number of the colors that satisfy the following condition: the edges of that color connect vertex *u**i* and
 vertex *v**i* directly
 or indirectly.




Input


The first line of the input contains space-separated two integers — *n* and *m* (2 ≤ *n* ≤ 100, 1 ≤ *m* ≤ 100),
 denoting the number of the vertices and the number of the edges, respectively.


The next *m* lines contain space-separated three integers — *a**i*, *b**i* (1 ≤ *a**i* < *b**i* ≤ *n*)
 and *c**i* (1 ≤ *c**i* ≤ *m*).
 Note that there can be multiple edges between two vertices. However, there are no multiple edges of the same color between two vertices, that is, if *i* ≠ *j*, (*a**i*, *b**i*, *c**i*) ≠ (*a**j*, *b**j*, *c**j*).


The next line contains a integer — *q* (1 ≤ *q* ≤ 100),
 denoting the number of the queries.


Then follows *q* lines, containing space-separated two integers — *u**i* and *v**i* (1 ≤ *u**i*, *v**i* ≤ *n*).
 It is guaranteed that *u**i* ≠ *v**i*.




Output


For each query, print the answer in a separate line.




Sample test(s)




input
4 5
1 2 1
1 2 2
2 3 1
2 3 3
2 4 3
3
1 2
3 4
1 4




output
2
1
0




input
5 7
1 5 1
2 5 1
3 5 1
4 5 1
1 2 2
2 3 2
3 4 2
5
1 5
5 1
2 5
1 5
1 4




output
1
1
1
1
2






Note


Let's consider the first sample.
![](http://espresso.codeforces.com/d61cb1423342776f2295bc90083a0a84f5a70c56.png)The figure above shows the first sample.- 
Vertex 1 and vertex 2 are connected by color 1 and 2.
- 
Vertex 3 and vertex 4 are connected by color 3.
- 
Vertex 1 and vertex 4 are not connected by any single color.





依然是在点之间连好多线问能不能从S到T的问题，只是这道题它的连线有好多种颜色（最多100种），问的是有多少种颜色可以从S到T（指的是如果用1的颜色必须一直用1的颜色的路径）。

这道题反正颜色不多，开101个图不就行了嘛~

我们定义 map[i][j][k] 为第i种颜色的图中，从j节点到k节点有没有路可以过去。

然后……简单的bfs咯~

### Code：



```cpp
#include <cmath> 
#include <queue>
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
int n,m,q;
int ans[101]={0};
int map[101][101][101]={0};

int bfs(int j,int u,int v)
{
	int vis[101]={0};
	queue<int> q;
	while(!q.empty())q.pop();
	q.push(u);
	while(!q.empty())
	{
		int now=q.front();q.pop();
		if(now==v) return 1;
		vis[now]=1;
		for(int i=0;i<=n;i++)
		{
			if(map[j][now][i]==1 && vis[i]==0)
			{
				vis[i]==1;
				q.push(i);
			}
		}
	}
	return 0;
} 

int main()
{
	cin>>n>>m;
	for(int i=0;i<m;i++)
	{
		int a,b,c;	scanf("%d%d%d",&a,&b,&c);
		map[c][a][b]=1;
		map[c][b][a]=1;
	}
	cin>>q;
	for(int i=0;i<q;i++)
	{
		int tu,tv;	scanf("%d%d",&tu,&tv);
		int u=min(tu,tv),v=max(tu,tv);
		for(int j=1;j<=m;j++)
		{
			ans[i]+=bfs(j,u,v);
		}
		cout<<ans[i]<<endl;
	} 
	return 0;
}
```








The Shuseki Islands are an archipelago of 30001 small islands in the Yutampo Sea. The islands are evenly spaced along a line, numbered from 0 to 30000 from
 the west to the east. These islands are known to contain many treasures. There are *n* gems in the Shuseki Islands in total, and the *i*-th
 gem is located on island *p**i*.


Mr. Kitayuta has just arrived at island 0. With his great jumping ability, he will repeatedly perform jumps between islands to the east according to the following
 process:
- 
First, he will jump from island 0 to island *d*.
- 
After that, he will continue jumping according to the following rule. Let *l* be the length of the previous jump, that is, if his previous jump was from island *prev* to
 island *cur*, let *l* = *cur* - *prev*.
 He will perform a jump of length *l* - 1, *l* or *l* + 1 to
 the east. That is, he will jump to island (*cur* + *l* - 1), (*cur* + *l*) or (*cur* + *l* + 1) (if
 they exist). The length of a jump must be positive, that is, he cannot perform a jump of length 0 when *l* = 1.
 If there is no valid destination, he will stop jumping.


Mr. Kitayuta will collect the gems on the islands visited during the process. Find the maximum number of gems that he can collect.




Input


The first line of the input contains two space-separated integers *n* and *d* (1 ≤ *n*, *d* ≤ 30000),
 denoting the number of the gems in the Shuseki Islands and the length of the Mr. Kitayuta's first jump, respectively.


The next *n* lines describe the location of the gems. The *i*-th
 of them (1 ≤ *i* ≤ *n*) contains a integer *p**i* (*d* ≤ *p*1 ≤ *p*2 ≤ ... ≤ *p**n* ≤ 30000),
 denoting the number of the island that contains the *i*-th gem.




Output


Print the maximum number of gems that Mr. Kitayuta can collect.




Sample test(s)




input
4 10
10
21
27
27




output
3




input
8 8
9
19
28
36
45
55
66
78




output
6




input
13 7
8
8
9
16
17
17
18
21
23
24
24
26
30




output
4






Note


In the first sample, the optimal route is 0  →  10 (+1 gem)  →  19  →  27
 (+2 gems)  → ...


In the second sample, the optimal route is 0  →  8  →  15  →  21 →  28
 (+1 gem)  →  36 (+1 gem)  →  45 (+1 gem)  →  55
 (+1 gem)  →  66 (+1 gem)  →  78 (+1 gem)  → ...


In the third sample, the optimal route is 0  →  7  →  13  →  18
 (+1 gem)  →  24 (+2 gems)  →  30 (+1 gem)  → ...





有一个数轴，从0到30000，这上面分布着n个宝石（位置在下方数据中表示，分别处于某个给出的点），你的第一步必须走p个单位长度，此后每一步可以选择走p+1或者p或者p-1个单位长度（但是至少为1），问最多可以获得多少个宝石。

这道题本来想，阿拉不就是简单的dfs么，然后看到了30000，以及每一步有3种分支……简直是当场吓哭的节奏……就去默默的推dp直至结束。结束之后看别人的AC代码……一口老血喷出来……DFS居然不会TLE么？！！！！ 好吧我胆子太小了……

### Code：



```cpp
#include <bits/stdc++.h>
using namespace std;
int gem[99999] = {}, dp[35000][500], d, x;

int dfs(int a, int b) {
	if(dp[a][b] + 1) return dp[a][b];
	int res = 0, i, v;
	for(i = -1; i < 2; i++) 
	{
		v = a + d + b + i;
		if(v > 30001 | v <= a) continue;
		res = max(res, dfs(v, b + i));
	}
	return dp[a][b] = res + gem[a];
}

int main() {
	cin >> x >> d;
	while(cin >> x)	gem[x]++;
	memset(dp, -1, sizeof dp);
	cout << dfs(d, 0);
	return 0;
}
```







