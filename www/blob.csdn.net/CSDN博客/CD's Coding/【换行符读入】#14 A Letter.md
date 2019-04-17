# 【换行符读入】#14 A. Letter - CD's Coding - CSDN博客





2014年04月26日 16:01:58[糖果天王](https://me.csdn.net/okcd00)阅读数：495
所属专栏：[CF-A Terminator](https://blog.csdn.net/column/details/cf-amaster.html)


















A boy Bob likes to draw. Not long ago he bought a rectangular graph (checked) sheet with *n* rows and *m* columns.
 Bob shaded some of the squares on the sheet. Having seen his masterpiece, he decided to share it with his elder brother, who lives in Flatland. Now Bob has to send his picture by post, but because of the world economic crisis and high oil prices, he wants
 to send his creation, but to spend as little money as possible. For each sent square of paper (no matter whether it is shaded or not) Bob has to pay 3.14 burles. Please, help Bob cut out of his masterpiece a rectangle of the minimum cost, that will contain
 all the shaded squares. The rectangle's sides should be parallel to the sheet's sides.




Input


The first line of the input data contains numbers *n* and *m* (1 ≤ *n*, *m* ≤ 50), *n* —
 amount of lines, and *m* — amount of columns on Bob's sheet. The following *n* lines
 contain *m* characters each. Character «.» stands for a
 non-shaded square on the sheet, and «*» — for a shaded square. It is guaranteed that Bob has shaded at least one square.




Output


Output the required rectangle of the minimum cost. Study the output data in the sample tests to understand the output format better.




Sample test(s)




input
6 7
.......
..***..
..*....
..***..
..*....
..***..




output
***
*..
***
*..
***




input
3 3
***
*.*
***




output
***
*.*
***












这道题的题意是个凸包，就是说用一个特定形状的东西往小了收，收到不能再小了就结束了。这道题因为是收成矩形，所以直接记住四个边的位置就好了。



以及，换行符也是个char类型会被%c类型输入一定要注意~ 

每行末尾都有个会被读入的回车你们造嘛？

连n、m之后都有个烦人的回车你们造嘛？

C++：



```cpp
#include<cstdio>
#include<algorithm>
using namespace std;

int main()
{
	int n,m;
	scanf("%d%d",&n,&m);
	int left=m-1,up=n-1,right=0,down=0;
	char graph[n][m+1];//+1 for endl;
	char end_l;
	scanf("%c",&end_l);
	for(int i=0;i<n;)
	{
		for(int j=0;j<=m;)//j=m for endl;
		{
			scanf("%c",&graph[i][j]);
			if(graph[i][j]=='*')
			{
				if(i<up)	up=i;
				if(i>down)	down=i;
				if(j<left)	left=j;
				if(j>right)	right=j; 
			}
			j++;
		}
		//scanf("%c",&end_l);//read 'endl'
		i++;
	}

	for(int i=up;i<=down;i++)
	{
		for(int j=left;j<=right;j++)
		printf("%c",graph[i][j]);
		printf("\n");
	}
	
	return 0;
}
```





