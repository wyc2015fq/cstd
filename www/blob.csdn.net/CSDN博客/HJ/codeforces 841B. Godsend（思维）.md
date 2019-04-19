# codeforces 841B. Godsend（思维） - HJ - CSDN博客
2018年03月13日 13:12:03[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：100
个人分类：[====ACM====																[---------OJ---------																[codeforce](https://blog.csdn.net/feizaoSYUACM/article/category/6353637)](https://blog.csdn.net/feizaoSYUACM/article/category/6308067)](https://blog.csdn.net/feizaoSYUACM/article/category/6308070)
所属专栏：[codeforces](https://blog.csdn.net/column/details/17151.html)
题目链接 [http://codeforces.com/problemset/problem/841/B](http://codeforces.com/problemset/problem/841/B)
题目大意：一组数据，两个人轮流取连续的数，要求A取出来的连续的数之和为奇数，B取出来的连续的数之和为偶数，谁先不能取谁输。
解题思路：只要里面存在奇数，A就能赢！！！（为什么大家可以自己想想看）
```cpp
#include<iostream>
#include<cstdio> 
using namespace std;
int main()
{
	int n,x;
	while(~scanf("%d",&n))
	{
		int blag=0;
		for(int i=0;i<n;i++)
		{
			scanf("%d",&x);
			if(x%2)
			  blag=1;
		}
		printf("%s\n",blag?"First":"Second");
	}
	return 0;
}
```
