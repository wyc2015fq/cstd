# Building A New Barn（POJ-3269） - Alex_McAvoy的博客 - CSDN博客





2018年06月26日 21:07:00[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：133








> 
# Problem Description

After scrimping and saving for years, Farmer John has decided to build a new barn. He wants the barn to be highly accessible, and he knows the coordinates of the grazing spots of all N (2 ≤ N ≤ 10,000 cows. Each grazing spot is at a point with integer coordinates (Xi, Yi) (-10,000 ≤ Xi ≤ 10,000; -10,000 ≤ Yi ≤ 10,000). The hungry cows never graze in spots that are horizontally or vertically adjacent.

The barn must be placed at integer coordinates and cannot be on any cow‘s grazing spot. The inconvenience of the barn for any cow is given the Manhattan distance formula | X - Xi | + | Y - Yi|, where (X, Y) and (Xi, Yi) are the coordinates of the barn and the cow‘s grazing spot, respectively. Where should the barn be constructed in order to minimize the sum of its inconvenience for all the cows?

# **Input**

Line 1: A single integer: N 

Lines 2..N+1: Line i+1 contains two space-separated integers which are the grazing location (Xi, Yi) of cow i

# Output

Line 1: Two space-separated integers: the minimum inconvenience for the barn and the number of spots on which Farmer John can build the barn to achieve this minimum.

# Sample Input

**4**

**1 -30 1-2 11 -1**

# Sample Output

**10 4**


—————————————————————————————————————————————

题意：给出n个点及其坐标，求一点到其它点曼哈顿距离总和最小值以及解的个数。

思路：一开始题都读不明白，问了队里数学系的大佬，才发现是一道及其简单的中学数学题。。数学题。。。

两点曼哈顿距离公式：d=|x1−x2|+|y1−y2|，由于两点曼哈顿距离的特性，单独求 x 与单独求 y 互不影响

![](https://img-blog.csdn.net/20180626205737116?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



因此，题目即为求 |x−x1|+|x−x2|+…+|x−xn| 的最小值，求 |y−y1|+|y−y2|+…+|y−yn| 的最小值，直接求两者中位数即可。

接对 x、y 各自进行排序比较：

    1）当n为奇数时，取（ x[n/2+1]，y[n/2+1] ）

　　若该点为给出点，枚举它的上下左右四个方向上的点能求的最小的 d，然后统计当且仅当这4个点的方案数；

　　若该点不为给出点，则直接记录最小距离，方案数为1。

    2）当n为偶数时，取（ x[n/2]，y[n/2] ）和（ x[n/2+1]，y[n/2+1] ）

        由曼哈顿距离的特性知：共有（ x[n/2+1]−x[n/2]+1）*（ y[n/2+1]−y[n/2]+1）个点，且它们到给定的n个点的曼哈顿距离和d相等。

        因此枚举每个点是否为给定的点，求一次最小距离，再先让方案数为点的个数，每次更新减小方案数即可。

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
#include<vector>
#define INF 0x3f3f3f3f
#define PI acos(-1.0)
#define N 10001
#define MOD 123
#define E 1e-6
using namespace std;
int dx[4]={0,1,0,-1};
int dy[4]={1,0,-1,0};
struct Node{
	int x;
    int y;
}a[N];
int x[N],y[N];
int minn,plan;
int main()
{

    int n;
    scanf("%d",&n);
	for(int i=1;i<=n;i++)
	{
		scanf("%d%d",&a[i].x,&a[i].y);
		x[i]=a[i].x;
		y[i]=a[i].y;
	}

	sort(x+1,x+n+1);//对x排序
	sort(y+1,y+n+1);//对y排序

	if(n%2)//n为奇数时
	{
		int temp=(n/2)+1;
		for(int i=1;i<=n;i++)
		{
			if(a[i].x==x[temp]&&a[i].y==y[temp])//若点为给出点
			{
				int Min=INF;
				for(int l=0;l<4;l++)//枚举四个方向
				{
					int xx=x[temp]+dx[l];
					int yy=y[temp]+dy[l];
					int sum=0;
					for(i=1;i<=n;i++)//求最小的距离
					  sum+=abs(a[i].x-xx)+abs(a[i].y-yy);
					if(sum<Min)
                    {
                        Min=sum;
                        plan=1;
                    }
					else if (sum==Min)
                        plan++;
				}
				printf("%d %d\n",Min,plan);
				return 0;
			}
			else//若点不为给出点
            {
                minn+=abs(a[i].x-x[temp])+abs(a[i].y-y[temp]);//记录最小距离
                plan=1;//方案数为1
            }
		}
		printf("%d %d\n",minn,plan);
	}
	else//n为偶数时
	{
		int temp1=n/2,temp2=n/2+1;
		plan=(x[temp2]-x[temp1]+1)*(y[temp2]-y[temp1]+1);//令方案数等于点的个数
		for(int i=1;i<=n;i++)
		{
			minn+=abs(a[i].x-x[temp1])+abs(a[i].y-y[temp1]);//记录最小距离
			int x0=a[i].x,y0=a[i].y;
			if (x[temp1]<=x0&&x0<=x[temp2]&&y[temp1]<=y0&&y0<=y[temp2])//更新方案数
                plan--;
		}
		printf("%d %d\n",minn,plan);
	}
	return 0;
}
```





