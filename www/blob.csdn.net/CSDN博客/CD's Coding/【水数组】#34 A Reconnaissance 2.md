# 【水数组】#34 A. Reconnaissance 2 - CD's Coding - CSDN博客





2014年07月20日 08:49:39[糖果天王](https://me.csdn.net/okcd00)阅读数：445
所属专栏：[CF-A Terminator](https://blog.csdn.net/column/details/cf-amaster.html)

















*n* soldiers stand in a circle. For each soldier his height *a**i* is
 known. A reconnaissance unit can be made of such two neighbouringsoldiers, whose heights difference is minimal, i.e. |*a**i* - *a**j*| is
 minimal. So each of them will be less noticeable with the other. Output any pair of soldiers that can form a reconnaissance unit.




Input


The first line contains integer *n* (2 ≤ *n* ≤ 100) —
 amount of soldiers. Then follow the heights of the soldiers in their order in the circle —*n* space-separated integers *a*1, *a*2, ..., *a**n* (1 ≤ *a**i* ≤ 1000).
 The soldier heights are given in clockwise or counterclockwise direction.




Output


Output two integers — indexes of neighbouring soldiers, who should form a reconnaissance unit. If there are many optimum solutions, output any of them. Remember, that the soldiers stand in a circle.




Sample test(s)




input
5
10 12 13 15 10




output
5 1




input
4
10 20 30 40




output
1 2















一个循环数组找相邻最小值



```cpp
#include <cmath>   
#include <cstdio>  
#include <string>  
#include <cstring>  
#include <iostream>  
#include <algorithm>
using namespace std;

int main()
{
	int n,a0,alast,anow;
	int index,minimal=9999;
	cin>>n;
	cin>>a0;
	alast=a0;
	for(int i=1;i<n;i++)
	{
		cin>>anow;
		if(abs(anow-alast)<minimal)
		{
			minimal=abs(anow-alast);
			index=i+1;
		}
		alast=anow;
	}
	if(abs(a0-anow)<minimal)	cout<<n<<" 1";
	else cout<<index-1<<" "<<index;
	return 0;
}
```





