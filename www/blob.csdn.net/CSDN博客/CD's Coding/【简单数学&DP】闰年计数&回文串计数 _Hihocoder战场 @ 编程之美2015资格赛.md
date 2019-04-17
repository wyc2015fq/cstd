# 【简单数学&DP】闰年计数&回文串计数 _Hihocoder战场 @ 编程之美2015资格赛 - CD's Coding - CSDN博客





2015年04月20日 01:07:13[糖果天王](https://me.csdn.net/okcd00)阅读数：690标签：[回文字符序列																[2月29日																[编程之美																[2015																[Hihocoder](https://so.csdn.net/so/search/s.do?q=Hihocoder&t=blog)
个人分类：[Coding Competition](https://blog.csdn.net/okcd00/article/category/2133459)









## 题目1 : 2月29日





时间限制:2000ms

单点时限:1000ms

内存限制:256MB



### 描述

给定两个日期，计算这两个日期之间有多少个2月29日（包括起始日期）。

只有闰年有2月29日，满足以下一个条件的年份为闰年：

1. 年份能被4整除但不能被100整除

2. 年份能被400整除

### 输入

第一行为一个整数T，表示数据组数。

之后每组数据包含两行。每一行格式为"month day, year"，表示一个日期。month为{"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November" , "December"}中的一个字符串。day与year为两个数字。

数据保证给定的日期合法且第一个日期早于或等于第二个日期。

### 输出

对于每组数据输出一行，形如"Case #X: Y"。X为数据组数，从1开始，Y为答案。

### 数据范围

1 ≤ T ≤ 550

小数据：

2000 ≤ year ≤ 3000

大数据：

2000 ≤ year ≤ 2×109



样例输入
4
January 12, 2012
March 19, 2012
August 12, 2899
August 12, 2901
August 12, 2000
August 12, 2005
February 29, 2004
February 29, 2012

样例输出
Case #1: 1
Case #2: 0
Case #3: 1
Case #4: 3





这次在Hihocoder战场只看了两道题，两道都是1A，现在判定也结束了，两题的大小数据均AC，现在写一写题解作为备忘。

首先就是这个第一题，给两个日期，问你两个日期之间经历了多少个闰年。

首先，隔了那么几百万年我们肯定是不会逐年计算的对吧，我们要考虑到这一点才行哦~

【每400年会经历97个闰年】（400/4）-（400/100）+（400/400）

那么我们直接把后一个年份减去前一个年份对400取余，直接问题就简化成两者年份不大于400年之间有多少个闰年啦~

但是这也不是一眼能看出来的哦~

日期是不是很讨厌，如果所有的日期都在1月1日，那么是否只要考虑年份是否是闰年的年份就行了呢？

那我们就把日期都变成下一年的1月1日吧（其实变成当年1月1日也一样，一个意思），用一个函数判断这样的日期推移会少算一个还是多算一个闰年，多退少补即可。

然后~ 就从年份A到年份B数闰年吧~（其实直接对里面年份是4的倍数的进行判定就好，还能省一大半时间呢~）

代码如下：

### Code：



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

string m_name[12]={"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November" , "December"}; 

bool cmp(const int a, const int b)
{
	return a > b;
}

struct date_bag
{
	int year;
	int month;
	int day;
};

int getM(string s)
{
	for(int i=0;i<12;i++)
	{
		if(m_name[i]==s) return i+1;
	}
	return -1;
}

int jrun(int a)
{
	if(a%4==0 && a%100!=0) return 1;
	if(a%400==0) return 1;
	return 0;
}
 

int judge(date_bag db)	//移至下一年 Jan.1st 是否会经历闰年 
{
	if(jrun(db.year)==0) return 0;
	else
	{
		if(db.month>2) return 0;
		else return 1;
	}
}

int getrun(date_bag a,date_bag b)
{
	int cnt=0,y1=a.year+1,y2=b.year+1;
	if(jrun(b.year) && b.month==2 && b.day==29)cnt++;
	cnt+=judge(a);
	cnt-=judge(b);
	if(y1%4) y1+=4-y1%4;
	for(int i=y1;i<y2;i++)
	{
		if(jrun(i))cnt++;
	}
	return cnt;
}

int main()
{
	int cases=0;
	scanf("%d",&cases);
	for(int _case=1;_case<=cases;_case++)
	{
		int ans=0;
		date_bag date[3];
		string m1,m2,d1,d2;
		cin>>m1>>d1>>date[1].year;
		cin>>m2>>d2>>date[2].year;
		date[1].month=getM(m1);	date[2].month=getM(m2);
		if(d1.length()==2) date[1].day=d1[0]-'0';
		else date[1].day=(d1[0]-'0')*10 + (d1[1]-'0');
		if(d2.length()==2) date[2].day=d2[0]-'0';
		else date[2].day=(d2[0]-'0')*10 + (d2[1]-'0');
		if(date[2].year-date[1].year>400)
		{
			int round=(date[2].year-date[1].year)/400;
			date[2].year-=round*400;
			ans+=round*97;
		}
		ans+=getrun(date[1],date[2]);
		printf("Case #%d: %d\n",_case,ans);
	}
	return 0;
}
```









## 题目2 : 回文字符序列





时间限制:2000ms

单点时限:1000ms

内存限制:256MB



### 描述

给定字符串，求它的回文子序列个数。回文子序列反转字符顺序后仍然与原序列相同。例如字符串aba中，回文子序列为"a", "a", "aa", "b", "aba"，共5个。内容相同位置不同的子序列算不同的子序列。

### 输入

第一行一个整数T，表示数据组数。之后是T组数据，每组数据为一行字符串。

### 输出

对于每组数据输出一行，格式为"Case #X: Y"，X代表数据编号（从1开始），Y为答案。答案对100007取模。

### 数据范围

1 ≤ T ≤ 30

小数据

字符串长度 ≤ 25

大数据

字符串长度 ≤ 1000






样例输入
5
aba
abcbaddabcba
12111112351121
ccccccc
fdadfa


样例输出
Case #1: 5
Case #2: 277
Case #3: 1333
Case #4: 127
Case #5: 17





给定一个字符串，问其中共有多少个回文子串（长度为1的也算），以及记得对100007取模。

b数组是指，从s[i]到s[j]的回文子串数目，然后DP。

具体的思路大概是这样。

首先，i==j的时候就是说每一个单独的字符，是长度为1的回文子串，所以先做好b[i][i]=1初始化。

然后，以每一个字符为中心位置，另一个变量枚举从0到len-i的位置进行更新，唔，感觉文字好难能讲的清楚，推荐在纸上画一下，照着下面的方程，状态转移方程如下：



```cpp
if(s[j]==s[i+j]) { b[j][i+j]=(b[j][i+j-1]+b[j+1][i+j]+1+vmod)%vmod; }
else{tmp= (i+j-1<j+1)?0:b[j+1][i+j-1];
b[j][i+j]=(b[j][i+j-1]+b[j+1][i+j]-tmp+vmod)%vmod; }
```
大概是这种感觉。



具体代码如下：

### Code:



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
typedef long long ll;
#define Max(a,b) ((a)>(b)?(a):(b))
#define Min(a,b) ((a)<(b)?(a):(b))

int vmod=100007;
int b[1024][1024];

bool cmp(const int a, const int b)
{
	return a > b;
}

int solve(string s)
{
	int len=s.length();
	for(int i=0;i<len;i++) b[i][i]=1;	//init
	for(int i=1;i<len;i++)
		for(int j=0;j<len-i;j++)
		{
			int tmp=0;
			if(s[j]==s[i+j])
			{
				b[j][i+j]=(b[j][i+j-1]+b[j+1][i+j]+1+vmod)%vmod;
			}
			else
			{
				tmp= (i+j-1<j+1)?0:b[j+1][i+j-1];
				b[j][i+j]=(b[j][i+j-1]+b[j+1][i+j]-tmp+vmod)%vmod;
			}
		}
	return (b[0][len-1]+vmod)%vmod;
}

int main()
{
	int cases=0;
	scanf("%d",&cases);
	for(int _case=1;_case<=cases;_case++)
	{
		memset(b,0,sizeof b);
		string s;	cin>>s;
		int ans=0; ans=solve(s); 
		printf("Case #%d: %d\n",_case,ans);
	}
	return 0;
}
```





## 题目3 : 基站选址





时间限制:2000ms

单点时限:1000ms

内存限制:256MB



### 描述

需要在一个N × M的网格中建立一个通讯基站，通讯基站仅必须建立在格点上。

网格中有A个用户，每个用户的通讯代价是用户到基站欧几里得距离的平方。

网格中还有B个通讯公司，维护基站的代价是基站到最近的一个通讯公司的路程（路程定义为曼哈顿距离）。

在网格中建立基站的总代价是用户通讯代价的总和加上维护基站的代价，最小总代价。

### 输入

第一行为一个整数T，表示数据组数。

每组数据第一行为四个整数：N, M, A, B。

接下来的A+B行每行两个整数x, y，代表一个坐标，前A行表示各用户的坐标，后B行表示各通讯公司的坐标。

### 输出

对于每组数据输出一行"Case #X: Y"，X代表数据编号（从1开始），Y代表所求最小代价。

### 数据范围

1 ≤ T ≤ 20

1 ≤ x ≤ N

1 ≤ y ≤ M

1 ≤ B ≤ 100

小数据

1 ≤ N, M ≤ 100

1 ≤ A ≤ 100

大数据

1 ≤ N, M ≤ 107

1 ≤ A ≤ 1000



样例输入
2
3 3 4 1
1 2
2 1
2 3
3 2
2 2
4 4 4 2
1 2
2 4
3 1
4 3
1 4
1 3

样例输出
Case #1: 4
Case #2: 13




想了想，虽然未能有勇气去挑战第三题，不过既然从童鞋那里获得了第三题的AC代码，还是贴于此可用于以后细细揣摩，善哉善哉。

### Code：



```cpp
#include <iostream>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <algorithm>
using namespace std;
int num;
typedef long long ll;
ll res;
int n,m,a,b;
const double eps = 1e-8;
int ax[1010], ay[1010];
int bx[1010], by[1010];
const ll INF = 0x3f3f3f3f3f3f3f3f;
ll solve(int x, int y)
{
	ll res = 0;
	for(int i=1; i<=a; i++)
		res += (x-ax[i])*(x-ax[i]) + (y-ay[i])*(y-ay[i]);
	ll t = INF;
	for(int j=1; j<=b; j++)
	{
		ll temp = abs(x - bx[j]) + abs(y-by[j]);
		t = min(temp, t);
	}
	res += t; 
	return res;
}
ll sanfeny(int x)
{
	ll temp = INF;
	int l = 0;
	int r = m;
	while(l<r)
	{
		int m1 = (r+l)/2;
		int m2 = (m1+r)/2;
		ll t1 = solve(x, m1);
		ll t2 = solve(x, m2);
		if(t1>t2)
			l = m1;
		else
			r = m2;
		temp = min(temp, min(t1,t2));
	}
	return temp;
}
int main()
{
	cin>>num;
	int cn = 0;
	while(num--)
	{
		res = INF;
		cin>>n>>m>>a>>b;
		for(int i=1; i<=a; i++)
			scanf("%d %d", ax+i, ay+i);
		for(int j=1; j<=b; j++)
			scanf("%d %d", bx+j, by+j);
		int l = 0;
		int r = n;
		while(l<r)
		{
			int m1 = (l+r)>>1;
			int m2 = (m1+r)>>1;
			ll t1 = sanfeny(m1);
			ll t2 = sanfeny(m2);
			if(t1>t2)
				l = m1;
			else
				r = m2;
			res = min(res, min(t1, t2));
		}
		printf("Case #%d: ", ++cn);
		cout<<res<<endl;
	}
	return 0;
}
```










把题目链接和结果放在这里，大家可以去Hiho上自己尝试一下哦~


|编号|名称|通过率|通过人数|提交人数|
|----|----|----|----|----|
|[A](http://hihocoder.com/contest/msbop2015qual/problem/1)|[2月29日](http://hihocoder.com/contest/msbop2015qual/problem/1)|48%|2827|5870|
|[B](http://hihocoder.com/contest/msbop2015qual/problem/2)|[回文字符序列](http://hihocoder.com/contest/msbop2015qual/problem/2)|25%|684|2719|
|[C](http://hihocoder.com/contest/msbop2015qual/problem/3)|[基站选址](http://hihocoder.com/contest/msbop2015qual/problem/3)|17%|330|1925|](https://so.csdn.net/so/search/s.do?q=2015&t=blog)](https://so.csdn.net/so/search/s.do?q=编程之美&t=blog)](https://so.csdn.net/so/search/s.do?q=2月29日&t=blog)](https://so.csdn.net/so/search/s.do?q=回文字符序列&t=blog)




