# hdu1241——Oil Deposits - westbrook1998的博客 - CSDN博客





2018年08月12日 19:34:57[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：28








> 
The GeoSurvComp geologic survey company is responsible for detecting underground oil deposits. GeoSurvComp works with one large rectangular region of land at a time, and creates a grid that divides the land into numerous square plots. It then analyzes each plot separately, using sensing equipment to determine whether or not the plot contains oil. A plot containing oil is called a pocket. If two pockets are adjacent, then they are part of the same oil deposit. Oil deposits can be quite large and may contain numerous pockets. Your job is to determine how many different oil deposits are contained in a grid.  

  Input 

  The input file contains one or more grids. Each grid begins with a line containing m and n, the number of rows and columns in the grid, separated by a single space. If m = 0 it signals the end of the input; otherwise 1 <= m <= 100 and 1 <= n <= 100. Following this are m lines of n characters each (not counting the end-of-line characters). Each character corresponds to one plot, and is either `*’, representing the absence of oil, or `@’, representing an oil pocket.  

  Output 

  For each grid, output the number of distinct oil deposits. Two different pockets are part of the same oil deposit if they are adjacent horizontally, vertically, or diagonally. An oil deposit will not contain more than 100 pockets.  

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
经典的dfs求连通块的问题，第二次写了，这次自己写了出来，虽然还是写了很久

代码：

```cpp
#include <cstdio>
#include <algorithm>
#include <cstring>
using namespace std;
int n,m;
const int MAXN=105;
char mp[MAXN][MAXN];
int vis[MAXN][MAXN];
int cnt;
int dir[8][2]={{-1,-1},{0,-1},{1,-1},{-1,0},{1,0},{-1,1},{0,1},{1,1}};
bool check(int i,int j){
    if(i<0 || i>=n || j<0 || j>=m || vis[i][j] || mp[i][j]=='*'){
        return false;
    }
    return true;
}
void dfs(int i,int j,int idx){
    vis[i][j]=idx;
    int ti;
    int tj;
    for(int k=0;k<8;k++){
        ti=i+dir[k][0];
        tj=j+dir[k][1];
        if(check(ti,tj)){
            dfs(ti,tj,idx);
        }
    }
}
void init(){
    memset(mp,'\0',sizeof(mp));
    memset(vis,0,sizeof(vis));
    cnt=0;
}
int main(void){
    //freopen("data.txt","r",stdin);
    while(~scanf("%d%d",&n,&m) && n && m){
        init();
        for(int i=0;i<n;i++){
            scanf("%s",mp[i]);
        }
        for(int i=0;i<n;i++){
            for(int j=0;j<m;j++){
                if(mp[i][j]=='@' && !vis[i][j]){
                    dfs(i,j,++cnt);
                }
            }
        }
        printf("%d\n",cnt);
    }
    return 0;
}
```





