# poj2251——Dungeon Master - westbrook1998的博客 - CSDN博客





2018年06月28日 10:41:22[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：44标签：[BFS																[搜索](https://so.csdn.net/so/search/s.do?q=搜索&t=blog)](https://so.csdn.net/so/search/s.do?q=BFS&t=blog)
个人分类：[ACM练习题																[ACM练习赛](https://blog.csdn.net/westbrook1998/article/category/7635377)](https://blog.csdn.net/westbrook1998/article/category/7652684)







> 
You are trapped in a 3D dungeon and need to find the quickest way out! The dungeon is composed of unit cubes which may or may not be filled with rock. It takes one minute to move one unit north, south, east, west, up or down. You cannot move diagonally and the maze is surrounded by solid rock on all sides.  

  Is an escape possible? If yes, how long will it take?  

  Input 

  The input consists of a number of dungeons. Each dungeon description starts with a line containing three integers L, R and C (all limited to 30 in size).  

  L is the number of levels making up the dungeon.  

  R and C are the number of rows and columns making up the plan of each level.  

  Then there will follow L blocks of R lines each containing C characters. Each character describes one cell of the dungeon. A cell full of rock is indicated by a ‘#’ and empty cells are represented by a ‘.’. Your starting position is indicated by ‘S’ and the exit by the letter ‘E’. There’s a single blank line after each level. Input is terminated by three zeroes for L, R and C. 

  Output 

  Each maze generates one line of output. If it is possible to reach the exit, print a line of the form  

  Escaped in x minute(s). 

  where x is replaced by the shortest time it takes to escape.  

  If it is not possible to escape, print the line  

  Trapped! 

  Sample Input 

  3 4 5
```
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
```

Sample Output 

Escaped in 11 minute(s). 

Trapped!
三维的迷宫 BFS一波 搞错了很久 三维要对应正确 然后到现在居然还出现把==写成=的情况…

代码：

```cpp
#include <cstdio>
#include <algorithm>
#include <queue>
#include <cstring>
using namespace std;
int n,m,k,sx,sy,sz,ex,ey,ez;
char map[50][50][50];
int vis[50][50][50];
int to[6][3]={{0,0,1},{0,0,-1},{0,1,0},{0,-1,0},{1,0,0},{-1,0,0}};
struct node{
    int x,y,z,step;
};
int check(int x,int y,int z){
    if(x<0 || y<0 || z<0 || x>=k || y>=n || z>=m){
        return 0;
    }
    else if(map[x][y][z]=='#'){
        return 0;
    }
    else if(vis[x][y][z]){
        return 0;
    }
    else{
        return 1;
    }
}
int bfs(){
    node a,next;
    queue<node> que;
    a.x=sx;
    a.y=sy;
    a.z=sz;
    a.step=0;
    vis[sx][sy][sz]=1;
    que.push(a);
    while(!que.empty()){
        a=que.front();
        que.pop();
        if(a.x==ex && a.y==ey && a.z==ez){
            return a.step;
        }
        for(int i=0;i<6;i++){
            next=a;
            next.x=a.x+to[i][0];
            next.y=a.y+to[i][1];
            next.z=a.z+to[i][2];
            if(check(next.x,next.y,next.z)){
                vis[next.x][next.y][next.z]=1;
                next.step=a.step+1;
                que.push(next);
            }
        }
    }
    return 0;
}
int main(void){
    while(~scanf("%d%d%d",&k,&n,&m)){
        if(n==0 && m==0 && k==0){
            break;
        }
        for(int i=0;i<k;i++){
            for(int j=0;j<n;j++){
                scanf("%s",map[i][j]);
                for(int r=0;r<m;r++){
                    //确定起点和终点
                    if(map[i][j][r]=='S'){
                        sx=i;
                        sy=j;
                        sz=r;
                    }
                    else if(map[i][j][r]=='E'){
                        ex=i;
                        ey=j;
                        ez=r;
                    }
                }
            }
        }
        memset(vis,0,sizeof(vis));
        int ans=bfs();
        if(ans>0){
            printf("Escaped in %d minute(s).\n",ans);
        }
        else{
            printf("Trapped!\n");
        }
    }
    return 0;
}
```








