# Knight Moves（HDU-1372） - Alex_McAvoy的博客 - CSDN博客





2018年03月16日 23:25:06[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：82
个人分类：[HDU																[搜索——广度优先搜索 BFS](https://blog.csdn.net/u011815404/article/category/8115771)](https://blog.csdn.net/u011815404/article/category/7923257)








# Problem Description

    A friend of you is doing research on the Traveling Knight Problem (TKP) where you are to find the shortest closed tour of knight moves that visits each square of a given set of n squares on a chessboard exactly once. He thinks that the most difficult part of the problem is determining the smallest number of knight moves between two given squares and that, once you have accomplished this, finding the tour would be easy.

    Of course you know that it is vice versa. So you offer him to write a program that solves the "difficult" part. 

    Your job is to write a program that takes two squares a and b as input and then determines the number of knight moves on a shortest route from a to b.

# Input

    The input file will contain one or more test cases. Each test case consists of one line containing two squares separated by one space. A square is a string consisting of a letter (a-h) representing the column and a digit (1-8) representing the row on the chessboard. 

# Output

     For each test case, print one line saying "To get from xx to yy takes n knight moves.". 

# Sample Input

**e2 e4    a1 b2    b2 c3    a1 h8    a1 h7    h8 a1    b1 c3    f6 f6**

# Sample Output

**   To get from e2 to e4 takes 2 knight moves.   To get from a1 to b2 takes 4 knight moves.   To get from b2 to c3 takes 2 knight moves.   To get from a1 to h8 takes 6 knight moves.   To get from a1 to h7 takes 5 knight moves.   To get from h8 to a1 takes 6 knight moves.   To get from b1 to c3 takes 1 knight moves.   To get from f6 to f6 takes 0 knight moves.**

————————————————————————————————————————————

思路：

    用一个8*8的二维数组来棋盘，在还没有开始行走时，棋盘上所有的格都置为0，以后，马跳到哪个格，就将马跳跃的步数加1后的值记录在相应的空格里；开始点在行走前设为1，进行bfs搜索即可

    马的跳跃方向：如图，可用一组坐标增量来描述这8个方向

![](https://img-blog.csdn.net/20180317151344882?watermark/2/text/Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

# Source Program

```cpp
#include<cstdio>
#include<cstring>
int map[8][8];
int direction[8][8]={{-2,1},{-2,-1},{2,-1},{2,1},{-1,2},{-1,-2},{1,-2},{1,2}};//方向数组

struct node
{
	int x;
	int y;
}start,end,quene[500];

bool judge(int x0,int y0)//判断坐标是否在规定范围内
{
	if(x0>=0&&x0<8&&y0>=0&&y0<8)
		return true;
	else
		return false;
}

int bfs()
{
	int head,tail;
	int x,y;
	int i;

	map[start.x][start.y]=0;
	head=0,tail=1;//设置队列首尾初值
	quene[1].x=start.x;quene[1].y=start.y;//初始状态存入队列
	while(head<tail)
	{
		head++;//队首加1，出队
		for(i=0;i<8;i++)//依次向八个方向搜索
		{
			x=quene[head].x+direction[i][0];
			y=quene[head].y+direction[i][1];
			
			if(map[x][y]==-1&&judge(x,y))//若子节点符合条件
			{
				map[x][y]=map[quene[head].x][quene[head].y]+1;//步数+1
				if(x==end.x&&y==end.y)	
					return map[end.x][end.y];
				tail++;//队尾+1
				quene[tail].x=x;//入队
				quene[tail].y=y;//入队				
			}
			
		}
	}
	return map[end.x][end.y];
}
int main()
{
	char x1,y1,x2,y2;
	int min;
	

	while(scanf("%c%c %c%c",&x1,&y1,&x2,&y2)!=EOF)//输入房间长与宽
	{
		getchar();
		/*记录起始点与结束点*/
		start.x=x1-'a';
		start.y=y1-'1';
		end.x=x2-'a';
		end.y=y2-'1';
		
		memset(map,-1,sizeof(map));//初始化棋盘
		
		min=bfs();//搜索

		printf("To get from %c%c to %c%c takes %d knight moves.\n",x1,y1,x2,y2,min);
    }
	return 0;
}
```






