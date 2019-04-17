# 【水模拟】#35 A.Shell Game - CD's Coding - CSDN博客





2014年07月20日 18:09:35[糖果天王](https://me.csdn.net/okcd00)阅读数：487标签：[水题																[codeforces](https://so.csdn.net/so/search/s.do?q=codeforces&t=blog)](https://so.csdn.net/so/search/s.do?q=水题&t=blog)
个人分类：[CF-A题终结者](https://blog.csdn.net/okcd00/article/category/2156945)

所属专栏：[CF-A Terminator](https://blog.csdn.net/column/details/cf-amaster.html)














Today the «Z» city residents enjoy a shell game competition. The residents are gathered on the main square to watch the breath-taking performance. The performer puts 3 non-transparent
 cups upside down in a row. Then he openly puts a small ball under one of the cups and starts to shuffle the cups around very quickly so that on the whole he makes exactly 3 shuffles. After that the spectators have exactly one attempt to guess in which cup
 they think the ball is and if the answer is correct they get a prize. Maybe you can try to find the ball too?




Input

The first input line contains an integer from 1 to 3 — index of the cup which covers the ball before the shuffles. The following three lines describe the shuffles. Each description
 of a shuffle contains two distinct integers from 1 to 3 — indexes of the cups which the performer shuffled this time. The cups are numbered from left to right and are renumbered after each shuffle from left to right again. In other words, the cup on the left
 always has index 1, the one in the middle — index 2 and the one on the right — index 3.




Output

In the first line output an integer from 1 to 3 — index of the cup which will have the ball after all the shuffles.




Sample test(s)




input
1
1 2
2 1
2 1




output
2




input
1
2 1
3 1
1 3




output
2














我居然第12次才AC…… 简直不高兴…… 首先是不知道要文件输入输出……



然后就是各种RE……一怒之下就不用数组了——

 Code：



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
	freopen("input.txt","r",stdin);
	freopen("output.txt","w",stdout);
	
	int ball,n=3;
	int a,b;
	cin>>ball;
	while(n--)
	{
		cin>>a>>b;
		ball= a==ball?b:b==ball?a:ball;
	} 
	cout<<ball;
/* 
	int n,ball[5];
	scanf("%d",&n);
	ball[n]=1;
	for(int i=1;i<=3;i++)
	{
		int a,b;
		scanf("%d%d",&a,&b);
		if(ball[a]+ball[b]==0);
		else
		{
			ball[a]=1-ball[a];
			ball[b]=1-ball[b];
		}
	}
	for(int j=1;j<=3;j++)	if(ball[j]==1) printf("%I64d",(__int64)j); 
*/ 
	fclose(stdin);
	fclose(stdout);
	return 0;
}
```





﻿﻿




