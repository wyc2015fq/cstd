# HDU1010 Tempter of the Bone DFS+剪枝 - 紫芝的博客 - CSDN博客





2018年05月24日 21:46:45[紫芝](https://me.csdn.net/qq_40507857)阅读数：50








# [点击打开链接](http://acm.hdu.edu.cn/showproblem.php?pid=1010)

# Tempter of the Bone

****Time Limit: 2000/1000 MS (Java/Others)    Memory Limit: 65536/32768 K (Java/Others)Total Submission(s): 141796    Accepted Submission(s): 37833****

Problem Description

The doggie found a bone in an ancient maze, which fascinated him a lot. However, when he picked it up, the maze began to shake, and the doggie could feel the ground sinking. He realized that the bone was a trap, and he tried desperately to get out of this maze.


The maze was a rectangle with sizes N by M. There was a door in the maze. At the beginning, the door was closed and it would open at the T-th second for a short period of time (less than 1 second). Therefore the doggie had to arrive at the door on exactly the T-th second. In every second, he could move one block to one of the upper, lower, left and right neighboring blocks. Once he entered a block, the ground of this block would start to sink and disappear in the next second. He could not stay at one block for more than one second, nor could he move into a visited block. Can the poor doggie survive? Please help him.

### **Input**

The input consists of multiple test cases. The first line of each test case contains three integers N, M, and T (1 < N, M < 7; 0 < T < 50), which denote the sizes of the maze and the time at which the door will open, respectively. The next N lines give the maze layout, with each line containing M characters. A character is one of the following:


'X': a block of wall, which the doggie cannot enter;

'S': the start point of the doggie;

'D': the Door; or

'.': an empty block.


The input is terminated with three 0's. This test case is not to be processed.

### **Output**

For each test case, print in one line "YES" if the doggie can survive, or "NO" otherwise.

### **Sample Input**
` `
4 4 5

S.X.

..X.

..XD

....

3 4 5

S.X.

..X.

...D

0 0 0

### **Sample Output**
` `
NO

YES

Author

ZHANG, Zheng

Source

[ZJCPC2004](http://acm.hdu.edu.cn/search.php?field=problem&key=ZJCPC2004&source=1&searchmode=source)

Recommend

JGShining   |   We have carefully selected several similar problems for you:  [1016](http://acm.hdu.edu.cn/showproblem.php?pid=1016)[1242](http://acm.hdu.edu.cn/showproblem.php?pid=1242)[1072](http://acm.hdu.edu.cn/showproblem.php?pid=1072)[1312](http://acm.hdu.edu.cn/showproblem.php?pid=1312)[1026](http://acm.hdu.edu.cn/showproblem.php?pid=1026)

## 题意：

输入一个n*m的迷宫，T为可以在迷宫中生存的最大时间，。S为起点，D为终点。.为路，X为障碍，每个格子只能踩一次，且只能维持一秒，然后该块地板就会塌陷。所以你必须每秒走一步，且到D点时，所用时间为T。

## 题解：

从起点开始向上下左右四个方向搜索，遇到路“."则继续向下搜索，遇到障碍”X“则结束搜索；当走到D点时且T=0时，输出YES，否则为NO；这样朴素的搜索会超时，所以需要剪枝

### 剪枝1：

如果剩余时间T小于当前点到终点的最短距离，则这条路径不可能按时到大，停止向下搜索

最短路径=abs(ex-x)+abs(ey-y)；

### 剪枝2:

初始化n*m的01矩阵

0 1 0 1 0 1

1 0 1 0 1 0

0 1 0 1 0 1

1 0 1 0 1 0

0 1 0 1 0 1

下标从1开始，a【i】【j】=（i+j）% 2

在地图上，权值相同的两点距离为偶数

权值不同的两点距离为奇数

若(a【x】【y】+a【ex】【ey】)为偶数，

如果剩余时间t为偶数，则可能到达

如果剩余时间t为奇数，则无法到达


若(a【x】【y】+a【ex】【ey】)为奇数，

如果剩余时间t为偶数，则无法到达

如果剩余时间t为奇数，则可能到达


综上，

若(a【x】【y】+a【ex】【ey】+t)为偶数，

则可能到达

如果为奇数，则无法到达

```cpp
#include<bits/stdc++.h>
using namespace std;
int sx,sy,ex,ey;
int n,m;
char mp[10][10];
int d[4][2]={{1,0},{-1,0},{0,1},{0,-1}};
bool flag;
void dfs(int x,int y,int t)
{
    if(flag==1)    return;//若找到解，则不需要找其他的路径

    //剪枝
    if(t<abs(ex-x)+abs(ey-y))   return ;//如果剩余时间小于当前点到达终点的横纵坐标之和
/*
    if((t-abs(ex-x)+abs(ey-y))%2)   return;//若无障碍，则差值为0，否则绕道
*/
if((t+x+y+ex+ey)%2)  return;//剪枝2
    if(t==0)
    {
        if(x==ex&&y==ey)
        {
            flag=1;return;
        }else  return;
    }
    else
    for(int i=0;i<4;i++)
    {
        int nx=x+d[i][0],ny=y+d[i][1];
        if(nx>0&&nx<=n&&ny>0&&ny<=m&&(mp[nx][ny]=='.'||mp[nx][ny]=='D')){
            mp[nx][ny]='X';//标记走过
            dfs(nx,ny,t-1);
            mp[nx][ny]='.';//回溯
        }
    }
}
int main()
{
    int t;char str[10];
    while(scanf("%d%d%d",&n,&m,&t)!=EOF){
        if(n==0&&m==0&&t==0)    break;
        for(int i=1;i<=n;i++){
            scanf("%s",str);
            for(int j=1;j<=m;j++)
        {
            mp[i][j]=str[j-1];
            if(mp[i][j]=='S')
                sx=i,sy=j;
            else if(mp[i][j]=='D')
                ex=i,ey=j;
        }
    }

        flag=0;
        dfs(sx,sy,t);
        if(flag)    printf("YES\n");
        else printf("NO\n");
    }
    return 0;
}
```





