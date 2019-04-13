
# 598. Range Addition II - OraYang的博客 - CSDN博客

2017年08月28日 15:50:28[OraYang](https://me.csdn.net/u010665216)阅读数：227标签：[C++																](https://so.csdn.net/so/search/s.do?q=C++&t=blog)[leetcode																](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=C++&t=blog)个人分类：[Leetcode																](https://blog.csdn.net/u010665216/article/category/7026962)
所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)



题目：
Given an m * n matrixMinitialized with all0's and several update operations.
Operations are represented by a 2D array, and each operation is represented by an array with twopositiveintegersaandb, which meansM[i][j]should
 beadded by onefor all0 <= i < aand0 <= j < b.
You need to count and return the number of maximum integers in the matrix after performing all the operations.
Example 1:
Input:m = 3, n = 3
operations = [[2,2],[3,3]]Output:4Explanation:Initially, M = 
[[0, 0, 0],
 [0, 0, 0],
 [0, 0, 0]]
After performing [2,2], M = 
[[1, 1, 0],
 [1, 1, 0],
 [0, 0, 0]]
After performing [3,3], M = 
[[2, 2, 1],
 [2, 2, 1],
 [1, 1, 1]]
So the maximum integer in M is 2, and there are four of it in M. So return 4.
Note:
The range of m and n is [1,40000].
The range of a is [1,m], and the range of b is [1,n].
The range of operations size won't exceed 10,000.
思路：本题，虽然题目挺长，但是追究本质，只需以下几个步骤：
step1：遍历数组ops 第0列、第1列，找到最小值min_first,min_second
step2：返回min_first*min_second
代码：

```python
class Solution {
public:
    int maxCount(int m, int n, vector<vector<int>>& ops) {
        int ops_size = ops.size(),min_first=m,min_second=n,count = 0;
        for(int i= 0;i<ops_size;i++)
        {
            min_first = min_first<ops[i][0]?min_first:ops[i][0];
            min_second = min_second<ops[i][1]?min_second:ops[i][1];
        }
        return min_first*min_second;
    }
};
```


