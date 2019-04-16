# poj2002 STL set - 紫芝的博客 - CSDN博客





2018年04月16日 18:53:35[紫芝](https://me.csdn.net/qq_40507857)阅读数：34
个人分类：[STL																[枚举																[解题模板](https://blog.csdn.net/qq_40507857/article/category/7565904)




题意：

给n个点，问最多能构成多少个正方形？

思路：

枚举两个点，计算组成正方形所需的另外两个点，若这两个点在所给的点集合中，则cnt++，最后的答案为cnt/2；

（set 里 count() 返回整数，且只为 0 或 1 find() 返回迭代器）


```cpp
//#include<bits/stdc++.h>
#include<iostream>
#include<set>

using namespace std;
typedef pair<int,int> Point;
const int maxn=1005;
int x[maxn],y[maxn];
set<Point> pset;
int main()
{
	ios::sync_with_stdio(false);
	int n; 
	while(cin>>n&&n)
	{
		pset.clear();
		for(int i=0;i<n;i++)
		{
			cin>>x[i]>>y[i];
			pset.insert(make_pair(x[i],y[i]));
		}
		int cnt=0;
		for(int i=0;i<n;i++)
		{
			for(int j=i+1;j<n;j++)
			{
				int mx=x[i]+x[j],dx=(max(x[i],x[j])-min(x[i],x[j]));
				int my=y[i]+y[j],dy=(max(y[i],y[j])-min(y[i],y[j]));
				if((mx+dy)&1||(my+dx)&1)	continue;
				int sg=((x[i]-x[j])*(y[i]-y[j])<0)?1:-1;
				if(pset.count(make_pair((mx+dy)/2,(my+sg*dx)/2))&&
				pset.count(make_pair((mx-dy)/2,(my-sg*dx)/2)))
				cnt++;
			}
		}
		cout<<cnt/2<<endl;
	}
	return 0;
}
```](https://blog.csdn.net/qq_40507857/article/category/7595712)](https://blog.csdn.net/qq_40507857/article/category/7595795)





