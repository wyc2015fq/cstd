# hdu1035——Robot Motion - westbrook1998的博客 - CSDN博客





2018年07月07日 21:39:07[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：39








> 
![这里写图片描述](https://odzkskevi.qnssl.com/af1fa5f57be25a1e9c85a7db5f80940e?v=1530722458)

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

  For each grid in the input there is one line of output. Either the robot follows a certain number of instructions and exits the grid on any one the four sides or else the robot follows the instructions on a certain number of locations once, and then the instructions on some number of locations repeatedly. The sample input below corresponds to the two grids above and illustrates the two forms of output. The word “step” is always immediately followed by “(s)” whether or not the number before it is 1.  

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

  0 0  

  Sample Output 

  10 step(s) to exit 

  3 step(s) before a loop of 8 step(s)
一道看似搜索其实很简单的模拟题，但是！！还是花了好久好久，细节的东西实在太难搞了，一开始是代码的布局思路搞错了 然后是输出环的长度的思路也错了 多此一举又去循环了一遍 其实只要循环的尾的vis值减去头的vis值就好了 然后最后就是结束条件判断没写在第一行向上的情况  还有边界判断没写好 tle了好几次 终于终于ac了

代码：

```cpp
#include <cstdio>
#include <algorithm>
#include <cstring>
#include <queue>
using namespace std;
char map[15][15];
int n,m,s;
//标记是否访问过 同时记录从起点到这个点走了多少步
int vis[15][15];
bool check(int r,int c){
    if(r>=0 && r<n && c>=0 && c<m && vis[r][c]!=-1){
        return false;
    }
    else{
        return true;
    }
}
int main(void){
    while(~scanf("%d%d",&n,&m)){
        if(n==0 && m==0){
            break;
        }
        scanf("%d",&s);
        for(int i=0;i<n;i++){
            scanf("%s",map[i]);
        }
        memset(vis,-1,sizeof(vis));
        vis[0][s-1]=0;
        int r=0;
        int c=s-1;
        while(1){
            if((r==n-1 && map[r][c]=='S') ||( c==0 && map[r][c]=='W') || (c==m-1 && map[r][c]=='E') || (r==0 && map[r][c]=='N')){
                printf("%d step(s) to exit\n",vis[r][c]+1);
                break;
            }
            //printf("%d %d %d\n",r,c,vis[r][c]);
            if(map[r][c]=='W'){
                if(check(r,c-1)){
                    c--;
                    vis[r][c]=vis[r][c+1]+1;
                }
                else{
                    printf("%d step(s) before a loop of %d step(s)\n",vis[r][c-1],vis[r][c]-vis[r][c-1]+1);
                    break;
                }
            }
            else if(map[r][c]=='E'){
                if(check(r,c+1)){
                    c++;
                    vis[r][c]=vis[r][c-1]+1;
                }
                else{
                    printf("%d step(s) before a loop of %d step(s)\n",vis[r][c+1],vis[r][c]-vis[r][c+1]+1);
                    break;
                }
            }
            else if(map[r][c]=='N'){
                if(check(r-1,c)){
                    r--;
                    vis[r][c]=vis[r+1][c]+1;
                }
                else{
                    printf("%d step(s) before a loop of %d step(s)\n",vis[r-1][c],vis[r][c]-vis[r-1][c]+1);
                    break;
                }
            }
            else if(map[r][c]=='S'){
                if(check(r+1,c)){
                    r++;
                    vis[r][c]=vis[r-1][c]+1;
                }
                else{
                    printf("%d step(s) before a loop of %d step(s)\n",vis[r+1][c],vis[r][c]-vis[r+1][c]+1);
                    break;
                }
            }
        }
    }
    return 0;
}
```





