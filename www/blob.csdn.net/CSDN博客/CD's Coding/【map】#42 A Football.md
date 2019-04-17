# 【map】#42 A. Football - CD's Coding - CSDN博客





2014年07月28日 18:27:57[糖果天王](https://me.csdn.net/okcd00)阅读数：772
所属专栏：[CF-A Terminator](https://blog.csdn.net/column/details/cf-amaster.html)


















One day Vasya decided to have a look at the results of Berland 1910 Football Championship’s finals. Unfortunately he didn't find the overall score of the match; however, he got hold of a profound description of the match's process. On the whole there are *n* lines
 in that description each of which described one goal. Every goal was marked with the name of the team that had scored it. Help Vasya, learn the name of the team that won the finals. It is guaranteed that the match did not end in a tie.




Input


The first line contains an integer *n* (1 ≤ *n* ≤ 100)
 — the number of lines in the description. Then follow *n*lines — for each goal the names of the teams that scored it. The names are non-empty lines consisting
 of uppercase Latin letters whose lengths do not exceed 10 symbols. It is guaranteed that the match did not end in a tie and the description contains no more than two different teams.




Output


Print the name of the winning team. We remind you that in football the team that scores more goals is considered the winner.




Sample test(s)




input
1
ABC




output
ABC




input
5
A
ABA
ABA
A
A




output
A















又是一个string，int类型的问题呢，这次我们用map玩~~~

map可好用了呢~~~ 用string来作为下标存储他们，找起来方便，存起来省心^_^



```cpp
#include <map> 
#include <queue>
#include <cmath> 
#include <memory> 
#include <cctype>
#include <cstdio>
#include <string>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <algorithm>
using namespace std;
map<string,int> msi;

int main()
{
	int n;	cin>>n;
	msi.clear();
	for(int i=0;i<n;i++)
	{
		string s;	cin>>s;
		msi[s]++;
	}
	int max=0;
	string ans;
	for(map<string,int>::iterator it=msi.begin();it!=msi.end();++it)
	{
		int num = it->second;
		if(max<num)
		{
			max=num;
			ans=it->first;
		}
	}
	cout<<ans<<endl;
	return 0;
}
```





