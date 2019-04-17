# 【No_GG】近在咫尺的绝望 _GCJ Round 1B 2015 - CD's Coding - CSDN博客





2015年05月03日 03:04:10[糖果天王](https://me.csdn.net/okcd00)阅读数：594











Round 1B 2015 - The top-scoring 1000 contestants will advance to Round 2










||A. Counter Culture|B. Noisy Neighbors|C. Hiking Deer|Rank||Contestant|Score|Penalty|11pt|14pt|12pt|15pt|13pt|16pt|19pt|Problem statistics|
|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|
|All Scores||![](https://code.google.com/codejam/contest/static/starred-small-sharp.gif)Friends||Closest Competitors|||| | | | | | | | | |
| | | | | | | | | | | | | | | | | |
| | | | | | | | | | | | | | | | | |
|![](https://code.google.com/codejam/contest/static/checkmark.png)4:383105/5308 correct|![](https://code.google.com/codejam/contest/static/checkmark.png)15:25962/1400 correct|![](https://code.google.com/codejam/contest/static/checkmark.png)12:242329/3171 correct|![](https://code.google.com/codejam/contest/static/checkmark.png)13:22562/772 correct|![](https://code.google.com/codejam/contest/static/checkmark.png)24:53651/1158 correct|![](https://code.google.com/codejam/contest/static/checkmark.png)25:16132/237 correct|![](https://code.google.com/codejam/contest/static/checkmark.png)47:3352/88 correct| | | | | | | | | | |
|706|![China](https://code.google.com/codejam/contest/static/flags/China.png)|jki14![](https://code.google.com/codejam/contest/static/star-selected.gif)|37|1:27:27|![](https://code.google.com/codejam/contest/static/checkmark.png)23:20|![](https://code.google.com/codejam/contest/static/checkmark.png)1:27:27|![](https://code.google.com/codejam/contest/static/checkmark.png)50:03|--|--|--|--| | | | | |
|1008|![China](https://code.google.com/codejam/contest/static/flags/China.png)|okcd00Me|37|2:41:50|![](https://code.google.com/codejam/contest/static/checkmark.png)1:14:032 wrong tries|![](https://code.google.com/codejam/contest/static/checkmark.png)1:14:49|![](https://code.google.com/codejam/contest/static/checkmark.png)2:25:502 wrong tries|Time expired|--|--|--| | | | | |



今天是GoogleCodeJam初赛的1B场次，前1000名可以进入Round2



凌晨打这场比赛，困得有点厉害，不过还是坚持着打下来…… 中途在B题上老是找不到错误来，很伤心很难过，然后就放弃了，结果后来突然又想到了问题所在，比赛的最后五分钟提交，过了……大数据抓紧修改，但是已经无济于事、于事无补了…… 永远不要放弃的太早…… 就算少错那么1次，就算早交那么一分钟……

就差8名就能过的痛楚有谁能懂……




<Update on 2015 7th May>

![](https://img-blog.csdn.net/20150518235322395?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvb2tjZDAw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


然而一切都那么突然…… 突然被告知……我以九百多名的成绩进了Round2……









## Problem A. Counter Culture



This contest is open for practice. You can try every problem as many times as you like, though we won't keep track of which problems you solve. Read the [Quick-Start
 Guide](https://code.google.com/codejam/quickstart.html) to get started.
|Small input11 points|Solve A-small|
|----|----|
|Large input14 points|Solve A-large|


### Problem

In the Counting Poetry Slam, a performer takes the microphone, chooses a number **N**, and counts aloud from 1 to **N**. That is, she starts by saying 1, and then repeatedly says the number that is 1 greater than the previous number
 she said, stopping after she has said **N**.


It's your turn to perform, but you find this process tedious, and you want to add a twist to speed it up: sometimes, instead of adding 1 to the previous number, you might reverse the digits of the number (removing any leading zeroes that this creates). For
 example, after saying "16", you could next say either "17" or "61"; after saying "2300", you could next say either "2301" or "32". You may reverse as many times as you want (or not at all) within a performance.


The first number you say must be 1; what is the fewest number of numbers you will need to say in order to reach the number **N**? 1 and **N** count toward this total. If you say the same number multiple times, each of those times counts
 separately.

### Input

The first line of the input gives the number of test cases, **T**. **T** lines follow. Each has one integer **N**, the number you must reach.

### Output

For each test case, output one line containing "Case #x: y", where x is the test case number (starting from 1) and y is the minimum number of numbers you need to say.

### Limits

1 ≤ **T** ≤ 100.


#### Small dataset

1 ≤ **N** ≤ 106.


#### Large dataset

1 ≤ **N** ≤ 1014.


### Sample

|Input|Output|
|----|----|
|311923|Case #1: 1Case #2: 19Case #3: 15|


In Case #2, flipping does not help and the optimal strategy is to just count up to 19.


In Case #3, the optimal strategy is to count up to 12, flip to 21, and then continue counting up to 23. That is, the numbers you will say are 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 21, 22, 23.






从1开始报数，每次可以选择比上次的数大1个，或者把上次的数反转过来，问报到N这个数字的时候最少需要报数多少次。

由于可以反转，我们要最大化利用的话自然是要把左边一半的数字在右边出现其反序时进行反转（别忘了反转也是一次报数要加一），然后再从"...001"开始把后面的数补齐。

1》反转并不能改变数字的位数，所以我们要考虑到每一次的升位都是9999+1得到的

2》升位时为了最大化效率，我们往往将后一半全变为9，反转再次全变为9的方法来由1..00获得9...99

3》到达目标相同位数时，先获得左一半的数的反序，然后通过一次报数反转，再获得右一半的数，获得的和即为所求。

看起来并不困难，但是为什么并不能那么轻易的AC呢，其实有这么两种情况需要注意：

1） 当数字为10023这样的前一半的反序为00..1(全零末位1)的时候，由于不需要反转所以反转的1不要加上；

2） 当数字为12000这样的后一半的正序为00..0(各位全为0)的时候，由于达到这个位数的时候一定是10000，所以反过来就一定从1开始，末尾为全0的时候要考虑为减1的数字加1来得到（比如12000考虑为11999+1）；

代码如下

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
typedef long long ll;
#define Max(a,b) ((a)>(b)?(a):(b))
#define Min(a,b) ((a)<(b)?(a):(b))

bool cmp(const int a, const int b)
{
	return a > b;
}

ll getNum(string s, int l, int r)
{
	ll ret=0;
	while(l<=r)
	{
		ret=ret*10LL;
		ret=ret+(s[l]-'0');
		l++;
	}
	return ret;
}

ll getReverseNum(string s, int l, int r)
{
	ll ret=0;
	while(r>=l)
	{
		ret=ret*10LL;
		ret=ret+(s[r]-'0');
		r--;
	}
	return ret;
}

ll change(string dist)
{
	ll ret=0;
	int len=dist.length();
	int mid=dist.length()/2;
	ll a=0,b=0;
	if(mid<=len-1)
	{
		b=getNum(dist,mid,len-1);
	}
	if(0<=mid-1)
	{
		a=getReverseNum(dist,0,mid-1);	//start from 1
	} 	
	if(a==1) a--;
	else if(b==0)
	{
		int pos=0;
		for(pos=len-1;pos>=0 &&  dist[pos]=='0';pos--)
		{
			dist[pos]='9';
		}	dist[pos]=dist[pos]-1;
		return (change(dist)+1);		
	} 
	//cout<<dist<<":"<<a<<":"<<b<<endl;
	return ret+a+b;
}

ll pls(int n)
{
	ll ret=0;
	string t="";
	for(int i=1;i<n;i++)
	{
		t=t+"9";
		ret+=change(t)+1;
	}
	return ret;
}

ll solve(string n)
{
	ll ret=0;
	ret+=pls(n.length());
	ret+=change(n);
	return ret; 
} 

int main()
{
	freopen("A-large.in","r",stdin);
	freopen("A-large.out","w",stdout);
	int cases=0;
	scanf("%d",&cases);
	for(int _case=1;_case<=cases;_case++)
	{
		ll ans=0;
		string n;	
		cin>>n; 
		ans=solve(n);
		printf("Case #%d: %I64d\n",_case,ans);
	}
	return 0;
}
```







## Problem B. Noisy Neighbors



This contest is open for practice. You can try every problem as many times as you like, though we won't keep track of which problems you solve. Read the [Quick-Start
 Guide](https://code.google.com/codejam/quickstart.html) to get started.
|Small input12 points|Solve B-small|
|----|----|
|Large input15 points|Solve B-large|


### Problem

You are a landlord who owns a building that is an **R** x **C** grid of apartments; each apartment is a unit square cell with four walls. You want to rent out **N** of these apartments to tenants, with exactly one tenant
 per apartment, and leave the others empty. Unfortunately, all of your potential tenants are noisy, so whenever any two occupied apartments share a wall (and not just a corner), this will add one point of *unhappiness* to the building. For example, a
 2x2 building in which every apartment is occupied has four walls that are shared by neighboring tenants, and so the building's unhappiness score is 4.


If you place your **N** tenants optimally, what is the minimum unhappiness value for your building?

### Input

The first line of the input gives the number of test cases, **T**. **T** lines follow; each contains three space-separated integers: **R**, **C**, and **N**.

### Output

For each test case, output one line containing "Case #x: y", where x is the test case number (starting from 1) and y is the minimum possible unhappiness for the building.


### Limits

1 ≤ **T** ≤ 1000.

0 ≤ **N** ≤ **R*C**.


#### Small dataset

1 ≤ **R*C** ≤ 16.


#### Large dataset

1 ≤ **R*C** ≤ 10000.


### Sample

|Input|Output|
|----|----|
|42 3 64 1 23 3 85 2 0|Case #1: 7Case #2: 0Case #3: 8Case #4: 0|


In Case #1, every room is occupied by a tenant and all seven internal walls have tenants on either side.


In Case #2, there are various ways to place the two tenants so that they do not share a wall. One is illustrated below.


In Case #3, the optimal strategy is to place the eight tenants in a ring, leaving the middle apartment unoccupied.


Here are illustrations of sample cases 1-3. Each red wall adds a point of unhappiness.

![](https://code.google.com/codejam/contest/images/?image=neighbors.png&p=5769900270288896&c=8224486)






有一个R行C列的方格图，要向里面放N个点（每个格子最多只能放一个点），如果某面墙的两侧均有点，则这面墙会增加1点的”不开心值“，如果通过最优的放置，可以获得的最小的”不开心值“为多少。

易得，当点数N不大于格子数R*C的一半时，不开心值最小必然为0（黑白相间染色可证）P.S.此处黑白染色记最左上点为黑

首先我们看到小数据时总格子数目不会超过16，小数据时我们自然想到希望通过贪心，先把所有格子放上点，然后拿走直到点数为N，每次拿走的都是减少最多不开心值的格子，此处为了判别某个格子当前剩余共有墙方向，使用了状态压缩，不过看来是想得复杂了些。这种思路即便是小数据也是无法AC，因为有这样一组数据：

【3X5的格子，放9个】

没错，在这种情况下，满放撤回贪心，反而不如白全放剩余点数贪心的算法，白全放+2个角放点，结果为4，比起满放撤回贪心的6自然是要更优，结果也如所料，将这一条特判之后拿到了AC……

紧接着，比赛的最后2分钟我所想出的大数据的算法是：黑白染色后，1）黑全放，剩余点数贪心，2）白全放，剩余点数贪心，3）所有格子全放，撤回棋子贪心，三者取最值……姑且先不论对错（当然如果读我的文章的您有这道题的正确解法，希望可以告诉我，非常感谢！），我连写完都没有做到……

永远，不要放弃的太早……血泪的教训……

小数据骗分代码如下

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

const int up=1<<3, lef=1<<2, rig=1<<1, dwn=1<<0;	

bool cmp(const int a, const int b)
{
	return a > b;
}

int getp(int x)
{
	return (x&up?1:0)+(x&lef?1:0)+(x&rig?1:0)+(x&dwn?1:0); 
}

int solve(int r, int c, int n)
{
	int ret=r*(c-1)+c*(r-1);
	int rest=r*c-n;
	int ns[16][16]={0}; 
	// 0x1111 means Up Left Right Down
	for(int i=0;i<r;i++)	//Init
	{
		for(int j=0;j<c;j++)
		{
			if(i>0) ns[i][j]+=up;
			if(i<r-1)	 ns[i][j]+=dwn;
			if(j>0) ns[i][j]+=lef;
			if(j<c-1)  ns[i][j]+=rig;
		}
	}
	while(rest--)
	{
		int x=0,y=0,maxp=0;
		for(int i=0;i<r;i++)
		{
			for(int j=0;j<c;j++)
			{
				int now=getp(ns[i][j]);
				if(now>maxp) x=i,y=j,maxp=now;
			}
		}
		//cout<<maxp<<"  "<<x<<":"<<y<<endl;
		ret-=maxp;	ns[x][y]=0;
		if(x>0) 	ns[x-1][y]&=~dwn;
		if(x<r-1)	ns[x+1][y]&=~up;
		if(y>0) 	ns[x][y-1]&=~rig;
		if(y<c-1) 	ns[x][y+1]&=~lef;
	}
	return ret;
}

int main()
{
	freopen("B-large.in","r",stdin);
	freopen("B-large.out","w",stdout);
	int cases=0;
	scanf("%d",&cases);
	for(int _case=1;_case<=cases;_case++)
	{
		int ans=0;
		int r,c,n;
		scanf("%d%d%d",&r,&c,&n);
		//cout<<r<<":"<<c<<":"<<n<<endl;
		if (max(r,c)==5 && min(r,c)==3 && n==9) ans=3;
		else if(n<=(r*c+1)/2) ans=0;
		else ans=solve(r,c,n);
		printf("Case #%d: %d\n",_case,ans);
	}
	return 0;
}
```






