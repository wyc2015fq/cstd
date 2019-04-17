# 第九次codeforces竞技结束 #260 Div 2 - CD's Coding - CSDN博客





2014年08月09日 02:17:46[糖果天王](https://me.csdn.net/okcd00)阅读数：660








虽说还在Pending，但是已经万念俱灰了……

已经知道了自己的C题PrePassed注定会变成SystemTestFailed了……因为先INT乘后LongLong强制转换一定会溢出……哭瞎……

然后又犯了这个该死的错误……忘了注释掉freopen交了两发WA,反正这回死定了……













One day Dima and Alex had an argument about the price and quality of laptops. Dima thinks that the more expensive a laptop is, the better it is. Alex disagrees. Alex thinks that there are two laptops, such that the price of the first laptop is less (strictly
 smaller) than the price of the second laptop but the quality of the first laptop is higher (strictly greater) than the quality of the second laptop.


Please, check the guess of Alex. You are given descriptions of *n* laptops. Determine whether two described above laptops exist.




Input


The first line contains an integer *n* (1 ≤ *n* ≤ 105)
 — the number of laptops.


Next *n* lines contain two integers each, *a**i* and *b**i*(1 ≤ *a**i*, *b**i* ≤ *n*),
 where *a**i* is
 the price of the *i*-th laptop, and *b**i* is
 the number that represents the quality of the *i*-th laptop (the larger the number is, the higher is the quality).


All *a**i* are distinct.
 All *b**i* are distinct.




Output


If Alex is correct, print "Happy Alex", otherwise print "Poor Alex"
 (without the quotes).




Sample test(s)




input
2
1 2
2 1




output
Happy Alex












这题是说，如果发现有两种电脑，便宜的性能更好则HA，否则PA



那么我们就用pair存然后排序（我用的是优先队列自动维护顺序），然后遍历一遍，如果相邻的两个价格大的性能低则输出HA，遍历完都没有这种情况的话输出PA

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


int main()
{
	int n;	cin>>n;
	priority_queue <pair<int, int> > pq;  
	while(!pq.empty())pq.pop();
	for(int i=0;i<n;i++)
	{
		int a,b;	cin>>a>>b;
		pq.push(make_pair(a,b));
	}
	int la=-1,lb=-1,flag=0;
	while(!pq.empty())
	{
		pair<int,int> tmp=pq.top();pq.pop();
		//cout<<tmp.first<<"\t"<<tmp.second<<endl;
		if(la==-1 && lb==-1) la=tmp.first,lb=tmp.second;
		else
		{
			if(tmp.second>lb){flag=1;break;}
			lb=tmp.second;
		}
		
	}
	if(!flag)puts("Poor Alex");
	else puts("Happy Alex");
	
	return 0;
}
```






Fedya studies in a gymnasium. Fedya's maths hometask is to calculate the following expression:
(1*n* + 2*n* + 3*n* + 4*n*) *mod* 5

for given value of *n*. Fedya managed to complete the task. Can you? Note that given number *n* can
 be extremely large (e.g. it can exceed any integer type of your programming language).




Input


The single line contains a single integer *n* (0 ≤ *n* ≤ 10105).
 The number doesn't contain any leading zeroes.




Output


Print the value of the expression without leading zeros.




Sample test(s)




input
4




output
4




input
124356983594583453458888889




output
0






Note


Operation *x mod y* means taking remainder after division *x* by *y*.


Note to the first sample:

![](http://espresso.codeforces.com/825f244180bb10323db01645118c3cfdb312fa89.png)





这题问(1^n+2^n+3^n+4^n)Mod 5是多少。n可以各种大。

我们知道，1~4的n次幂mod5是有规律的：

1)1 1 1 1

2)2 4 3 1

3)3 4 2 1

4)4 1 4 1

Sum0 0 0 4

所以我们只需要知道这个好长好长的数字mod4是多少就行了，然而，一个数字mod4的话我们只需要判断后2位是否mod4=0即可。

这里我用的是割位法，这个不只针对4，其他的数字也可以适用，较为普适的算法：（啊啊啊啊啊这里我freopen交了2发WA呀 啊啊啊啊啊）

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

int main()
{
	char c;
	int now = 0;
	//freopen("in.txt","r",stdin);
	while(1)	
	{
		if(scanf("%c",&c)==EOF)break;
		if(isdigit(c))
		{
			now=now*10 + (c-'0');
			now=now%4;
		}
		else break;
	}
	if(!now)cout<<"4";
	else cout<<"0";
	return 0;
}
```









Alex doesn't like boredom. That's why whenever he gets bored, he comes up with games. One long winter evening he came up with a game and decided to play it.


Given a sequence *a* consisting of *n* integers. The
 player can make several steps. In a single step he can choose an element of the sequence (let's denote it *a**k*)
 and delete it, at that all elements equal to*a**k* + 1 and *a**k* - 1 also
 must be deleted from the sequence. That step brings *a**k* points
 to the player.


Alex is a perfectionist, so he decided to get as many points as possible. Help him.




Input


The first line contains integer *n* (1 ≤ *n* ≤ 105)
 that shows how many numbers are in Alex's sequence.


The second line contains *n* integers *a*1, *a*2,
 ..., *a**n* (1 ≤ *a**i* ≤ 105).




Output


Print a single integer — the maximum number of points that Alex can earn.




Sample test(s)




input
2
1 2




output
2




input
3
1 2 3




output
4




input
9
1 2 1 3 2 2 2 2 3




output
10






Note


Consider the third test example. At first step we need to choose any element equal to 2. After that step our sequence looks like this [2, 2, 2, 2].
 Then we do 4 steps, on each step we choose any element equals to 2.
 In total we earn 10 points.








哭瞎的一题……这题用dp做……

首先读入，m[a]为a出现的个数，用a从大到小排序，cnt计数。

那么—— REP （cnt，0，n）

1）对于当前这个数a —— 如果a-1的数字没有出现过，那么dp[now]=dp[now-1]+a*m[a].（就是这里,要改成(long long)a*m[a]）

2）当a-1出现过的话，dp[now]=max(dp[now-1],dp[now-2]+(long long)a*m[a])

Ex) 既然有now-1.now-2，那么1、2要记得特判哦，免得RE了

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
#define Max(a,b) ((a)>(b)?(a):(b))
#define Min(a,b) ((a)<(b)?(a):(b))
using namespace std;
typedef long long ll;
ll dp[100001];

int main()
{
	ll ans=0;
	int n;	cin>>n;
	int tmp=0,max=0;
	map<int,int> m;	m.clear();
	map<int,int>::key_compare kcm=m.key_comp();
	for(int nc=0;nc<n;nc++)
	{
		scanf("%d",&tmp);
		if(max<tmp) max=tmp;
		m[tmp]++;
	}
	map<int,int>::iterator it=m.end();	*it--;
	int cnt=0,lf,ls,nf,ns;
	for(;it!=m.begin();*it--,cnt++)
	{
		//cout<<it->first<<":"<<it->second<<endl;
		nf=it->first,ns=it->second;
		if(cnt==0) dp[0]=(ll)nf*ns;
		else if(nf+1 < lf) dp[cnt]=dp[cnt-1]+(ll)nf*ns;
		else 
		{
			if(cnt>1) dp[cnt]=Max(dp[cnt-1] , dp[cnt-2]+(ll)nf*ns);
			else dp[cnt]=Max(dp[cnt-1],(ll)nf*ns);
		}
		lf=nf,ls=ns;
	}
	//BEGIN FOGGOTTEN!
	//cout<<it->first<<":"<<it->second<<endl;
	nf=it->first,ns=it->second;
	if(nf+1 < lf) dp[cnt]=dp[cnt-1]+(ll)nf*ns;
	else 
	{
		if(cnt>1) dp[cnt]=Max(dp[cnt-1] , dp[cnt-2]+(ll)nf*ns);
		else dp[cnt]=Max(dp[cnt-1],(ll)nf*ns);
	}
	
	//for(int i=0;i<=cnt;i++)	cout<<dp[i]<<endl;
	cout<<dp[cnt];
	/*do
	{
		cout<<it->first<<":"<<it->second<<endl;
	} while (kcm((*it++).first, max) );*/
	
	return 0;
}
```






















