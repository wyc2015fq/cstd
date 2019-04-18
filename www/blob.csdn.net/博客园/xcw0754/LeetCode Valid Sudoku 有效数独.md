# LeetCode Valid Sudoku 有效数独 - xcw0754 - 博客园
# [LeetCode Valid Sudoku 有效数独](https://www.cnblogs.com/xcw0754/p/4504859.html)
![](https://images0.cnblogs.com/blog2015/641737/201505/150001533925953.png)
题意：给一个9*9的矩阵，如果该矩阵是有效的数独盘的话，返回true。注意同一行或同一列的数字只能出现1次，同时其中的9个小的数独盘也不能重复。
思路：扫整个盘一遍，在扫的同时先考虑行，列，小盘。用二维数组维护列的，用一维数组维护行的，再用个二维的维护小盘。用哈希来判断是否已经存在。
```
1 bool isValidSudoku(vector< vector<char> >& board) {
 2     int vect[10][10]={0};
 3     int nine[3][10];
 4     for(int i=0; i<9; i++)
 5     {
 6         if(i%3==0)    memset(nine,0,sizeof(nine));//每次3个小盘
 7         int vis[10]={0};
 8         for(int j=0; j<9; j++)
 9         {
10             if(board[i][j]=='.')    continue;
11             int tmp=board[i][j]-'0';    
12             if(nine[j/3][tmp]==1)    return false;//小盘
13             nine[j/3][tmp]=1;
14             if(vis[tmp]==1)    return false;//同行
15             vis[tmp]=1;
16             if(vect[j][tmp]==1)    return false;//同列
17             vect[j][tmp]=1;
18         }
19     }
20     return true;
21 }
AC代码
```

