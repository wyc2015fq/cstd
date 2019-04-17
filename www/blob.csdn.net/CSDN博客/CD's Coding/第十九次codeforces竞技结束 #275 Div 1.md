# 第十九次codeforces竞技结束 #275 Div 1 - CD's Coding - CSDN博客





2014年10月25日 04:00:36[糖果天王](https://me.csdn.net/okcd00)阅读数：527











Permutation*p* is an ordered set of integers *p*1,   *p*2,   ...,   *p**n*,
 consisting of *n* distinct positive integers not larger than *n*.
 We'll denote as *n* the length of permutation *p*1,   *p*2,   ...,   *p**n*.


Your task is to find such permutation *p* of length *n*,
 that the group of numbers |*p*1 - *p*2|, |*p*2 - *p*3|, ..., |*p**n* - 1 - *p**n*| has
 exactly *k* distinct elements.




Input


The single line of the input contains two space-separated positive integers *n*, *k* (1 ≤ *k* < *n* ≤ 105).




Output


Print *n* integers forming the permutation. If there are multiple answers, print any of them.




Sample test(s)




input
3 2




output
1 3 2




input
3 1




output
1 2 3




input
5 2




output
1 3 2 4 5






Note


By |*x*| we denote the absolute value of number *x*.





吾辈的Div1的第一场，只做出来了A题（也就是Div2的C），19Min出的名次为

我采取的是来回弹的方式~ 咱们1、1+k、2、k、3、k-1…… 这样是不是就是k、k-1、k-2……了呀^_^，等k一路到1之后，把没用到的数字按顺序排一遍~

这样为啥成呢~ 

首先，这样一定可以保证k一路到1的绝对值都存在，请使用数学归纳法~

其次，为啥按顺序把剩下的写出来不会超出k呢？ 因为我们是从1和k+1两侧向中间走，所以最后剩一个数字和他后头那个数字的差距是k/2，依旧可行。

代码如下：



```cpp
#include <cmath> 
#include <cctype>
#include <cstdio>
#include <string>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <algorithm>
using namespace std;

#define Max(a,b) ((a)>(b)?(a):(b))
#define Min(a,b) ((a)<(b)?(a):(b))

bool cmp(const int a, const int b)
{
	return a > b;
}

bool usd[100086];

int main()
{
	int n,k,f=1,now;	
	scanf("%d%d",&n,&k);
	memset(usd,false,sizeof usd);
	for(int i=1;i<=k;i++)
	{
		if(f) now=(i+1)/2;
		else now=2+k-i/2;
		f=1-f;
		if(i-1)	printf(" %d",now);
		else printf("%d",now);
		usd[now]=true;
	}
	//cout<<"then"<<endl;
	for(int i=1;i<=n;i++)
	{
		if(usd[i]==false)
		printf(" %d",i);
	}
	return 0;
}
```











