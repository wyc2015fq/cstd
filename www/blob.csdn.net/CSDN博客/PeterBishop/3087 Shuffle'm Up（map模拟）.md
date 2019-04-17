# 3087 Shuffle'm Up（map模拟） - PeterBishop - CSDN博客





2018年07月26日 22:09:13[PeterBishop0](https://me.csdn.net/qq_40061421)阅读数：16
个人分类：[POJ](https://blog.csdn.net/qq_40061421/article/category/7589386)









```cpp
#include<cstdio>
#include<cstring>
#include<iostream>
#include<algorithm>
#include<map>
#include<string>
using namespace std;
int t;
string a, b, c, p;
map<string, int>node;//用来记录重复
bool bfs()
{
	int i;
	c = "";
	for (i = 0; i < t; i++)
	{
		c = c + b[i];
		c = c + a[i];
	}
	if (node[c])
	{
		node[c]++;
		return false;
	}
	else{
		node[c]++;
		for (i = 0; i < t; i++)
			a[i] = c[i];
		for (i = t; i <= 2 * t - 1; i++)
			b[i-t] = c[i];
		return true;
	}
}
int main()
{
	int n, i, j, ans, N;
	cin >> n;
	N = 1;
	while (N<=n)
	{
		N++;
		cin >> t;
		getchar();
		cin >> a;
		getchar();
		cin >> b;
		getchar();
		cin >> p;
		node[p]++;
		for (i = 1;; i++)
		{
			if (!bfs())break;
		}
		ans = i;
		if (node[p] == 2)
			cout << N-1 << " " << ans << endl;
		else
			cout << N-1 << " " << "-1" << endl;
	}
	return 0;
}
```





