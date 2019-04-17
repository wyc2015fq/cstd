# 第三十二次codeforces竞技结束 #299 Div 2 - CD's Coding - CSDN博客





2015年04月14日 23:47:07[糖果天王](https://me.csdn.net/okcd00)阅读数：493








写到第100000B次还在写Div2的感觉着实有点丢人…… 只能怪我的成长速度不如cf的题目难度提升速度了……

果然，还是太弱了……

这次绝对觉得E题是非常有希望出的，结果果然还是想多了……就算到现在都是出AC的最少的题…… 事实是白白消耗了比赛中七成的时间……




Problems

![](http://st.codeforces.com/images/icons/control.png)






|#|Name|||
|----|----|----|----|
|[A](http://codeforces.com/contest/534/problem/A)|[Exam](http://codeforces.com/contest/534/problem/A)standard input/output1 s, 256 MB|![Submit](http://st.codeforces.com/images/icons/submit-22x22.png)![Add to favourites](http://st.codeforces.com/images/icons/star_gray_16.png)|![](http://st.codeforces.com/images/icons/user.png) x3149|
|[B](http://codeforces.com/contest/534/problem/B)|[Covered Path](http://codeforces.com/contest/534/problem/B)standard input/output1 s, 256 MB|![Submit](http://st.codeforces.com/images/icons/submit-22x22.png)![Add to favourites](http://st.codeforces.com/images/icons/star_gray_16.png)|![](http://st.codeforces.com/images/icons/user.png) x2105|
|[C](http://codeforces.com/contest/534/problem/C)|[Polycarpus' Dice](http://codeforces.com/contest/534/problem/C)standard input/output1 s, 256 MB|![Submit](http://st.codeforces.com/images/icons/submit-22x22.png)![Add to favourites](http://st.codeforces.com/images/icons/star_gray_16.png)|![](http://st.codeforces.com/images/icons/user.png) x1582|
|[D](http://codeforces.com/contest/534/problem/D)|[Handshakes](http://codeforces.com/contest/534/problem/D)standard input/output1 s, 256 MB|![Submit](http://st.codeforces.com/images/icons/submit-22x22.png)![Add to favourites](http://st.codeforces.com/images/icons/star_gray_16.png)|![](http://st.codeforces.com/images/icons/user.png) x716|
|[E](http://codeforces.com/contest/534/problem/E)|[Berland Local Positioning System](http://codeforces.com/contest/534/problem/E)standard input/output2 s, 256 MB|![Submit](http://st.codeforces.com/images/icons/submit-22x22.png)![Add to favourites](http://st.codeforces.com/images/icons/star_gray_16.png)|![](http://st.codeforces.com/images/icons/user.png) x103|














An exam for *n* students will take place in a long and narrow room, so the students will sit in a line in some order. The teacher suspects that students with
 adjacent numbers (*i* and *i* + 1) always studied side
 by side and became friends and if they take an exam sitting next to each other, they will help each other for sure.


Your task is to choose the maximum number of students and make such an arrangement of students in the room that no two students with adjacent numbers sit side by side.




Input


A single line contains integer *n* (1 ≤ *n* ≤ 5000) —
 the number of students at an exam.




Output


In the first line print integer *k* — the maximum number of students who can be seated so that no two students with adjacent numbers sit next to each other.


In the second line print *k* distinct integers *a*1, *a*2, ..., *a**k* (1 ≤ *a**i* ≤ *n*),
 where *a**i* is
 the number of the student on the *i*-th position. The students on adjacent positions mustn't have adjacent numbers. Formally, the following should be true: |*a**i* - *a**i* + 1| ≠ 1 for
 all *i* from 1 to*k* - 1.

If there are several possible answers, output any of them.




Sample test(s)




input
6



output
6
1 5 3 6 2 4



input
3




output
2
1 3











有n个数字，要求任意相邻的两个不能是相邻的数（即差值的绝对值大于1），随意输出一种排列的方法。



我们选取最中间的一个数，偶数的话取中位数左右两侧任意选一个，它就是MID，放在末尾的那个了，一边儿呆着去，然后从这个MID的左右开始，左一个，右一个，左一个，右一个，直到放完（这里要注意哦，左右有的时候不会同时取完哦，当n是偶数的时候一边会比另一边多一个），当（左边到了0&&右边到了n）的时候，结束for，把MID输出出来。ok啦~啊对了，n=2的时候除外哦。

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
	int n;	cin>>n;
	if(n==1 || n==2) cout<<1<<endl<<1<<endl;
	else if(n==3) cout<<2<<endl<<"1 3"<<endl;
	else 
	{
		printf("%d\n",n);
		int mid=n/2+1;
		for(int i=1;mid-i>0 || mid+i<=n;i++)
		{
			if(mid-i>0) printf("%d ",mid-i);
			if(mid+i<=n) printf("%d ",mid+i);
		}
		printf("%d",n/2+1);
	}
	return 0;
}
```









The on-board computer on Polycarp's car measured that the car speed at the beginning of some section of the path equals *v*1 meters
 per second, and in the end it is *v*2 meters
 per second. We know that this section of the route took exactly *t* seconds to pass.


Assuming that at each of the seconds the speed is constant, and between seconds the speed can change at most by *d* meters per second in absolute value (i.e.,
 the difference in the speed of any two adjacent seconds does not exceed *d* in absolute value), find the maximum possible length of the path section in meters.




Input


The first line contains two integers *v*1 and *v*2 (1 ≤ *v*1, *v*2 ≤ 100)
 — the speeds in meters per second at the beginning of the segment and at the end of the segment, respectively.


The second line contains two integers *t* (2 ≤ *t* ≤ 100)
 — the time when the car moves along the segment in seconds, *d*(0 ≤ *d* ≤ 10) —
 the maximum value of the speed change between adjacent seconds.


It is guaranteed that there is a way to complete the segment so that:
- 
the speed in the first second equals *v*1,
- 
the speed in the last second equals *v*2,
- 
the absolute value of difference of speeds between any two adjacent seconds doesn't exceed *d*.




Output


Print the maximum possible length of the path segment in meters.




Sample test(s)




input
5 6
4 2




output
26



input
10 10
10 0




output
100





Note


In the first sample the sequence of speeds of Polycarpus' car can look as follows: 5, 7, 8, 6. Thus, the total path is 5 + 7 + 8 + 6 = 26meters.


In the second sample, as *d* = 0, the car covers the whole segment at constant speed *v* = 10.
 In *t* = 10 seconds it covers the distance of 100 meters.










第一眼看以为是物理吓我一跳……



初速度是v1，末速度是v2，这个车最快的速度变量（注意不是加速度，这车的速度是瞬间变化的，厉害吧）绝对值不大于d，问在t秒的范围内，车最多走多远。

我们知道，总路程=Sum（每秒钟的当前秒速）

所以在保证每两个相邻秒内速度差的绝对值不大于d的同时，尽可能让他们大，就行了。

据说叫尺取法？这个名词没有很懂呢，有懂的麻烦告诉我一声哦~

我是在起点和终点两端开始，同时向中间走，每次+d，到了中间碰头（啊奇数的时候我处理了一下，左边的递增序列拿到中位数的那个点），然后开始while(1)判断，当左线的末端与右线的末端差值的绝对值不大于d时结束。否则哪边高了哪边就退一步让矮的那边来升一步，这道题说一定有解就说明这种方法一定会获得解，如果不一定有解的话，聪明的小读者（哈哈哈我早就想这么说一次了）你们会进行怎样的改动呢？

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

#define Max(a,b) ((a)>(b)?(a):(b))
#define Min(a,b) ((a)<(b)?(a):(b))

bool cmp(const int a, const int b)
{
	return a > b;
}

int v[128]={0};

int getsum(int t)
{
	int s=0;	//就这里忘了初始化0，就给我算错么Q^Q 
	for(int i=0;i<t;i++)
	{
		s+=v[i];
		//cout<<i<<":"<<v[i]<<endl;
	}
	return s;
}

int main()
{
	int v1=0,v2=0,t=0,d=0,l=0,r=0;
	scanf("%d%d%d%d",&v1,&v2,&t,&d);
	v[0]=v1,v[t-1]=v2,r=t-1;//r=t-1 forgotten
	for(int i=1;i<t-i-1;i++)
	{
		l=i,r=t-i-1;
		v[l]=v[l-1]+d;
		v[r]=v[r+1]+d;
		//cout<<v[l]<<":"<<v[r]<<endl;
	}
	if(t%2)
	{
		l++;
		v[l]=v[l-1]+d;
	}
	//cout<<l<<":"<<r<<endl;
	while(abs(v[l]-v[r])>d)
	{
		if(v[l]>v[r]) v[l--]=v[r--]+d;
		else v[r++]=v[l++]+d;
	}
	cout<<getsum(t)<<endl;
	return 0;
}
```









Polycarp has *n* dice *d*1, *d*2, ..., *d**n*.
 The *i*-th dice shows numbers from 1 to *d**i*.
 Polycarp rolled all the dice and the sum of numbers they showed is *A*. Agrippina didn't see which dice showed what number, she knows only the sum *A* and
 the values *d*1, *d*2, ..., *d**n*.
 However, she finds it enough to make a series of statements of the following type: dice *i* couldn't show number *r*.
 For example, if Polycarp had two six-faced dice and the total sum is *A* = 11, then Agrippina can state that each of the two dice couldn't show a value less
 than five (otherwise, the remaining dice must have a value of at least seven, which is impossible).


For each dice find the number of values for which it can be guaranteed that the dice couldn't show these values if the sum of the shown values is *A*.




Input


The first line contains two integers *n*, *A* (1 ≤ *n* ≤ 2·105, *n* ≤ *A* ≤ *s*)
 — the number of dice and the sum of shown values where *s* = *d*1 + *d*2 + ... + *d**n*.


The second line contains *n* integers *d*1, *d*2, ..., *d**n* (1 ≤ *d**i* ≤ 106),
 where *d**i* is
 the maximum value that the *i*-th dice can show.




Output


Print *n* integers *b*1, *b*2, ..., *b**n*,
 where *b**i* is
 the number of values for which it is guaranteed that the *i*-th dice couldn't show them.




Sample test(s)




input
2 8
4 4




output
3 3 



input
1 3
5




output
4 



input
2 3
2 3




output
0 1 





Note


In the first sample from the statement *A* equal to 8 could be obtained in the only case when both the first and the second dice show 4. Correspondingly, both
 dice couldn't show values 1, 2 or 3.


In the second sample from the statement *A* equal to 3 could be obtained when the single dice shows 3. Correspondingly, it couldn't show 1, 2, 4 or 5.


In the third sample from the statement *A* equal to 3 could be obtained when one dice shows 1 and the other dice shows 2. That's why the first dice doesn't
 have any values it couldn't show and the second dice couldn't show 3.










有n个骰子，他们掷出的点数和为A。



然后告诉你每个骰子的最大点数di（意为他们的点数只能在1-di间取得）。问，对于每个骰子，它绝对不可能是的点数有多少种情况。

我们记di的总和为s

我们要这么想：对于某个骰子，除去它以外，别的骰子的点数和的取值区间，应该是不小于骰子数（n-1），不大于除去当前骰子以外的最大点数和（s-di），能想到这一步的话，不能掷出的点数种类数就明了了吧？好吧还是不清楚么？当前骰子总共有di种点数，其中它不能太小，小到其它骰子都是最大点数都不够到A，也不能太大，大到其他骰子都是最小点数1也超过A了。

Code：



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

bool cmp(const int a, const int b)
{
	return a > b;
}

int n=0,d[200086]={0};
ll  a=0,b[200086]={0},s=0;

int main()
{
	scanf("%d%I64d",&n,&a);
	for(int i=0;i<n;i++)
	{
		scanf("%d",&d[i]);
		s+=d[i];
	}
	for(int i=0;i<n;i++)
	{
		//不能取的有两种情况： 
		ll si=a-s+d[i]-1; //small_impossible=其他都取最大时，不能小于某值 
		ll bi=d[i]-a+n-1;//big_impossible=其他都取最小值1时，不能大于某值 
		ll ai=0LL;	//all_impossible 
		if(si>=0 && bi>=0) ai=si+bi;
		else if(si>=0) ai=si;
		else if(bi>=0) ai=bi;
		else ai=0LL;
		b[i]=max(0LL,ai);
		printf("%I64d",b[i]);
		if(i<n-1)printf(" "); 
	}
	return 0;
}
```



以及悲惨的自以为能过的E题……

因为错了还是不说解题思路了，因为可能思路就是错的……

Code：



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
bool cmp(const int a, const int b)
{
	return a > b;
}

ll ans=0LL,tmp=0LL;
ll dp[200086]={0};
int mrk[200086]={0},tmrk[200086]={0};

int main()
{
	int n=0, now=0, i=0;	cin>>n;
	for(i=1;i<=n;i++)
	{
		scanf("%I64d",&dp[i]);
	}
	int m=0;	cin>>m;
	for(i=0;i<m;i++)
	{
		scanf("%d",&now);
		mrk[now]++;
		tmrk[now]++;
	}
	if(n==2)
	{
		printf("%I64d",(dp[2]-dp[1])*(mrk[1]+mrk[2]-1));
		return 0;
	}
	int mr=min(mrk[1],mrk[n]);
	for(int i=2;i<=n-1;i++)
	{
		tmrk[i]/=2;
		mr=min(mr,tmrk[i]);
	} 
	if(mrk[1]>1 || mrk[n]>1 || mr>=1)
	{
		int round=min(mrk[1],mrk[n]);
		round=min(mr,round);
		mrk[1]-=round; mrk[n]-=round;
		for(int i=2;i<=n-1;i++)
		mrk[i]-=round*2;
		ans+=(dp[n]-dp[1])*2*round;
//		cout<<round<<":"<<mrk[1]<<mrk[2]<<mrk[3];
//		cout<<"now"<<ans<<endl;
	}
	if(m==n*2-2)
	{
		ll dist=dp[2]-dp[1];
		if(n!=2) for(int i=3;i<=n;i++)
		{
			if(dp[i]-dp[i-1]!=dist)
			{
				printf("-1");
				return 0;
			}
		}
		cout<<(ll)(dp[n]-dp[1])*2LL-dist<<endl;
		return 0;
	}
	int f1=0,f2l=0,f2r=0;
	int l1=0,l2=0,
		r1=0,r2=0;
	i=1;	while(mrk[i]==0)i++; 
	if(mrk[1]==1 && mrk[2]==2)
	{
		mrk[1]=2;
		f2l=l2=1;
		while(mrk[l2]==2 && l2<=n)l2++;l2--;
	} 
	if(mrk[n]==1 && mrk[n-1]==2)
	{
		mrk[n]=2;
		f2r=1;
		r2=n;
		while(mrk[r2]==2 && r2>=1)r2--;r2++;
	}
	for(i=max(l2,i);i<=n;i++)
	{
		if(f1==0 && mrk[i]==1)
		{
			f1=1;
			l1=r1=i;
			while(mrk[r1]==1 && r1<=n) r1++;r1--;
		}
	}
	//ALL	l2===================r2 <already considered>
	//f2l\f2r\f1 =========l2/l1--------------r1/r2============
	//f2l		==========l2
	//f2r		r2========== 
	//f1\f2r     l1-------------r1/r2=============
	//f1\f2l	============l2/l1------------
	//f1		l1---------------------l2
	
	//cout<<l1<<":"<<r1<<"  "<<l2<<":"<<r2<<endl;
	if(f1) ans+=(ll)(dp[r1]-dp[l1]);
	if(f2l) ans+=(ll)(dp[l2]-dp[1])*2LL;
	if(f2r) ans+=(ll)(dp[n]-dp[r2])*2LL;
	if(f1&&f2l) ans+=(ll)(dp[l1]-dp[l1-1]);
	if(f1&&f2r) ans+=(ll)(dp[r1+1]-dp[r1]);
	cout<<ans<<endl;
	return 0;
}
```





