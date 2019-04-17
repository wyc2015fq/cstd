# Red and Black（HDU-1312） - Alex_McAvoy的博客 - CSDN博客





2018年03月16日 23:21:12[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：106








> 
# Problem Description

    There is a rectangular room, covered with square tiles. Each tile is colored either red or black. A man is standing on a black tile. From a tile, he can move to one of four adjacent tiles. But he can't move on red tiles, he can move only on black tiles.

    Write a program to count the number of black tiles which he can reach by repeating the moves described above. 

# Input

    The input consists of multiple data sets. A data set starts with a line containing two positive integers W and H; W and H are the numbers of tiles in the x- and y- directions, respectively. W and H are not more than 20.

    There are H more lines in the data set, each of which includes W characters. Each character represents the color of a tile as follows.

    '.' - a black tile，'#' - a red tile，'@' - a man on a black tile(appears exactly once in a data set)

# Output

     For each data set, your program should output a line which contains the number of tiles he can reach from the initial tile (including itself).

# Sample Input

**6 9    ....#.    .....#    ......    ......    ......    ......    ......    #@...#    .#..#.    11 9    .#.........    .#.#######.    .#.#.....#.    .#.#.###.#.    .#.#..@#.#.    .#.#####.#.    .#.......#.    .#########.    ...........    11 6    ..#..#..#..    ..#..#..#..    ..#..#..###    ..#..#..#@.    ..#..#..#..    ..#..#..#..    7 7    ..#.#..    ..#.#..    ###.###    ...@...    ###.###    ..#.#..    ..#.#..    0 0**

# Sample Output

**   45   59   6   13**


思路：经典的红与黑问题

思路：bfs、dfs均可，注意输入房间长宽时，先输入列数再输入行数

# Source Program

```cpp
#include<cstdio>  
int w,h;//w是列，h是行  
int sum;  
char map[21][21];  
int direction[4][2]={{-1,0},{1,0},{0,-1},{0,1}};//方向数组  
  
struct node  
{  
    int x;  
    int y;  
}quene[500];  
  
bool judge(int x0,int y0)//判断坐标是否在规定范围内  
{  
    if(x0>=0&&x0<h&&y0>=0&&y0<w)  
        return true;  
    else  
        return false;  
}  
  
void bfs(int x0,int y0)  
{  
    int head,tail;  
    int x,y;  
    int i;  
  
    head=0,tail=1;//设置队列首尾初值  
    quene[1].x=x0;quene[1].y=y0;//初始状态存入队列  
    while(head<tail)  
    {  
		head++;//队首加1，出队  
        for(i=0;i<4;i++)//依次向上下左右搜索  
        {  
            x=quene[head].x+direction[i][0];  
            y=quene[head].y+direction[i][1];  
            if(map[x][y]=='.'&&judge(x,y))//若子节点符合条件  
            {  
                map[x][y]='#';//若走过，标记，不能重复走  
                
                quene[++tail].x=x;//入队  
                quene[tail].y=y;//入队  
                sum++;//走过数+1  
            }  
        }
    }  
}  
int main()  
{  
    int x0,y0;  
    int i,j;  
  
    while(scanf("%d %d",&w,&h)!=EOF)//输入房间长与宽  
    {  
        getchar();  
        if(w==0&&h==0)  break;//大小为0时终止循环  
  
        for(i=0;i<h;i++)  
        {  
            for(j=0;j<w;j++)  
            {  
                scanf("%c",&map[i][j]);//输入每块瓷砖  
                if(map[i][j]=='@')//记录起始位置  
                {  
                    x0=i;  
                    y0=j;  
                }  
            }  
			getchar();
        }  
        sum=1;  
        bfs(x0,y0);//搜索  
        printf("%d\n",sum);  
    }  
    return 0;  
}
```



