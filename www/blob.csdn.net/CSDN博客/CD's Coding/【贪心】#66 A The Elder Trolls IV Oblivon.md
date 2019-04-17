# 【贪心】#66 A. The Elder Trolls IV: Oblivon - CD's Coding - CSDN博客





2014年10月10日 17:16:46[糖果天王](https://me.csdn.net/okcd00)阅读数：642标签：[Codeforces																[Beta Round #66](https://so.csdn.net/so/search/s.do?q=Beta Round #66&t=blog)](https://so.csdn.net/so/search/s.do?q=Codeforces&t=blog)
个人分类：[CF-A题终结者](https://blog.csdn.net/okcd00/article/category/2156945)

所属专栏：[CF-A Terminator](https://blog.csdn.net/column/details/cf-amaster.html)

















Vasya plays The Elder Trolls IV: Oblivon. Oh, those creators of computer games! What they do not come up with! Absolutely unique monsters have been added to the The Elder Trolls IV: Oblivon. One of these monsters is Unkillable Slug. Why it is "Unkillable"?
 Firstly, because it can be killed with cutting weapon only, so lovers of two-handed amber hammers should find suitable knife themselves. Secondly, it is necessary to make so many cutting strokes to Unkillable Slug. Extremely many. Too many!


Vasya has already promoted his character to 80-th level and in order to gain level 81 he was asked to kill Unkillable Slug. The monster has a very interesting shape. It looks like a rectangular parallelepiped with size *x* × *y* × *z*,
 consisting of undestructable cells 1 × 1 × 1. At one stroke Vasya can cut the Slug along an imaginary grid, i.e. cut with a plane parallel to one of the parallelepiped
 side. Monster dies when amount of parts it is divided reaches some critical value.


All parts of monster do not fall after each cut, they remains exactly on its places. I. e. Vasya can cut several parts with one cut.


Vasya wants to know what the maximum number of pieces he can cut the Unkillable Slug into striking him at most *k* times.


Vasya's character uses absolutely thin sword with infinite length.




Input


The first line of input contains four integer numbers *x*, *y*, *z*, *k* (1 ≤ *x*, *y*, *z* ≤ 106, 0 ≤ *k* ≤ 109).




Output


Output the only number — the answer for the problem.


Please, do not use %lld specificator to read or write 64-bit integers in C++. It is preffered to use cout (also
 you may use%I64d).




Sample test(s)




input
2 2 2 3




output
8



input
2 2 2 1




output
2





Note


In the first sample Vasya make 3 pairwise perpendicular cuts. He cuts monster on two parts with the first cut, then he divides each part on two with the second cut, and finally he divides each of the 4 parts on two.












有这么一个大方块，是一个X x Y x Z的方块，由这么多个1 x 1 x 1的方块组成，你可以切k刀，问最多可以切成多少块。

我们知道平方差公式 (x+y)(x-y)=x*x-y*y ，即两个数的和一定，差越小他们俩的乘积越大。同理我们可以知道三个数字也是这样。

所以我们要尽可能的让长宽高被切的刀数尽可能相近。 可以的话最好三个都相同，无法被3整除的时候余数也应该尽可能的平分。

为了分隔的尽量最优，我们应该从边长最小的开始切，看是否满足k/3，不够的话切满剩下的分给另外两边，以此类推。

代码也比较简单易懂，看代码也会比较容易帮助理解一些



```cpp
#include <cstdio>
#include <string>
#include <cstring> 
#include <iostream>
#include <algorithm>
using namespace std;
typedef long long ll;

ll c[3];
int main()
{
	ll x,y,z,k;
	cin>>x>>y>>z>>k;
	c[0]=x-1,c[1]=y-1,c[2]=z-1;
	sort(c,c+3);
	if(c[0]>=k/3) c[0]=k/3,c[1]=(k-c[0])/2,c[2]=min(c[2],k-c[0]-c[1]);
	else
	{
		if(c[1]>=(k-c[0])/2) c[1]=(k-c[0])/2,c[2]=min(c[2],k-c[0]-c[1]);
		else c[2]=min(c[2],k-c[1]-c[0]);
	}
	//cout<<(c[0]+1)<<" "<<(c[1]+1)<<" "<<(c[2]+1)<<endl;
	cout<<(c[0]+1)*(c[1]+1)*(c[2]+1);
	return 0;
}
```









