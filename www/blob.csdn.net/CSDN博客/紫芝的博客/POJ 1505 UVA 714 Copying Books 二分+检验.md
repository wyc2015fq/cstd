# POJ 1505 UVA 714 Copying Books 二分+检验 - 紫芝的博客 - CSDN博客





2018年07月22日 23:51:43[紫芝](https://me.csdn.net/qq_40507857)阅读数：50








Copying Books
|**Time Limit:** 3000MS||**Memory Limit:** 10000K|
|----|----|----|
|**Total Submissions:** 9617||**Accepted:** 3006|

Description

Before the invention of book-printing, it was very hard to make a copy of a book. All the contents had to be re-written by hand by so called scribers. The scriber had been given a book and after several months he finished its copy. One of the most famous scribers lived in the 15th century and his name was Xaverius Endricus Remius Ontius Xendrianus (Xerox). Anyway, the work was very annoying and boring. And the only way to speed it up was to hire more scribers. 


Once upon a time, there was a theater ensemble that wanted to play famous Antique Tragedies. The scripts of these plays were divided into many books and actors needed more copies of them, of course. So they hired many scribers to make copies of these books. Imagine you have m books (numbered 1, 2 ... m) that may have different number of pages (p1, p2 ... pm) and you want to make one copy of each of them. Your task is to divide these books among k scribes, k <= m. Each book can be assigned to a single scriber only, and every scriber must get a continuous sequence of books. That means, there exists an increasing succession of numbers 0 = b0 < b1 < b2, ... < bk-1 <= bk = m such that i-th scriber gets a sequence of books with numbers between bi-1+1 and bi. The time needed to make a copy of all the books is determined by the scriber who was assigned the most work. Therefore, our goal is to minimize the maximum number of pages assigned to a single scriber. Your task is to find the optimal assignment. 

Input

The input consists of N cases. The first line of the input contains only positive integer N. Then follow the cases. Each case consists of exactly two lines. At the first line, there are two integers m and k, 1 <= k <= m <= 500. At the second line, there are integers p1, p2, ... pm separated by spaces. All these values are positive and less than 10000000.

Output

For each case, print exactly one line. The line must contain the input succession p1, p2, ... pm divided into exactly k parts such that the maximum sum of a single part should be as small as possible. Use the slash character ('/') to separate the parts. There must be exactly one space character between any two successive numbers and between the number and the slash. 


If there is more than one solution, print the one that minimizes the work assigned to the first scriber, then to the second scriber etc. But each scriber must be assigned at least one book.

Sample Input

```
2
9 3
100 200 300 400 500 600 700 800 900
5 4
100 100 100 100 100
```

Sample Output

```
100 200 300 400 500 / 600 700 / 800 900
100 / 100 / 100 / 100 100
```

Source

[Central Europe 1998](http://poj.org/searchproblem?field=source&key=Central+Europe+1998)

题意：

给一些书，这些书有不同的页数，让把这些书分成k份，必须是连续的，问这些份中页数和的最大值最小是多少。

解题思路：

知道了页数和的范围，而且书都是连续的，要找到页数和最大值的最小值可以直接二分答案。

```cpp
#include<iostream>
#include<cstdlib>
#include<cstring>
using namespace std;

const int MAXM = 505;
int m,k;
int book[MAXM];
bool vis[MAXM];

int divide(int x)
{
	int sum=0,cnt=1;
	memset(vis,false,sizeof(vis));
	for(int i=m;i>=1;i--)
	{
		sum+=book[i];
		if(sum>x)
		{
			cnt++;
			sum=book[i];
			vis[i]=true;
		}
	}
	return cnt;
}

void print()
{
	cout<<book[1];
	for(int i=2;i<=m;i++)
	{
		if(vis[i-1]) cout<<" /";
		cout<<' '<<book[i];
	}
	cout<<endl;
}

int main()
{
	int cas;
	cin>>cas;
	while(cas--)
	{
		cin>>m>>k;
		int l=0,r=0,mid=0;;
		for(int i=1;i<=m;i++)
		{
			cin>>book[i];
			r+=book[i];
			if(l<book[i]) l=book[i];
		}
		while(l<r)	//二分搜索最大值中的最小值（参见刘汝佳《算法竞赛入门经典》P151）
		{
			mid=(l+r)>>1;
			if(divide(mid)<=k) r=mid;	//这里不能是r=mid-1，原因可参见《算法竞赛入门经典》
			else l=mid+1;
		}
		int cnt=divide(r);
		for(int i=1;i<=m && cnt<k;i++)	//可能分得不足k次，那么就把前面的每一本书分给一个人超(题目要求)
		{
			if(!vis[i])
			{
				vis[i]=true;
				cnt++;
			}
		}
		print();
	}
	return 0;
}
```

UVA 714

```cpp
#include<iostream>
#include<cstdlib>
#include<cstring>
using namespace std;
typedef long long LL;
const int MAXM = 505;
int m,k;
LL book[MAXM];
bool vis[MAXM];

int divide(LL x)
{
	LL sum=0;
	int cnt=1;
	memset(vis,false,sizeof(vis));
	for(int i=m;i>=1;i--)
	{
		sum+=book[i];
		if(sum>x)
		{
			cnt++;
			sum=book[i];
			vis[i]=true;
		}
	}
	return cnt;
}

void print()
{
	cout<<book[1];
	for(int i=2;i<=m;i++)
	{
		if(vis[i-1]) cout<<" /";
		cout<<' '<<book[i];
	}
	cout<<endl;
}

int main()
{
	int cas;
	cin>>cas;
	while(cas--)
	{
		cin>>m>>k;
		LL l=0,r=0,mid=0;
		for(int i=1;i<=m;i++)
		{
			cin>>book[i];
			r+=book[i];
			if(l<book[i]) l=book[i];
		}
		while(l<r)	//二分搜索最大值中的最小值（参见刘汝佳《算法竞赛入门经典》P151）
		{
			mid=(r-l)/2+l;
			if(divide(mid)<=k) r=mid;	//这里不能是r=mid-1，原因可参见《算法竞赛入门经典》
			else l=mid+1;
		}
		int cnt=divide(r);
		for(int i=1;i<=m && cnt<k;i++)	//可能分得不足k次，那么就把前面的每一本书分给一个人(题目要求)
		{
			if(!vis[i])
			{
				vis[i]=true;
				cnt++;
			}
		}
		print();
	}
	return 0;
}
```





