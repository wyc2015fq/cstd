# 【水状态表示】#48 A. Cheaterius's Problem - CD's Coding - CSDN博客





2014年07月30日 16:00:09[糖果天王](https://me.csdn.net/okcd00)阅读数：459标签：[codeforces](https://so.csdn.net/so/search/s.do?q=codeforces&t=blog)
个人分类：[CF-A题终结者](https://blog.csdn.net/okcd00/article/category/2156945)

所属专栏：[CF-A Terminator](https://blog.csdn.net/column/details/cf-amaster.html)


















Cheaterius is a famous in all the Berland astrologist, magician and wizard, and he also is a liar and a cheater. One of his latest inventions is Cheaterius' amulets! They bring luck and wealth, but are rather expensive. Cheaterius makes them himself. The technology
 of their making is kept secret. But we know that throughout long nights Cheaterius glues together domino pairs with super glue to get squares 2 × 2 which are the
 Cheaterius' magic amulets!
![](http://espresso.codeforces.com/32c49b4a35af64883bc299e4a1ae0f44ea4d63c8.png)That's what one of Cheaterius's
 amulets looks like

After a hard night Cheaterius made *n* amulets. Everyone of them represents a square 2 × 2,
 every quarter contains 1 to 6 dots. Now he wants sort them into piles, every pile must contain similar amulets. Two amulets are called similar if they can be rotated by 90, 180 or 270 degrees so that the following condition is met: the numbers of dots in the
 corresponding quarters should be the same. It is forbidden to turn over the amulets.


Write a program that by the given amulets will find the number of piles on Cheaterius' desk.




Input


The first line contains an integer *n* (1 ≤ *n* ≤ 1000),
 where *n* is the number of amulets. Then the amulet's descriptions are contained. Every description occupies two lines and contains two numbers (from 1 to
 6) in each line. Between every pair of amulets the line "**" is located.




Output


Print the required number of piles.




Sample test(s)




input
4
31
23
**
31
23
**
13
32
**
32
13




output
1




input
4
51
26
**
54
35
**
25
61
**
45
53




output
2















有一种2X2的牌，四个块上都有写数字，他不能翻只能旋转，90、180、270度，所以一种牌可以显示出共4种状态。

给一堆牌，问有几种。

把四个块标号为1234的话，那么一种牌就有1234、2413、4321、3142四种，用一个数组来存当前已经有的种类，然后每读入一个跟已有种类匹配，匹配完都没有找到一致的则设为新的一种。






```cpp
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
#define gm(a,b,c,d) (a*1000+b*100+c*10+d)
#define out {flag=0;break;}
using namespace std;
int mrk[1000];

int main()
{
	int n;	cin>>n;
	int ans=0;
	memset(mrk,0,sizeof mrk);
	for(int ni=0;ni<n;ni++)
	{
		int flag=1;
		int n1,n2;	cin>>n1>>n2;
		int a=n1/10,b=n1%10;
		int c=n2/10,d=n2%10;
		string s; if(ni<n-1)cin>>s; 
		for(int i=0;i<ans;i++)
		{
		<span style="white-space:pre">	</span>if(gm(a,b,c,d)==mrk[i])out	//1234
			else if(gm(b,d,a,c)==mrk[i])out	//2413
			else if(gm(d,c,b,a)==mrk[i])out	//4321
			else if(gm(c,a,d,b)==mrk[i])out //3142
		}
		if(mrk[0]==0 || flag) mrk[ans++]=gm(a,b,c,d);
	}
	cout<<ans<<endl;
	return 0;
}
```








