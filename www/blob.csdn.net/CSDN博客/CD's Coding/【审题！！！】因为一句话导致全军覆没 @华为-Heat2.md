# 【审题！！！】因为一句话导致全军覆没 @华为-Heat2 - CD's Coding - CSDN博客





2014年04月29日 21:08:20[糖果天王](https://me.csdn.net/okcd00)阅读数：686








                
笨笨熊搬家打包篇 

描述: 森林里的笨笨熊今天可开心啦——他买了新房子，乔迁新喜要搬家了。因此，笨笨熊请了许多好朋友来帮忙搬家，并准备了很多小纸盒用来装需要搬的物品，不过，这些纸盒的容积都是相同的，***并且最多只能装两个物品***。但是，为了不打扰太多的朋友，笨笨熊想了个“聪明”办法：让每个纸盒使用效率最高(注：只要纸盒容积大于物品的体积之和就认为可以装下；物品体积不会大于纸盒容积)，这样需要的纸盒最少。为了帮助笨笨熊提前通知朋友，请你根据笨笨熊的办法，帮忙算出：需要纸盒的最少数目是多少？
   


运行时间限制: 无限制 

内存限制: 无限制 

输入: 整数V——纸盒的容积；

整数N——物品的总数目N；

共N个整数(对应N个物品的体积,每个整数用空格隔开)。


输出: 整数M——需要纸盒的最少数目；    


样例输入: 10

2 

2 3 

样例输出: 1 




最多只能俩……？！！！

**后记：共两题，吾辈最终得分123**



```cpp
#include <cstdio>
#include <algorithm>
#include <cstring>
#include <cstdlib>
#include <string>
#include <iostream>
using namespace std;
int cmp (const void * a, const void * b)
{
  return ( *(int*)b - *(int*)a );
}

int main()
{
	int cnts=0;
	int V=0;
	scanf("%d",&V);
	int N=0;
	scanf("%d",&N);
	int* val=new int[N];
	for(int i=0;i<N;i++)
	{
		scanf("%d",&val[i]);
	}
	qsort(val,N,sizeof(int),cmp);
	for(int j=0;j<N;j++)
	{
		if(val[j]==0)continue;
		else if(val[j]==V)
		{
			cnts++;
			continue;
		}
		else
		{
			int remain=V-val[j]	;
			for(int k=j+1;k<N&&remain!=0;k++)
			{
				if(val[k]<=remain)
				{
					remain-=val[k];
					val[k]=0;
					break; //能装俩还是更多 就差这一行！！！！！！//又出现了新的问题——加了之后从78分降低成44了……这题怎么回事……
				}
			}
			cnts++;
		}
	}
	
	printf("%d",cnts);
	return 0;
}
```


然后以下是一样的题微博上的解法：



```cpp
#include<stdio.h>
int js(int a[], int n,int v)
{
int m=0,j=0;
while (a[j]!='\0')
{
if ((a[j] + a[j + 1]) > v)
{
m += 1;
j += 1;
}
else if ((a[j] + a[j + 1]) <= v)
{
m += 1;
j += 2;
}
}
return m;
}

int main()
{
int v, n, a[100] = { 0 },i,m=0;
scanf("%d\n%d", &v, &n);
getchar();
for (i = 0; i < n; i++)
scanf("%d", &a[i]);
m = js(a, n,v);
printf("%d", m);
}
```









