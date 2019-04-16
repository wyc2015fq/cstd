# POJ 1573 BFS&DFS - 紫芝的博客 - CSDN博客





2018年04月06日 18:23:08[紫芝](https://me.csdn.net/qq_40507857)阅读数：45









Robot Motion

|**Time Limit:** 1000MS||**Memory Limit:** 10000K|
|----|----|----|
|**Total Submissions:** 15275||**Accepted:** 7269|
Description

![](http://poj.org/images/1573_1.jpg)
A robot has been programmed to follow the instructions in its path. Instructions for the next direction the robot is to move are laid down in a grid. The possible instructions are 

N north (up the page) 
S south (down the page) 
E east (to the right on the page) 
W west (to the left on the page) 

For example, suppose the robot starts on the north (top) side of Grid 1 and starts south (down). The path the robot follows is shown. The robot goes through 10 instructions in the grid before leaving the grid. 

Compare what happens in Grid 2: the robot goes through 3 instructions only once, and then starts a loop through 8 instructions, and never exits. 

You are to write a program that determines how long it takes a robot to get out of the grid or how the robot loops around. 


Input

There will be one or more grids for robots to navigate. The data for each is in the following form. On the first line are three integers separated by blanks: the number of rows in the grid, the number of columns in the grid, and the number of the column in which the robot enters from the north. The possible entry columns are numbered starting with one at the left. Then come the rows of the direction instructions. Each grid will have at least one and at most 10 rows and columns of instructions. The lines of instructions contain only the characters N, S, E, or W with no blanks. The end of input is indicated by a row containing 0 0 0.

Output

For each grid in the input there is one line of output. Either the robot follows a certain number of instructions and exits the grid on any one the four sides or else the robot follows the instructions on a certain number of locations once, and then the instructions on some number of locations repeatedly. The sample input below corresponds to the two grids above and illustrates the two forms of output. The word "step" is always immediately followed by "(s)" whether or not the number before it is 1.

Sample Input
3 6 5
NEESWE
WWWESS
SNWWWW
4 5 1
SESWE
EESNW
NWEEN
EWSEN
0 0 0
Sample Output
10 step(s) to exit
3 step(s) before a loop of 8 step(s)





题意：

有一个N*M的区域，机器人从第一行的第几列进入，该区域全部由'N' , 'S' , 'W' , 'E' ，走到某个区域的时候只能按照该区域指定的方向进行下一步，问你机器人能否走出该片区域，若不能，输入开始绕圈的步数和圈的大小。

# BFS

```cpp
//POJ 1573
//#include<bits/stdc++.h>
#include<iostream>
#include<queue>
#include<cstdio>
#include<cstring>
using namespace std;
struct node{
	int x,y;
	//node(int x=0,int y=0):x(x),y(y){};
};
int n,m,start;
int step[15][15];
char s[12][12];
bool vis[12][12];

void bfs(){
	node begin;
	begin.x=0;
	begin.y=start;

	step[0][start]=0;
	queue<node> q;
	q.push(begin);
	int stepp=-1;
	while(!q.empty()){
		node next=q.front();
		q.pop();	stepp++;
		int a=next.x,b=next.y;
		if(a<0||a>=n||b<0||b>=m)
		{
			printf("%d step(s) to exit\n",stepp);
			return;
		}
		if(vis[a][b]) 
		{
			printf("%d step(s) before a loop of %d step(s)\n",step[a][b],stepp-step[a][b]);
			return;
		}	
		vis[a][b]=true;
		step[a][b]=stepp;
		
		node temp=next;
		if(s[a][b]=='N')
		{
			temp.x--;
			q.push(temp);
		}
		if(s[a][b]=='S')
		{
			temp.x++;
			q.push(temp);
		}
		if(s[a][b]=='W')
		{
			temp.y--;
			q.push(temp);
		}
		if(s[a][b]=='E')
		{
			temp.y++;
			q.push(temp);
		}
	}
}

int main()
{
	while(cin>>n>>m>>start&&n&&m&&start){
		start--;
		memset(vis,0,sizeof(vis));
		for(int i=0;i<n;i++)
		{
			scanf("%s",s[i]);
		}
		bfs();
	}
	return 0;
 }
```


DFS






```cpp
//POJ 1573
//#include<bits/stdc++.h>
#include<iostream>
#include<queue>
#include<cstdio>
#include<cstring>
using namespace std;
int n,m,start;
int step[15][15],stepp;
char s[12][12];
bool vis[12][12];

void dfs(int a,int b){
		stepp++;
		if(a<0||a>=n||b<0||b>=m)
		{
			printf("%d step(s) to exit\n",stepp);
			return;
		}
		if(vis[a][b]) 
		{
			printf("%d step(s) before a loop of %d step(s)\n",step[a][b],stepp-step[a][b]);
			return;
		}	
		vis[a][b]=true;
		step[a][b]=stepp;
		int x=a,y=b;
		if(s[a][b]=='N')
		{
			x--;	
		}
		if(s[a][b]=='S')
		{
			x++;
		}
		if(s[a][b]=='W')
		{
			y--;
		}
		if(s[a][b]=='E')
		{
			y++;
		}
		dfs(x,y);
	}

int main()
{
	while(cin>>n>>m>>start&&n&&m&&start){
		start--;stepp=-1;
		memset(vis,0,sizeof(vis));
		for(int i=0;i<n;i++)
		{
			scanf("%s",s[i]);
		}
		dfs(0,start);
	}
	return 0;
 }
```




