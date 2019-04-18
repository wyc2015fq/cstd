# LeetCode  Search a 2D Matrix（二分查找） - xcw0754 - 博客园
# [LeetCode  Search a 2D Matrix（二分查找）](https://www.cnblogs.com/xcw0754/p/4937391.html)
题意：
　　有一个矩阵，每行都有序，每行接在上一行尾后仍然有序。在此矩阵中查找是否存在某个数target。
思路：
　　这相当于用一个指针连续扫二维数组一样，一直p++就能到最后一个元素了。由于用vector装的，但是也是满足线性的。
　　二分：O(log n*m)
```
1 class Solution {
 2 public:
 3     bool searchMatrix(vector<vector<int>>& matrix, int target) 
 4     {
 5         int n=matrix.size(), m=matrix[0].size();
 6         int L=0, R=n*m-1;
 7         while(L<R)
 8         {
 9             int mid=L+(R-L+1)/2;
10             if(matrix[mid/m][mid%m]<=target)    L=mid;
11             else    R=mid-1;
12         }
13         return matrix[L/m][L%m]==target;
14     }
15 };
AC代码
```
　　迭代：O(n+m)
```
1 class Solution {
 2 public:
 3     bool searchMatrix(vector<vector<int>>& matrix, int target) 
 4     {
 5         int n=matrix.size(), m=matrix[0].size();
 6         int i=0, j=m-1;
 7         while(i<n && j>=0)
 8         {
 9             if(target==matrix[i][j])    return true;
10             if(target>matrix[i][j])    i++;
11             else    j--;
12         }
13         return false;
14     }
15 };
AC代码
```

