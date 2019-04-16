# POJ 2251 Dungeon Master（三维BFS求最短路径） - 紫芝的博客 - CSDN博客





2018年04月22日 22:01:12[紫芝](https://me.csdn.net/qq_40507857)阅读数：54








## 3D dungeon

时间限制: 1 Sec  内存限制: 128 MB

提交: 2  解决: 2

[[提交](http://101.200.61.174/JudgeOnline/submitpage.php?cid=1004&pid=11&langmask=0)][[状态](http://101.200.61.174/JudgeOnline/problemstatus.php?id=1748)][[讨论版](http://101.200.61.174/JudgeOnline/bbs.php?pid=1748&cid=1004)][命题人:[201506020829](http://101.200.61.174/JudgeOnline/userinfo.php?user=201506020829)][[Edit](http://101.200.61.174/JudgeOnline/admin/problem_edit.php?id=1748&getkey=76871FA9F7)] [TestData]

## 题目描述

You are trapped in a 3D dungeon and need to find the quickest way out! The dungeon is composed of unit cubes which may or may not be filled with rock. It takes one minute to move one unit north south east west up or down. You cannot move diagonally and the maze is surrounded by solid rock on all sides. 


Is an escape possible? If yes how long will it take? 

## 输入

The input consists of a number of dungeons. Each dungeon description starts with a line containing three integers L R and C (all limited to 30 in size). 

L is the number of levels making up the dungeon. 

R and C are the number of rows and columns making up the plan of each level. 

Then there will follow L blocks of R lines each containing C characters. Each character describes one cell of the dungeon. A cell full of rock is indicated by a '#' and empty cells are represented by a '.'. Your starting position is indicated by 'S' and the exit by the letter 'E'. There's a single blank line after each level. Input is terminated by three zeroes for L R and C.

## 输出

Each maze generates one line of output. If it is possible to reach the exit print a line of the form 

Escaped in x minute(s).


where x is replaced by the shortest time it takes to escape. 

If it is not possible to escape print the line 

Trapped!

## 样例输入

3 4 5
S....
.###.
.##..
###.#

#####
#####
##.##
##...

#####
#####
#.###
####E

1 3 3
S##
#E#
###

0 0 0
## 样例输出

Escaped in 11 minute(s).
Trapped!
二维矩阵BFS求最短路径模板

[点击打开链接](https://blog.csdn.net/qq_40507857/article/details/79822538)

三维问题注意广搜的方向，以及三维矩阵的表示

```cpp
//POJ 2251 Dungeon Master（三维简单广搜）
//#include<bits/stdc++.h>
#include<iostream>
#include<cstdio>
#include<queue>
#include<cstring>
using namespace std;
int l,r,c;
int sx,sy,sz;
int ans;
char s[35][35][35];
bool vis[35][35][35];
struct node{
	int x,y,z,step;
};
bool check(int x,int y,int z)
{
	if(x<0||x>=r||y<0||y>=c||z<0||z>=l||s[z][x][y]=='#'||vis[z][x][y])
	return 1;
	return 0;
}
int dic[6][3]={{0,0,1},{0,1,0},{1,0,0},{-1,0,0},{0,-1,0},{0,0,-1}};
void bfs(){
	queue<node> q;
	node start;
	start.step=0;
	start.x=sx;start.y=sy;start.z=sz;
	vis[sz][sx][sy]=1;
	q.push(start);
	while(!q.empty()){
		node now=q.front();
		q.pop();
		int x=now.x,y=now.y,z=now.z,step=now.step;
		vis[z][x][y]=1;
		if(s[z][x][y]=='E')
		{
			ans=step;
			return;
		}
		for(int i=0;i<6;i++)
		{
			node next=now;
			next.x+=dic[i][0];
			next.y+=dic[i][1];
			next.z+=dic[i][2];
			if(check(next.x,next.y,next.z))
			continue;
			next.step++;
			vis[next.z][next.x][next.y]=1;
			q.push(next);
		}
	}
	ans=-1;
}
int main()
{
	while(cin>>l>>r>>c&&l&&r&&c)
	{
		for(int i=0;i<l;i++)
		{
			for(int j=0;j<r;j++)
			{
				scanf("%s",s[i][j]);
				for(int k=0;k<c;k++)
				{
					if(s[i][j][k]=='S')
					{
						sz=i;sx=j;sy=k;
					}
				}
			}
		}
		memset(vis,0,sizeof(vis));
		ans=0;
		bfs();
		if(ans==-1)
		cout<<"Trapped!\n";
		else 
		printf("Escaped in %d minute(s).\n",ans);
	}
	return 0;
}
```





