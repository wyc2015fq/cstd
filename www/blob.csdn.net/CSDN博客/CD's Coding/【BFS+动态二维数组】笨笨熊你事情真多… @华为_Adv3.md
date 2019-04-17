# 【BFS+动态二维数组】笨笨熊你事情真多… @华为_Adv3 - CD's Coding - CSDN博客





2014年05月07日 20:54:18[糖果天王](https://me.csdn.net/okcd00)阅读数：537标签：[c++																[华为编程大赛																[bfs](https://so.csdn.net/so/search/s.do?q=bfs&t=blog)
个人分类：[Coding Competition](https://blog.csdn.net/okcd00/article/category/2133459)





华为Adv3

![](https://img-blog.csdn.net/20140507205025265?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvb2tjZDAw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)





纯BFS，和友人一起玩华为的比赛…… 被小霸王服务器折磨的生不如死……暂时还不知道这些代码对不对……

但是你们知道么……听盆友说……似乎，那个国庆是个包袱……国庆是7天，每个格子是一天的行程……so……最多走七步！七步！步！！！！！

Nooooooooooooo……




吾辈的代码：



```cpp
//Author: Royalmiki
//Name： Chendian
//Exam: Huawei-Adv3 

#include<cstdio>
#include<iostream>
#include<queue>
#include<stack>
using namespace std;
int StartR,StartC,EndR,EndC;
char huiche;

struct point{
    int x;
    int y;
};
char **Maze;     //Init
point **Pre;    //Save Steps
point move[4]={ {-1,0},{0,-1},{0,1},{1,0} };      //4-Dir

void Create(int row,int column){
    int i,j;
    for(i=0; i<row+2; i++)
		Maze[i][0] = Maze[i][column+1] = '#';
    for(j=0; j<column+2; j++)
		Maze[0][j] = Maze[row+1][j] = '#';
    for(i=1; i<=row; i++){
    	scanf("%c",&huiche);
        for(j=1; j<=column; j++){
            scanf("%c",&Maze[i][j]);
            if(Maze[i][j]=='B')
			{
				StartR=i;
				StartC=j;
            	Maze[i][j]='-';
			}
            if(Maze[i][j]=='H')
			{
				EndR=i;
				EndC=j;
        		Maze[i][j]='-';	
			}
		}
    }
}
bool MazePath(int row,int column,int x,int y){
    if(x == row && y == column)return true;
    queue<point> q;     
    point now; 
    now.x = x;
    now.y = y;
    q.push(now);
    Maze[now.x][now.y] = -1;
    while(!q.empty()){
        now = q.front();
        q.pop();
        for(int i=0; i<4; i++){
            if(now.x + move[i].x == EndR && now.y + move[i].y == EndC)
			{
                Maze[now.x + move[i].x][now.y + move[i].y] = -1;
                Pre[row][column] = now;
                return true;
            }
            if(Maze[now.x + move[i].x][now.y + move[i].y] == '-'){
                point temp;     
                temp.x = now.x + move[i].x;
                temp.y = now.y + move[i].y;
                q.push(temp);
                Maze[temp.x][temp.y] = -1;
                Pre[temp.x][temp.y] = now;

            }
        }
    }
    return false;
}

int main(){
    
    int row;        //迷宫行数
    int column;     //迷宫列数
        scanf("%d%d",&row,&column);
        Maze = new char*[row + 2];
        Pre = new point*[row + 2];
        for(int i=0; i<row+2; i++){
            Maze[i] = new char[column + 2];
            Pre[i] = new point[column + 2];
        }
        Create(row,column);
		if(MazePath(row,column,StartR,StartC))		    printf("Y");
		else printf("N");
    
    return 0;
}
```








ZoeCUR的代码：



```cpp
#include<iostream>
#include<cstdio>
#include<string.h>

void bfs(int x,int y);

int* q;
int** vis;
char** maze;
int** fa;
int** last_dir;
int r,c;
int dx[4] = {1,0,-1,0};
int dy[4] = {0,1,0,-1};
int main()
{
	
	scanf("%d%d",&r,&c);
	q = new int[r*c];
	vis = new int*[r];
	maze = new char*[r];
	fa = new int*[r];
	last_dir = new int*[r];
	int bg[2];
	int end[2];
	for(int i = 0;i < r;i++)
	{
		vis[i] = new int[c];
		maze[i] = new char[c];
		fa[i] = new int[c];
		last_dir[i] = new int[c];
	}
	char huiche;
	for(int i = 0;i<r;i++)
	{
		scanf("%c",&huiche);
		for(int j = 0;j<c;j++)
		{
			vis[i][j] = 0;
			scanf("%c",&maze[i][j]);
			if(maze[i][j] == 'B')
			{
				bg[0] = i;
				bg[1] = j;
			}
			if(maze[i][j] == 'H')
			{
				end[0] = i;
				end[1] = j;
			}
		}
	}
 		bfs(bg[0],bg[1]);
 		if(vis[end[0],end[1]] == 0)printf("N");
 		else printf("Y");
 		return 0;
}
void bfs(int x,int y)
{
	int front = 0,rear = 0,d,u;
	u = x*c+y;
	vis[x][y] = 1;fa[x][y] = u;
	q[rear++] = u;
	while(front<rear)
	{
		u = q[front++];
		x = u/c;
		y = u%c;
		for(d = 0;d<4;d++)
		{
			int nx = x+dx[d],ny = y+dy[d];
			if(nx >= 0 && nx<r && ny>=0 && ny<c && maze[nx][ny]!='#' && !vis[nx][ny])
			{
				int v = nx*c + ny;
				q[rear++] = v;
				vis[nx][ny] = 1;
				fa[nx][ny] = u;
				last_dir[nx][ny] = d;
			}
		}
	}
}
```](https://so.csdn.net/so/search/s.do?q=华为编程大赛&t=blog)](https://so.csdn.net/so/search/s.do?q=c++&t=blog)




