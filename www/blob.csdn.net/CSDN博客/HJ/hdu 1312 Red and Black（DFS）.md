# hdu  1312  Red and Black（DFS） - HJ - CSDN博客
2016年08月08日 20:59:46[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：251
个人分类：[====ACM====																[-----------搜索-----------																[DFS																[---------OJ---------																[HDU](https://blog.csdn.net/feizaoSYUACM/article/category/6308041)](https://blog.csdn.net/feizaoSYUACM/article/category/6308067)](https://blog.csdn.net/feizaoSYUACM/article/category/6317663)](https://blog.csdn.net/feizaoSYUACM/article/category/6307975)](https://blog.csdn.net/feizaoSYUACM/article/category/6308070)
Red and Black
Description 
There is a rectangular room, covered with square tiles. Each tile is colored either red or black. A man is standing on a black tile. From a tile, he can move to one of four adjacent tiles. But he can’t move on red tiles, he can move only on black tiles. 
Write a program to count the number of black tiles which he can reach by repeating the moves described above.  
Input 
The input consists of multiple data sets. A data set starts with a line containing two positive integers W and H; W and H are the numbers of tiles in the x- and y- directions, respectively. W and H are not more than 20. 
There are H more lines in the data set, each of which includes W characters. Each character represents the color of a tile as follows. 
‘.’ - a black tile  
‘#’ - a red tile  
‘@’ - a man on a black tile(appears exactly once in a data set) 
Output 
For each data set, your program should output a line which contains the number of tiles he can reach from the initial tile (including itself).
Sample Input 
6 9
```
....#.
.....#
......
......
......
......
......
#@...#
.#..#.
```
11 9
```
.#.........
 .#.#######.
.#.#.....#.
.#.#.###.#.
.#.#..@#.#.
.#.#####.#.
.#.......#.
.#########.
...........
```
11 6
```
..#..#..#..
..#..#..#..
..#..#..###
..#..#..#@.
..#..#..#..
..#..#..#..
```
7 7
```
..#.#..
..#.#..
###.###
...@...
###.###
..#.#..
..#.#..
```
0 0
Sample Output 
45 
59 
6 
13
解析：本题的意思是在不能翻墙（不能跳过‘#’）的情况下，最多能走遍多少个点
思路：还是一道经典的用dfs解决的题目，注意边界条件即可
```
#include<iostream>
using namespace std;
int cnt,a[4]={-1,0,1,0},b[4]={0,1,0,-1},n,m,vis[22][22];
char s[22][22];
void dfs(int x,int y)
{
    for(int i=0;i<4;i++)
    {
        int p=x+a[i];
        int q=y+b[i];
        if(p>=0&&p<m&&q>=0&&q<n&&!vis[p][q]&&s[p][q]=='.')       //判断递归条件，包括在数组边界之内，该点未被标记 
        {
            vis[p][q]=1;            //标记该点 
            cnt++;               //计数变量加一 
            dfs(p,q);               //递归搜索 
        } 
    }
}
int main()
{
    while(cin>>n>>m)
    {
        if(n==0&&m==0)
          break;
        cnt=0;
        int r,l;
        for(int i=0;i<m;i++)
          for(int j=0;j<n;j++)
          {
               cin>>s[i][j];
               vis[i][j]=0;
               if(s[i][j]=='@')
               {
                    r=i;
                    l=j;
               }
          }
        cnt=1;          //赋初值 
        vis[r][l]=1;         //标记起点位置 
        dfs(r,l);          //递归搜索 
        cout<<cnt<<endl;
    }
}
```
