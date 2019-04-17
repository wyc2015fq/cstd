# 10474 Where is the Marble?（sort） - PeterBishop - CSDN博客





2018年07月11日 10:55:17[PeterBishop0](https://me.csdn.net/qq_40061421)阅读数：16
个人分类：[UVA](https://blog.csdn.net/qq_40061421/article/category/7793583)









```cpp
#include<cstdio>//这题看懂题目就好了
#include<algorithm>
using namespace std;
const int maxn = 10000;
int main()
{
	int n, q, x, a[maxn], kcase = 0;
	while (scanf("%d%d", &n, &q) == 2 && n)
	{
		printf("CASE# %d:\n", ++kcase);
		for (int i = 0; i < n; i++)
			scanf("%d", &a[i]);
		sort(a, a + n);
		while (q--)

		{
			scanf("%d", &x);
			int p = lower_bound(a, a + n, x) - a;//二分寻找位置
			if (a[p] == x)
				printf("%d found at %d\n", x, p + 1);
			else
				printf("%d not found\n", x);
		}
	}
	return 0;
}
```




