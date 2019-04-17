# 【string】#84 A. Nearly Lucky Number - CD's Coding - CSDN博客





2014年11月29日 21:24:33[糖果天王](https://me.csdn.net/okcd00)阅读数：1200
所属专栏：[CF-A Terminator](https://blog.csdn.net/column/details/cf-amaster.html)

















Petya loves lucky numbers. We all know that lucky numbers are the positive integers whose decimal representations contain only the lucky digits 4 and 7.
 For example, numbers 47, 744, 4 are lucky and 5, 17, 467 are
 not.


Unfortunately, not all numbers are lucky. Petya calls a number nearly lucky if the number of lucky digits in it is a lucky number. He wonders whether number *n* is
 a nearly lucky number.




Input


The only line contains an integer *n* (1 ≤ *n* ≤ 1018).


Please do not use the %lld specificator to read or write 64-bit numbers in С++. It is preferred to use the cin, cout streams or the %I64d specificator.




Output


Print on the single line "YES" if *n* is a nearly lucky
 number. Otherwise, print "NO" (without the quotes).




Sample test(s)




input
40047




output
NO




input
7747774




output
YES




input
1000000000000000000




output
NO






Note


In the first sample there are 3 lucky digits (first one and last two), so the answer is "NO".


In the second sample there are 7 lucky digits, 7 is lucky number, so the answer is "YES".


In the third sample there are no lucky digits, so the answer is "NO".










字符串的妙用~



虽说题目给了1e18的范围摆明了想让我们用longlong，但是这样的一次一次 /=10 还是很烦躁呀，直接用string，每一位不就是当前位数数值了嘛（其实也要减去'0'）

题目意思是说，一个所有数位都是4或者7的数字称为幸运数，一个数“包含4或7的个数”为幸运数的称为接近幸运数，问给出的数是不是“接近幸运数”

模拟咯~

数出4、7的个数，因为就18位，所以只能是4个或者7个，都不用再判断一遍的。

Code：



```cpp
#include <cstdio>
#include <string>
#include <cstring> 
#include <iostream>
#include <algorithm>
using namespace std;
typedef long long ll;
// http://codeforces.com/contest/110
// Nearly Lucky Number
int main()
{
	int cnt=0;
	string s;	cin>>s; 
	for(int i=0;i<s.length();i++)
		if(s[i]=='4'||s[i]=='7')cnt++;
	if(cnt==4 || cnt==7)cout<<"YES";
	else cout<<"NO";
	return 0;
}
```














