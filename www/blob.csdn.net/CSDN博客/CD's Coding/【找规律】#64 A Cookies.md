# 【找规律】#64 A. Cookies - CD's Coding - CSDN博客





2014年10月10日 10:47:55[糖果天王](https://me.csdn.net/okcd00)阅读数：565
所属专栏：[CF-A Terminator](https://blog.csdn.net/column/details/cf-amaster.html)


















Fangy collects cookies. Once he decided to take a box and put cookies into it in some way. If we take a square *k* × *k* in size, divided into blocks 1 × 1 in
 size and paint there the main diagonal together with cells, which lie above it, then the painted area will be equal to the area occupied by one cookie *k* in
 size. Fangy also has a box with a square base 2*n* × 2*n*,
 divided into blocks 1 × 1in size. In a box the cookies should not overlap, and they should not be turned over or rotated. See cookies of sizes 2 and 4respectively
 on the figure:
![](http://espresso.codeforces.com/70aa0ac6c7fc89720c6c9afecd519aea52c62a1c.png)

To stack the cookies the little walrus uses the following algorithm. He takes out of the repository the largest cookie which can fit in some place in the box and puts it there. Everything could be perfect but alas, in the repository the little walrus has infinitely
 many cookies of size 2 and larger, and there are no cookies of size 1,
 therefore, empty cells will remain in the box. Fangy wants to know how many empty cells will be left in the end.




Input


The first line contains a single integer *n* (0 ≤ *n* ≤ 1000).




Output


Print the single number, equal to the number of empty cells in the box. The answer should be printed modulo 106 + 3.




Sample test(s)




input
3




output
9





Note


If the box possesses the base of 23 × 23 (as
 in the example), then the cookies will be put there in the following manner:
![](http://espresso.codeforces.com/f753f6b8573ff0efacc4aadca43dd2668f20bc20.png)












细心的大家应该可以发现，这是个从2开始公比为3的等比数列。

看不出来的话注意看图，把每张图分成四个象限，每一个n都是n+1的图的第三象限的部分~

既然知道了规律，取模的时候注意一下 a%b要写成 (a+b)%b 的形式即可~



```cpp
#include <cstdio>
#include <string>
#include <cstring> 
#include <iostream>
using namespace std;

int main()
{
	int n,ans=1;	cin>>n;
	const int mod=(int)1e6+3;
	for(int i=2;i<=n;i++) 
	{
		ans*=3;
		ans=(ans+mod)%mod;
	}
	cout<<ans; 
	return 0;
}
```











