# LeetCode  Number of Islands  岛的数量（DFS，BFS） - xcw0754 - 博客园
# [LeetCode  Number of Islands  岛的数量（DFS，BFS）](https://www.cnblogs.com/xcw0754/p/4679850.html)
题意：0代表水，1代表陆地，那么被水围起来的就是岛了，给一个01矩阵，问有多少个岛？
思路：DFS还是比较短，实现了一下。如果一个点已经被遍历过了，那就将其置为0就行了，不要去搜0的。
```
1 class Solution {
 2 public:
 3     bool isok(vector<vector<char> >& grid,int x,int y)
 4     {
 5         return x>=0 && y>=0 && x<grid.size() && y<grid[0].size();
 6     }
 7 
 8     void DFS( vector<vector<char> >& grid,int x,int y )
 9     {
10         if( !isok(grid,x,y) || grid[x][y]=='0'  )   return ;
11 
12         grid[x][y]='0';
13         DFS( grid, x, y+1 );
14         DFS( grid, x, y-1 );
15         DFS( grid, x+1, y );
16         DFS( grid, x-1, y );
17     }
18 
19     int numIslands(vector<vector<char> >& grid) {
20         int ans=0;
21         for(int i=0; i<grid.size(); i++)
22         {
23             for(int j=0; j<grid[0].size(); j++)
24             {
25                 if(grid[i][j]=='1')
26                 {
27                     DFS(grid, i, j);
28                     ans++;
29                 }
30             }
31         }
32         return ans;
33     }
34 };
AC代码
```

