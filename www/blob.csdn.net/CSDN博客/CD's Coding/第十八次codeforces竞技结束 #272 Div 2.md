# 第十八次codeforces竞技结束 #272 Div 2 - CD's Coding - CSDN博客





2014年10月17日 17:20:07[糖果天王](https://me.csdn.net/okcd00)阅读数：519








这一场……我终于进入紫名了~

这一场……ABD写的挺快的……然后就有点自负了想要去写写E题……结果把C给放弃了……真是可惜……




Problems

![](http://worker.codeforces.ru/static/images/icons/control.png)






|#|Name|||
|----|----|----|----|
|[A](http://codeforces.com/contest/476/problem/A)|[Dreamoon and Stairs](http://codeforces.com/contest/476/problem/A)standard input/output1 s, 256 MB|![Submit](http://worker.codeforces.ru/static/images/icons/submit-22x22.png)![Add to favourites](http://worker.codeforces.ru/static/images/icons/star_gray_16.png)|![](http://worker.codeforces.ru/static/images/icons/user.png) x3245|
|[B](http://codeforces.com/contest/476/problem/B)|[Dreamoon and WiFi](http://codeforces.com/contest/476/problem/B)standard input/output1 s, 256 MB|![Submit](http://worker.codeforces.ru/static/images/icons/submit-22x22.png)![Add to favourites](http://worker.codeforces.ru/static/images/icons/star_gray_16.png)|![](http://worker.codeforces.ru/static/images/icons/user.png) x2467|
|[C](http://codeforces.com/contest/476/problem/C)|[Dreamoon and Sums](http://codeforces.com/contest/476/problem/C)standard input/output1.5 s, 256 MB|![Submit](http://worker.codeforces.ru/static/images/icons/submit-22x22.png)![Add to favourites](http://worker.codeforces.ru/static/images/icons/star_gray_16.png)|![](http://worker.codeforces.ru/static/images/icons/user.png) x1067|
|[D](http://codeforces.com/contest/476/problem/D)|[Dreamoon and Sets](http://codeforces.com/contest/476/problem/D)standard input/output1 s, 256 MB|![Submit](http://worker.codeforces.ru/static/images/icons/submit-22x22.png)![Add to favourites](http://worker.codeforces.ru/static/images/icons/star_gray_16.png)|![](http://worker.codeforces.ru/static/images/icons/user.png) x569|
|[E](http://codeforces.com/contest/476/problem/E)|[Dreamoon and Strings](http://codeforces.com/contest/476/problem/E)standard input/output1 s, 256 MB|![Submit](http://worker.codeforces.ru/static/images/icons/submit-22x22.png)![Add to favourites](http://worker.codeforces.ru/static/images/icons/star_gray_16.png)|![](http://worker.codeforces.ru/static/images/icons/user.png) x140|














Dreamoon wants to climb up a stair of *n* steps. He can climb 1 or 2 steps
 at each move. Dreamoon wants the number of moves to be a multiple of an integer *m*.


What is the minimal number of moves making him climb to the top of the stairs that satisfies his condition?




Input


The single line contains two space separated integers *n*, *m* (0 < *n* ≤ 10000, 1 < *m* ≤ 10).




Output


Print a single integer — the minimal number of moves being a multiple of *m*. If there is no way he can climb satisfying condition print - 1 instead.




Sample test(s)




input
10 2




output
6




input
3 5




output
-1






Note


For the first sample, Dreamoon could climb in 6 moves with following sequence of steps: {2, 2, 2, 2, 1, 1}.


For the second sample, there are only three valid sequence of steps {2, 1}, {1, 2}, {1, 1, 1} with 2, 2, and 3 steps respectively. All these numbers are not multiples of 5.




有n个台阶，每次可以上一级或者两级，问最少走几步可以走完这些台阶，并且步数为m的整数倍。

那么，我们先用2步走完，然后一点点用一步换掉两步，直到步数为m倍为止。

当换完成1步都不行，那就真的是不行了。

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
	int n,m;	cin>>n>>m;
	int steps=n%2+n/2;
	if(n==0){cout<<"0"; return 0;}
	if(steps%m!=0) steps+=(m-steps%m);
	if(steps>n) steps=-1;
	cout<<steps;	return 0;
}
```




Dreamoon is standing at the position 0 on a number line. Drazil is sending a list of commands through Wi-Fi to Dreamoon's smartphone and Dreamoon follows
 them.


Each command is one of the following two types:
- 
Go 1 unit towards the positive direction, denoted as '+'
- 
Go 1 unit towards the negative direction, denoted as '-'


But the Wi-Fi condition is so poor that Dreamoon's smartphone reports some of the commands can't be recognized and Dreamoon knows that some of them might even be wrong though successfully recognized. Dreamoon decides to follow every recognized command and toss
 a fair coin to decide those unrecognized ones (that means, he moves to the 1 unit to the negative or positive direction with the same probability 0.5).


You are given an original list of commands sent by Drazil and list received by Dreamoon. What is the probability that Dreamoon ends in the position originally supposed to be final by Drazil's commands?




Input


The first line contains a string *s*1 —
 the commands Drazil sends to Dreamoon, this string consists of only the characters in the set {'+', '-'}.


The second line contains a string *s*2 —
 the commands Dreamoon's smartphone recognizes, this string consists of only the characters in the set {'+', '-', '?'}. '?' denotes
 an unrecognized command.


Lengths of two strings are equal and do not exceed 10.




Output


Output a single real number corresponding to the probability. The answer will be considered correct if its relative or absolute error doesn't exceed 10 - 9.




Sample test(s)




input
++-+-
+-+-+




output
1.000000000000




input
+-+-
+-??




output
0.500000000000




input
+++
??-




output
0.000000000000






Note


For the first sample, both *s*1 and *s*2 will
 lead Dreamoon to finish at the same position  + 1.


For the second sample, *s*1 will
 lead Dreamoon to finish at position 0, while there are four possibilites for *s*2:
 {"+-++", "+-+-", "+--+", "+---"}
 with ending position {+2, 0, 0, -2} respectively. So there are 2 correct cases out of 4,
 so the probability of finishing at the correct position is 0.5.


For the third sample, *s*2 could
 only lead us to finish at positions {+1, -1, -3}, so the probability to finish at the correct position  + 3 is 0.





这道题是说，先给一个左右移动的字符串，+是右移，-是左移，然后第二个字符串（其中可能有问号代表不知道这里是+还是-），问这个字符串到达第一个字符串移动的终点的概率是多少。那么很显然，如果第二个字符串没有问号，那就是直接判断位置，概率那就是1或0，每出现一个问号就多一个不确定项。我们先把可以确认的移动方向去定下来，看还距离多少，看剩下的问号数有没有可能完成剩下的任务（问号比剩余距离短或者达成目标之后剩余的问号数不是偶数），顺便说一下，为啥达成目标之后剩余需要是偶数呢，因为可以一个加号一个减号抵消掉呀~

so，那就是个简单概率题咯~，通过计算，问号们应该是几个+几个-才可以完成，那么就直接C(all,need)*1/(2^all)即可^_^

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
int Jc[12];
#define Max(a,b) ((a)>(b)?(a):(b))
#define Min(a,b) ((a)<(b)?(a):(b))

bool cmp(const int a, const int b)
{
	return a > b;
}

void Jc_init()
{
	Jc[1]=Jc[0]=1;
	for(int i=2;i<12;i++)
		Jc[i]=Jc[i-1]*i;
}

double C(int n,int m)
{
	double ret=1.0;
	for(int i=1;i<=n;i++)	ret/=2.0;
	ret= ret*(double)Jc[n]/(double)Jc[m]/(double)Jc[n-m];
	return ret;
}


int main()
{
	string s1,s2;
	cin>>s1>>s2;
	double rate=0.0;
	int pos1=0,pos2=0,unc=0;
	Jc_init();
	for(int i=0;i<s1.length();i++)
	{
		if(s1[i]=='+')pos1++;
		else pos1--;
		if(s2[i]=='+')pos2++;
		else if(s2[i]=='-') pos2--;
		else unc++;
	}
	int pos=abs(pos2-pos1);
	if(pos<=unc)
	{
		int res=unc-pos;
		if(res%2==0)
		{
			int need=res/2+pos;
			rate=C(unc,need);
		}
	}
	printf("%.12f",rate);
	return 0;
}
```







Dreamoon loves summing up something for no reason. One day he obtains two integers *a* and *b* occasionally.
 He wants to calculate the sum of all nice integers. Positive integer *x* is
 called nice if ![](http://espresso.codeforces.com/747b668d05dce59a14c79f7573cf84d4af25536c.png) and ![](http://espresso.codeforces.com/f2c3c9a716cef71591af317f816d0d89605de8cd.png),
 where *k* is some integer number in range [1, *a*].


By ![](http://espresso.codeforces.com/1e6fb3a33a9e71aae77ea40b847b4751cad6a9b2.png) we
 denote the quotient of integer division of *x* and *y*.
 By ![](http://espresso.codeforces.com/13ccd3404ca33681bef79230f269517728677a3a.png) we
 denote the remainder of integer division of *x* and*y*.
 You can read more about these operations here: http://goo.gl/AcsXhT.


The answer may be large, so please print its remainder modulo 1 000 000 007 (109 + 7).
 Can you compute it faster than Dreamoon?




Input


The single line of the input contains two integers *a*, *b* (1 ≤ *a*, *b* ≤ 107).




Output


Print a single integer representing the answer modulo 1 000 000 007 (109 + 7).




Sample test(s)




input
1 1




output
0




input
2 2




output
8






Note


For the first sample, there are no nice integers because ![](http://espresso.codeforces.com/03b1dc6bae5180f8a2d8eb85789e8b393e585970.png) is
 always zero.


For the second sample, the set of nice integers is {3, 5}.





===》 ![](http://espresso.codeforces.com/f2c3c9a716cef71591af317f816d0d89605de8cd.png)


范围内满足这些各项范围条件内的，满足条件的x的和是多少

这道题是当时没能写出来的题……

数学要学好哇，推公式挺重要……

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
const ll lim=(ll)1e9+7;
#define Max(a,b) ((a)>(b)?(a):(b))
#define Min(a,b) ((a)<(b)?(a):(b))

int main()
{
    ll n,a,b;
    const ll mod=1000000007;
    scanf("%I64d%I64d",&a,&b);
    if(b==1){printf("0\n");return 0;}
    ll fa,fb,an=(b+1+b*a+1)*a/2;
    ll ans=((a*(a+1)/2)%mod*b+a)%mod;
    ans=ans*((b*(b-1)/2)%mod)%mod;
    printf("%I64d\n",ans);
    return 0;
}
```


### Python_Code：





```python
a, b = tuple(map(int, input().split()))
print((b*(b-1)*a*(a+1)*b//4 + a*b*(b-1) // 2) % 1000000007)
```









Dreamoon likes to play with sets, integers and ![](http://espresso.codeforces.com/3f35bbc2586102b2f94133a8461976bd1b6f93dc.png). ![](http://espresso.codeforces.com/47ab79f1c5210d5c6156290f6f8e9af1075ae896.png) is
 defined as the largest positive integer that divides both *a* and *b*.


Let *S* be a set of exactly four distinct integers greater than 0.
 Define *S* to be of rank *k* if
 and only if for all pairs of distinct elements *s**i*,*s**j* from *S*, ![](http://espresso.codeforces.com/a6ed29214ad36b6a974c8ebd241c2caa5589089e.png).


Given *k* and *n*, Dreamoon
 wants to make up *n* sets of rank *k* using
 integers from 1 to *m* such that
 no integer is used in two different sets (of course you can leave some integers without use). Calculate the minimum *m* that makes it possible
 and print one possible solution.




Input


The single line of the input contains two space separated integers *n*, *k* (1 ≤ *n* ≤ 10 000, 1 ≤ *k* ≤ 100).




Output


On the first line print a single integer — the minimal possible *m*.


On each of the next *n* lines print four space separated integers representing the *i*-th
 set.


Neither the order of the sets nor the order of integers within a set is important. If there are multiple possible solutions with minimal *m*,
 print any one of them.




Sample test(s)




input
1 1




output
5
1 2 3 5




input
2 2




output
22
2 4 6 22
14 18 10 16






Note


For the first example it's easy to see that set {1, 2, 3, 4} isn't a valid set of rank 1 since ![](http://espresso.codeforces.com/e2af04e5e60e1fe79a4d74bf22dfa575f0b0f7bb.png).












这道题的意思是请找出n组数，每组4个数，要求每组的4个数字两两之间的公约数都是k……

那我们就弄成每组数字都互质，然后全体乘以k不就行了么……

然后看了看样例……

这个该怎么说……我是觉得额这道题的output暴露了数据规律怎么破……我算是看出规律再证明的方式A的这道题

当时我看到22那个样例的时候……在想他干嘛要把22放这里……然后发现 2/4/6/10 14/16/18/22也是行的哇……

化成乘以k之前的数据…… 1/2/3/5和7/8/9/11……我就试了试——每组公差都是6？……然后就A了……

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
	int n,k;	cin>>n>>k;
	int v[4]={1,2,3,5};
	printf("%d\n",(n*6-1)*k);
	for(int i=1;i<=n;i++)
	{
		for(int j=0;j<4;j++)
		{
			if(j==0) printf("%d",v[j]*k);
			else printf(" %d",v[j]*k);
			v[j]+=6;
		}
		printf("\n");
	}
	return 0;
}
```
















