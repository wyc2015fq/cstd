# 第十七次codeforces竞技结束 #270 Div 2 - CD's Coding - CSDN博客





2014年10月17日 15:00:15[糖果天王](https://me.csdn.net/okcd00)阅读数：482








这一场居然是七道题，当时还吓了一跳，只做了3道题怎么破之类的……

但是，嘛，稳定三道题是很重要的啦~不要在意这些细节~~~








One way to create a task is to learn from math. You can generate some random math statement or modify some theorems to get something new and build a new task from that.


For example, there is a statement called the "Goldbach's conjecture". It says: "each even number no less than four can be expressed as the sum of two primes". Let's modify it. How about a statement like that: "each integer no less than 12 can be expressed as
 the sum of two composite numbers." Not like the Goldbach's conjecture, I can prove this theorem.


You are given an integer *n* no less than 12, express it as a sum of two composite numbers.




Input


The only line contains an integer *n*(12 ≤ *n* ≤ 106).




Output


Output two composite integers *x* and *y*(1 < *x*, *y* < *n*) such
 that *x* + *y* = *n*. If there are multiple solutions, you can output any of them.




Sample test(s)




input
12




output
4 8




input
15




output
6 9




input
23




output
8 15




input
1000000




output
500000 500000






Note


In the first example, 12 = 4 + 8 and both 4, 8 are composite numbers. You can output "6 6" or "8 4" as well.


In the second example, 15 = 6 + 9. Note that you can't output "1 14" because 1 is not a composite number.





我被这道题弄得简直哭了要…… 当时被坑得半死……

其实想多了啦大家~ 因为2以外的偶数都是合数对不对^_^，所以，奇数的话就是9 + （N-9），偶数的话是4 + （N-4）~~~


|[8018751](http://codeforces.com/contest/472/submission/8018751)|Sep/29/2014 03:38|Accepted|
|----|----|----|
|[8018742](http://codeforces.com/contest/472/submission/8018742)|Sep/29/2014 03:36|Compilation error|
|[8011448](http://codeforces.com/contest/472/submission/8011448)|Sep/28/2014 21:25|Wrong answer on test 32|
|[7999693](http://codeforces.com/contest/472/submission/7999693)|Sep/28/2014 19:48|Hacked|


### Code（先给大家看看当时我的水代码）：



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
const int N = (int)1e6;
#define Max(a,b) ((a)>(b)?(a):(b))
#define Min(a,b) ((a)<(b)?(a):(b))

int n,prime[N]={0},nump=0;
int isNotPrime[N]={1,1};

bool cmp(const int a, const int b)
{
	return a > b;
}

void getpri()
{
	for(int i=2;i<=N/2;i++)
	{
		if(!isNotPrime[i]) prime[nump++]=i;//cout<<i<<' ';
		for(int j=0;j<nump && i*prime[j]<N;j++)
		{
			isNotPrime[i*prime[j]]=1;
			if(!(i%prime[j]))break;
		}
	}
}

int main()
{
	cin>>n;
	int pos=1;
	getpri();
	
	for(int i=4;i<=n/2;i++)
	{
		if(i> prime[pos])pos++;
		if(i==prime[pos])continue;
		if(count(prime,prime+55555,n-i)==1)continue;
		else
		{
			cout<<i<<" "<<n-i<<endl;
			return 0;
		}
	}
	
	return 0;
}
```



嘛，实际上~

### Code：



```cpp
bool f=(n%2==1);
cout<<f?9:4<<" "<<f?n-9:n-4<<endl;
```

```cpp
#include <iostream>
#include <algorithm>
using namespace std;
#define Max(a,b) ((a)>(b)?(a):(b))
#define Min(a,b) ((a)<(b)?(a):(b))

int main()
{
	int n;	cin>>n;
	if(n%2==0)cout<<4<<" "<<n-4<<endl;
	else cout<<9<<" "<<n-9<<endl;
	return 0;
}
```




One way to create a task is to learn from life. You can choose some experience in real life, formalize it and then you will get a new task.


Let's think about a scene in real life: there are lots of people waiting in front of the elevator, each person wants to go to a certain floor. We can formalize it in the following way. We have *n* people
 standing on the first floor, the *i*-th person wants to go to the *f**i*-th
 floor. Unfortunately, there is only one elevator and its capacity equal to *k* (that is at most *k* people
 can use it simultaneously). Initially the elevator is located on the first floor. The elevator needs |*a* - *b*| seconds to move from
 the *a*-th floor to the *b*-th
 floor (we don't count the time the people need to get on and off the elevator).


What is the minimal number of seconds that is needed to transport all the people to the corresponding floors and then return the elevator to the first floor?




Input


The first line contains two integers *n* and *k*(1 ≤ *n*, *k* ≤ 2000) —
 the number of people and the maximal capacity of the elevator.


The next line contains *n* integers: *f*1, *f*2, ..., *f**n*(2 ≤ *f**i* ≤ 2000),
 where *f**i* denotes
 the target floor of the *i*-th person.




Output


Output a single integer — the minimal time needed to achieve the goal.




Sample test(s)




input
3 2
2 3 4




output
8




input
4 2
50 100 50 100




output
296




input
10 3
2 2 2 2 2 2 2 2 2 2




output
8






Note


In first sample, an optimal solution is:
- 
The elevator takes up person #1 and person #2.
- 
It goes to the 2nd floor.
- 
Both people go out of the elevator.
- 
The elevator goes back to the 1st floor.
- 
Then the elevator takes up person #3.
- 
And it goes to the 2nd floor.
- 
It picks up person #2.
- 
Then it goes to the 3rd floor.
- 
Person #2 goes out.
- 
Then it goes to the 4th floor, where person #3 goes out.
- 
The elevator goes back to the 1st floor.




有一帮人想坐电梯上楼，如何安排可以让电梯经历的楼层数最少呢~



这道题正着想略烦……但是倒着来~~^_^

我们采取倒放的方式，先把楼层最高的几个送上去，下来的路上送第二第三……顺次的，到底层再送现在最高的几个（当然是装满）

这样一来就可以保证总楼层经历最少~

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

int des[2048];

int main()
{
	int n,k;	cin>>n>>k;
	memset(des,0,sizeof des);
	for(int i=0;i<n;i++)
	{
		scanf("%d",&des[i]);
	}
	sort(des,des+n,cmp);
	int rest=k,t=0;
	for(int i=0;i<n;i++)
	{
		if(rest>0)
		{
			if(rest==k) t+=2*(des[i]-1);
			rest--;
		}
		if(rest==0) rest=k;
		//cout<<i<<":"<<rest<<":"<<t<<endl;
	}
	cout<<t<<endl;
	return 0;
}
```




A way to make a new task is to make it nondeterministic or probabilistic. For example, the hard task of Topcoder SRM 595, Constellation, is the probabilistic version of a convex hull.


Let's try to make a new task. Firstly we will use the following task. There are *n* people, sort them by their name. It is just an ordinary
 sorting problem, but we can make it more interesting by adding nondeterministic element. There are *n* people, each person will use either
 his/her first name or last name as a handle. Can the lexicographical order of the handles be exactly equal to the given permutation *p*?


More formally, if we denote the handle of the *i*-th person as *h**i*,
 then the following condition must hold: ![](http://espresso.codeforces.com/7da83a168f60958e75fb0519eaef2bfeb7b2a382.png).




Input


The first line contains an integer *n*(1 ≤ *n* ≤ 105) —
 the number of people.


The next *n* lines each contains two strings. The *i*-th
 line contains strings *f**i* and *s**i*(1 ≤ |*f**i*|, |*s**i*| ≤ 50) —
 the first name and last name of the *i*-th person. Each string consists only of lowercase English letters. All of the given 2*n* strings
 will be distinct.


The next line contains *n* distinct integers: *p*1, *p*2, ..., *p**n*(1 ≤ *p**i* ≤ *n*).




Output


If it is possible, output "YES", otherwise output "NO".




Sample test(s)




input
3
gennady korotkevich
petr mitrichev
gaoyuan chen
1 2 3




output
NO




input
3
gennady korotkevich
petr mitrichev
gaoyuan chen
3 1 2




output
YES




input
2
galileo galilei
nicolaus copernicus
2 1




output
YES




input
10
rean schwarzer
fei claussell
alisa reinford
eliot craig
laura arseid
jusis albarea
machias regnitz
sara valestin
emma millstein
gaius worzel
1 2 3 4 5 6 7 8 9 10




output
NO




input
10
rean schwarzer
fei claussell
alisa reinford
eliot craig
laura arseid
jusis albarea
machias regnitz
sara valestin
emma millstein
gaius worzel
2 4 9 6 5 7 1 3 8 10




output
YES






Note


In example 1 and 2, we have 3 people: tourist, Petr and me (cgy4ever). You can see that whatever handle is chosen, I must be the first, then tourist and Petr must be the last.


In example 3, if Copernicus uses "copernicus" as his handle, everything will be alright.




这道题的意思是说每个人都有两个名字，可以任选其中一种来作为排序用，是否存在一种选择，令他们的名字可以按照给出的顺序字典序排序。



那么，我们就按照字典序把这所有的名字都排好序标记上是属于谁的名字，然后看是否存在满足这种条件的子序列即可~

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
typedef pair<string,int> psi;
#define Max(a,b) ((a)>(b)?(a):(b))
#define Min(a,b) ((a)<(b)?(a):(b))

struct cmp
{  
    bool operator()(psi a,psi b) 
    {  
        if(a.first!=b.first)
            return a.first>b.first;  
    }  
};  


priority_queue <psi,vector<psi>,cmp> p_q;

int main()
{
	int n;	cin>>n;
	for(int i=1;i<=n;i++)
	{
		string s1,s2;
		cin>>s1>>s2;
		p_q.push(psi(s1,i));
		p_q.push(psi(s2,i));
	}
	//cout<<p_q.top().first<<"\t"<< p_q.top().second<<endl;
	for(int i=1;i<=n;i++)
	{
		int pos;	cin>>pos;
		while(!p_q.empty() && p_q.top().second!=pos) p_q.pop();
		if(p_q.empty())
		{
			cout<<"NO"<<endl;
			return 0;
		}
		p_q.pop();
	}
	cout<<"YES"<<endl;
	return 0;
}
```


























