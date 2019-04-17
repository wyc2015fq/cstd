# 【水比例】#41 A. Guilty — to the kitchen! - CD's Coding - CSDN博客





2014年07月28日 15:53:11[糖果天王](https://me.csdn.net/okcd00)阅读数：660标签：[codeforces](https://so.csdn.net/so/search/s.do?q=codeforces&t=blog)
个人分类：[CF-A题终结者](https://blog.csdn.net/okcd00/article/category/2156945)

所属专栏：[CF-A Terminator](https://blog.csdn.net/column/details/cf-amaster.html)











首先用每个b去除以a找到木桶效应的那个短板，就是商最小的那个，记住这个值，然后用这个值来乘以总权值，和V进行比较即可。






```cpp
#include <queue>
#include <cmath> 
#include <memory> 
#include <cctype>
#include <cstdio>
#include <string>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <algorithm>
using namespace std;

int main()
{
	int n,v,index;
	cin>>n>>v;
	pair<double,double> p[21];
	double a[21],b[21];
	memset(a,0,sizeof a);
	for(int i=0;i<n;i++)	cin>>a[i];
	for(int i=0;i<n;i++)	cin>>b[i];
	double min=10009.9;
	for(int i=0;i<n;i++)	
	{
		p[i].first=a[i];
		p[i].second=b[i]/a[i];
		if(min>p[i].second)	min=p[i].second;
	}
	double ans=0.0;
	for(int i=0;i<n;i++)	ans+=p[i].first;
	ans*=min;
	ans =(ans>v?v:ans);
	cout<<ans;
	return 0;
}
```












﻿﻿

﻿﻿



