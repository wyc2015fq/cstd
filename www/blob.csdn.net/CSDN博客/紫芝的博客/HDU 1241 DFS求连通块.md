# HDU 1241 DFS求连通块 - 紫芝的博客 - CSDN博客





2018年04月06日 14:36:20[紫芝](https://me.csdn.net/qq_40507857)阅读数：48









## Oil Deposit




The GeoSurvComp geologic survey company is responsible for detecting underground oil deposits. GeoSurvComp works with one large rectangular region of land at a time, and creates a grid that divides the land into numerous square plots. It then analyzes each plot separately, using sensing equipment to determine whether or not the plot contains oil. A plot containing oil is called a pocket. If two pockets are adjacent, then they are part of the same oil deposit. Oil deposits can be quite large and may contain numerous pockets. Your job is to determine how many different oil deposits are contained in a grid. 

InputThe input file contains one or more grids. Each grid begins with a line containing m and n, the number of rows and columns in the grid, separated by a single space. If m = 0 it signals the end of the input; otherwise 1 <= m <= 100 and 1 <= n <= 100. Following this are m lines of n characters each (not counting the end-of-line characters). Each character corresponds to one plot, and is either `*', representing the absence of oil, or `@', representing an oil pocket. 

OutputFor each grid, output the number of distinct oil deposits. Two different pockets are part of the same oil deposit if they are adjacent horizontally, vertically, or diagonally. An oil deposit will not contain more than 100 pockets. 



Sample Input1 1
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
0 0 Sample Output

0
1
2
2
```cpp
//hdu 1241
//#include<bits/stdc++.h>
#include<iostream>
#include<cstdio>
using namespace std;
int n,m,ans;
char s[105][105];
void dfs(int x,int y)
{
	if(x<0||y<0||x>=n||y>=m||s[x][y]=='*')
	return;
	
	s[x][y]='*';
	
	dfs(x-1,y-1);
	dfs(x-1,y);
	dfs(x-1,y+1);
	
	dfs(x+1,y-1);
	dfs(x+1,y);
	dfs(x+1,y+1);
	
	dfs(x,y-1);
	dfs(x,y+1);
}
int main()
{
    while(cin>>n>>m&&n&&m){
    	for(int i=0;i<n;i++)
    	{
    		scanf("%s",s[i]);
		}
    ans=0;
    for(int i=0;i<n;i++)
    {
    	for(int j=0;j<m;j++)
    	{
    		if(s[i][j]=='@')
    		{
    			dfs(i,j);
				ans++;	
			}
		}
	}
	cout<<ans<<endl;
		}
   return 0;
}
```




