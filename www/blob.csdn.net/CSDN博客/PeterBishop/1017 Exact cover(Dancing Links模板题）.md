# 1017 Exact cover(Dancing Links模板题） - PeterBishop - CSDN博客





2018年12月27日 08:47:46[PeterBishop0](https://me.csdn.net/qq_40061421)阅读数：31
个人分类：[HUST](https://blog.csdn.net/qq_40061421/article/category/8563492)









```cpp
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>
#include <set>
#include <map>
#include <string>
#include <math.h>
#include <stdlib.h>
#include <time.h>
using namespace std;
const int maxnode = 100010;
const int MaxM = 1010;
const int MaxN = 1010;
struct DLX
{
	int n, m, size;//n是行，m是列，size是个数，也是节点标号
	int U[maxnode], D[maxnode], R[maxnode], L[maxnode], Row[maxnode], Col[maxnode];
	int H[MaxN], S[MaxM];//H是行首指针，S是该列结点总数
	int ansd, ans[MaxN];//节点总数，答案
	void init(int _n, int _m)
	{
		n = _n;
		m = _m;
		for (int i = 0; i <= m; i++)
		{
			S[i] = 0;
			U[i] = D[i] = i;
			L[i] = i - 1;
			R[i] = i + 1;
		}
		R[m] = 0; L[0] = m;
		size = m;
		for (int i = 1; i <= n; i++)
			H[i] = -1;
	}
	void Link(int r, int c)
	{
		++S[Col[++size] = c];//记录节点，双十字连接
		Row[size] = r;
		D[size] = D[c];//同时利用传入的点为后面的点提供更新（对于列而言）
		U[D[c]] = size;
		U[size] = c;
		D[c] = size;//迭代U和D首尾相连

		if (H[r] < 0)
			H[r] = L[size] = R[size] = size;
		else
		{
			R[size] = R[H[r]];//LR加入新节点，看图很好理解，动态链表以当前元素为跟来构建新链表，相对顺序不变，又知道首节点，那么顺序就知道了
			L[R[H[r]]] = size;
			L[size] = H[r];
			R[H[r]] = size;
		}
	}
	//对某一列进行删除 同时删除列中为1的行
	void remove(int c)
	{
		L[R[c]] = L[c]; R[L[c]] = R[c];
		for (int i = D[c]; i != c; i = D[i])
		{
			for (int j = R[i]; j != i; j = R[j])
			{
				U[D[j]] = U[j];
				D[U[j]] = D[j];
				--S[Col[j]];
			}
		}
	}
	//反着恢复状态
	void resume(int c)
	{
		for (int i = U[c]; i != c; i = U[i])
			for (int j = L[i]; j != i; j = L[j])
				++S[Col[U[D[j]] = D[U[j]] = j]];
		L[R[c]] = R[L[c]] = c;
	}
	//d为递归深度
	bool Dance(int d)
	{
		if (R[0] == 0)
		{
			ansd = d;
			return true;
		}
		int c = R[0];
		//一个优化 找到列中包含1最多的列 因为这样有助于减少递归深度 （很显然1多了 删掉的行也多 矩阵缩小得就快）
		for (int i = R[0]; i != 0; i = R[i])
			if (S[i] < S[c])
				c = i;
		remove(c);
		//搜索
		for (int i = D[c]; i != c; i = D[i])
		{
			ans[d] = Row[i];
			for (int j = R[i]; j != i; j = R[j])
				remove(Col[j]);
			if (Dance(d + 1))
				return true;
			for (int j = L[i]; j != i; j = L[j])
				resume(Col[j]);
		}
		resume(c);
		return false;
	}
};

DLX g;
int main()
{
	int n, m;
	while (scanf("%d%d", &n, &m) == 2)
	{
		g.init(n, m);
		for (int i = 1; i <= n; i++)
		{
			int num, j;
			scanf("%d", &num);
			while (num--)
			{
				scanf("%d", &j);
				g.Link(i, j);
			}
		}
		if (!g.Dance(0))printf("NO\n");
		else
		{
			printf("%d", g.ansd);
			for (int i = 0; i < g.ansd; i++)
				printf(" %d", g.ans[i]);
			printf("\n");
		}
	}
	return 0;
}
```





