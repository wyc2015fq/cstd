# 【水】#16 A. Flag - CD's Coding - CSDN博客





2014年05月07日 16:02:15[糖果天王](https://me.csdn.net/okcd00)阅读数：560
所属专栏：[CF-A Terminator](https://blog.csdn.net/column/details/cf-amaster.html)


















According to a new ISO standard, a flag of every country should have a chequered field *n* × *m*, each square should be of one of 10 colours, and the
 flag should be «striped»: each horizontal row of the flag should contain squares of the same colour, and the colours of adjacent horizontal rows should be different. Berland's government asked you to find out whether their flag meets the new ISO standard.




Input


The first line of the input contains numbers *n* and *m* (1 ≤ *n*, *m* ≤ 100), *n* —
 the amount of rows, *m* — the amount of columns on the flag of Berland. Then there follows the description of the flag: each of the following *n* lines
 contain *m* characters. Each character is a digit between 0 and 9,
 and stands for the colour of the corresponding square.




Output


Output YES, if the flag meets the new ISO standard, and NO otherwise.




Sample test(s)




input
3 3
000
111
222




output
YES




input
3 3
000
000
111




output
NO




input
3 3
000
111
002




output
NO














就是说，每一行必须一样，相邻两行必须不一样，判断是不是这样的Yes/No

因为没有发现难点所以就不多说了~



```cpp
#include<algorithm>
#include<iostream>
#include<cstdio>
#include<cstdlib>
using namespace std;
	int n,m,tmp;
	char huiche;
	char clr[100][100];

bool solve()
{
	
	for(int i=0;i<n;i++)
	{
		scanf("%c",&huiche);
		//scanf("%c",&clr[i][0]);
		for(int j=0;j<m;j++)
		{
			scanf("%c",&clr[i][j]);
			if(j!=0) if(clr[i][j]!=clr[i][0])return false;
		}
		if(i!=0) if(clr[i][0]==clr[i-1][0]) return false;
	}	
	return true;
}


int main()
{
	scanf("%d%d",&n,&m);
	printf(solve()==true?"YES":"NO");
	return 0;
}
```








