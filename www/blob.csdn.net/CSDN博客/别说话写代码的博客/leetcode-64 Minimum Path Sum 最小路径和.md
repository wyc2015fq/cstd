# leetcode-64. Minimum Path Sum 最小路径和 - 别说话写代码的博客 - CSDN博客





2019年01月26日 23:10:40[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：16标签：[最小路径和																[leetcode																[Minimum Path Sum](https://so.csdn.net/so/search/s.do?q=Minimum Path Sum&t=blog)
个人分类：[数据结构/算法/刷题](https://blog.csdn.net/qq_21997625/article/category/6193466)

所属专栏：[leetcode刷题c++/python](https://blog.csdn.net/column/details/29882.html)](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)



|Given a *m* x *n* grid filled with non-negative numbers, find a path from top left to bottom right which *minimizes* the sum of all numbers along its path.**Note:** You can only move either down or right at any point in time.**Example:****Input:**[  [1,3,1],  [1,5,1],  [4,2,1]]**Output:** 7**Explanation:** Because the path 1→3→1→1→1 minimizes the sum.|给定一个包含非负整数的 *m* x *n* 网格，请找出一条从左上角到右下角的路径，使得路径上的数字总和为最小。**说明：**每次只能向下或者向右移动一步。**示例:****输入:**[  [1,3,1],  [1,5,1],  [4,2,1]]**输出:** 7**解释:** 因为路径 1→3→1→1→1 的总和最小。|
|----|----|

思路：dp，用一个二维数组放 当前路径的最小值，res[i][j]=grid[i][j]+min(res[i-1][j],res[i][j-1])。当然也可以更简化只使用一维数组就可以了，就不写出来了。

```cpp
class Solution {
public:
    int minPathSum(vector<vector<int>>& grid) {
        int m=grid.size(),n=grid[0].size(),i,j;
        if(!n || !m) return 0;
        int res[m][n];
        res[0][0]=grid[0][0];
        for(i=1;i<m;++i) res[i][0]=grid[i][0]+res[i-1][0];
        for(i=1;i<n;++i) res[0][i]=grid[0][i]+res[0][i-1];
        for(i=1;i<m;++i)
            for(j=1;j<n;++j)
            {
                res[i][j]=grid[i][j]+min(res[i-1][j],res[i][j-1]);
            }
        return res[m-1][n-1];  //注意这里返回m-1,n-1，不要使用i-1,j-1作为返回下标
    }
};
```](https://so.csdn.net/so/search/s.do?q=最小路径和&t=blog)




