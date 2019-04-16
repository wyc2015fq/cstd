# 1043 Eight（八数码问题 康托展开 A*算法） - PeterBishop - CSDN博客





2018年08月22日 21:15:09[PeterBishop0](https://me.csdn.net/qq_40061421)阅读数：48








### 首先介绍一下A*算法：

[https://blog.csdn.net/qq_40061421/article/details/81915573](https://blog.csdn.net/qq_40061421/article/details/81915573)

### 然后是康托展开：

[https://blog.csdn.net/qq_40061421/article/details/81915838](https://blog.csdn.net/qq_40061421/article/details/81915838)

A*：f=g+h函数。g表示从起点到当前点的移动步数，h表示对当前点到目标点的最小移动步数的预测。除去起点和目标点，我们走在任意一点上的时候，下一步很容易想到应该选择f较小的继续。（对于h的计算我们可以用曼哈顿距离公式）

康托展开：这道题里面的作用在于实施hash函数，对于当前这一步后得到一个新的矩阵，用康托展开公式计算这个矩阵的hash值，用在宽搜时判断。

```cpp
#include<iostream>
#include<cstdio>
#include<cstring>
#include<cstdlib>
#include<cmath>
#include<algorithm>
#include<queue>
#include<vector>
#define inf 0x7fffffff
using namespace std;
const int maxn = 10;
const int M = 400000 + 10;

struct node
{
	int mase[3][3];
	int x, y;//存储x的位置
	int f, g, h;//A*的三个参数
	int flag;
	friend bool operator < (node a, node b)
	{
		return a.f > b.f;
	}
}start, tail;
int pre[M], v[M];

char str[4] = { 'u','d','l','r' };
int Can[10] = { 1,1,2,6,24,120,720,5040,40320 };//阶乘结果
const int destination = 46234;//最终结果的康托展开值
int Cantor(node cur) ///康托展开
{
	int an[10], k = 0;
	for (int i = 0; i<3; i++)
		for (int j = 0; j<3; j++)
			an[k++] = cur.mase[i][j];
	int sum = 0;
	for (int i = 0; i<9; i++)
	{
		int k = 0;
		for (int j = i + 1; j<9; j++)
			if (an[i]>an[j]) k++;
		sum += k*Can[9 - i - 1];
	}
	return sum + 1;
}

int is_ok(node an) //判断此时奇偶性
{
	//判断当前矩阵是否可以达到目标矩阵（矩阵里两个数实施交换后，逆序数的奇偶性和目标矩阵一致才可以有机会达到目标矩阵）
	int a[10], k = 0;
	for (int i = 0; i<3; i++)
		for (int j = 0; j<3; j++)
			a[k++] = an.mase[i][j];
	int sum = 0;
	for (int i = 0; i<k; i++) 
		if (a[i] != 0)
			for (int j = 0; j<i; j++)
				if (a[j] != 0 && a[j]>a[i]) 
					sum++;
	if (sum & 1) //奇数
		return 0;
	return 1;//偶数符合
}

void print(node cur)
{
	string ans;
	int sum = destination;
	while (pre[sum] != -1)
	{
		switch (v[sum]) {
		case 0: ans += str[0]; break;
		case 1: ans += str[1]; break;
		case 2: ans += str[2]; break;
		case 3: ans += str[3]; break;
		}
		sum = pre[sum];
	}
	int len = ans.size();
	for (int i = len - 1; i >= 0; i--) putchar(ans[i]);
	return;
}

pair<int, int> pii[10];
int getH(node cur)
{
	int r = 0, c = 0;
	for (int i = 1; i <= 9; i++)
	{
		pii[i % 9].first = r;
		pii[i % 9].second = c;
		c++;
		if (c == 3) 
		{ 
			r++; 
			c = 0;
		}//每个里存储横纵坐标
	}
	int sum = 0;
	for (int i = 0; i<3; i++)//计算每个位置距离理想位置的曼哈顿距离
	{
		for (int j = 0; j<3; j++)
		{
			int u = cur.mase[i][j];
			sum += abs(pii[u].first - i) + abs(pii[u].second - j);
		}
	}
	return sum;
}

int vis[M];
int an[4][2] = { -1,0, 1,0, 0,-1, 0,1 };
void A_star(node cur)
{
	priority_queue<node> Q;
	cur.g = 0; 
	cur.h = getH(cur);
	cur.f = cur.g + cur.h;
	cur.flag = -1;
	Q.push(cur);
	memset(vis, -1, sizeof(vis));
	memset(pre, -1, sizeof(pre));
	memset(v, -1, sizeof(v));
	vis[Cantor(cur)] = 1;
	while (!Q.empty())
	{
		cur = Q.top(); 
		Q.pop();
		if (Cantor(cur) == destination)
		{
			print(cur);
			return;
		}
		for (int i = 0; i<4; i++)
		{
			tail.x = cur.x + an[i][0];
			tail.y = cur.y + an[i][1];
			int x = cur.x, y = cur.y;
			for (int u = 0; u<3; u++)
				for (int v = 0; v<3; v++)
					tail.mase[u][v] = cur.mase[u][v];
			if (tail.x<0 || tail.x >= 3 || tail.y<0 || tail.y >= 3) 
				continue;
			swap(tail.mase[tail.x][tail.y], tail.mase[x][y]);//移动后交换位置
			int sum = Cantor(tail);//康托展开防止重复
			if (vis[sum] == -1)
			{
				if (is_ok(tail) == 0) continue;
				vis[sum] = 1;
				tail.g = cur.g + 1;
				tail.h = getH(tail);
				tail.f = tail.g + tail.h;
				if (tail.x == x + 1) tail.flag = 1;
				else if (tail.x == x - 1) tail.flag = 0;
				else if (tail.y == y - 1) tail.flag = 2;
				else if (tail.y == y + 1) tail.flag = 3;
				pre[sum] = Cantor(cur);//记录sum的顺序
				v[sum] = i;//sum对应找到方向i
				Q.push(tail);
			}
		}
	}
	return;
}

int main()
{
	char str[100];
	while (gets(str))
	{
		int r = 0, c = 0;
		int len = strlen(str);
		int ok = 0;
		for (int i = 0; i<len; i++)
		{
			if (str[i] >= '0' && str[i] <= '9')
			{
				start.mase[r][c] = str[i] - '0';
				c++;
				if (c == 3) { r++; c = 0; }
			}
			else if (str[i] == 'x')
			{
				start.mase[r][c] = 0;
				start.x = r; start.y = c;
				c++;
				if (c == 3) { r++; c = 0; }
			}
		}
		int sum = Cantor(start);
		if (sum == destination) //已经满足
		{ 
			printf("\n"); 
			continue; 
		}
		if (is_ok(start) == 0)//不可能实现的
		{ 
			printf("unsolvable\n"); 
			continue; 
		}
		A_star(start);
		printf("\n");
	}
	return 0;
}
```





