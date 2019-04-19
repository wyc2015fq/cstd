# codeforces 869B. The Eternal Immortality（思维） - HJ - CSDN博客
2018年03月13日 13:15:08[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：97
个人分类：[====ACM====																[---------OJ---------																[codeforce](https://blog.csdn.net/feizaoSYUACM/article/category/6353637)](https://blog.csdn.net/feizaoSYUACM/article/category/6308067)](https://blog.csdn.net/feizaoSYUACM/article/category/6308070)
所属专栏：[codeforces](https://blog.csdn.net/column/details/17151.html)
题目链接 [http://codeforces.com/problemset/problem/869/B](http://codeforces.com/problemset/problem/869/B)
解题思路：可以发现，只要a和b两个数之差超过9，那么最后一个数一定是‘0’，因此只要考虑两个数只差小于10的情况，剩下的遍历一下就可以了。
```cpp
#include<iostream>
using namespace std;
typedef long long LL;
int main()
{
	LL a,b,i;
	while(cin>>b>>a)    //注意a比b大 
	{
		if(a-b>9)
		  cout<<"0"<<endl;
		else
		{
			LL ans=1;
			for(i=b+1;i<=a;i++)
			  ans=(ans*(i%10))%10;
			cout<<ans<<endl;
		}
	}
	return 0;
}
```
