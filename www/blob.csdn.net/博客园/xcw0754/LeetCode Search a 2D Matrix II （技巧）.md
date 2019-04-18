# LeetCode  Search a 2D Matrix II （技巧） - xcw0754 - 博客园
# [LeetCode  Search a 2D Matrix II （技巧）](https://www.cnblogs.com/xcw0754/p/4937401.html)
题意：
　　有一个矩阵，每行有序，每列也有序。判断一个数target是否存在于此矩阵中。
思路：
　　从右上角开始，如果当前数字<target，则该行作废。若当前数字>target，该列作废。这样下去要么找到，要么到达边界退出。
```
1 class Solution {
 2 public:
 3     bool searchMatrix(vector<vector<int>>& matrix, int target) {
 4         int n=matrix.size(), m=matrix[0].size();
 5         int i=0, j=m-1;
 6         while(i<n && j>=0)
 7         {
 8             if(target==matrix[i][j])    return true;
 9             if(target>matrix[i][j])    i++;
10             else    j--;
11         }
12         return false;
13     }
14 };
AC代码
```

