# LeetCode  Rotate Image （模拟） - xcw0754 - 博客园
# [LeetCode  Rotate Image （模拟）](https://www.cnblogs.com/xcw0754/p/4937261.html)
题意：
　　将一个n*n的矩阵顺时针旋转90度。
思路：
　　都是差不多的思路，交换3次也行，反转再交换也是行的。
```
1 class Solution {
 2 public:
 3     void rotate(vector<vector<int>>& matrix) {
 4         int i=0, n=matrix.size();
 5         while(i*2<n)
 6         {
 7             for(int j=i; j<n-i-1; j++)
 8             {
 9                 swap(matrix[n-j-1][i],matrix[n-i-1][n-j-1]);
10                 swap(matrix[n-i-1][n-j-1],matrix[j][n-i-1]);
11                 swap(matrix[j][n-i-1],matrix[i][j]);
12             }
13             i++;
14         }
15     }
16 };
AC代码
```

