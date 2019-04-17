# 【水排序】#32 A. Reconnaissance - CD's Coding - CSDN博客





2014年07月09日 20:34:34[糖果天王](https://me.csdn.net/okcd00)阅读数：555标签：[codeforces																[水题](https://so.csdn.net/so/search/s.do?q=水题&t=blog)](https://so.csdn.net/so/search/s.do?q=codeforces&t=blog)
个人分类：[CF-A题终结者](https://blog.csdn.net/okcd00/article/category/2156945)

所属专栏：[CF-A Terminator](https://blog.csdn.net/column/details/cf-amaster.html)

















According to the regulations of Berland's army, a reconnaissance unit should consist of exactly two soldiers. Since these two soldiers shouldn't differ much, their heights can differ by at most *d* centimeters.
 Captain Bob has *n* soldiers in his detachment. Their heights are*a*1, *a*2, ..., *a**n* centimeters.
 Some soldiers are of the same height. Bob wants to know, how many ways exist to form a reconnaissance unit of two soldiers from his detachment.


Ways (1, 2) and (2, 1) should be regarded as different.




Input


The first line contains two integers *n* and *d* (1 ≤ *n* ≤ 1000, 1 ≤ *d* ≤ 109)
 — amount of soldiers in Bob's detachment and the maximum allowed height difference respectively. The second line contains *n* space-separated integers — heights
 of all the soldiers in Bob's detachment. These numbers don't exceed 109.




Output


Output one number — amount of ways to form a reconnaissance unit of two soldiers, whose height difference doesn't exceed *d*.




Sample test(s)




input
5 10
10 20 50 60 65




output
6




input
5 1
55 30 29 31 55




output
6










排序后遍历，每个元素向后找m以内差距的元素个数乘二求和





```cpp
#include <cmath>   
#include <cstdio>  
#include <string>  
#include <cstring>  
#include <iostream>  
#include <algorithm>
using namespace std;
int hgt[1001];
int main()
{
	int n,m,cnt,ans=0;
	cin>>n>>m;
	memset(hgt,0,sizeof hgt);
	for(int i=0;i<n;i++)
	{
		cin>>hgt[i];
	}
	sort(hgt,hgt+n);
	for(int i=0;i<n;i++)
	{
		cnt=0;
		for(int j=i+1; hgt[j]<=hgt[i]+m && j<n; j++ )
		{
			cnt++;
		}
		ans+= 2*cnt;
	}
	printf("%d",ans);
	return 0;
}
```






