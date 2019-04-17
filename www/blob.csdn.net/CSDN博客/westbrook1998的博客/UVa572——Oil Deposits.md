# UVa572——Oil Deposits - westbrook1998的博客 - CSDN博客





2018年05月14日 22:54:02[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：41








> 
The GeoSurvComp geologic survey company is responsible for detecting underground oil deposits. 

  GeoSurvComp works with one large rectangular region of land at a time, and creates a grid that divides 

  the land into numerous square plots. It then analyzes each plot separately, using sensing equipment to 

  determine whether or not the plot contains oil. 

  A plot containing oil is called a pocket. If two pockets are adjacent, then they are part of the 

  same oil deposit. Oil deposits can be quite large and may contain numerous pockets. Your job is to 

  determine how many different oil deposits are contained in a grid. 

  Input 

  The input file contains one or more grids. Each grid begins with a line containing m and n, the number 

  of rows and columns in the grid, separated by a single space. If m = 0 it signals the end of the input; 

  otherwise 1 ≤ m ≤ 100 and 1 ≤ n ≤ 100. Following this are m lines of n characters each (not counting 

  the end-of-line characters). Each character corresponds to one plot, and is either ‘*’, representing the 

  absence of oil, or ‘@’, representing an oil pocket. 

  Output 

  For each grid, output the number of distinct oil deposits. Two different pockets are part of the same 

  oil deposit if they are adjacent horizontally, vertically, or diagonally. An oil deposit will not contain 

  more than 100 pockets. 

  Sample Input 

  1 1 

  * 

  3 5 
*@*@* 
**@**
*@*@* 

  1 8 

  @@****@* 

  5 5 
****@ 
*@@*@ 
*@**@ 

  @@@*@ 

  @@**@ 

  0 0 

  Sample Output 

  0 

  1 

  2 

  2
紫书上一道经典的dfs求连通分量例题，值得一做

代码：

```cpp
#include <cstdio>
#include <algorithm>
#include <cstring>
using namespace std;
const int MAXN=150;
char o[MAXN][MAXN];
int idx[MAXN][MAXN];
int n,m;
void dfs(int r,int c,int id){
    if(r<0 || r>=m || c<0 || c>=n){
        return;
    }
    if(idx[r][c]>0 || o[r][c]!='@'){
        return;
    }
    idx[r][c]=id;
    for(int dr=-1;dr<=1;dr++){
        for(int dc=-1;dc<=1;dc++){
            if(dr!=0 || dc!=0){
                dfs(r+dr,c+dc,id);
            }
        }
    }
}
int main(void){
    while(scanf("%d%d",&m,&n)!=EOF){
        if(m==0 && n==0){
            break;
        }
        for(int i=0;i<m;i++){
            scanf("%s",o[i]);
        }
        int cnt=0;
        memset(idx,0,sizeof(idx));
        for(int i=0;i<m;i++){
            for(int j=0;j<n;j++){
                if(idx[i][j]==0 && o[i][j]=='@'){
                    dfs(i,j,++cnt);
                }
            }
        }
        printf("%d\n",cnt);
    }
    return 0;
}
```





