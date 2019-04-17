# 第七次codeforces竞技结束 #258 Div 2 - CD's Coding - CSDN博客





2014年07月25日 12:23:45[糖果天王](https://me.csdn.net/okcd00)阅读数：910








这次出了两题，A和B，C的话我居然就差一个(n%3)就能ac了Q^Q 哭……

要是C加了那一行出了的话多帅气呀……






## A. Game With Sticks



time limit per test

1 second



memory limit per test

256 megabytes



input

standard input



output

standard output



After winning gold and silver in IOI 2014, Akshat and Malvika want to have some fun. Now they are playing a game on a grid made of *n* horizontal and *m* vertical
 sticks.


An intersection point is any point on the grid which is formed by the intersection of one horizontal stick and one vertical stick.


In the grid shown below, *n* = 3 and *m* = 3. There are *n* + *m* = 6 sticks
 in total (horizontal sticks are shown in red and vertical sticks are shown in green). There are *n*·*m* = 9 intersection points, numbered from 1 to 9.
![](http://espresso.codeforces.com/91558f9cc7c96dff04bc29434dac2798ddbcf518.png)

The rules of the game are very simple. The players move in turns. Akshat won gold, so he makes the first move. During his/her move, a player must choose any remaining intersection point and remove from the grid all sticks which pass through this point. A player
 will lose the game if he/she cannot make a move (i.e. there are no intersection points remaining on the grid at his/her move).


Assume that both players play optimally. Who will win the game?




Input


The first line of input contains two space-separated integers, *n* and *m* (1 ≤ *n*, *m* ≤ 100).




Output


Print a single line containing "Akshat" or "Malvika" (without
 the quotes), depending on the winner of the game.




Sample test(s)




input
2 2




output
Malvika




input
2 3




output
Malvika




input
3 3




output
Akshat






Note

Explanation of the first sample:


The grid has four intersection points, numbered from 1 to 4.
![](http://espresso.codeforces.com/91877e206bb6f5266de63fa72a87c2de47f00e83.png)

If Akshat chooses intersection point 1, then he will remove two sticks (1 - 2 and 1 - 3).
 The resulting grid will look like this.
![](http://espresso.codeforces.com/fb4a3537dc75368670f749402f956a788cf0d39a.png)

Now there is only one remaining intersection point (i.e. 4). Malvika must choose it and remove both remaining sticks. After her move the grid will be empty.


In the empty grid, Akshat cannot make any move, hence he will lose.


Since all 4 intersection points of the grid are equivalent, Akshat will lose no matter which one he picks.








这道题呢意思是有这么多棒子摆成网格状，一人一次选一个交叉点拿走那俩棒子，没有交叉点可以拿的输。

因为一个交叉点由一纵一横组成，每次消除一纵一横，所以自然是纵横中某一个拿完了就结束了：

### Code：



```cpp
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

#define Max(a,b) ((a)>(b)?(a):(b))
#define Min(a,b) ((a)<(b)?(a):(b))

int cmp(const void *a, const void *b)
{
	return(*(int *)a-*(int *)b);
}

int main()
{
	int n,m;
	cin>>n>>m;
	cout<< (min(n,m)%2==0?"Malvika":"Akshat");
	return 0;
}
```








## B. Sort the Array



time limit per test

1 second



memory limit per test

256 megabytes



input

standard input



output

standard output



Being a programmer, you like arrays a lot. For your birthday, your friends have given you an array *a*consisting of *n*distinct integers.


Unfortunately, the size of *a* is too small. You want a bigger array! Your friends agree to give you a bigger array, but only if you are able to answer the
 following question correctly: is it possible to sort the array *a* (in increasing order) by reversing exactly
 one segment of *a*? See definitions of segment and reversing in the notes.




Input


The first line of the input contains an integer *n* (1 ≤ *n* ≤ 105)
 — the size of array *a*.


The second line contains *n* distinct space-separated integers: *a*[1], *a*[2], ..., *a*[*n*] (1 ≤ *a*[*i*] ≤ 109).




Output


Print "yes" or "no" (without quotes), depending on the answer.


If your answer is "yes", then also print two space-separated integers denoting start and end (start must not be greater than end) indices of the segment to be
 reversed. If there are multiple ways of selecting these indices, print any of them.




Sample test(s)




input
3
3 2 1




output
yes
1 3




input
4
2 1 3 4




output
yes
1 2




input
4
3 1 2 4




output
no




input
2
1 2




output
yes
1 1






Note


Sample 1. You can reverse the entire array to get [1, 2, 3], which is sorted.


Sample 3. No segment can be reversed such that the array will be sorted.

Definitions


A segment [*l*, *r*] of array *a* is the sequence *a*[*l*], *a*[*l* + 1], ..., *a*[*r*].


If you have an array *a* of size *n* and you reverse
 its segment [*l*, *r*], the array will become:

*a*[1], *a*[2], ..., *a*[*l* - 2], *a*[*l* - 1], *a*[*r*], *a*[*r* - 1], ..., *a*[*l* + 1], *a*[*l*], *a*[*r* + 1], *a*[*r* + 2], ..., *a*[*n* - 1], *a*[*n*].








B题的话呢就是说有一个数组，我们只能选择其中一个连续的数组序列反序，问是否能一次操作后使整个数组升序。

用dp做标记找一段持续降序的序列反向，然后检测反向后的时不时严格升序即可：

### Code：



```cpp
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

#define Max(a,b) ((a)>(b)?(a):(b))
#define Min(a,b) ((a)<(b)?(a):(b))

int a[100001];
int dp[100001];

int cmp(const void *a, const void *b)
{
	return(*(int *)a-*(int *)b);
}

void arr_rev(int s,int e)
{
	int temp[100001];
	for(int i=s;i<=e;i++)
	{
		temp[i]=a[e+s-i];
	}
	for(int i=s;i<=e;i++)
	{
		a[i]=temp[i];
	}
}

int main()
{
	int n;	cin>>n;
	memset(a,0,sizeof a);
	memset(dp,0,sizeof dp);
	cin>>a[0];
	bool flag=true;
	int pos_start=0;
	for(int ni=1;ni<n;ni++)
	{
		cin>>a[ni];
		if(a[ni]>a[ni-1]) dp[ni]=dp[ni-1]+1;
		else 
		{
			dp[ni]=0;
			flag=false;
			if(pos_start==0)pos_start=ni;
		}
	}
	if(flag)
	{
		cout<<"yes"<<endl<<"1 1";
		return 0;
	}
	//for(int i=0;i<n;i++)cout<<dp[i]<<" ";
	int start=0,end=0;
	if(dp[1]==0)
	{
		end=1;
		int sup[100001];
		memset(sup,0,sizeof sup);
		while(dp[end]==0)
		{
			end++;
			if(end==n)break;
		}
		arr_rev(0,end-1);
	}
	else
	{
		start=pos_start-1;
		end=pos_start;
		while(dp[end]==0)
		{
			end++;
			if(end==n)break;
		}
		arr_rev(start,end-1);
	} 
	bool aflag=true;
	for(int i=1;i<n;i++)
	{
		//cout<<a[i]<<endl;
		if(a[i]<a[i-1])aflag=false;
	}
	puts(aflag?"yes":"no");
	if(aflag)cout<<start+1<<" "<<end<<endl;
	return 0;
}
```








## C. Predict Outcome of the Game



time limit per test

2 seconds



memory limit per test

256 megabytes



input

standard input



output

standard output



There are *n* games in a football tournament. Three teams are participating in it. Currently *k* games
 had already been played.


You are an avid football fan, but recently you missed the whole *k* games. Fortunately, you remember a guess of your friend for these *k* games.
 Your friend did not tell exact number of wins of each team, instead he thought that absolute difference between number of wins of first and second team will be *d*1and
 that of between second and third team will be *d*2.


You don't want any of team win the tournament, that is each team should have the same number of wins after *n* games. That's why you want to know: does there
 exist a valid tournament satisfying the friend's guess such that no team will win this tournament?


Note that outcome of a match can not be a draw, it has to be either win or loss.




Input


The first line of the input contains a single integer corresponding to number of test cases *t*(1 ≤ *t* ≤ 105).


Each of the next *t* lines will contain four space-separated integers *n*, *k*, *d*1, *d*2(1 ≤ *n* ≤ 1012; 0 ≤ *k* ≤ *n*; 0 ≤ *d*1, *d*2 ≤ *k*) —
 data for the current test case.




Output


For each test case, output a single line containing either "yes" if it is possible to have no winner of tournament, or "no"
 otherwise (without quotes).




Sample test(s)




input
5
3 0 0 0
3 3 0 0
6 4 1 0
6 3 3 0
3 3 3 2




output
yes
yes
yes
no
no






Note


Sample 1. There has not been any match up to now (*k* = 0, *d*1 = 0, *d*2 = 0).
 If there will be three matches (1-2, 2-3, 3-1) and each team wins once, then at the end each team will have 1 win.


Sample 2. You missed all the games (*k* = 3). As *d*1 = 0 and *d*2 = 0,
 and there is a way to play three games with no winner of tournament (described in the previous sample), the answer is "yes".


Sample 3. You missed 4 matches, and *d*1 = 1, *d*2 = 0.
 These four matches can be: 1-2 (win 2), 1-3 (win 3), 1-2 (win 1), 1-3 (win 1). Currently the first team has 2 wins, the second team has 1 win, the third team has 1 win. Two remaining matches can be: 1-2 (win 2), 1-3 (win 3). In the end all the teams have equal
 number of wins (2 wins).








这道题的意思是有3个队伍，总共要打n场比赛，已经打了k场，这k场之后一队和二队的得分差的绝对值为d1，二队和三队得分差绝对值为d2，问有没有可能n场都打完了之后三个队伍平手。

经枚举题意，这道题呢有这么几个隐含条件：

1、n可以不是3的倍数 Test 4 n=999999980 ，这就是我wa的原因……我没有写(n%3==0)cout<<"no"<<endl;

2、Note中说明了不是循环打，可以就两个队打到天荒地老……

1-2 (win 2), 1-3 (win 3), 1-2 (win 1), 1-3 (win 1)，1-2
 (win 2), 1-3 (win 3). 


3、存在 【k场之后不可能存在d1、d2的此种数据】 的可能性： Test 5 n=1,k=1,d1=0,d2=0

这道题如下判定即可：

1、n是否能被3整除（场数都不能被3整除的话最后三个人怎么可能win数相同）

2、d1=d2=0 的时候直接看n-k是否能被3整除

3、绝对值d1,d2 上符号，四种情况：++,+-,-+,--

依次判断1) 此种情况下最大的那个队伍当前胜场有没有超过n/3

2) 三个队按照当前情况下已打场数和是否超过k

3) 三个队按照最大队补齐之后k场剩余场是否为非负




### Code：



```cpp
#include <cmath> 
#include <memory> 
#include <cstdio>
#include <string>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <algorithm>
using namespace std;
typedef __int64 ll;
#define Max(a,b) ((a)>(b)?(a):(b))
#define Min(a,b) ((a)<(b)?(a):(b))

int cmp(const void *a, const void *b)
{
	return(*(int *)a-*(int *)b);
}

int main()
{
	int cases=0;
	scanf("%d",&cases);
	for(int _case=1;_case<=cases;_case++)
	{
		ll n,k,d1,d2,rest=0;
		cin>>n>>k>>d1>>d2;
		rest=n-k;
		if(n%3!=0)
		{
			cout<<"no"<<endl;
			continue;
		} 
		if(rest==0 && d1==0 && d2==0)
		{
			cout<<"yes"<<endl;
			continue;
		}
		if(d1==0 && d2==0)
		{
			cout<<(rest%3==0?"yes":"no")<<endl;
			continue;
		}
		if(rest<0)
		{
			cout<<"no"<<endl;
			continue;
		}
		
		ll rest1=rest-d1-d1-d2;		// >>
		if((d1+d2)>(n/3)) rest1=-1;
		if(d1+d2+d2>k) rest1=-1;
		ll rest2=rest-max(d1,d2)-(max(d1,d2)-min(d1,d2));// >< 
		if(max(d1,d2)>(n/3)) rest2=-1;
		if(d1+d2>k)rest2=-1;
		ll rest3=rest-d1-d2;		// <>
		if(max(d1,d2)>(n/3)) rest3=-1;
		if(max(d1,d2)+max(d1,d2)-min(d1,d2)>k) rest3=-1;
		ll rest4=rest-d1-d2-d2;		// <<
		if((d1+d2)>(n/3)) rest4=-1;
		if(d1+d1+d2>k) rest4=-1;
		
		if(rest1>=0 && rest1%3==0)cout<<"yes"<<endl;
		else if(rest2>=0 && rest2%3==0)cout<<"yes"<<endl;
		else if(rest3>=0 && rest3%3==0)cout<<"yes"<<endl;
		else if(rest4>=0 && rest4%3==0)cout<<"yes"<<endl;
		else cout<<"no"<<endl;
	}
	return 0;
}
```



##                                                                         D. Count Good Substrings




We call a stringgood, if after merging all the consecutive equal characters, the resulting string is
 palindrome. For example, "aabba" is good, because after the merging step it will become "aba".

Given a string, you have to find two values:
- 
the number of good substrings of even length;
- 
the number of good substrings of odd length.




Input

The first line of the input contains a single string of length*n*(1 ≤ *n* ≤ 105).
 Each character of the string will be either 'a' or 'b'.




Output

Print two space-separated integers: the number of good substrings of even length and the number of good substrings of odd length.




Sample test(s)




input
bb




output
1 2




input
baab




output
2 4




input
babb




output
2 5




input
babaa




output
2 7






Note

In example 1, there are three good substrings ("b", "b",
 and "bb"). One of them has even length and two of them have odd length.

In example 2, there are six good substrings (i.e. "b", "a",
 "a", "b", "aa",
 "baab"). Two of them have even length and four of them have odd length.

In example 3, there are seven good substrings (i.e. "b", "a",
 "b", "b", "bb",
 "bab", "babb"). Two of them have even length and five of them
 have odd length.

Definitions

A substring*s*[*l*, *r*](1 ≤ *l* ≤ *r* ≤ *n*)of
 string*s* = *s*1*s*2...*s**n*is
 string*s**l**s**l* + 1...*s**r*.

A string*s* = *s*1*s*2...*s**n*is
 a palindrome if it is equal to string*s**n**s**n* - 1...*s*1.











这道题呢是说一个【仅由a、b组成】的字符串，其中任意一个子串压缩（压缩的意思是所有连续的a或连续的b都由一个a或b来代替）后为回文的话成为好子串，问偶数长度的偶子串有多少个，奇数长度的偶子串有多少个。

技巧：

1、压缩后获得的一定是a、b相间的字符串，如a\ab\aba\ababababababa\bababa

2、压缩后的长度若为奇数则为回文

3、原字符串的偶数位到偶数位、奇数位到奇数位的子串长度为奇数，奇数位到偶数位、偶数位到奇数位的子串长度为偶数

4、所以这是个数学题

### Code：



```
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
const int maxn=1e5+5;

string s;
char str[maxn];
int len,f[2][2];
long long ans[2];

int main()
{
    while(scanf("%s",str)==1)
	{
        len=strlen(str);
        memset(f,0,sizeof f);
        memset(ans,0,sizeof ans);
        for(int i=0;i<len;i++)
		{
            f[str[i]-'a'][i%2]++;	 
            ans[0]+=f[str[i]-'a'][1-i%2];
            ans[1]+=f[str[i]-'a'][i%2];
        }
        cout<<ans[0]<<' '<<ans[1]<<endl;
    }
    return 0;
}
```

























﻿﻿



