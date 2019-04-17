# Codeforces Round #503（Div.2) 1020A  1020B 1019A总结 - PeterBishop - CSDN博客





2018年08月12日 15:13:47[PeterBishop0](https://me.csdn.net/qq_40061421)阅读数：97








A题我肿么会脑残一开始去搜索呢？明摆着只要分情况算就好了啊不说了真的傻了

分清楚两个y的相对位置

```cpp
#include<stdio.h>

int abs(int a)
{
	if (a > 0)
		return a;
	return -a;
}
int main()
{
	int n, h, a, b, k;
	while (~scanf("%d %d %d %d %d", &n, &h, &a, &b, &k))
	{

		while (k--)
		{
			
			int bx, by, ex, ey;
			scanf("%d %d %d %d", &bx, &by, &ex, &ey);
			if (bx > ex)
			{
				int t = bx;
				bx = ex;
				ex = t;
				t = by;
				by = ey;
				ey = t;
			}
			if (bx == ex)
				printf("%d\n", abs(ey - by));
			else
			{
				if (by >= b&&ey >= b)
					printf("%d\n", by - b + ex - bx + ey - b);
				else if (by <= a&&ey <= a)
					printf("%d\n", a - by + ex - bx + a - ey);
				else
					printf("%d\n", ex - bx + abs(ey - by));
			}
		}
	}
}
```

B题暴力但是一开始看不懂题意很难受啊！主要是输入输出的要求要看懂，还是要多读题啊！暴力模拟

```cpp
#include<stdio.h>
#include<string.h>
int n;
int a[1001];
int count[1001];
int main()
{
	while (~scanf("%d", &n))
	{
		memset(a, 0, sizeof(a));
		
		for (int i = 1; i <= n; i++)
			scanf("%d", &a[i]);
		for (int i = 1; i <= n; i++)
		{
			memset(count, 0, sizeof(count));
			int index = i;
			while (true)
			{
				count[index]++;
				if (count[index] == 2)
					break;
				index = a[index];
			}
			if(i==n)
				printf("%d\n", index);
			else
				printf("%d ", index);
		}
	}
}
```

C题又是一道暴力模拟，题意很好理解的，贪心就好了，但是数组和优先队列的运用给满分！

```cpp
#include <queue>
#include <cstdio>
#include <algorithm>
#include<functional>
using namespace std;

vector<int> v[3008];

int main() 
{
	int n, m, p, c;
	scanf("%d%d", &n, &m);
	for (int i = 0; i < n; i++) 
	{
		scanf("%d%d", &p, &c);
		v[p].push_back(c);//相当于二维数组
	}
	for (int i = 2; i <= m; i++)
		sort(v[i].begin(), v[i].end(), greater<int>());//由大到小排列
	long long ans = 1ll << 60;
	for (int tar = v[1].size(); tar <= n; tar++)//每次增加一个人去尝试
	{
		long long cost = 0; 
		int vot = v[1].size();
		priority_queue<int, vector<int>, greater<int>> q;//从小到大的顺序
		for (int i = 2; i <= m; i++)
		{
			for (int j = 0; j < v[i].size(); j++)
			{
				if (j < tar - 1)
					q.push(v[i][j]);
				else
					vot++, cost += v[i][j];
			}
		}
		while (vot++ < tar) //如果投票人还是不够，再从队列里面取
			cost += q.top(), q.pop();
		if (ans > cost) 
			ans = cost;
	}
	printf("%I64d\n", ans);
	return 0;
}
```





