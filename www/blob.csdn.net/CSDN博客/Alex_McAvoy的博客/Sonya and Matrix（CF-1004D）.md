# Sonya and Matrix（CF-1004D） - Alex_McAvoy的博客 - CSDN博客





2018年08月26日 18:48:19[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：84








# Problem Description

Since Sonya has just learned the basics of matrices, she decided to play with them a little bit.

Sonya imagined a new type of matrices that she called rhombic matrices. These matrices have exactly one zero, while all other cells have the Manhattan distance to the cell containing the zero. The cells with equal numbers have the form of a rhombus, that is why Sonya called this type so.

The Manhattan distance between two cells (x1, y1) and (x2, y2) is defined as |x1−x2|+|y1−y2|. For example, the Manhattan distance between the cells (5,2) and (7,1) equals to |5−7|+|2−1|=3.

![](https://img-blog.csdn.net/20180826184042885?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

Example of a rhombic matrix.

Note that rhombic matrices are uniquely defined by nn, mm, and the coordinates of the cell containing the zero.

She drew a n×m rhombic matrix. She believes that you can not recreate the matrix if she gives you only the elements of this matrix in some arbitrary order (i.e., the sequence of n⋅m numbers). Note that Sonya will not give you nn and mm, so only the sequence of numbers in this matrix will be at your disposal.

Write a program that finds such an n×mn×m rhombic matrix whose elements are the same as the elements in the sequence in some order.

# Input

The first line contains a single integer t (1≤t≤106) — the number of cells in the matrix.

The second line contains tt integers a1,a2,…,at (0≤ai<t) — the values in the cells in arbitrary order.

# Output

In the first line, print two positive integers n and m (n×m=t) — the size of the matrix.

In the second line, print two integers x and y (1≤x≤n, 1≤y≤m) — the row number and the column number where the cell with 00 is located.

If there are multiple possible answers, print any of them. If there is no solution, print the single integer −1.

# Examples

**Input**

20

1 0 2 3 5 3 2 1 3 2 3 1 4 2 1 4 2 3 2 4

**Output**

4 5

2 2

**Input**

18

2 2 3 2 4 3 3 3 0 2 4 2 1 3 2 1 1 1

**Output**

3 6

2 3

**Input**

6

2 1 0 2 1 2

**Output**

-1

————————————————————————————————————————————

题意：

给 T 个数，要求构造一个 n*m 的矩阵满足：

① 其中有且只有一个0；

② 其它所有位置上的数字正好等于它到 0 的曼哈顿距离；

③ 矩阵中所有数字出现次数刚好和给出的T个数一致

如果能构造出来，输出任意一种（只需要输出 n 和 m 还有 0 的位置），否则输出 -1

思路来源：[点击这里](https://blog.csdn.net/Jaihk662/article/details/80939688)

# Source Program

```cpp
#include<iostream>
#include<cstdio>
#include<cstring>
#include<cmath>
#include<algorithm>
#include<string>
#include<cstdlib>
#include<queue>
#include<set>
#include<map>
#include<stack>
#include<ctime>
#include<vector>
#define INF 0x3f3f3f3f
#define PI acos(-1.0)
#define N 1000001
#define MOD 1e9+7
#define E 1e-6
#define LL long long
using namespace std;
int sum[N];
int main()
{
	int t;
	scanf("%d",&t);

	int now=0;
	for(int i=1;i<=t;i++)
	{
	    int x;
		scanf("%d",&x);
		sum[x]++;
		if(sum[x]==1)
			now++;
	}

	if(sum[0]!=1)
	{
		printf("-1\n");
		return 0;
	}

	int i;
	for(i=1;i<=2005;i++)
	{
		if(sum[i]>4*i)
		{
			printf("-1\n");
			return 0;
		}
		if(sum[i]<4*i)
			break;
	}

	int p=i;
	for(int n=1;n*n<=t;n++)
	{
		int m=t/n;
		if(t%n || n<p*2-1 || m<p*2-1)
			continue;

        int x=p,y=p;
        int temp;
		for(int i=1;i<=n;i++)
		{
			for(int j=1;j<=m;j++)
			{
				temp=abs(i-x)+abs(j-y);
				sum[temp]--;

				if(sum[temp]==0)
					now--;
				if(sum[temp]==-1)
					now++;
			}
		}
		if(now==0)
		{
			printf("%d %d\n%d %d\n", n, m, x, y);
			return 0;
		}
		for(y++;y+p-1<=m;y++)
		{
			for(i=1;i<=n;i++)
			{
				temp=abs(i-x)+abs(1-y);
				sum[temp]--;
				if(sum[temp]==0)
					now--;
				if(sum[temp]==-1)
					now++;
				temp = abs(i-x)+abs(m+1-y);
				sum[temp]++;
				if(sum[temp]==0)
					now--;
				if(sum[temp]==1)
					now++;
			}
			if(now==0)
			{
				printf("%d %d\n%d %d\n", n, m, x, y);
				return 0;
			}
		}
		for(y--,x++;x+p-1<=n;x++)
		{
			for(i=1;i<=m;i++)
			{
				temp = abs(1-x)+abs(i-y);
				sum[temp]--;
				if(sum[temp]==0)
					now--;
				if(sum[temp]==-1)
					now++;
				temp = abs(n+1-x)+abs(i-y);
				sum[temp]++;
				if(sum[temp]==0)
					now--;
				if(sum[temp]==1)
					now++;
			}
			if(now==0)
			{
				printf("%d %d\n%d %d\n", n, m, x, y);
				return 0;
			}
		}
		x--;
		for(int i=1;i<=n;i++)
		{
			for(int j=1;j<=m;j++)
			{
				temp=abs(i-x)+abs(j-y);
				sum[temp]++;
				if(sum[temp]==0)
					now--;
				if(sum[temp]==1)
					now++;
			}
		}
	}
	printf("-1\n");
	return 0;
}
```





