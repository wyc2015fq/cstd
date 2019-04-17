# Oil Deposits（HDU-1241） - Alex_McAvoy的博客 - CSDN博客





2018年03月16日 14:57:00[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：79








> 
# Problem Description

    The GeoSurvComp geologic survey company is responsible for detecting underground oil deposits. GeoSurvComp works with one large rectangular region of land at a time, and creates a grid that divides the land into numerous square plots. It then analyzes each plot separately, using sensing equipment to determine whether or not the plot contains oil. A plot containing oil is called a pocket. If two pockets are adjacent, then they are part of the same oil deposit. Oil deposits can be quite large and may contain numerous pockets. Your job is to determine how many different oil deposits are contained in a grid. 

# Input

    The input file contains one or more grids. Each grid begins with a line containing m and n, the number of rows and columns in the grid, separated by a single space. If m = 0 it signals the end of the input; otherwise 1 <= m <= 100 and 1 <= n <= 100. Following this are m lines of n characters each (not counting the end-of-line characters). Each character corresponds to one plot, and is either `*', representing the absence of oil, or `@', representing an oil pocket.

# Output

     For each grid, output the number of distinct oil deposits. Two different pockets are part of the same oil deposit if they are adjacent horizontally, vertically, or diagonally. An oil deposit will not contain more than 100 pockets.

# Sample Input

**1 1    *    3 5    *@*@*    **@**    *@*@*    1 8    @@****@*    5 5     ****@    *@@*@    *@**@    @@@*@    @@**@    0 0 **

# Sample Output

**   0   1   2   2**


题意：给一个 n*m 的图，其中 @ 代表有油，* 代表已经开采，一片连续的 @ 视为一个油田，问油田个数

思路：基础的 dfs，向8个方向深搜即可

# Source Program

```cpp
#include<cstring>
#include<iostream>
using namespace std;

int m,n;
char mapp[105][105];
int direction[8][2]={{-1,0},{1,0},{0,-1},{0,1},{-1,-1},{-1,1},{1,-1},{1,1}};//方向向量

void dfs(int x0,int y0)
{
    int x,y;
    int i;

    for(i=0;i<8;i++)
    {
        x=x0+direction[i][0];
        y=y0+direction[i][1];

        if(x>=0&&x<m&&y>=0&&y<n)//判断是否越界
        {
            if(mapp[x][y]=='@')//判断是否有
            {
                mapp[x][y]='*';//开采
                dfs(x,y);//继续深搜
            }
        }
    }
}
int main()
{
    int sum;
    int i,j;

    while(cin>>m>>n)//输入地图大小
    {
        if(m==0)    break;

        for(i=0;i<m;i++)
            for(j=0;j<n;j++)
                cin>>mapp[i][j];

        sum=0;//计数器归零
        for(i=0;i<m;i++)
        {
            for(j=0;j<n;j++)
            {
                if(mapp[i][j]=='@')//如果有油
                {
                    mapp[i][j]='*';//开采
                    dfs(i,j);//进行深搜
                    sum++;
                }
            }
        }
        printf("%d\n",sum);
    }
    return 0;
}
```





