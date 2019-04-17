# 【题意string】#78 A. Help Far Away Kingdom - CD's Coding - CSDN博客





2014年11月28日 17:38:35[糖果天王](https://me.csdn.net/okcd00)阅读数：679
所属专栏：[CF-A Terminator](https://blog.csdn.net/column/details/cf-amaster.html)


















In a far away kingdom lived the King, the Prince, the Shoemaker, the Dressmaker and many other citizens. They lived happily until great trouble came into the Kingdom. The ACMers settled there.


Most damage those strange creatures inflicted upon the kingdom was that they loved high precision numbers. As a result, the Kingdom healers had already had three appointments with the merchants who were asked to sell, say, exactly 0.273549107 beer
 barrels. To deal with the problem somehow, the King issued an order obliging rounding up all numbers to the closest integer to simplify calculations. Specifically, the order went like this:
- 
If a number's integer part does not end with digit 9 and its fractional part is strictly less than 0.5,
 then the rounded up number coincides with the number’s integer part.
- 
If a number's integer part does not end with digit 9 and its fractional part is not less than 0.5,
 the rounded up number is obtained if we add 1 to the last digit of the number’s integer part.
- 
If the number’s integer part ends with digit 9, to round up the numbers one should go to Vasilisa the Wise. In the whole Kingdom she is the only one who can perform
 the tricky operation of carrying into the next position.


Merchants found the algorithm very sophisticated and they asked you (the ACMers) to help them. Can you write a program that would perform the rounding according to the King’s order?




Input


The first line contains a single number to round up — the integer part (a non-empty set of decimal digits that do not start with 0 — with the exception of a case
 when the set consists of a single digit — in this case 0 can go first), then follows character «.» (a dot), and then follows the fractional part (any non-empty
 set of decimal digits). The number's length does not exceed 1000 characters, including the dot. There are no other characters in the input data.




Output


If the last number of the integer part is not equal to 9, print the rounded-up number without leading zeroes. Otherwise, print the message "GOTO
 Vasilisa." (without the quotes).




Sample test(s)




input
0.0




output
0



input
1.49




output
1



input
1.50




output
2



input
2.71828182845904523536




output
3



input
3.14159265358979323846




output
3



input
12345678901234567890.1




output
12345678901234567890



input
123456789123456789.999




output
GOTO Vasilisa.














这是一个把浮点数四舍五入成整数的程序，但是如果需要进位的同时整数部分的末位是0，则输出 GOTO Vasilisa。

所以相当于一个翻译题，翻译题意即可……

使用string来存储可以省下不少麻烦。

解题：找到点在哪里，然后点的后一位和前一位决定了全局，后一位决定进不进，进位的话前一位决定输出还是GOTO

Code：



```cpp
#include <cstdio>
#include <string>
#include <cstring> 
#include <iostream>
#include <algorithm>
using namespace std;
typedef long long ll;
// http://codeforces.com/contest/99
// Help Far Away Kingdom
int main()
{
	string s;	cin>>s;
	string ans="";
	int flag=0;
	for(int i=0;i<s.length();i++)
	{
		if(s[i]!='.' && !flag) ans+=s[i];
		else if(s[i]=='.' && !flag) flag=1;
		else 
		{
			int lst=ans[i-2]-'0';
			int num=s[i]-'0';
			if(num<5) break;
			else if(lst!=9) ans[i-2]=ans[i-2]+1;
			else 
			{
				cout<<"GOTO Vasilisa.";
				return 0;	
			}
			break;
		}
	}
	cout<<ans;
	return 0;
}
```








