# The Grove（POJ-3182） - Alex_McAvoy的博客 - CSDN博客





2018年07月20日 00:06:28[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：53
个人分类：[POJ																[图论——图的搜索](https://blog.csdn.net/u011815404/article/category/8237087)](https://blog.csdn.net/u011815404/article/category/7504500)








> 
# Problem Description

The pasture contains a small, contiguous grove of trees that has no 'holes' in the middle of the it. Bessie wonders: how far is it to walk around that grove and get back to my starting position? She's just sure there is a way to do it by going from her start location to successive locations by walking horizontally, vertically, or diagonally and counting each move as a single step. Just looking at it, she doesn't think you could pass 'through' the grove on a tricky diagonal. Your job is to calculate the minimum number of steps she must take. 

Happily, Bessie lives on a simple world where the pasture is represented by a grid with R rows and C columns (1 <= R <= 50, 1 <= C <= 50). Here's a typical example where '.' is pasture (which Bessie may traverse), 'X' is the grove of trees, '*' represents Bessie's start and end position, and '+' marks one shortest path she can walk to circumnavigate the grove (i.e., the answer): 

...+...

..+X+..

.+XXX+.

..+XXX+

..+X..+

...+++*

The path shown is not the only possible shortest path; Bessie might have taken a diagonal step from her start position and achieved a similar length solution. Bessie is happy that she's starting 'outside' the grove instead of in a sort of 'harbor' that could complicate finding the best path.

# **Input**

Line 1: Two space-separated integers: R and C

Lines 2..R+1: Line i+1 describes row i with C characters (with no spaces between them).

# Output

Line 1: The single line contains a single integer which is the smallest number of steps required to circumnavigate the grove.

# Sample Input

**6 7..........X.....XXX.....XXX....X.........***

# Sample Output

**13**


题意：给出一 R 行 C 列的图，“ . ” 表示空地，“ X ” 表示树林，“ * ” 表示起点，可以上下左右走，也可以走对角线，求最少多少步能绕树林一圈。

思路：

只能想到这个题应该用 bfs 来做，但如何实现搜索范围包含连通块毫无头绪，看了大量题解稍微有了些思路，采用 “射线法”，即在树林边界上任取一点，画一条射线，进行搜索时对搜索方向加以限制，使得搜索时只能从一个方向穿过，这样保证能搜索方向是一个方向，从而形成回路。

用状态：(x, y, step)，表示到达点 (x, y) 时，是否是穿过射线时的最小步数，最后 minn=min{f[i][j][0]+f[i][j][1]} 即可。

# Source Program

```cpp
#include<iostream>
#include<cstdio>
#include<cstring>
#include<cmath>
#include<algorithm>
#include<string>
#include<cstdlib>
#include<queue>
#include<set>
#include<map>
#include<stack>
#include<vector>
#define INF 0x3f3f3f3f
#define PI acos(-1.0)
#define N 51
#define MOD 123
#define E 1e-6
using namespace std;
int r,c;
int g[N][N],f[N][N][2];
int dir[8][2]={{1,0},{-1,0},{0,1},{0,-1},{1,1},{1,-1},{-1,1},{-1,-1}};
int start_x,start_y;
int line_x,line_y;
struct Node{
    int x;
    int y;
    bool step;
}q[N*100],temp;
bool judge(int x,int y)
{
    if( x>=1 && y>=1 && x<=r && y<=c && g[x][y]==1 )
        return 1;
	else
        return 0;
}
void bfs()
{
    memset(f,-1,sizeof(f));
    q[0]=(Node){start_x,start_y,0};
    f[start_x][start_y][0]=0;
 
    int head=0,tail=0;
    while(head<=tail)
    {
        temp=q[head++];
        for(int i=0;i<8;i++)//向8个方向搜索
        {
            int x=temp.x+dir[i][0];
            int y=temp.y+dir[i][1];
            if(judge(x,y))//如果没有越界
            {
                bool step=temp.step;
                if( (temp.x<=line_x&&x<=line_x) || (temp.x>line_x&&x>line_x) || (temp.y>=line_y&&y>=line_y) )
                {
				if(f[x][y][step]==-1)
				{
					f[x][y][step]=f[temp.x][temp.y][step]+1;
					q[++tail]=(Node){x,y,step};
				}
				}
				else if(temp.x<=line_x)
				{
					if(f[x][y][1]==-1)
					{
						f[x][y][1]=f[temp.x][temp.y][step]+1;
						q[++tail]=(Node){x,y,1};
					}
				}
 
            }
        }
    }
 
}
int main()
{
    bool flag=true;
    scanf("%d%d",&r,&c);
    for(int i=1;i<=r;i++)
    {
        char ch[N];
        scanf("%s",ch+1);
        for(int j=1;j<=c;j++)
        {
            if(ch[j]=='*')//记录始点
            {
                start_x=i;
                start_y=j;
            }
            else if(ch[j]=='.')//记录可以走的点
                g[i][j]=1;
            else if(flag)//记录读入的第一个X的坐标，作为射线的端点
            {
                line_x=i;
                line_y=j;
                flag=false;
            }
        }
    }
 
    bfs();
 
    int minn=INF;
    for(int i=1;i<=r;i++)
        for(int j=1;j<=c;j++)
            if( f[i][j][0]!=-1 && f[i][j][1]!=-1 && f[i][j][0]+f[i][j][1]<minn )
                minn=f[i][j][0]+f[i][j][1];
 
    printf("%d\n",minn);
 
	return 0;
}
```






