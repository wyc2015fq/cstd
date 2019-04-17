# 【水打表】#69 A. Panoramix's Prediction - CD's Coding - CSDN博客





2014年10月25日 23:30:12[糖果天王](https://me.csdn.net/okcd00)阅读数：720
所属专栏：[CF-A Terminator](https://blog.csdn.net/column/details/cf-amaster.html)

















A prime number is a number which has exactly two distinct divisors: one and itself. For example, numbers 2, 7, 3 are
 prime, and 1, 6,4 are
 not.

The next prime number after *x* is the smallest prime
 number greater than *x*. For example, the next prime number after 2 is 3,
 and the next prime number after 3 is 5.
 Note that there is exactly one next prime number after each number. So 5is not the
 next prime number for 2.


One cold April morning Panoramix predicted that soon Kakofonix will break free from his straitjacket, and this will be a black day for the residents of the Gallic countryside.


Panoramix's prophecy tells that if some day Asterix and Obelix beat exactly *x* Roman soldiers, where *x* is
 a prime number, and next day they beat exactly *y* Roman soldiers, where *y* is the
 next prime number after *x*, then it's time to wait for Armageddon, for nothing can shut Kakofonix up while he sings his infernal song.


Yesterday the Gauls beat *n* Roman soldiers and it turned out that the number *n* was
 prime! Today their victims were a troop of *m*Romans (*m* > *n*).
 Determine whether the Gauls should wait for the black day after today's victory of Asterix and Obelix?




Input


The first and only input line contains two positive integers — *n* and *m* (2 ≤ *n* < *m* ≤ 50).
 It is guaranteed that *n* is prime.


Pretests contain all the cases with restrictions 2 ≤ *n* < *m* ≤ 4.




Output


Print YES, if *m* is the
 next prime number after *n*, or NO otherwise.




Sample test(s)




input
3 5




output
YES



input
7 11




output
YES



input
7 9




output
NO























问m是不是n的下一个整数，那就先把50以内的素数背出来呗~

然后从第一个开始一个一个往下看~ 啊当然如果m不在这个素数表里那肯定就是NO啦，然后看n是否在m和m前的素数间啦，要注意的是区间范围是左闭右开哦



```cpp
#include <cstdio>
#include <string>
#include <cstring> 
#include <iostream>
#include <algorithm>
using namespace std;
typedef long long ll;
// http://codeforces.com/contest/80  
// Panoramix's Prediction1 

int prime[]={2,3,5,7,11,13,17,19,23,29,31,37,41,43,47};

int main()
{
	int n,m;	cin>>n>>m;
	for(int i=0;i<=14;i++)
	{
		if(prime[i]>n)
		{
			if(prime[i]==m)
			{
				cout<<"YES";
				return 0;
			}
			cout<<"NO";
			return 0;
		}
	}
	cout<<"NO";
	return 0;
}
```







