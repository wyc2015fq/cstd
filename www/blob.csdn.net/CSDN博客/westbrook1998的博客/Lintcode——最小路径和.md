# Lintcode——最小路径和 - westbrook1998的博客 - CSDN博客





2018年02月19日 19:23:11[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：83








## 题目：

> 
给定一个只含非负整数的m*n网格，找到一条从左上角到右下角的可以使数字和最小的路径。

注意事项 

  你在同一时间只能向下或者向右移动一步


## 题解：

```
public class Solution {
    /*
     * @param grid: a list of lists of integers
     * @return: An integer, minimizes the sum of all numbers along its path
     */
    public int minPathSum(int[][] grid) {
        // write your code here
        int m=grid.length;
        int n=grid[0].length;
        int[][] dp=new int[m][n];
        dp[0][0]=grid[0][0];
        for(int i=0;i<m;i++){
            for(int j=0;j<n;j++){
                if(i==0 && j==0)
                    continue;
                else if(i==0){
                    dp[i][j]=dp[i][j-1]+grid[i][j];
                }
                else if(j==0){
                    dp[i][j]=dp[i-1][j]+grid[i][j];
                }
                else{
                    dp[i][j]=Math.min(dp[i-1][j],dp[i][j-1])+grid[i][j];
                }
            }
        }
        return dp[m-1][n-1];
    }
}
```



