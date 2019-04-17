# Tempter of the Bone（HDU-1010） - Alex_McAvoy的博客 - CSDN博客





2018年03月19日 19:20:32[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：73
个人分类：[HDU																[图论——图的搜索](https://blog.csdn.net/u011815404/article/category/8237087)](https://blog.csdn.net/u011815404/article/category/7923257)








> 
# Problem Description

    The doggie found a bone in an ancient maze, which fascinated him a lot. However, when he picked it up, the maze began to shake, and the doggie could feel the ground sinking. He realized that the bone was a trap, and he tried desperately to get out of this maze.

    The maze was a rectangle with sizes N by M. There was a door in the maze. At the beginning, the door was closed and it would open at the T-th second for a short period of time (less than 1 second). Therefore the doggie had to arrive at the door on exactly the T-th second. In every second, he could move one block to one of the upper, lower, left and right neighboring blocks. Once he entered a block, the ground of this block would start to sink and disappear in the next second. He could not stay at one block for more than one second, nor could he move into a visited block. Can the poor doggie survive? Please help him.

# Input

    The input consists of multiple test cases. The first line of each test case contains three integers N, M, and T (1 < N, M < 7; 0 < T < 50), which denote the sizes of the maze and the time at which the door will open, respectively. The next N lines give the maze layout, with each line containing M characters. A character is one of the following:

    'X': a block of wall, which the doggie cannot enter; 

    'S': the start point of the doggie; 

    'D': the Door; 

    or'.': an empty block.

    The input is terminated with three 0's. This test case is not to be processed.

# Output

    For each test case, print in one line "YES" if the doggie can survive, or "NO" otherwise.

# Sample Input

**4 4 5    S.X.    ..X.    ..XD    ....    3 4 5    S.X.    ..X.    ...D    0 0 0**

# Sample Output

**   NO****YES**


思路：基本的dfs，但需要进行奇偶剪枝

# Source Program

```cpp
#include<cstdio>
#include<cmath>
#include<cstring>

int m,n,t;
char maps[10][10];
int vis[10][10];
int direction[4][2]={{1,0},{-1,0},{0,1},{0,-1}};
bool flag;

struct node
{
    int x;
    int y;
}start,endd;

bool judge(int x,int y)//越界判断
{
    if(x<0||x>m-1||y<0||y>n-1||maps[x][y]=='X'||vis[x][y])
        return false;
    return true;
}

void dfs(int x0,int y0,int time)
{
    int x,y;
    int step;
    int i;

    if(maps[x0][y0]=='D')//到达终点时
    {
        if(time==t)//花费时间正好与要求时间相等
        {
            flag=true;//成功逃脱
            return;//结束搜索
        }
        return;
    }
    
    if(flag)    return;
        
    /*奇偶剪枝*/
    step=fabs(x0-endd.x)+fabs(y0-endd.y);
    if((t-step-time)%2!=0||step+time>t)
        return;

    

    for(i=0;i<4;i++)
    {
        x=x0+direction[i][0];
        y=y0+direction[i][1];

        if(judge(x,y))//判断新坐标是否越界
        {
            vis[x][y]=1;//标记为已走过
            time++;//时间+1

            dfs(x,y,time);//向新坐标搜索

            vis[x][y]=0;//回溯
            time--;//回溯
        }
    }
}
int main()
{
    int begins;
    int i,j;
    
    while(scanf("%d%d%d",&m,&n,&t)!=EOF)
    {
        if(m==0&&n==0&&t==0)    break;
        memset(vis,0,sizeof(vis));
        flag=false;

        for(i=0;i<m;i++)
        {
            scanf("%s",maps[i]);
            for(j=0;j<n;j++)
            {
                if(maps[i][j]=='S')
                {
                    start.x=i;
                    start.y=j;
                    vis[i][j]=1;
                }
                if(maps[i][j]=='D')
                {
                    endd.x=i;
                    endd.y=j;
                }
            }
        }

        /*在开始进行奇偶剪枝*/
        begins=fabs(endd.x-start.x)+fabs(endd.y-start.y);
        if((begins+t)%2!=0)
        {
             printf("NO\n");
             continue;
        }

        dfs(start.x,start.y,0);

        if(flag)
            printf("YES\n");
        else
            printf("NO\n");
    }
    return 0;
}
```






