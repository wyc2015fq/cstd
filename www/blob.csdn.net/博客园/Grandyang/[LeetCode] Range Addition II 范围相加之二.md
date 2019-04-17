# [LeetCode] Range Addition II 范围相加之二 - Grandyang - 博客园







# [[LeetCode] Range Addition II 范围相加之二](https://www.cnblogs.com/grandyang/p/6974232.html)







Given an m * n matrix M initialized with all 0's and several update operations.

Operations are represented by a 2D array, and each operation is represented by an array with two positive integers a and b, which means M[i][j] should be added by one for all 0 <= i < a and 0 <= j < b.

You need to count and return the number of maximum integers in the matrix after performing all the operations.

Example 1:
Input: 
m = 3, n = 3
operations = [[2,2],[3,3]]
Output: 4
Explanation: 
Initially, M = 
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
- The range of m and n is [1,40000].
- The range of a is [1,m], and the range of b is [1,n].
- The range of operations size won't exceed 10,000.



这道题看起来像是之前那道[Range Addition](http://www.cnblogs.com/grandyang/p/5628786.html)的拓展，但是感觉实际上更简单一些。每次在ops中给定我们一个横纵坐标，将这个子矩形范围内的数字全部自增1，让我们求最大数字的个数。原数组初始化均为0，那么如果ops为空，没有任何操作，那么直接返回m*n即可，我们可以用一个优先队列来保存最大数字矩阵的横纵坐标，我们可以通过举些例子发现，只有最小数字组成的边界中的数字才会被每次更新，所以我们想让最小的数字到队首，更优先队列的排序机制是大的数字在队首，所以我们对其取相反数，这样我们最后取出两个队列的队首数字相乘即为结果，参见代码如下：



解法一：

```
class Solution {
public:
    int maxCount(int m, int n, vector<vector<int>>& ops) {
        if (ops.empty() || ops[0].empty()) return m * n;
        priority_queue<int> r, c;
        for (auto op : ops) {
            r.push(-op[0]);
            c.push(-op[1]);
        }
        return r.top() * c.top();
    }
};
```



我们可以对空间进行优化，不使用优先队列，而是每次用ops中的值来更新m和n，取其中较小值，这样遍历完成后，m和n就是最大数矩阵的边界了，参见代码如下：



解法二：

```
class Solution {
public:
    int maxCount(int m, int n, vector<vector<int>>& ops) {
        for (auto op : ops) {
            m = min(m, op[0]);
            n = min(n, op[1]);
        }
        return m * n;
    }
};
```



类似题目：

[Range Addition](http://www.cnblogs.com/grandyang/p/5628786.html)



参考资料：

[https://discuss.leetcode.com/topic/90540/c-java-clean-code](https://discuss.leetcode.com/topic/90540/c-java-clean-code)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












