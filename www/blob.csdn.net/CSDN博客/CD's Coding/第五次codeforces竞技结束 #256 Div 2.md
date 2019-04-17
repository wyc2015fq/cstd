# 第五次codeforces竞技结束 #256 Div 2 - CD's Coding - CSDN博客





2014年07月19日 17:10:13[糖果天王](https://me.csdn.net/okcd00)阅读数：849








这次……惨败……只出了2题……而第二题居然在final 的时候判出wa了……

话说这简直坑啊Q^Q 我的--写成了++居然都过了pretest……










Bizon the Champion is called the Champion for a reason.


Bizon the Champion has recently got a present — a new glass cupboard with *n* shelves and he decided to put all his presents there. All the presents can be
 divided into two types: medals and cups. Bizon the Champion has *a*1 first
 prize cups, *a*2 second
 prize cups and *a*3 third
 prize cups. Besides, he has *b*1 first
 prize medals, *b*2 second
 prize medals and *b*3 third
 prize medals.


Naturally, the rewards in the cupboard must look good, that's why Bizon the Champion decided to follow the rules:
- 
any shelf cannot contain both cups and medals at the same time;
- 
no shelf can contain more than five cups;
- 
no shelf can have more than ten medals.


Help Bizon the Champion find out if we can put all the rewards so that all the conditions are fulfilled.




Input


The first line contains integers *a*1, *a*2 and *a*3(0 ≤ *a*1, *a*2, *a*3 ≤ 100).
 The second line contains integers *b*1, *b*2 and *b*3(0 ≤ *b*1, *b*2, *b*3 ≤ 100).
 The third line contains integer *n*(1 ≤ *n* ≤ 100).


The numbers in the lines are separated by single spaces.




Output


Print "YES" (without the quotes) if all the rewards can be put on the shelves in the described manner. Otherwise, print "NO"
 (without the quotes).




Sample test(s)




input
1 1 1
1 1 1
4




output
YES




input
1 1 3
2 3 4
2




output
YES




input
1 0 0
1 0 0
1




output
NO












```cpp
#include <cmath>   
#include <vector>  
#include <cctype>  
#include <cstdio>  
#include <string>  
#include <cstdlib>  
#include <cstring>  
#include <iostream>  
#include <algorithm>  
using namespace std;

int main()
{
	int a1,a2,a3;
	int b1,b2,b3;
	int n;
	cin>>a1>>a2>>a3>>b1>>b2>>b3>>n;
	int c=a1+a2+a3;
	int m=b1+b2+b3;
	while(c+m)
	{
		if(c)
		{
			if(c>=5) c-=5,n--;
			else c=0,n--;
			
		}
		else
		{
			if(m)
			{
				if(m>=10) m-=10,n--;
				else m=0,n--;
			}
		}
		
		if(n<0) 
		{
			puts("NO");
			return 0;
		}
	}
	puts("YES");
	return 0;
}
```



A的话没什么好说的，贪心最简单背包即可……













Bizon the Champion isn't just a bison. He also is a favorite of the "Bizons" team.


At a competition the "Bizons" got the following problem: "You are given two distinct words (strings of English letters), *s* and *t*.
 You need to transform word *s* into word *t*". The task
 looked simple to the guys because they know the suffix data structures well. Bizon Senior loves suffix automaton. By applying it once to a string, he can remove from this string any single character. Bizon Middle knows suffix array well. By applying it once
 to a string, he can swap any two characters of this string. The guys do not know anything about the suffix tree, but it can help them do much more.


Bizon the Champion wonders whether the "Bizons" can solve the problem. Perhaps, the solution do not require both data structures. Find out whether the guys can solve the problem and if they can, how do they do it? Can they solve it either only with use of suffix
 automaton or only with use of suffix array or they need both structures? Note that any structure may be used an unlimited number of times, the structures may be used in any order.




Input


The first line contains a non-empty word *s*. The second line contains a non-empty word *t*.
 Words *s* and *t* are different. Each word consists
 only of lowercase English letters. Each word contains at most 100 letters.




Output


In the single line print the answer to the problem. Print "need tree" (without the quotes) if word *s* cannot
 be transformed into word *t*even with use of both suffix array and suffix automaton. Print "automaton"
 (without the quotes) if you need only the suffix automaton to solve the problem. Print "array" (without the quotes) if you need only the suffix array to solve
 the problem. Print "both" (without the quotes), if you need both data structures to solve the problem.


It's guaranteed that if you can solve the problem only with use of suffix array, then it is impossible to solve it only with use of suffix automaton. This is also true for suffix automaton.




Sample test(s)




input
automaton
tomat




output
automaton




input
array
arary




output
array




input
both
hot




output
both




input
need
tree




output
need tree






Note


In the third sample you can act like that: first transform "both" into "oth"
 by removing the first character using the suffix automaton and then make two swaps of the string using the suffix array and get "hot".










B题可以算是模拟，只要理解清楚它想让我们做什么就行：



可以通过删除字母来完成转换【automaton】

可以通过改变字母位置来完成转换【array】

两个都得用【both】

都用都转不了【need tree】

所以—— 字符串长度一样且使用了一样数量的每种字母的话：array ，否则nt

字符串长度不同>A包含B子序列：automaton

不包含时若A包含B所有字母及相应数量：both，反之nt

++ 和 -- 的纰漏……叹气…… 

Code：



```cpp
#include <cmath>   
#include <vector>  
#include <cctype>  
#include <cstdio>  
#include <string>  
#include <cstdlib>  
#include <cstring>  
#include <iostream>  
#include <algorithm>  
using namespace std;
int mark[26];

int main()
{	
	memset(mark,0,sizeof mark);
	string s,t;
	cin>>s>>t;
	if(s.length()==t.length())
	{
		for(int i=0;i<s.length();i++)
		{
			mark[s[i]-'a']++;
			mark[t[i]-'a']--;
		}
		
		int flag=0;
		for(int i=0;i<26;i++)
		{
			if(mark[i]!=0) flag=1;
		}
		
		if(flag==0)
		{
			puts("array");
			return 0;
		}
		else
		{
			puts("need tree");
			return 0;
		}
	}
	else if(s.length()<t.length())
	{
		puts("need tree");
		return 0;
	}
	else
	{
		int pos=0,flag=0;
		for(int i=0;i<t.length();i++)
		{
			while(s[pos]!=t[i])
			{
				pos++;
				if(pos>=s.length()) 
				{
					flag=1;
					break;
				}
			}
			pos++;
			if(flag==1)break;
		}
		if(flag==0)
		{
			puts("automaton");
			return 0;
		}
		else
		{
			for(int i=0;i<s.length();i++)
			{
				mark[s[i]-'a']++;
			}
			for(int i=0;i<t.length();i++)
			{
				mark[t[i]-'a']--;
			}
			for(int i=0;i<26;i++)
			{
				if(mark[i]<0)
				{
					puts("need tree");
					return 0;
				}
			}
			puts("both");
			return 0;
		}
	}
	return 0;
}
```








C：篱笆按给出数字顺序为高度，每次只能横着刷或者数着刷，问最小刷的次数（贪心DFS）





Bizon the Champion isn't just attentive, he also is very hardworking.


Bizon the Champion decided to paint his old fence his favorite color, orange. The fence is represented as *n* vertical planks, put in a row.
 Adjacent planks have no gap between them. The planks are numbered from the left to the right starting from one, the *i*-th plank has the width
 of 1 meter and the height of *a**i*meters.


Bizon the Champion bought a brush in the shop, the brush's width is 1 meter. He can make vertical and horizontal strokes with the brush. During a
 stroke the brush's full surface must touch the fence at all the time (see the samples for the better understanding). What minimum number of strokes should Bizon the Champion do to fully paint the fence? Note that you are allowed to paint the same area of the
 fence multiple times.




Input


The first line contains integer *n*(1 ≤ *n* ≤ 5000) —
 the number of fence planks. The second line contains*n* space-separated integers *a*1, *a*2, ..., *a**n*(1 ≤ *a**i* ≤ 109).




Output


Print a single integer — the minimum number of strokes needed to paint the whole fence.




Sample test(s)




input
5
2 2 1 2 1




output
3




input
2
2 2




output
2




input
1
5




output
1






Note


In the first sample you need to paint the fence in three strokes with the brush: the first stroke goes on height 1 horizontally along all the planks. The second stroke goes on height 2 horizontally and paints the first and second planks and the third stroke
 (it can be horizontal and vertical) finishes painting the fourth plank.


In the second sample you can paint the fence with two strokes, either two horizontal or two vertical strokes.


In the third sample there is only one plank that can be painted using a single vertical stroke.







贪心DFSCode:



```cpp
#include <cmath>   
#include <cstdio>  
#include <string>  
#include <cstring>  
#include <iostream>  
#include <algorithm>
using namespace std;
typedef long long ll;
#define N 5005
int a[N];
int n;

int solve(int l,int r){
  int ans=r-l+1;
  int m=a[l],i,j;
  if (l>r) return 0;						//stop
  for (i=l+1;i<=r;++i) m=min(m,a[i]);		//find m=min
  for (i=l;i<=r;++i) a[i]-=m;				//All decrease m
  for (j=l;j<=r;++j) if (a[j]!=0) break;	//find first segment's left
  for (i=j;i<=r;++i) if (a[i]==0)			//find first segment's right
					{
      					m+=solve(j,i-1);	//solve it
      					if (m>ans) return ans;//specially,if reach the end
      					else j=i+1;			//go ahead to next segment
    				}
  if (j<=r) m+=solve(j,r);					//reach the r:right
  return min(m,ans);						//m can not beyond ans
}

int main()
{
  int i,j;
  cin>>n; 
  for (i=0;i<n;++i) cin>>a[i];
  cout<<solve(0,n-1);
  return 0;
}
```





先贴一个帅瞎不偿命的DP做法  pflueger@Codeforces

Code：



```cpp
#include <iostream>

using namespace std;

int main() {
    int n; cin >> n;
    int a[n+1];
    a[0] = 0;
    for (int i=1; i<=n; i++) cin >> a[i];
    int dp[n+1][n+1];
    for (int j=0; j<=n; j++) dp[n][j] = 0;
    for (int i=n-1; i>=0; i--) for (int j=0; j<=i; j++) {
	if (a[j] >= a[i+1]) dp[i][j] = dp[i+1][i+1]; //Already painted
	else {
	    dp[i][j] = min( 1 + dp[i+1][j], a[i+1]-a[j] + dp[i+1][i+1]);
	}
    }
    cout << dp[0][0] << endl;
}
```









Bizon the Champion isn't just charming, he also is very smart.


While some of us were learning the multiplication table, Bizon the Champion had fun in his own manner. Bizon the Champion painted an*n* × *m* multiplication
 table, where the element on the intersection of the *i*-th row and *j*-th
 column equals *i*·*j* (the rows and columns of the table are numbered starting from 1). Then he was asked: what number in the table
 is the *k*-th largest number? Bizon the Champion always answered correctly and immediately. Can you repeat his success?


Consider the given multiplication table. If you write out all *n*·*m* numbers from the table in the non-decreasing order, then the *k*-th
 number you write out is called the *k*-th largest number.




Input


The single line contains integers *n*, *m* and *k*(1 ≤ *n*, *m* ≤ 5·105; 1 ≤ *k* ≤ *n*·*m*).




Output


Print the *k*-th largest number in a *n* × *m* multiplication
 table.




Sample test(s)




input
2 2 2




output
2




input
2 3 4




output
3




input
1 10 5




output
5






Note


A 2 × 3 multiplication table looks like this:
1 2 3
2 4 6





D：nXm的矩阵中aij=i*j，问nXm矩阵中第k大的数字是哪个（二分查找）

Code：



```cpp
#include <cmath>   
#include <cstdio>  
#include <string>  
#include <cstring>  
#include <iostream>  
#include <algorithm>
using namespace std;
typedef long long ll;
const int N = 1e6 + 6;

ll f(ll x, int n, int m)//f: there are f() less than x in nxm blanks 
{
    ll res = 0;
    --x;
    for(int i=1;i<=n;++i) res+=min((ll)m, x/i);
    return res;
}

int main(){
    ll n,m,k;
    cin>>n>>m>>k;
    ll l=1, r=1LL*n*m+1, x; 
    while(l<r)				//binary 
	{
        x = (l+r)>>1;	
        if(f(x,n,m)<k) l=x+1; else r=x;
    }
    cout<<l-1<<endl;		
    return 0;
}
```


E：看到就被吓住，没敢写







Offical Solution：



[448A - Rewards](http://codeforces.com/contest/448/problem/A)


Solution:[7139559](http://codeforces.com/contest/448/submission/7139559)


Because rewards of one type can be on one shelf, lets calculate number of cups — *a* and number of medals — *b*.
 Minimum number of shelves that will be required for all cups can be found by formula (*a* + 5 - 1) / 5. The same with shelves with medals: (*b* + 10 - 1) / 10.
 If sum of this two values more than *n* then answer is "NO" and "YES" otherwise.

[448B - Suffix Structures](http://codeforces.com/contest/448/problem/B)


Solution:[7139584](http://codeforces.com/contest/448/submission/7139584)


Consider each case separately. If we use only suffix automaton then *s* transform to some of its subsequence. Checking that *t* is
 a subsequence of *s* can be performed in different ways. Easiest and fastest — well-known two pointers method. In case of using suffix array we can get every
 permutation of *s*. If it is not obvious for you, try to think. Thus, *s* and *t* must
 be anagrams. If we count number of each letter in each string, we can check this. If every letter appears in *s* the same times as in *t* then
 words are anagrams. In case of using both structures strategy is: remove some letters and shuffle the rest. It is possible if every letter appears in *s* not
 less times than in *t*. Otherwise it is impossible to make *t* from *s*.
 Total complexity *O*(|*s*| + |*t*| + 26).

[448C - Painting Fence](http://codeforces.com/contest/448/problem/C)


Solution:[7139610](http://codeforces.com/contest/448/submission/7139610)


To solve this problem we need to understand some little things. First, every horizontally stroke must be as widely as possible. Second, under every horizontally stroke should be only horizontally strokes. So, if bottom of fence painted by horizontally stroke
 then number of this strokes must at least *min*(*a*1, *a*2, ..., *a**n*).
 These strokes maybe divides fence into some unpainted disconnected parts. For all of these parts we need to sum they answers. Now its clearly that solution is recursive. It takes segment [*l*, *r*] and
 height of painted bottom *h*. But we must not forget about situation when all planks painted with vertically strokes. In this case answer must be limited
 by *r* - *l* + 1 (length of segment). With given constrains of *n* we
 can find minimum on segment by looking all the elements from segment. Complexity in this case will be *O*(*n*2).
 But if we use for example segment tree, we can achieve *O*(*nlogn*) complexity.

[448D - Multiplication Table](http://codeforces.com/contest/448/problem/D)


Solution:[7139620](http://codeforces.com/contest/448/submission/7139620)


Solution is binary search by answer. We need to find largest *x* such that amount of numbers from table, least than *x*,
 is strictly less than *k*. To calculate this count we sum counts from rows. In *i* th
 row there will be ![](http://espresso.codeforces.com/ff7a4018dc8b9d8655be143444ca4f435a052ea8.png).
 Total complexity is *O*(*nlog*(*nm*)).

[448E - Divisors](http://codeforces.com/contest/448/problem/E)


Solution:[7139644](http://codeforces.com/contest/448/submission/7139644)


Learn how to transform *X**i* into *X**i* + 1.
 For this we need to concatenate lists of divisors for all elements of *X**i*.
 To do this efficiently, precalculate divisors of *X* (because for every *i**X**i* consist
 of its divisors). It can be done by well-known method with ![](http://espresso.codeforces.com/6a50bf098f1c6c2c202d6ed065157d06f7724110.png)complexity.
 How to calculate divisors of divisors? Need to know that for the given constrains for *X* maximum number of divisors *D*(*X*)will
 be 6720 (in the number 963761198400), so divisors of divisors can be calculated in *O*(*D*2(*X*)) time.
 With this lists we can transform *X**i* into *X**i* + 1 in *O*(*N*) time,
 were *N* = 105 — is the limit
 of numbers in output. Now learn how to transform *X**i* into *X*2*i*.
 What says *X**i*?
 Besides what would be *X* after *i* steps, it can tell
 where goes everyone divisor of *X* after *i* - 1 steps.
 Actually, *X**i* is
 concatenation of all *Y**i* - 1,
 where *Y* is divisor of *X*. For example, 103 = [1, 1, 1, 2, 1, 1, 5, 1, 1, 2, 1, 5, 1, 2, 5, 10] = [1] + [1, 1, 2] + [1, 1, 5] + [1, 1, 2, 1, 5, 1, 2, 5, 10] = 12 + 22 + 52 + 102.
 How to know which segment corresponds for some *Y*? Lets*pos*(*Y*) be
 the first index of *Y* in *X**i*.
 Then needed segment starts from *pos*(*prev*(*Y*)) + 1 and ends in *pos*(*Y*),
 where *prev*(*Y*) is previous divisor before *Y* in
 sorted list of divisors. So, to make *X*2*i* from *X**i* we
 need to know where goes every element from *X**i* after *i*steps.
 We know all its divisors — it is one step, and for every divisor we know where it goes after *i* - 1 step. Thus, we again need to concatenate some segments
 in correct order. It also can be done in *O*(*N*) time. How to find now *X**k* for
 every *k*? The method is similar as fast exponentiation:

*X**k* = [*X*] when *k* = 0,


if *k* is odd then transform *X**k* - 1 to *X**k*,


if *k* is even then transform *X**k* / 2 to *X**k*.


This method takes *O*(*logk*) iterations. And one small trick: obviously that for *X* > 1*X**k* starts
 from *k* ones, so *k* can be limited by *N*.
 Total complexity of solution is ![](http://espresso.codeforces.com/dbfa96de7a80c249e45e2c892fe3680b9c03b4f7.png).




