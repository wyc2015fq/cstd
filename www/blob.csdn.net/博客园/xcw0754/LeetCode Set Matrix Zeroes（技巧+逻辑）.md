# LeetCode  Set Matrix Zeroes（技巧+逻辑） - xcw0754 - 博客园
# [LeetCode  Set Matrix Zeroes（技巧+逻辑）](https://www.cnblogs.com/xcw0754/p/4940850.html)
题意：
　　给一个n*m的矩阵，如果某个格子中的数字为0,则将其所在行和列全部置为0。（注：新置的0不必操作）
思路：
　　主要的问题是怎样区分哪些是新来的0？
　　方法（1）：将矩阵复制多一个，根据副本来操作原矩阵。
　　方法（2）：发现空间还可以用O(n)来解决。
　　方法（3）：若m[i][j]=0，则将m[i][0]和m[0][j]标记为0，表示i行和j列都为0，但是这样的问题是，首行和首列会冲突？那就将首列先预处理一下，用另外的常量标记就行了。时间复杂度O(n*m)，空间O(1)。
```
1 class Solution {
 2 public:
 3     void setZeroes(vector<vector<int>>& matrix) {
 4         int col0=1, n=matrix.size(), m=matrix[0].size();
 5         for(int i=0; i<n; i++)
 6         {
 7             if(!matrix[i][0])    col0=0;            
 8             for(int j=1; j<m; j++)
 9             {
10                 if(!matrix[i][j])
11                     matrix[i][0]=matrix[0][j]=0;
12             }
13         }
14         for(int i=n-1; i>=0; i--)
15         {
16             for(int j=m-1; j>0; j--)
17             {
18                 if(!matrix[0][j]||!matrix[i][0])
19                     matrix[i][j]=0;
20             }
21             if(col0==0)    matrix[i][0]=0;
22         }
23     }
24 };
AC代码
```

