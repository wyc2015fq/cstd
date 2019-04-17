# 第二十五次codeforces竞技结束 #285 Div 2 - CD's Coding - CSDN博客





2015年01月28日 17:26:15[糖果天王](https://me.csdn.net/okcd00)阅读数：479











Problems

![](http://codeforces.com/static/images/icons/control.png)






|#|Name|||
|----|----|----|----|
|[A](http://codeforces.com/contest/501/problem/A)|[Contest](http://codeforces.com/contest/501/problem/A)standard input/output1 s, 256 MB|![Submit](http://codeforces.com/static/images/icons/submit-22x22.png)![Add to favourites](http://codeforces.com/static/images/icons/star_gray_16.png)|![](http://codeforces.com/static/images/icons/user.png) x3271|
|[B](http://codeforces.com/contest/501/problem/B)|[Misha and Changing Handles](http://codeforces.com/contest/501/problem/B)standard input/output1 s, 256 MB|![Submit](http://codeforces.com/static/images/icons/submit-22x22.png)![Add to favourites](http://codeforces.com/static/images/icons/star_gray_16.png)|![](http://codeforces.com/static/images/icons/user.png) x2473|





这一场比赛是在算法考试前日晚上做的，做了一小时然后去吃饭了，姑且只有AB两题……










Misha and Vasya participated in a Codeforces contest. Unfortunately, each of them solved only one problem, though successfully submitted it at the first attempt. Misha solved the problem that costs *a* points
 and Vasya solved the problem that costs *b* points. Besides, Misha submitted the problem *c* minutes
 after the contest started and Vasya submitted the problem *d* minutes after the contest started. As you know, on Codeforces the cost of a problem reduces
 as a round continues. That is, if you submit a problem that costs *p* points *t*minutes
 after the contest started, you get ![](http://espresso.codeforces.com/e7bb7983e9c15baf66651c7643406c249354c8c1.png) points.


Misha and Vasya are having an argument trying to find out who got more points. Help them to find out the truth.




Input


The first line contains four integers *a*, *b*, *c*, *d* (250 ≤ *a*, *b* ≤ 3500, 0 ≤ *c*, *d* ≤ 180).


It is guaranteed that numbers *a* and *b* are divisible
 by 250 (just like on any real Codeforces round).




Output


Output on a single line:


"Misha" (without the quotes), if Misha got more points than Vasya.


"Vasya" (without the quotes), if Vasya got more points than Misha.


"Tie" (without the quotes), if both of them got the same number of points.




Sample test(s)




input
500 1000 20 30




output
Vasya




input
1000 1000 1 1




output
Tie




input
1500 1000 176 177




output
Misha












这题使用了Codeforces上的算分机智，就是某题的得分 ![](http://espresso.codeforces.com/e7bb7983e9c15baf66651c7643406c249354c8c1.png) ，然后有四个数字，分别是Misha做的题目总分，Vasya做的题目总分，Misha出题时间，Vasya出题时间。
 问谁得分高（平局的时候输出Tie）



题意理解了之后这就是一个简单的代入计算问题啦~



```cpp
double mp=max(3*a/10 , a-(a/250*c) );
double vp=max(3*b/10 , b-(b/250*d) );
```

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

bool cmp(const int a, const int b)
{
	return a > b;
}

int main()
{
	int a=0,b=0,c=0,d=0;
	cin>>a>>b>>c>>d;
	double mp=max(3*a/10 , a-(a/250*c) );
	double vp=max(3*b/10 , b-(b/250*d) );
	if(mp>vp) cout<<"Misha";
	else if(mp<vp) cout<<"Vasya";
	else cout<<"Tie";
	return 0;
}
```












Misha hacked the Codeforces site. Then he decided to let all the users change their handles. A user can now change his handle any number of times. But each new handle must not be equal to any handle that is already used or that was used at some point.


Misha has a list of handle change requests. After completing the requests he wants to understand the relation between the original and the new handles of the users. Help him to do that.




Input


The first line contains integer *q* (1 ≤ *q* ≤ 1000),
 the number of handle change requests.


Next *q* lines contain the descriptions of the requests, one per line.


Each query consists of two non-empty strings *old* and *new*,
 separated by a space. The strings consist of lowercase and uppercase Latin letters and digits. Strings *old* and *new* are
 distinct. The lengths of the strings do not exceed 20.


The requests are given chronologically. In other words, by the moment of a query there is a single person with handle *old*, and handle*new* is
 not used and has not been used by anyone.




Output


In the first line output the integer *n* — the number of users that changed their handles at least once.


In the next *n* lines print the mapping between the old and the new handles of the users. Each of them must contain two strings, *old* and*new*,
 separated by a space, meaning that before the user had handle *old*, and after all the requests are completed, his handle is *new*.
 You may output lines in any order.


Each user who changes the handle must occur exactly once in this description.




Sample test(s)




input
5
Misha ILoveCodeforces
Vasya Petrov
Petrov VasyaPetrov123
ILoveCodeforces MikeMirzayanov
Petya Ivanov




output
3
Petya Ivanov
Misha MikeMirzayanov
Vasya VasyaPetrov123












这道题说的是Codeforces的更名服务，每个人都可以改名字，经过一系列的改名之后，问最后列出所有人的原始名和最终名的对应关系。



我们可以意识到，中间的名字根本没有意义，当A变成B，B变成C的时候等价于A变成C，那么我们就直接记录当前的“原始-现名”关系即可，这里我使用了map，map<string,string>就可以实现 m[A]=B,然后当新的B变成C时更改为m[A]=C即可，这需要对STL中map的理解，当然用struct node也是可以的，题目中放宽要求可以in any order应该就是这个考虑吧，map可以自动把对应关系按照原始名的字典序排序，也是相当便利的事情。

### Code：



```cpp
#include <map>
#include <cmath> 
#include <cctype>
#include <cstdio>
#include <string>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <algorithm>
using namespace std;
typedef map<string, string> mss;
#define Max(a,b) ((a)>(b)?(a):(b))
#define Min(a,b) ((a)<(b)?(a):(b))

bool cmp(const int a, const int b)
{
	return a > b;
}

int main()
{
	int n=0;	cin>>n;
	mss mp;
	while(!mp.empty()) mp.clear();
	for(int i=0;i<n;i++)
	{
		int flag=1;
		string a,b;		cin>>a>>b;
		for (map<string,string>::iterator it=mp.begin(); it!=mp.end(); ++it)
		{
			if(it->second==a)
			{
				mp[it->first]=b;
				flag=0;
				break;
			}
		}
		if(flag) mp[a]=b;
	}
	cout<<mp.size()<<endl;
	for (map<string,string>::iterator it=mp.begin(); it!=mp.end(); ++it)
	{
		cout << it->first << " " << it->second << endl;
	}
	return 0;
}
```

















