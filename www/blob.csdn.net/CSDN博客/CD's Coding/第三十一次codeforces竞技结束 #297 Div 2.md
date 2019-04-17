# 第三十一次codeforces竞技结束 #297 Div 2 - CD's Coding - CSDN博客





2015年03月29日 16:13:37[糖果天王](https://me.csdn.net/okcd00)阅读数：613









|[A](http://codeforces.com/contest/525/problem/A)|[Vitaliy and Pie](http://codeforces.com/contest/525/problem/A)standard input/output2 s, 256 MB|![Submit](http://st.codeforces.com/images/icons/submit-22x22.png)![Add to favourites](http://st.codeforces.com/images/icons/star_gray_16.png)|![](http://st.codeforces.com/images/icons/user.png) x3268|
|----|----|----|----|
|[B](http://codeforces.com/contest/525/problem/B)|[Pasha and String](http://codeforces.com/contest/525/problem/B)standard input/output2 s, 256 MB|![Submit](http://st.codeforces.com/images/icons/submit-22x22.png)![Add to favourites](http://st.codeforces.com/images/icons/star_gray_16.png)|![](http://st.codeforces.com/images/icons/user.png) x2385|
|[C](http://codeforces.com/contest/525/problem/C)|[Ilya and Sticks](http://codeforces.com/contest/525/problem/C)standard input/output2 s, 256 MB|![Submit](http://st.codeforces.com/images/icons/submit-22x22.png)![Add to favourites](http://st.codeforces.com/images/icons/star_gray_16.png)|![](http://st.codeforces.com/images/icons/user.png) x1884|
|[D](http://codeforces.com/contest/525/problem/D)|[Arthur and Walls](http://codeforces.com/contest/525/problem/D)standard input/output2 s, 512 MB|![Submit](http://st.codeforces.com/images/icons/submit-22x22.png)![Add to favourites](http://st.codeforces.com/images/icons/star_gray_16.png)|![](http://st.codeforces.com/images/icons/user.png) x320|
|[E](http://codeforces.com/contest/525/problem/E)|[Anya and Cubes](http://codeforces.com/contest/525/problem/E)standard input/output2 s, 256 MB|![Submit](http://st.codeforces.com/images/icons/submit-22x22.png)![Add to favourites](http://st.codeforces.com/images/icons/star_gray_16.png)|![](http://st.codeforces.com/images/icons/user.png) x411|


在这么下去不行啊…… 数据结构依赖症需要改正了，明明数组简单粗暴而且不容易错，我非要用优先队列到底是为了什么……（C再次被Fst，哭）













After a hard day Vitaly got very hungry and he wants to eat his favorite potato pie. But it's not that simple. Vitaly is in the first room of the house with *n* room
 located in a line and numbered starting from one from left to right. You can go from the first room to the second room, from the second room to the third room and so on — you can go from the (*n* - 1)-th
 room to the *n*-th room. Thus, you can go to room *x* only
 from room *x* - 1.


The potato pie is located in the *n*-th room and Vitaly needs to go there.


Each pair of consecutive rooms has a door between them. In order to go to room *x* from room *x* - 1,
 you need to open the door between the rooms with the corresponding key.


In total the house has several types of doors (represented by uppercase Latin letters) and several types of keys (represented by lowercase Latin letters). The key of type *t* can
 open the door of type *T* if and only if *t* and *T* are
 the same letter, written in different cases. For example, key f can open door F.


Each of the first *n* - 1 rooms contains exactly one key of some type that Vitaly can use to get to next rooms. Once the door is open with some key, Vitaly
 won't get the key from the keyhole but he will immediately run into the next room. In other words, each key can open no more than one door.


Vitaly realizes that he may end up in some room without the key that opens the door to the next room. Before the start his run for the potato pie Vitaly can buy any number of keys of any type that is guaranteed to get to room *n*.


Given the plan of the house, Vitaly wants to know what is the minimum number of keys he needs to buy to surely get to the room *n*, which has a delicious potato
 pie. Write a program that will help Vitaly find out this number.




Input


The first line of the input contains a positive integer *n* (2 ≤ *n* ≤ 105) — the
 number of rooms in the house.


The second line of the input contains string *s* of length 2·*n* - 2.
 Let's number the elements of the string from left to right, starting from one.


The odd positions in the given string *s* contain lowercase Latin letters — the types of the keys that lie in the corresponding rooms. Thus, each odd position *i* of
 the given string *s* contains a lowercase Latin letter — the type of the key that lies in room number (*i* + 1) / 2.


The even positions in the given string contain uppercase Latin letters — the types of doors between the rooms. Thus, each even position *i* of the given string *s* contains
 an uppercase letter — the type of the door that leads from room *i* / 2 to room *i* / 2 + 1.




Output


Print the only integer — the minimum number of keys that Vitaly needs to buy to surely get from room one to room *n*.




Sample test(s)




input
3
aAbB




output
0




input
4
aBaCaB




output
3




input
5
xYyXzZaZ




output
2












有n个房间，每个房间放着一个钥匙，到下一个房间都会有一个门，每个门和钥匙的对应方式是同一个字母的大小写。问初始的时候需要多少钥匙才能到达终点。



这个就想象成游戏的时候的钥匙寄存数组就好，发现钥匙则 record[钥匙]++,遇到门的时候如果有钥匙就record[钥匙]--，没有钥匙就说明初始的时候需要带着这个钥匙，则ans++，遍历完了之后输出ans即可。

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

#define Max(a,b) ((a)>(b)?(a):(b))
#define Min(a,b) ((a)<(b)?(a):(b))

bool cmp(const int a, const int b)
{
	return a > b;
}

map<char,int> mci;
int main()
{
	mci.clear();
	int n,ans=0; cin>>n;
	string s;	cin>>s;
	for(int i=0;i<s.length();i++)
	{
		if(islower(s[i])) mci[s[i]]++;
		if(isupper(s[i])) 
		{
			if(mci[tolower(s[i])]==0) ans++;
			else mci[tolower(s[i])]--;
		}
		//cout<<i<<":"<<ans<<endl;
	}
	cout<<ans;
	return 0;
}
```









Pasha got a very beautiful string *s* for his birthday, the string consists of lowercase Latin letters. The letters in the string are numbered from 1 to |*s*| from
 left to right, where |*s*| is the length of the given string.


Pasha didn't like his present very much so he decided to change it. After his birthday Pasha spent *m* days performing the following transformations on his
 string — each day he chose integer *a**i* and reversed a
 piece of string (a segment) from position *a**i* to
 position |*s*| - *a**i* + 1.
 It is guaranteed that 2·*a**i* ≤ |*s*|.


You face the following task: determine what Pasha's string will look like after *m* days.




Input


The first line of the input contains Pasha's string *s* of length from 2 to 2·105 characters,
 consisting of lowercase Latin letters.


The second line contains a single integer *m* (1 ≤ *m* ≤ 105) — 
 the number of days when Pasha changed his string.


The third line contains *m* space-separated elements *a**i* (1 ≤ *a**i*; 2·*a**i* ≤ |*s*|) — the
 position from which Pasha started transforming the string on the *i*-th day.




Output


In the first line of the output print what Pasha's string *s* will look like after *m* days.




Sample test(s)




input
abcdef
1
2




output
aedcbf




input
vwxyz
2
2 2




output
vwxyz




input
abcdef
3
1 2 3




output
fbdcea















有一个字符串，在m天的时间内，这位同学每天都会选择一个位置a，然后把从左边数第a个和右边数第a个字符**以及他们中间**的字符组成的字符串反转顺序，问这m天过去之后字符串是什么样子的。

我们要知道，每个字符只有2种可能，原字符，或者和他对称的右边那个字符，因为无论怎么颠倒，每个字符也不会在这两种位置以外的地方。那么我们用一个数组来标记这个字符是否在原位，那有人问了，难道用线段树维护，区间加一减一么？不好意思，本人的线段树也不熟呢，话说你会的话求教我一下呢哈哈哈~这里的话我们记忆的仅仅只是每个位置被a挑中的奇偶性，因为奇数次相当于颠倒1次，偶数次等于没做事嘛。For example，如果现在第i位的字符，被选中偶数次，那么对于它来说，他和他前面的那个字符是一样的情况，前面那个字符i-1位如果是交换状态，那么这个第i位也应该是处于要交换的状态，反之，我相当于颠倒了一次，那么是否交换应该和前一个的情况相反。

这个想清楚了的话那就简单的多啦~

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

int main()
{
	string s;
	int m=0,now=0,a[100086]={0},b[200086]={0};
	cin>>s>>m;
	int len=s.length();
	for(int i=0;i<m;i++)
	{
		scanf("%d",&now);
		a[now-1]=1-a[now-1];
	}
	int rev=0;
	for(int i=0;i<len/2;i++)
	{
		if(a[i]!=0) rev=1-rev;
		b[i]=b[len-i-1]=rev;
	}
	for(int i=0;i<len;i++)
	{
		if(b[i]==1) printf("%c",s[len-i-1]);
		else printf("%c",s[i]);
	}
	return 0;
}
```









In the evening, after the contest Ilya was bored, and he really felt like maximizing. He remembered that he had a set of *n* sticks and an instrument. Each
 stick is characterized by its length *l**i*.


Ilya decided to make a rectangle from the sticks. And due to his whim, he decided to make rectangles in such a way that maximizes their total area. Each stick is used in making at most one rectangle, it is possible that some of sticks remain unused. Bending
 sticks is not allowed.


Sticks with lengths *a*1, *a*2, *a*3 and *a*4 can
 make a rectangle if the following properties are observed:
- *a*1 ≤ *a*2 ≤ *a*3 ≤ *a*4
- *a*1 = *a*2
- *a*3 = *a*4


A rectangle can be made of sticks with lengths of, for example, 3 3 3 3 or 2 2 4 4.
 A rectangle cannot be made of, for example, sticks 5 5 5 7.


Ilya also has an instrument which can reduce the length of the sticks. The sticks are made of a special material, so the length of each stick can be reduced by at most one. For example, a stick with length 5 can
 either stay at this length or be transformed into a stick of length 4.


You have to answer the question — what maximum total area of the rectangles can Ilya get with a file if makes rectangles from the available sticks?




Input


The first line of the input contains a positive integer *n* (1 ≤ *n* ≤ 105) — the
 number of the available sticks.


The second line of the input contains *n* positive integers *l**i* (2 ≤ *l**i* ≤ 106) — the
 lengths of the sticks.




Output


The first line of the output must contain a single non-negative integer — the maximum total area of the rectangles that Ilya can make from the available sticks.




Sample test(s)




input
4
2 4 4 2




output
8




input
4
2 2 3 5




output
0




input
4
100003 100004 100005 100006




output
10000800015












有一堆棍子，对于每个棍子，我们有“使用原长度”和“使用原长度减一”两种选择，问组成的矩形总面积的最大值。



我们从平方差公式知道，距离越小的长宽之积越大，一边长一定的时候，面积与另一边长成正比增加。

那么就简单了，排序之后从大到小找可以做对边的两根棍子做长边，再找一对做短边，乘积加进ans里，然后再找长边……如此循环重复，O(n)结束战斗

### Code：



```cpp
#include <cmath> 
#include <queue>
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

int n=0, ct=0, a[100086]={0}, b[100086]={0};

int main() 
{
    cin>>n;
    for(int i=1;i<=n;++i) scanf("%d",&a[i]);
    sort(a+1,a+n+1);
    for(int i=n; i>=0; i-=2) 
	{
        if(a[i] == a[i-1]) b[ct++]=a[i];
        else 
		{
            if(a[i]-a[i-1] == 1) b[ct++]=a[i-1];
            else ++i;
        }
    }
    ll ans=0;
    for(int i=0;i<=ct-1;i+=2) ans+=(ll)b[i]*b[i+1];
    cout<<ans;
    return 0;
}
```









Finally it is a day when Arthur has enough money for buying an apartment. He found a great option close to the center of the city with a nice price.


Plan of the apartment found by Arthur looks like a rectangle *n* × *m* consisting of squares of size 1 × 1.
 Each of those squares contains either a wall (such square is denoted by a symbol "*" on the plan) or a free space (such square is denoted on the plan by a symbol
 ".").


Room in an apartment is a maximal connected area consisting of free squares. Squares are considered adjacent if they share a common side.


The old Arthur dream is to live in an apartment where all rooms are rectangles. He asks you to calculate minimum number of walls you need to remove in order to achieve this goal. After removing a wall from a square it becomes a free square. While removing the
 walls it is possible that some rooms unite into a single one.




Input


The first line of the input contains two integers *n*, *m* (1 ≤ *n*, *m* ≤ 2000)
 denoting the size of the Arthur apartments.


Following *n* lines each contain *m* symbols — the plan
 of the apartment.


If the cell is denoted by a symbol "*" then it contains a wall.


If the cell is denoted by a symbol "." then it this cell is free from walls and also this cell is contained in some of the rooms.




Output


Output *n* rows each consisting of *m* symbols that show
 how the Arthur apartment plan should look like after deleting the minimum number of walls in order to make each room (maximum connected area free from walls) be a rectangle.


If there are several possible answers, output any of them.




Sample test(s)




input
5 5
.*.*.
*****
.*.*.
*****
.*.*.




output
.*.*.
*****
.*.*.
*****
.*.*.




input
6 7
***.*.*
..*.*.*
*.*.*.*
*.*.*.*
..*...*
*******




output
***...*
..*...*
..*...*
..*...*
..*...*
*******




input
4 5
.....
.....
..***
..*..




output
.....
.....
.....
.....












有一张图，星号是墙，点号是空地，要求推倒最少数量的墙，令所有的房间都是矩形。



简单的说，如果发现L形，那么那个阻碍成为矩形的墙就需要被消灭掉,然后接着看会造成的其他的影响一个个磨消掉，直到达成最终要求即可。

### Code：



```cpp
#include <bits/stdc++.h>
//Code By cikofte@Codeforces

#include <cmath> 
#include <queue>
#include <cctype>
#include <cstdio>
#include <string>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <algorithm>
using namespace std;
typedef long long ll;
#define	st first
#define	nd second
#define	mp make_pair
#define	pb push_back
#define Max(a,b) ((a)>(b)?(a):(b))
#define Min(a,b) ((a)<(b)?(a):(b))

#define	ctrl( xx,yy )	( xx>=1 and yy>=1 and xx<=m and yy<=n and s[xx][yy]=='.' )

using namespace std;

int	m,n;
char	s[3000][3000];
char	h[3000][3000];

int	bozuk( int x,int y )	//Bad mood .Turkey
{
	if( s[x][y]!='*' )	return	0;
	if( ctrl( x-1,y ) and ctrl( x-1,y+1 ) and ctrl( x,y+1 ) )	return	1;
	if( ctrl( x,y+1 ) and ctrl( x+1,y+1 ) and ctrl( x+1,y ) )	return	1;
	if( ctrl( x+1,y ) and ctrl( x+1,y-1 ) and ctrl( x,y-1 ) )	return	1;
	if( ctrl( x,y-1 ) and ctrl( x-1,y-1 ) and ctrl( x-1,y ) )	return	1;
	return	0;
}

int main()
{
	cin>>m>>n;
	for(ll i=1;i<=m;i++)	scanf("%s",s[i]+1);
	queue< pair<int,int> >	Q;
	for(ll i=1;i<=m;i++)
		for(ll j=1;j<=n;j++)
			if( bozuk( i,j ) )	Q.push( mp(i,j) ), h[i][j]=1;
	int	x,y,xx,yy;
	while( Q.size() )
	{
		x = Q.front().st;
		y = Q.front().nd;
		Q.pop();
		s[x][y] = '.';
		h[x][y] = 0;
		for(ll i=-1;i<=1;i++)
			for(ll j=-1;j<=1;j++)
			{
				if( !i and !j )	continue;
				xx = x+i;
				yy = y+j;
				if( xx<1 or xx>m or yy<1 or yy>n 
					or h[xx][yy] or !bozuk( xx,yy ) )	continue;
				Q.push( mp(xx,yy) ), h[xx][yy] = 1;
				s[xx][yy] = '.';
			}
	}
	for(ll i=1;i<=m;i++) printf("%s\n",s[i]+1);
	return 0;
}
```











