# LeetCode Unique Paths （简单DP） - xcw0754 - 博客园
# [LeetCode Unique Paths （简单DP）](https://www.cnblogs.com/xcw0754/p/4922088.html)
题意：
　　给出一个m*n的矩阵，robot要从[1][1]走到[m][n]，每次只能往下/右走，问有多少种走法？
思路：
　　DP的经典问题。先将[1][1]设为1，然后两种走法就是分别用[i][j]去更新[i+1][j]和[i][j+1]。
　　观察一下数组的更新次序就可以知道，这很像完全背包的更新方式，那么就可以用一维数组就行了。更新方式从左到右就行了。
　　由于这是DP题，就没有必要去研究数学公式了（涉及等差数列求和）。
```
1 class Solution {
 2 public:
 3     int uniquePaths(int m, int n) {
 4         vector<int> p(n,1);
 5         for(int i=1; i<m; i++)
 6             for(int j=0; j+1<n; j++)
 7                 p[j+1]+=p[j];
 8         return p[n-1];
 9     }
10 };
AC代码
```

