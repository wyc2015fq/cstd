# 【水模拟】#77 A. Football - CD's Coding - CSDN博客





2014年11月15日 23:40:19[糖果天王](https://me.csdn.net/okcd00)阅读数：1503
所属专栏：[CF-A Terminator](https://blog.csdn.net/column/details/cf-amaster.html)


















Petya loves football very much. One day, as he was watching a football match, he was writing the players' current positions on a piece of paper. To simplify the situation he depicted it as a string consisting of zeroes and ones. A zero corresponds to players
 of one team; a one corresponds to players of another team. If there are at least 7players of some team standing one after another, then the situation
 is considered dangerous. For example, the situation 00100110111111101 is dangerous and 11110111011101 is
 not. You are given the current situation. Determine whether it is dangerous or not.




Input


The first input line contains a non-empty string consisting of characters "0" and "1", which represents players. The length of the string does not exceed 100 characters.
 There's at least one player from each team present on the field.




Output


Print "YES" if the situation is dangerous. Otherwise, print "NO".




Sample test(s)




input
001001




output
NO




input
1000000001




output
YES















题意是问是否存在7个或以上相同的数字相连

读入从左到右依次遍历计数即可

Code：



```cpp
#include <cstdio>
#include <string>
#include <cstring> 
#include <iostream>
#include <algorithm>
using namespace std;
typedef long long ll;
// http://codeforces.com/contest/96
// Football
int main()
{
	string s;	cin>>s;
	int cnt=1;
	for(int i=1;i<s.length();i++)
	{
		if(s[i]==s[i-1]) cnt++;
		else cnt=1;
		if(cnt==7) {cout<<"YES";return 0;} 
	} 
	cout<<"NO";
	return 0;
}
```










