# 【水循环数组】#STC 3 A. Codecraft III - CD's Coding - CSDN博客





2014年07月29日 00:07:47[糖果天王](https://me.csdn.net/okcd00)阅读数：511标签：[codeforces																[水题](https://so.csdn.net/so/search/s.do?q=水题&t=blog)](https://so.csdn.net/so/search/s.do?q=codeforces&t=blog)
个人分类：[CF-A题终结者](https://blog.csdn.net/okcd00/article/category/2156945)

















Today Vasya visited a widely known site and learned that the continuation of his favourite game Codecraft II will appear after exactly *k* months. He looked
 at the calendar and learned that at the moment is the month number *s*. Vasya immediately got interested in what month Codecraft III will appear. Help him
 understand that.


All the twelve months in Vasya's calendar are named using their usual English names: January,February, March, April, May, June, July, August, September, October, November, December.




Input


The first input line contains the name of the current month. It is guaranteed that it is a proper English name of one of twelve months. The first letter is uppercase, the rest are lowercase. The second line contains integer *k*(0 ≤ *k* ≤ 100)
 — the number of months left till the appearance of Codecraft III.




Output


Print starting from an uppercase letter the name of the month in which the continuation of Codeforces II will appear. The printed name must be contained in the list January, February, March, April, May, June,July, August, September, October, November, December.




Sample test(s)




input
November
3




output
February




input
May
24




output
May












给一个月份的字符串，给一个数字，问你过去了这么多个月之后是几月。



循环数组用取模实现即可：



```cpp
#include <map> 
#include <queue>
#include <cmath> 
#include <memory> 
#include <cctype>
#include <cstdio>
#include <string>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <algorithm>
using namespace std;

string m[12]={"January","February","March","April","May","June",
"July","August","September","October","November","December"};

int main()
{
	string str;	cin>>str;
	int n;	cin>>n;
	int idx=0;
	for(int i=0;i<12;i++)	if(m[i]==str)idx=i; 
	idx= (idx+n)%12;
	cout<<m[idx];
	return 0;
}
```















