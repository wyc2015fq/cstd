# C++之数字计数算法 - fanyun的博客 - CSDN博客
2018年07月29日 21:33:28[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：677
1、问题
        输入n,m，求n~m范围内的所有数字中，分别输出0~9出现的总数是多少？
 2、思路
        枚举0在个十百千位上出现的次数 
个：个位为0时，后面不需要考虑，只需考虑前面，因为0比4小，所以前面即使取到最大也不会过限，所以前面可以是1~203(因为当前位是0，所以前面不能是0)。一共203种。 
十：十位为0时，前面取1～20，后面取0～9。一共123＊10种。 
百：百位为0时，因为0与当前位上限0相等，所以前面取1时，后面可以取0～99，前面取2时，后面只能取0～34。一共1＊100＋35种。 
千位显然不能为0，所以总数为0。 0和1～9要分开处理，是因为前缀0的问题。因为当某一位取0时，前面部分的数是不能为0的，而取1～9是可以前面为0的。
把上述思想转化为代码即可。
```cpp
// NumberFrq.cpp : 定义控制台应用程序的入口点。
//
#include "stdafx.h"
#include <iostream>
#include <algorithm>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <cmath>
#include <map>
using namespace std;
#define LL long long
LL p[20];
LL ans[10] = {};
void init()
{
	p[0] = 1;
	for (int i = 1; i < 18; i++)
		p[i] = p[i - 1] * 10;
}
void solve(LL x, int f)
{
	if (x == -1)
	{
		ans[0]++;
		return;
	}
	for (int k = 1; k < 10; k++)
	{
		for (int i = 1; ; i++)
		{
			LL l = x / p[i];
			LL r = x%p[i - 1];
			LL now = x%p[i] / p[i - 1];
			if (now > k)
				ans[k] += (l + 1)*p[i - 1] * f;
			else if (now == k)
				ans[k] += (l*p[i - 1] + r + 1)*f;
			else
				ans[k] += l*p[i - 1] * f;
			if (p[i] > x)
				break;
		}
	}
	for (int i = 1; ; i++)
	{
		LL l = x / p[i];
		LL r = x%p[i - 1];
		LL now = x%p[i] / p[i - 1];
		if (now > 0)
			ans[0] += l*p[i - 1] * f;
		else
			ans[0] += ((l - 1)*p[i - 1] + r + 1)*f;
		if (p[i] > x)
			break;
	}
}
int main()
{
	LL n, m;
	init();
	cin >> n >> m;
	solve(m, 1);
	solve(n - 1, -1);
	for (int i = 0; i < 9; i++)
		printf("%lld ", ans[i]);
	printf("%lld\n", ans[9]);
	return 0;
}
```
