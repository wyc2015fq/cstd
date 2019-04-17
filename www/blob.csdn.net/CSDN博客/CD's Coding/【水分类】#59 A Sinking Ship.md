# 【水分类】#59 A. Sinking Ship - CD's Coding - CSDN博客





2014年08月20日 11:58:27[糖果天王](https://me.csdn.net/okcd00)阅读数：634标签：[codeforces](https://so.csdn.net/so/search/s.do?q=codeforces&t=blog)
个人分类：[CF-A题终结者](https://blog.csdn.net/okcd00/article/category/2156945)

所属专栏：[CF-A Terminator](https://blog.csdn.net/column/details/cf-amaster.html)


















The ship crashed into a reef and is sinking. Now the entire crew must be evacuated. All *n* crew members have already lined up in a row (for convenience let's
 label them all from left to right with positive integers from 1 to *n*) and await further instructions. However, one should evacuate the crew properly, in
 a strict order. Specifically:


The first crew members to leave the ship are rats. Then women and children (both groups have the same priority) leave the ship. After that all men are evacuated from the ship. The captain leaves the sinking ship last.


If we cannot determine exactly who should leave the ship first for any two members of the crew by the rules from the previous paragraph, then the one who stands to the left in the line leaves the ship first (or in other words, the one whose number in the line
 is less).


For each crew member we know his status as a crew member, and also his name. All crew members have different names. Determine the order in which to evacuate the crew.




Input


The first line contains an integer *n*, which is the number of people in the crew (1 ≤ *n* ≤ 100).
 Then follow *n* lines. The *i*-th of those lines contains
 two words — the name of the crew member who is *i*-th in line, and his status on the ship. The words are separated by exactly one space. There are no other
 spaces in the line. The names consist of Latin letters, the first letter is uppercase, the rest are lowercase. The length of any name is from 1 to 10 characters. The status can have the following values: rat for
 a rat, woman for a woman, child for a child, man for
 a man, captain for the captain. The crew contains exactly one captain.




Output


Print *n* lines. The *i*-th of them should contain the
 name of the crew member who must be the *i*-th one to leave the ship.




Sample test(s)




input
6
Jack captain
Alice woman
Charlie man
Teddy rat
Bob child
Julia woman




output
Teddy
Alice
Bob
Julia
Charlie
Jack















船要沉了，船上有n个人，要求他们按照rat<women&child<man<captain的顺序下船，所以我们要做的是把每个人按照身份拍成4队，然后按照这四队的顺序下船

我在这里用的是queue，实际上，数组，vector，map，list都可以，选用最熟悉的存储结构就好

为了标明这些都代表什么，用同样可以运行的另一种写法的代码注释在一边了^_^



```cpp
#include <queue>
#include <cstdio>
#include <memory>
#include <string>
#include <cstring>
#include <iostream>
#include <algorithm>
using namespace std;
//queue<string> rat,w_c,man,cap;
queue<string> q[4];

int main()
{
	int n;	cin>>n;
	string name, r;
	for(int ni=0;ni<n;ni++)
	{
		cin>>name>>r;
		if(r=="rat")	q[0].push(name);//rat.push(name);
		else if(r=="man")	q[2].push(name);//man.push(name);
		else if(r=="captain")	q[3].push(name);//cap.push(name);
		else if(r=="woman" || r=="child")	q[1].push(name);//w_c.push(name);
	} 
	for(int i=0;i<4;i++)
	{
		while(!q[i].empty())
		{
			string s=q[i].front();
			cout<<s<<endl;
			q[i].pop();
		}
	} 
	/* 
	while(!rat.empty())	
	{
		string s=rat.front();	rat.pop(); 
		cout<<s<<endl;	
	}
	while(!w_c.empty())	
	{
		string s=w_c.front();	w_c.pop(); 
		cout<<s<<endl;	
	}
	while(!man.empty())	
	{
		string s=man.front();	man.pop(); 
		cout<<s<<endl;	
	}
	while(!cap.empty())	
	{
		string s=cap.front();	cap.pop(); 
		cout<<s<<endl;	
	}
	*/ 
	return 0;
}
```








