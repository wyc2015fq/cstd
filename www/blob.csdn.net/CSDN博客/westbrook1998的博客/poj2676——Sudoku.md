# poj2676——Sudoku - westbrook1998的博客 - CSDN博客





2018年08月12日 19:50:28[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：62








> 
Sudoku is a very simple task. A square table with 9 rows and 9 columns is divided to 9 smaller squares 3x3 as shown on the Figure. In some of the cells are written decimal digits from 1 to 9. The other cells are empty. The goal is to fill the empty cells with decimal digits from 1 to 9, one digit per cell, in such way that in each row, in each column and in each marked 3x3 subsquare, all the digits from 1 to 9 to appear. Write a program to solve a given Sudoku-task.  
![这里写图片描述](https://odzkskevi.qnssl.com/56e7e6182093bbca488d6c73ba833af6?v=1534005124)

  Input 

  The input data will start with the number of the test cases. For each test case, 9 lines follow, corresponding to the rows of the table. On each line a string of exactly 9 decimal digits is given, corresponding to the cells in this line. If a cell is empty it is represented by 0. 

  Output 

  For each test case your program should print the solution in the same format as the input data. The empty cells have to be filled according to the rules. If solutions is not unique, then the program may print any one of them. 

  Sample Input 

  1 

  103000509 

  002109400 

  000704000 

  300502006 

  060000050 

  700803004 

  000401000 

  009205800 

  804000107 

  Sample Output 

  143628579 

  572139468 

  986754231 

  391542786 

  468917352 

  725863914 

  237481695 

  619275843 

  854396127
解数独的题目 

自己写了一大堆越写越乱，最后还是参考了一下题解，感觉写得比较巧妙，把dfs起点的遍历也放在dfs里，而不是枚举起点再dfs

代码：

```cpp
#include <cstdio>
#include <algorithm>
#include <cstring>
using namespace std;
bool row[15][15];
bool col[15][15];
bool ge[15][15];
char map[15][15];
int mp[15][15];
int t;
void init(){
    memset(row,false,sizeof(row));
    memset(col,false,sizeof(col));
    memset(ge,false,sizeof(ge));
    memset(mp,'\0',sizeof(mp));
}
bool dfs(int i,int j){
    //9行已经完成
    if(i==9){
        return true;
    }
    bool flag=false;
    //已有数字，搜索下一个位置
    if(mp[i][j]){
        if(j==8){
            //下一行
            flag=dfs(i+1,0);
        }
        else{
            //往右
            flag=dfs(i,j+1);
        }
        //回溯
        if(flag){
            return true;
        }
        else{
            return false;
        }
    }
    //没有数字
    else{
        int k=(i/3)*3+(j+3)/3;
        for(int t=1;t<=9;t++){
            if(!row[i][t] && !col[j][t] && !ge[k][t]){
                mp[i][j]=t;
                row[i][t]=true;
                col[j][t]=true;
                ge[k][t]=true;
                //判断是否换行
                if(j==8){
                    flag=dfs(i+1,0);
                }
                else{
                    flag=dfs(i,j+1);
                }
                //回溯
                if(!flag){
                    //搜索失败，重置
                    mp[i][j]=0;
                    row[i][t]=false;
                    col[j][t]=false;
                    ge[k][t]=false;
                }
                else{
                    return true;
                }
            }
        }
    }
    return false;
}
int main(void){
    //freopen("data.txt","r",stdin);
    //freopen("out.txt","w",stdout);
    scanf("%d",&t);
    while(t--){
        init();
        for(int i=0;i<9;i++){
            scanf("%s",map[i]);
            for(int j=0;j<9;j++){
                mp[i][j]=map[i][j]-'0';
                if(mp[i][j]){
                    row[i][mp[i][j]]=true;
                    col[j][mp[i][j]]=true;
                    ge[(i/3)*3+(j+3)/3][mp[i][j]]=true;
                }
            }
        }
        dfs(0,0);
        for(int i=0;i<9;i++){
            for(int j=0;j<9;j++){
                printf("%d",mp[i][j]);
            }
            printf("\n");
        }
    }
    return 0;
}
```





