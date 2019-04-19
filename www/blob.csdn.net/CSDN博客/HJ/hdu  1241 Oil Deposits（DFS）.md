# hdu   1241  Oil Deposits（DFS） - HJ - CSDN博客
2016年08月08日 20:33:51[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：247
个人分类：[====ACM====																[-----------搜索-----------																[DFS																[---------OJ---------																[HDU](https://blog.csdn.net/feizaoSYUACM/article/category/6308041)](https://blog.csdn.net/feizaoSYUACM/article/category/6308067)](https://blog.csdn.net/feizaoSYUACM/article/category/6317663)](https://blog.csdn.net/feizaoSYUACM/article/category/6307975)](https://blog.csdn.net/feizaoSYUACM/article/category/6308070)
Oil Deposits
Problem Description 
The GeoSurvComp geologic survey company is responsible for detecting underground oil deposits. GeoSurvComp works with one large rectangular region of land at a time, and creates a grid that divides the land into numerous square plots. It then analyzes each plot separately, using sensing equipment to determine whether or not the plot contains oil. A plot containing oil is called a pocket. If two pockets are adjacent, then they are part of the same oil deposit. Oil deposits can be quite large and may contain numerous pockets. Your job is to determine how many different oil deposits are contained in a grid. 
Input 
The input file contains one or more grids. Each grid begins with a line containing m and n, the number of rows and columns in the grid, separated by a single space. If m = 0 it signals the end of the input; otherwise 1 <= m <= 100 and 1 <= n <= 100. Following this are m lines of n characters each (not counting the end-of-line characters). Each character corresponds to one plot, and is either `*', representing the absence of oil, or`@’, representing an oil pocket.
Output 
For each grid, output the number of distinct oil deposits. Two different pockets are part of the same oil deposit if they are adjacent horizontally, vertically, or diagonally. An oil deposit will not contain more than 100 pockets.
Sample Input 
1 1
```
*
```
3 5
```
*@*@*
**@**
*@*@*
```
1 8
```
@@****@*
```
5 5 
```
****@
*@@*@
*@**@
@@@*@
@@**@
```
0 0 
Sample Output 
0 
1 
2 
2
解析：看输入输出就可以明白，题目意思就是要你计算连通块的个数，基础DFS。
思路：先遍历一遍，找到第一个‘@’，计数变量自动加一，然后开始递归广搜，直到这个连通块搜索达到边界，标记搜索结果，接下来就继续找下一个‘@’，计数变量再接着加一，再递归广搜，标记的时候都把‘@’标记成‘#’或者别的也行，直到没有‘@’的时候出边界，输出结果。
注意：广搜的时候需要注意从当前节点到下一节点需要搜索的广度，有的时候是四个方向，而本题是八个方向，为了代码简洁，用数组保存开循环实现
```cpp
#include<iostream>
using namespace std;
char s[105][105];
int dx[8]={-1,-1,-1,0,0,1,1,1},dy[8]={-1,0,1,-1,1,-1,0,1};     //除了中心，有八个方向需要搜索 
int n,m;
void dfs(int x,int y)
{
    if(s[x][y]=='@')
    { 
        s[x][y]='#';              //对已经搜索的位置标记 
        for(int i=0;i<8;i++)
          if(dx[i]+x>=0&&dx[i]+x<n&&dy[i]+y>=0&&dy[i]+y<m)        //判断边界条件 
            dfs(dx[i]+x,dy[i]+y);                          //递归广度优先搜索 
    } 
}
int main()
{
    while(cin>>n>>m&&n&&m)
    {
        int cnt=0;
        for(int i=0;i<n;i++)
          cin>>s[i];
        for(int i=0;i<n;i++)
          for(int j=0;j<m;j++)
            if(s[i][j]=='@')
            {
                cnt++;             //每次发现一个连通块，计数变量加一 
                dfs(i,j);
            }
        cout<<cnt<<endl;
    }
    return 0;
}
```
