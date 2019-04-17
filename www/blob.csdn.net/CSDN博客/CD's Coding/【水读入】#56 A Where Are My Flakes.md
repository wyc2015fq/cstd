# 【水读入】#56 A. Where Are My Flakes? - CD's Coding - CSDN博客





2014年08月20日 11:34:21[糖果天王](https://me.csdn.net/okcd00)阅读数：607标签：[codeforces](https://so.csdn.net/so/search/s.do?q=codeforces&t=blog)
个人分类：[CF-A题终结者](https://blog.csdn.net/okcd00/article/category/2156945)

所属专栏：[CF-A Terminator](https://blog.csdn.net/column/details/cf-amaster.html)
















![](http://espresso.codeforces.com/2738d292238372d2fe541a532f9c03b9da329470.png)

One morning the Cereal Guy found out that all his cereal flakes were gone. He found a note instead of them. It turned out that his smart roommate hid the flakes in one of *n* boxes.
 The boxes stand in one row, they are numbered from 1 to *n* from
 the left to the right. The roommate left hints like "Hidden to the left of the *i*-th box" ("To
 the left of *i*"), "Hidden to the right of the *i*-th
 box" ("To the right of *i*"). Such hints mean that there
 are no flakes in the *i*-th box as well. The Cereal Guy wants to know the minimal number of boxes he necessarily needs to check to find the flakes considering
 all the hints. Or he wants to find out that the hints are contradictory and the roommate lied to him, that is, no box has the flakes.




Input


The first line contains two integers *n* and *m* (1 ≤ *n* ≤ 1000, 0 ≤ *m* ≤ 1000)
 which represent the number of boxes and the number of hints correspondingly. Next *m* lines contain hints like "To
 the left of *i*" and "To the right of *i*",
 where *i* is integer (1 ≤ *i* ≤ *n*). The hints
 may coincide.




Output


The answer should contain exactly one integer — the number of boxes that should necessarily be checked or "-1" if the hints are contradictory.




Sample test(s)




input
2 1
To the left of 2




output
1




input
3 2
To the right of 1
To the right of 2




output
1




input
3 1
To the left of 3




output
2




input
3 2
To the left of 2
To the right of 1




output
-1















这道题的意思是说给n个箱子（按照1-n的编号），有m句话，告诉你对的箱子在X号箱子的左边或者右边，问你最终符合条件的箱子有多少个，如果一个都没有输出-1

字符串读入咯~ 记住用getline哟，cin或者%s都是空格截断的~



```cpp
#include <cstdio>
#include <memory>
#include <string>
#include <cstring>
#include <iostream>
#include <algorithm>
using namespace std;

int main()
{
	string s;
	int n,m,l,r,ans;	cin>>n>>m;
	l=0,r=n+1,ans=0;
	char huiche,dir;	scanf("%c",&huiche);
	for(int i=0;i<m;i++)
	{
		getline(cin,s);
		int len=s.length()-1,num=0,tlen;
		dir=s[7],tlen=len;
		//cout<<dir<<endl;
		while(s[tlen]!=' ')tlen--;
		for(int j=1;j+tlen<=s.length()-1;tlen++)
			num= num*10 + s[tlen+j]-'0';
		if(dir=='l')
		{
			r=min(r,num);
			if(num<l || num==1) ans=-1;
		}
		else if(dir=='r')
		{
			l=max(l,num);
			if(num>r || num==n)	ans=-1;
		}
		if(r<l || ans==-1)break;
	}
	
	if(ans==-1 || r-l==1)	cout<<"-1";
	else
	{
		ans=r-l-1;
		cout<<ans;
	}
	return 0;
}
```











