# 【水hash】#27 A. Next Test - CD's Coding - CSDN博客





2014年06月05日 17:28:52[糖果天王](https://me.csdn.net/okcd00)阅读数：603标签：[水题																[hash																[codeforces](https://so.csdn.net/so/search/s.do?q=codeforces&t=blog)
个人分类：[CF-A题终结者](https://blog.csdn.net/okcd00/article/category/2156945)

所属专栏：[CF-A Terminator](https://blog.csdn.net/column/details/cf-amaster.html)](https://so.csdn.net/so/search/s.do?q=hash&t=blog)













«Polygon» is a system which allows to create programming tasks in a simple and professional way. When you add a test to the problem, the corresponding form asks you for the test index. As in most cases it is clear which index the next test will have, the system
 suggests the default value of the index. It is calculated as the smallest positive integer which is not used as an index for some previously added test.


You are to implement this feature. Create a program which determines the default index of the next test, given the indexes of the previously added tests.




Input


The first line contains one integer *n* (1 ≤ *n* ≤ 3000)
 — the amount of previously added tests. The second line contains *n* distinct integers *a*1, *a*2, ..., *a**n* (1 ≤ *a**i* ≤ 3000)
 — indexes of these tests.




Output


Output the required default value for the next test index.




Sample test(s)




input
3
1 7 2




output
3















这道题是一个hash表的问题，可以作为理解hash的一道入门题

题意是说：给一系列互不相同的数字，从小到大找第一个没有出现的数字。

我们就可以开一个bool型（int型用0/1也是一个道理）的数组mark[i]来记忆i这个数字有没有出现过，然后读完了之后遍历，第一个false的就是所求的数字了

代码如下：



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

bool mark[3001];

int main()
{
	memset(mark,false,sizeof(mark));
	int n=0;
	cin>>n;
	for(int i=1;i<=n;i++)
	{
		int now;
		cin>>now;
		mark[now]=true;
	}
	
	for(int j=1;j<=n+1;j++)
	{
		//cout<<mark[j];
		if(mark[j]==false)
		{
			cout<<j;
			return 0;
		}
	}
	return 0;
}
```](https://so.csdn.net/so/search/s.do?q=水题&t=blog)




