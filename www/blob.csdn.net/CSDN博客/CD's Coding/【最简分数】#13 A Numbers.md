# 【最简分数】#13 A. Numbers - CD's Coding - CSDN博客





2014年04月26日 15:26:04[糖果天王](https://me.csdn.net/okcd00)阅读数：776
所属专栏：[CF-A Terminator](https://blog.csdn.net/column/details/cf-amaster.html)


















Little Petya likes numbers a lot. He found that number 123 in base 16 consists of two digits: the first is 7 and the second is 11. So the sum of digits of 123 in base 16 is equal to 18.


Now he wonders what is an average value of sum of digits of the number *A* written in all bases from 2 to *A* - 1.


Note that all computations should be done in base 10. You should find the result as an irreducible fraction, written in base 10.




Input


Input contains one integer number *A* (3 ≤ *A* ≤ 1000).




Output


Output should contain required average value in format «X/Y», where X is
 the numerator and Y is the denominator.




Sample test(s)




input
5




output
7/3




input
3




output
2/1






Note


In the first sample number 5 written in all bases from 2 to 4 looks so: 101, 12, 11. Sums of digits are 2, 3 and 2, respectively.










题意是说，把一个数字A，变成二进制、三进制……一直到A-1进制，的这么多数的“数字和”的平均数，就是说所有的各位数之和除以(a-2).//因为二进制到A-1进制少了1和本身，所以是减二。

这道题的话，表达成最简分数看起来挺吓人的，实际上，分母分子同时处以最大公约数即可（最大公约数就是像这样的辗转相除法）。

C++：



```cpp
#include <cstdio>
#include <iostream>
#include <algorithm>
using namespace std;

int gcd(int a, int b)
{
	if(a%b==0)return b;
	return gcd(b,a%b);
}

int val[11];//1024 in base 2

int main()
{
	int a,ans=0;
	scanf("%d",&a);
	for(int i=2;i<a;i++)
	{
		int t=a;//t=temp for a
		int pos=0;
		while(t)
    	{
        	int mod=t%i;
        	ans+=mod;
        	t=(t-mod)/i;
        	if(!t) break;
    	}
		//while(pos>=0)ans+=val[pos--];
	}
	int Valgcd=gcd(ans,a-2);
	printf("%d/%d",ans/Valgcd,(a-2)/Valgcd);
	return 0;
}
```













