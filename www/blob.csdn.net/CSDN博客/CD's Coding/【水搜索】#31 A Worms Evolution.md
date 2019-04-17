# 【水搜索】#31 A. Worms Evolution - CD's Coding - CSDN博客





2014年07月09日 18:38:56[糖果天王](https://me.csdn.net/okcd00)阅读数：756标签：[codeforces																[水题](https://so.csdn.net/so/search/s.do?q=水题&t=blog)](https://so.csdn.net/so/search/s.do?q=codeforces&t=blog)
个人分类：[CF-A题终结者](https://blog.csdn.net/okcd00/article/category/2156945)

所属专栏：[CF-A Terminator](https://blog.csdn.net/column/details/cf-amaster.html)

















Professor Vasechkin is studying evolution of worms. Recently he put forward hypotheses that all worms evolve by division. There are *n*forms of worms. Worms
 of these forms have lengths *a*1, *a*2,
 ..., *a**n*. To prove
 his theory, professor needs to find 3 different forms that the length of the first form is equal to sum of lengths of the other two forms. Help him to do this.




Input


The first line contains integer *n* (3 ≤ *n* ≤ 100) —
 amount of worm's forms. The second line contains *n* space-separated integers *a**i* (1 ≤ *a**i* ≤ 1000)
 — lengths of worms of each form.




Output


Output 3 distinct integers *i**j**k* (1 ≤ *i*, *j*, *k* ≤ *n*)
 — such indexes of worm's forms that *a**i* = *a**j* + *a**k*.
 If there is no such triple, output -1. If there are several solutions, output any of them. It possible that *a**j* = *a**k*.




Sample test(s)




input
5
1 2 3 5 7




output
3 2 1




input
5
1 8 1 5 1




output
-1










还亏我想了半天……这道题暴力O (n^3)都能无压力A……



那就直接水掉吧~ FORFORFOR 检测flag~ over~

Code：8k 30ms AC



```cpp
#include <cmath>   
#include <cstdio>  
#include <string>  
#include <cstring>  
#include <iostream>  
#include <algorithm>
#define FOR(a,b) for(a=b;a<num;a++)
using namespace std;
int list[101];
int main()
{
	int h,i,j,k,num=0;
	scanf("%d",&num);
	memset(list,0,sizeof list);
	FOR(h,0)	scanf("%d",&list[h]);
	int flag=0;
	FOR(i,0)
	{
		FOR(j,i+1)
		{
			FOR(k,j+1)
			{
				flag= 	(list[i]==list[j]+list[k])?1:
						(list[j]==list[k]+list[i])?2:
						(list[k]==list[i]+list[j])?3:0;	
				if(flag)break;
			}
			if(flag)break;
		}
		if(flag)break;
	}
	if(flag)
	{
		int a = (flag==1)?i:(flag==2)?j:k;
		int b = (flag==1)?j:(flag==2)?k:i;
		int c = (flag==1)?k:(flag==2)?i:j;
		printf("%d %d %d",a+1,b+1,c+1);
	}
	else printf("-1");
	return 0;
}
```

















