# 第六次codeforces竞技结束 #257 Div 2 - CD's Coding - CSDN博客





2014年07月20日 07:50:43[糖果天王](https://me.csdn.net/okcd00)阅读数：549








历史总是惊人的相似……Final的B题被WA at 10…… 掉落深渊

无奈……以后一定要仔细…… 每次都是加减的问题情何以堪……





There are *n* children in Jzzhu's school. Jzzhu is going to give some candies to them. Let's number all the children from 1 to *n*.
 The *i*-th child wants to get at least *a**i* candies.


Jzzhu asks children to line up. Initially, the *i*-th child stands at the *i*-th
 place of the line. Then Jzzhu start distribution of the candies. He follows the algorithm:
- 
Give *m* candies to the first child of the line.
- 
If this child still haven't got enough candies, then the child goes to the end of the line, else the child go home.
- 
Repeat the first two steps while the line is not empty.


Consider all the children in the order they go home. Jzzhu wants to know, which child will be the last in this order?




Input


The first line contains two integers *n*, *m*(1 ≤ *n* ≤ 100; 1 ≤ *m* ≤ 100).
 The second line contains *n* integers *a*1, *a*2, ..., *a**n*(1 ≤ *a**i* ≤ 100).




Output


Output a single integer, representing the number of the last child.




Sample test(s)




input
5 2
1 3 1 4 2




output
4




input
6 4
1 1 2 2 3 3




output
6






Note


Let's consider the first sample.


Firstly child 1 gets 2 candies and go home. Then child 2 gets 2 candies and go to the end of the line. Currently the line looks like [3, 4, 5, 2] (indices of the children in order of the line). Then child 3 gets 2 candies and go home, and then child 4 gets
 2 candies and goes to the end of the line. Currently the line looks like [5, 2, 4]. Then child 5 gets 2 candies and goes home. Then child 2 gets two candies and goes home, and finally child 4 gets 2 candies and goes home.


Child 4 is the last one who goes home.





以前有过一样的问题，不过那是上车，这次是发糖，糖发够了小孩子就回家了，否则到队尾继续排队，我直接用了相同的queue数据结构，使用了pair来锻炼锻炼这种写法——



```cpp
#include <cmath>   
#include <queue>
#include <cstdio>  
#include <string>  
#include <cstring>  
#include <iostream>  
#include <algorithm>
using namespace std;
typedef pair<int,int> p;

int main()
{
	int n,m;
	cin>>n>>m;
	queue<p> q;
	while(!q.empty())q.pop();
	for(int i=1;i<=n;i++)
	{
		int tmp;
		scanf("%d",&tmp);
		q.push(make_pair(tmp,i));
	}
	p now=make_pair(0,1);
	while(!q.empty())
	{
		now=q.front();
		q.pop();
		if(now.first<=m);//sorry for forgetting '='
		else q.push(make_pair(now.first-m,now.second));
	}
	cout<<now.second;
	return 0;
}
```












Jzzhu has invented a kind of sequences, they meet the following property:
![](http://espresso.codeforces.com/c84e959b7fd00c2ebce39e2b33db6bdb624ba8f3.png)

You are given *x* and *y*, please calculate *f**n* modulo 1000000007(109 + 7).




Input


The first line contains two integers *x* and *y*(|*x*|, |*y*| ≤ 109).
 The second line contains a single integer *n*(1 ≤ *n* ≤ 2·109).




Output


Output a single integer representing *f**n* modulo 1000000007(109 + 7).




Sample test(s)




input
2 3
3




output
1




input
0 -1
2




output
1000000006






Note


In the first sample, *f*2 = *f*1 + *f*3, 3 = 2 + *f*3, *f*3 = 1.


In the second sample, *f*2 =  - 1;  - 1 modulo (109 + 7) equals (109 + 6).










找规律题，这个只要写几步就发现其实6个一循环，我错在了这六个的第五个我居然减错了……





```cpp
#include <cmath>   
#include <cstdio>  
#include <string>  
#include <cstring>  
#include <iostream>  
#include <algorithm>
using namespace std;
typedef long long ll;
const ll mod=1000000007;
int main()
{
	ll x,y,n;
	cin>>x>>y;
	cin>>n;
	ll ans[6];
	//ll ans[6]={y-x,x,y,y-x,-x,y-2*x};
	ans[1]=(x+mod)%mod;
    ans[2]=(y+mod)%mod;
    ans[3]=(ans[2]-ans[1]+mod)%mod;
    ans[4]=(-x+mod)%mod;
    ans[5]=(-y+mod)%mod;
    ans[0]=(ans[1]-ans[2]+mod)%mod;
	cout<<(ans[n%6]%mod+mod)%mod;
	return 0;
}
```




Jzzhu has a big rectangular chocolate bar that consists of *n* × *m* unit squares. He wants to cut this bar exactly *k* times.
 Each cut must meet the following requirements:
- 
each cut should be straight (horizontal or vertical);
- 
each cut should go along edges of unit squares (it is prohibited to divide any unit chocolate square with cut);
- 
each cut should go inside the whole chocolate bar, and all cuts must be distinct.


The picture below shows a possible way to cut a 5 × 6 chocolate for 5 times.
![](http://espresso.codeforces.com/15ef03a74347ef850c56c54bb04b0a7b8fb3d1b3.png)

Imagine Jzzhu have made *k* cuts and the big chocolate is splitted into several pieces. Consider the smallest (by area) piece of the chocolate, Jzzhu wants
 this piece to be as large as possible. What is the maximum possible area of smallest piece he can get with exactly*k* cuts? The area of a chocolate piece
 is the number of unit squares in it.




Input


A single line contains three integers *n*, *m*, *k*(1 ≤ *n*, *m* ≤ 109; 1 ≤ *k* ≤ 2·109).




Output


Output a single integer representing the answer. If it is impossible to cut the big chocolate *k* times, print -1.




Sample test(s)




input
3 4 1




output
6




input
6 4 2




output
8




input
2 3 4




output
-1






Note


In the first sample, Jzzhu can cut the chocolate following the picture below:
![](http://espresso.codeforces.com/8845791751b665bf2158530fb6f2b3e1ddc850d2.png)

In the second sample the optimal division looks like this:
![](http://espresso.codeforces.com/85864264c5cfc7d0dd783485087eeae6df30b934.png)

In the third sample, it's impossible to cut a 2 × 3 chocolate 4 times.





这题说是nxm的巧克力画k条线，要求分得的最小的巧克力最大。

比赛时一直想的是两侧都分解因式，然后k分为a+b分配给两边或者直接全都用来分割一条边。然后就想的复杂了——

实际上：

1）特判k>m+n-2为-1

2）可以全为一边时记录答案

3）把其中一边分割完再分另一边记录答案

4）比较答案大小

即可



```cpp
#include <cmath>   
#include <cstdio>  
#include <string>  
#include <cstring>  
#include <iostream>  
#include <algorithm>
#define update(x) ans=ans>x?ans:x
using namespace std;
typedef long long ll;

int main()
{
	ll n,m,k,ans=0;
	cin>>n>>m>>k;
	if(k>n+m-2){cout<<"-1";return 0;}
	//if(k==n+m-2){cout<<"1";return 0;}
	if(k<m)	update(n*(m/(k+1)));
	if(k<n) update(m*(n/(k+1)));
	if(k>=m)	update(n/(k-m+2));
	if(k>=n)    update(m/(k-n+2));
	cout<<ans;
	return 0;
}

/*
int main()
{
	ll n,m,k;
	cin>>n>>m>>k;
	if(k>n+m-2){cout<<"-1";return 0;}
	if(k==n+m-2){cout<<"1";return 0;}
	ll ansn=0,ansm=0,ansb=0;
	if(k+1<=n) ansn=n*m/(k+1);
	if(k+1<=m) ansm=n*m/(k+1);
	if(k%2==0)
	{
		ll a=k/2+1,b=k/2+1;
		if(a>m){b=b+a-m,a=m;}
		if(b>m){a=a+b-m,b=m;}
		if(a>n){b=b+a-n,a=n;}
		if(b>n){a=a+b-n,b=n;}
		ansb=(m/a) * (n/b);
	} 
	else 
	{
		
		ll a=k/2+2,b=k/2+1;
		if(a>m){b=b+a-m,a=m;}
		if(b>m){a=a+b-m,b=m;}
		if(a>n){b=b+a-n,a=n;}
		if(b>n){a=a+b-n,b=n;}
		ansb=max ((m/a)*(n/b),(m/b)*(n/a));
	}
	cout<<max(ansn,max(ansm,ansb));
	return 0;
} 
*/
```





