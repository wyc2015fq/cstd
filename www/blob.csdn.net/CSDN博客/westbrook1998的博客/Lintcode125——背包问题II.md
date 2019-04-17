# Lintcode125——背包问题II - westbrook1998的博客 - CSDN博客





2018年02月19日 19:00:19[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：65








## 题目：

> 
给出n个物品的体积A[i]和其价值V[i]，将他们装入一个大小为m的背包，最多能装入的总价值有多大？

注意事项 

  A[i], V[i], n, m均为整数。你不能将物品进行切分。你所挑选的物品总体积需要小于等于给定的m。  


## 题解：

```
public class Solution {
    /*
     * @param m: An integer m denotes the size of a backpack
     * @param A: Given n items with size A[i]
     * @param V: Given n items with value V[i]
     * @return: The maximum value
     */
    public int backPackII(int m, int[] A, int[] V) {
        // write your code here
        int n=A.length;
        int[][] dp=new int[n+1][m+1];
        for(int i=0;i<n;i++){
            for(int j=0;j<m;j++){
                dp[i][j]=0;
            }
        }
        for(int i=1;i<=n;i++){
            for(int j=1;j<=m;j++){
                if(A[i-1]<=j){
                    dp[i][j]=dp[i-1][j]>(dp[i-1][j-A[i-1]]+V[i-1])?dp[i-1][j]:(dp[i-1][j-A[i-1]]+V[i-1]);
                }
                else{
                    dp[i][j]=dp[i-1][j];
                }
            }
        }
        return dp[n][m];
    }
}
```

数组越界问题调了很久。。背包问题简单题 
**可以优化为用一维dp数组来解决**

```
public class Solution {
    /*
     * @param m: An integer m denotes the size of a backpack
     * @param A: Given n items with size A[i]
     * @param V: Given n items with value V[i]
     * @return: The maximum value
     */
    public int backPackII(int m, int[] A, int[] V) {
        // write your code here
        int n = A.length;
        int[] dp = new int[m + 1];
        for (int j = 0; j <= m; j++) {
            dp[j] = 0;
        }
        for (int i = 1; i <= n; i++) {
            for (int j = m; j >= A[i-1]; j--) {
                if (dp[j] < dp[j - A[i-1]] + V[i-1])           //用一维数组要注意逆序，如果顺序的话，更新后面dp数组的值时使用状态转移方程本应该用上一层的值，却用了已经更新后的这一层的值，所以出错。
                    dp[j] = dp[j - A[i-1]] + V[i-1];
            }
        }
        return dp[m];
    }
}
```

参考博客[https://www.cnblogs.com/lanhj/archive/2012/12/05/2802437.html](https://www.cnblogs.com/lanhj/archive/2012/12/05/2802437.html)



