# 第二十次codeforces竞技结束 #276 Div 2 - CD's Coding - CSDN博客





2014年11月06日 04:48:45[糖果天王](https://me.csdn.net/okcd00)阅读数：2128











真是状况百出的一次CF啊……

最终还Unrated了，你让半夜打cf 的我们如何释怀（中途茫茫多的人都退场了）……虽说打得也不好……

在这里写一下这一场codeforces的解题报告，A-E的 题目及AC代码，部分题目有简单评析，代码还算清晰，主要阅读代码应该不难以理解。








Questions about problems

![](http://codeforces.com/static/images/icons/control.png)






|#|Author|Problem|When|Question|Answer|
|----|----|----|----|----|----|
||||2014-11-05 21:24:38|Announcement|General announcement*****Issue of problem locking was fixed. Now you should be able to lock task if you passed pretests.|
||||2014-11-05 21:18:46|Announcement|General announcement*****You may be unable to lock some problems for hacking. This will be fixed soon.|
||||2014-11-05 20:23:29|Announcement|General announcement*****This round will be unrated due to technical issues. Duration will be extended by 30 minutes. Testing queue is really long. Continue solving other problems. We are sorry for an inconvenience.|













One industrial factory is reforming working plan. The director suggested to set a mythical detail production norm. If at the beginning of the day there were *x* details
 in the factory storage, then by the end of the day the factory has to produce ![](http://espresso.codeforces.com/4085628a7f3a4ad1ec3a2d67435e8a7ce3eb797d.png) (remainder
 after dividing *x* by *m*)
 more details. Unfortunately, no customer has ever bought any mythical detail, so all the details produced stay on the factory.


The board of directors are worried that the production by the given plan may eventually stop (that means that there will be а moment when the current number of details on the factory is divisible by *m*).


Given the number of details *a* on the first day and number *m* check
 if the production stops at some moment.




Input


The first line contains two integers *a* and *m* (1 ≤ *a*, *m* ≤ 105).




Output


Print "Yes" (without quotes) if the production will eventually stop, otherwise print "No".




Sample test(s)




input
1 5




output
No




input
3 6




output
Yes












给两个数字a和m，工厂每次看a是多少就生产a个东西，然后把a变为a%m，如果a为0工序就崩盘了，问是否会崩盘



那就用大小为m的一个vis数组来记录当前余数是否被用过，然后模拟，每次记录当前余数，如果余数变成0了输出Yes，如果余数到达了曾经有过的余数位置，那么就会以此为一个循环永远循环下去，那么我们break，输出No

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

int main()
{
	int a,m;	cin>>a>>m;
	int vis[100086]={0};
	while(1)
	{
		if(a==0){cout<<"Yes";return 0;}
		if(vis[a]==1){cout<<"No";return 0;}
		vis[a]=1;
		a=(a+a)%m;
	}
	return 0;
}
```









Many computer strategy games require building cities, recruiting army, conquering tribes, collecting resources. Sometimes it leads to interesting problems.


Let's suppose that your task is to build a square city. The world map uses the Cartesian coordinates. The sides of the city should be parallel to coordinate axes. The map contains mines with valuable resources, located at some points with integer coordinates.
 The sizes of mines are relatively small, i.e. they can be treated as points. The city should be built in such a way that all the mines are inside or on the border of the city square.


Building a city takes large amount of money depending on the size of the city, so you have to build the city with the minimum area. Given the positions of the mines find the minimum possible area of the city.




Input


The first line of the input contains number *n* — the number of mines on the map (2 ≤ *n* ≤ 1000).
 Each of the next *n* lines contains a pair of integers *x**i* and *y**i* —
 the coordinates of the corresponding mine ( - 109 ≤ *x**i*, *y**i* ≤ 109).
 All points are pairwise distinct.




Output


Print the minimum area of the city that can cover all the mines with valuable resources.




Sample test(s)




input
2
0 0
2 2




output
4




input
2
0 0
0 3




output
9












有一个城市需要建造，给你许多矿坑d坐标点，问把这么多矿坑全都包进城市的话，城市所需最小面积是多少（注意，城市为平行于坐标轴的正方形）



这不知道算不算凸包，反正记录最大最小的x和y，然后相减获得最小矩形长宽，取两者较长边平方即可。

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
const int inf=(int)1e9+10086;
#define Max(a,b) ((a)>(b)?(a):(b))
#define Min(a,b) ((a)<(b)?(a):(b))

bool cmp(const int a, const int b)
{
	return a > b;
}

int main()
{
	int n;	cin>>n;
	int up=-inf,down=inf,left=inf,right=-inf;
	for(int i=0;i<n;i++)
	{
		int x,y;	cin>>x>>y;
		if(x<left)	left=x;
		if(x>right)	right=x;
		if(y<down)	down=y;
		if(y>up)	up=y;
	}
	int len=max(up-down,right-left);
	cout<<(long long)len*len;
	return 0;
}
```









Let's denote as ![](http://espresso.codeforces.com/6500127ba81ee2a9ffdacf23b301df518aa9cd65.png) the
 number of bits set ('1' bits) in the binary representation of the non-negative integer *x*.


You are given multiple queries consisting of pairs of integers *l* and *r*.
 For each query, find the *x*, such that *l* ≤ *x* ≤ *r*,
 and ![](http://espresso.codeforces.com/6500127ba81ee2a9ffdacf23b301df518aa9cd65.png)is
 maximum possible. If there are multiple such numbers find the smallest of them.




Input


The first line contains integer *n* — the number of queries (1 ≤ *n* ≤ 10000).


Each of the following *n* lines contain two integers *l**i*, *r**i* —
 the arguments for the corresponding query (0 ≤ *l**i* ≤ *r**i* ≤ 1018).




Output


For each query print the answer in a separate line.




Sample test(s)




input
3
1 2
2 4
1 10




output
1
3
7






Note


The binary representations of numbers from 1 to 10 are listed below:

110 = 12

210 = 102

310 = 112

410 = 1002

510 = 1012

610 = 1102

710 = 1112

810 = 10002

910 = 10012

1010 = 10102













这题是给一个范围（L是左边界，R是有边界）问你在这个范围内哪个数载二进制下1的数量是最多的（有多个解请输出最小数）。

也就是，要二进制的1尽量多，还要求尽量小，那就从低位开始把0变成1呗

那么我们就从左边界开始，从低位向高位按位或（0变成1，1也还是1）1，直到比R大停止，输出前一个（即比R小的最后一个数）。

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

int main()
{
	int cases=0;
	scanf("%d",&cases);
	for(int _case=1;_case<=cases;_case++)
	{
		ll l,r,t,p=1;	cin>>l>>r;
		for(ll i=0;i<63;i++)
		{
			ll t=l|p;
			if(t>r)break;
			l=t,p<<=1;
			//cout<<t<<endl;
		}
		cout<<l<<endl;
	}
	return 0;
}
```









You are given a sequence *a* consisting of *n* integers.
 Find the maximum possible value of ![](http://espresso.codeforces.com/fab6bb390f875e9dbc8692275347ad1e68de8347.png) (integer
 remainder of *a**i* divided
 by*a**j*),
 where 1 ≤ *i*, *j* ≤ *n* and *a**i* ≥ *a**j*.




Input


The first line contains integer *n* — the length of the sequence (1 ≤ *n* ≤ 2·105).


The second line contains *n* space-separated integers *a**i* (1 ≤ *a**i* ≤ 106).




Output


Print the answer to the problem.




Sample test(s)




input
3
3 4 5




output
2












短小精悍却烦人至深的题，我原先想的太简单了，写了个





```cpp
int n=0;	cin>>n;
	for(int i=0;i<n;i++)
		scanf("%d",&num[i]);
	sort(num,num+n,cmp);
	int modmax=0;
	for(int i=0; num[i]>modmax; i++)
		for(int j=i+1;num[j]>modmax && j<n; j++)
			update( num[i] % num[j]);
	cout<<modmax;
	return 0;
```
这样的东西……TLE的飞起……

想了想就不能暴力啊，暴力的话剪枝也没用

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
int n,a[200048]={0},ans=0;
#define Max(a,b) ((a)>(b)?(a):(b))
#define Min(a,b) ((a)<(b)?(a):(b))
#define update(x) ans=(ans<(x)?x:ans);

int main()
{
	scanf("%d",&n);
	for(int i=0;i<n;++i) scanf("%d",&a[i]);
	sort(a,a+n);
	for(int i=0;i<n-1;++i)
	if(i==0||a[i]!=a[i-1])
	{
		int j=a[i]+a[i],p;
		while(j <= a[n-1])
		{
			p = lower_bound(a,a+n,j)-a;
			if(p > 0) update(a[p-1] % a[i]);
			j+=a[i];
		}
		update(a[n-1] % a[i]); 
	}
	printf("%d\n",ans);
	return 0;
}
```









How many specific orders do you know? Ascending order, descending order, order of ascending length, order of ascending polar angle... Let's have a look at another specific order: *d*-sorting.
 This sorting is applied to the strings of length at least *d*, where *d* is
 some positive integer. The characters of the string are sorted in following manner: first come all the 0-th characters of the initial string, then the 1-st ones, then the 2-nd ones and so on, in the end go all the (*d* - 1)-th
 characters of the initial string. By the *i*-th characters we mean all the character whose positions are exactly *i* modulo *d*.
 If two characters stand on the positions with the same remainder of integer division by*d*, their relative order after the sorting shouldn't
 be changed. The string is zero-indexed. For example, for string 'qwerty':


Its 1-sorting is the string 'qwerty' (all characters stand on 0 positions),


Its 2-sorting is the string 'qetwry' (characters 'q',
 'e' and 't' stand on 0 positions
 and characters 'w', 'r' and 'y'
 are on 1 positions),


Its 3-sorting is the string 'qrwtey' (characters 'q'
 and 'r' stand on 0 positions, characters 'w'
 and 't' stand on 1 positions and characters 'e'
 and 'y' stand on 2 positions),


Its 4-sorting is the string 'qtwyer',


Its 5-sorting is the string 'qywert'.


You are given string *S* of length *n* and *m*shuffling operations
 of this string. Each shuffling operation accepts two integer arguments *k*and *d* and
 transforms string *S* as follows. For each *i* from 0 to *n* - *k* in
 the increasing order we apply the operation of *d*-sorting to the substring *S*[*i*..*i* + *k* - 1].
 Here *S*[*a*..*b*] represents a substring that consists of characters on positions from *a* to *b* inclusive.


After each shuffling operation you need to print string *S*.




Input


The first line of the input contains a non-empty string *S* of length *n*,
 consisting of lowercase and uppercase English letters and digits from 0 to 9.


The second line of the input contains integer *m* – the number of shuffling operations
 (1 ≤ *m*·*n* ≤ 106).


Following *m* lines contain the descriptions of the operations consisting of two integers *k* and *d* (1 ≤ *d* ≤ *k* ≤ *n*).




Output


After each operation print the current state of string *S*.




Sample test(s)




input
qwerty
3
4 2
6 3
5 2




output
qertwy
qtewry
qetyrw






Note


Here is detailed explanation of the sample. The first modification is executed with arguments *k* = 4, *d* = 2.
 That means that you need to apply 2-sorting for each substring of length 4 one by one moving from the left to the right. The string will transform in the following manner:

qwerty → qewrty → qerwty → qertwy


Thus, string *S* equals 'qertwy'
 at the end of first query.


The second modification is executed with arguments *k* = 6, *d* = 3.
 As a result of this operation the whole string *S* is replaced by its 3-sorting:

qertwy → qtewry


The third modification is executed with arguments *k* = 5, *d* = 2.

qtewry → qertwy → qetyrw










给一串字符串，每次给两个数字k和d，即要求从左到右每k个数进行一次 d-sorting，这种sorting的意思是，每d个数字选一个，这么分好组之后排序，详见hint



DIV2全场只有一个人（[joker99](http://codeforces.com/profile/joker99)）出了E，看了下代码暂时囫囵吞了下，贴一下代码等日后学习下。



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

const int size = 2 * 1000 * 1000 + 10;
const int ssize = 21;

char buf[size];
char nbuf[size];

int n=0, m=0;
int pwr[ssize][size];

void combine(int* tg, int* a, int* b, int shift) 
{
    for (int i = 0; i < n; i++) 
	{
        if (a[i] < shift) tg[i] = a[i];
        else tg[i] = b[a[i] - shift] + shift;
    }
}

int main() 
{
    scanf("%s", buf); n = strlen(buf);
    scanf("%d", &m);
    for (int i = 0; i < m; i++) 
	{
        int k, d;
        scanf("%d%d", &k, &d);
        int num = n - k + 1, cur = 0;
        for (int j = 0; j < d; j++) 
		{
            int p = j;
            while (p < k) 
			{
                pwr[0][p] = cur++;
                p += d;
            }
        }
        for (int j = k; j < n; j++)	pwr[0][j] = j;
        int lim = 0, vl = 1;
        while (vl <= num) 
		{
            vl *= 2;
            lim++;
        }
        for (int j = 1; j < lim; j++) 
            combine(pwr[j], pwr[j - 1], pwr[j - 1], (1 << (j - 1)));
        for (int j = 0; j < n; j++) 
		{
            int ps = j;
            int vl = num;
            int sh = 0;
            for (int h = lim - 1; h >= 0; h--)
                if (vl >= (1 << h)) 
				{
                    if (ps >= sh)	ps = pwr[h][ps - sh] + sh;
                    vl -= (1 << h);
                    sh += (1 << h);
                }
            nbuf[ps] = buf[j]; 
        }
        for (int j = 0; j < n; j++)	buf[j] = nbuf[j];
        printf("%s\n", buf);
    }

    return 0;
}
```






































