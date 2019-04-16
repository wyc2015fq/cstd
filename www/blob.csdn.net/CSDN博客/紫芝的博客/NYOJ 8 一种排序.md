# NYOJ 8 一种排序 - 紫芝的博客 - CSDN博客





2018年04月27日 18:48:22[紫芝](https://me.csdn.net/qq_40507857)阅读数：119
个人分类：[STL](https://blog.csdn.net/qq_40507857/article/category/7595795)









## [点击打开链接](http://acm.nyist.edu.cn/JudgeOnline/problem.php?pid=8)

## 一种排序

 时间限制：3000 ms  |           内存限制：65535 KB 

难度：3
- 描述 
现在有很多长方形，每一个长方形都有一个编号，这个编号可以重复；还知道这个长方形的宽和长，编号、长、宽都是整数；现在要求按照一下方式排序（默认排序规则都是从小到大）；

1.按照编号从小到大排序

2.对于编号相等的长方形，按照长方形的长排序；

3.如果编号和长都相同，按照长方形的宽排序；

4.如果编号、长、宽都相同，就只保留一个长方形用于排序,删除多余的长方形；最后排好序按照指定格式显示所有的长方形；- 输入
第一行有一个整数 0<n<10000,表示接下来有n组测试数据；
每一组第一行有一个整数 0<m<1000，表示有m个长方形；
接下来的m行，每一行有三个数 ，第一个数表示长方形的编号，

第二个和第三个数值大的表示长，数值小的表示宽，相等
说明这是一个正方形（数据约定长宽与编号都小于10000）；- 输出
顺序输出每组数据的所有符合条件的长方形的 编号 长 宽- 样例输入
1
8
1 1 1
1 1 1
1 1 2
1 2 1
1 2 2
2 1 1
2 1 2
2 2 1
- 样例输出
1 1 1
1 2 1
1 2 2
2 1 1
2 2 1
          AC代码：2ms
- 


```cpp
#include<cstdio> 
#include<cstring>  
#include<cstdlib>  
#include<algorithm>  
#include<iostream>
using namespace std;
struct node{
	int mark,lon,wid;
}str[10005]; 
bool cmp(node a,node b)
{
	if(a.mark<b.mark)
	    return true; 
	else if(a.lon<b.lon&&a.mark==b.mark)
    	return true;
	else if(a.lon==b.lon&&a.mark==b.mark&&a.wid<b.wid)
	    return true;
	return false;
} 
int main()
{
	int n,m,i,t,j;
	scanf("%d",&n);
	while(n--)
	{
		memset(str,0,sizeof(str));
		scanf("%d",&m);
		for(i=0;i<m;i++)
		{
			scanf("%d%d%d",&str[i].mark,&str[i].lon,&str[i].wid);	
			if(str[i].lon<str[i].wid)
			swap(str[i].lon,str[i].wid); //长＞宽 
			
			for(j=0;j<i;j++)
			if(str[i].mark==str[j].mark&&str[i].lon==str[j].lon&&str[i].wid==str[j].wid)
			{
			 	m--;i--;
//如果跟前面已经输入过的i组数据有相同的，这里用i--方式在下一次输入的时候覆盖掉，用m--记录有效数据数量。 
			}
		}
		sort(str,str+m,cmp);
		//也可以sort之后再查重 
		for(i=0;i<m;i++)
		printf("%d %d %d\n",str[i].mark,str[i].lon,str[i].wid);
	}
	
	return 0;
}
```

AC代码：4ms

优先队列


```cpp
#include<bits/stdc++.h>
#include<iostream>
#include<set>
#include<vector>
#include<algorithm>
using namespace std;
struct node
{
	int x;//宽 
	int y;//长 
	int z;//编号 
	friend bool operator<(node p, node q)
	{
		if ((q.y == p.y) && (q.z == p.z))
		{
			return q.x < p.x;
		}
		else
		{
			if (q.z == p.z)
			{
				return q.y < p.y;
			}
			else
			{
				return q.z < p.z;
			}
		}
	}
}s;
int main()
{
	int n, m;
	scanf("%d", &n);
	for (int i = 0; i < n; i++)
	{
		priority_queue<node>que;
		scanf("%d", &m);
		for (int j = 0; j < m; j++)
		{
			scanf("%d%d%d", &s.z, &s.y, &s.x);
			if(s.y<s.x)
			swap(s.y,s.x);
			que.push(s);
		}
		while (!que.empty())
		{
			int a[3] = { que.top().z, que.top().y, que.top().x };
			/*
			if (a[2]>a[1])
			{
				int tmp = a[2];
				a[2] = a[1];
				a[1] = tmp;
			}*/
			que.pop();
			while ( !que.empty()&&(a[0] == que.top().z) && (a[1] == que.top().y) && (a[2] == que.top().x))
			{
				
				que.pop();
			}
			printf("%d %d %d\n", a[0],a[1],a[2]);
		}
	}
	//system("pause");
	return 0;
}
```


AC代码：10ms

```cpp
//#include<bits/stdc++.h>
#include<iostream>
#include<set>
#include<vector>
#include<algorithm>
using namespace std;
const int MAX=1e3+5; 
struct node{
	int id;//编号
	int l;//长
	int w;//宽 
};
bool cmp(node a,node b)
{
	if(a.id==b.id)
	{
		if(a.l==b.l)
		return a.w<b.w;
		return a.l<b.l;
	}
	return a.id<b.id;
}
vector<node> a;
int main()
{
	ios::sync_with_stdio(0);
	int n;
	cin>>n;
	while(n--){
		int m;
		cin>>m;
		
		a.clear();
		int id,l,w;
		
		for(int i=0;i<m;i++){
			cin>>id>>l>>w;
			if(l<w)
			swap(l,w);
			bool flag=0;
			for(int j=0;j<i;j++)
			if(a[j].id==id&&a[j].l==l&&a[j].w==w) 
			flag=1;	
			if(!flag){
				node t;
				t.id=id;t.l=l;t.w=w;
				a.push_back(t);
			}
			
			}
		sort(a.begin(),a.end(),cmp);
		for(int i=0;i<a.size();i++)
		{
			cout<<a[i].id<<" "<<a[i].l<<" "<<a[i].w<<endl;
		}
	}
	return 0;
}
```
- 




