# 【map&list】int型数据处理之 桶放置 - CD's Coding - CSDN博客





2014年08月19日 13:08:21[糖果天王](https://me.csdn.net/okcd00)阅读数：561标签：[桶																[map																[list](https://so.csdn.net/so/search/s.do?q=list&t=blog)
个人分类：[模板记忆](https://blog.csdn.net/okcd00/article/category/1650751)





﻿﻿

MUTC中自己手敲的一个记录sequence中int型出现位置模板






```cpp
#include <map>
#include <list>
#include <cmath> 
#include <cctype>
#include <cstdio>
#include <string>
#include <memory>
#include <cstdlib>
#include <cstring>
#include <utility>
#include <iostream>
#include <algorithm>
using namespace std;

#define Max(a,b) ((a)>(b)?(a):(b))
#define Min(a,b) ((a)<(b)?(a):(b))
int a[100086],b[100086],c[100086];
map<int, list<int> > m;

int main()
{
	int n;
	while(scanf("%d",&n)!=EOF)
	{
		if(n==0)return 0;
		else m.clear();
		memset(a,0,sizeof a);
		memset(b,0,sizeof b);
		memset(c,0,sizeof c);
		for(int i=1;i<=n;i++)
		{
			scanf("%d",&a[i]);
			if(m.find(a[i])==m.end())
			{
				list<int> tmp;
				while(!tmp.empty())tmp.pop_back();
				tmp.push_back(i);
				m.insert(pair<int,list<int> >(a[i],tmp));
			}else m[a[i]].push_back(i);	
		}
		map<int,list<int> >::iterator mit;
				list<int>  ::iterator lit;
		for(mit=m.begin();mit!=m.end();++mit)
		{
			printf("%d:",mit->first);
			list<int> now=mit->second;
			for(lit=now.begin();lit!=now.end();++lit) printf("%d ",*lit);
			printf("\n");
		}
	}
	return 0;
}
```](https://so.csdn.net/so/search/s.do?q=map&t=blog)](https://so.csdn.net/so/search/s.do?q=桶&t=blog)




