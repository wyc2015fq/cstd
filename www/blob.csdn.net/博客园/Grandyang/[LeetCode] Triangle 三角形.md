# [LeetCode] Triangle 三角形 - Grandyang - 博客园







# [[LeetCode] Triangle 三角形](https://www.cnblogs.com/grandyang/p/4286274.html)







Given a triangle, find the minimum path sum from top to bottom. Each step you may move to adjacent numbers on the row below.

For example, given the following triangle
[
     [2],
    [3,4],
   [6,5,7],
  [4,1,8,3]
]



The minimum path sum from top to bottom is `11` (i.e., 2 + 3 + 5 + 1 = 11).

Note:
Bonus point if you are able to do this using only *O*(*n*) extra space, where *n* is the total number of rows in the triangle.



这道题给了我们一个二维数组组成的三角形，让我们寻找一条自上而下的路径，使得路径和最短。那么那道题后还是先考虑下暴力破解，我们可以发现如果要遍历所有的路径的话，那可以是阶乘级的时间复杂度啊，OJ必灭之，趁早断了念想比较好。必须要优化时间复杂度啊，题目中给的例子很容易把人带偏，让人误以为贪婪算法可以解题，因为看题例子中的红色数组，在根数字2的下方选小的数字3，在3的下方选小的数字5，在5的下方选小的数字1，每次只要选下一层相邻的两个数字中较小的一个，似乎就能得到答案了。其实是不对的，贪婪算法可以带到了局部最小，但不能保证每次都带到全局最小，很有可能在其他的分支的底层的数字突然变的超级小，但是贪婪算法已经将其他所有分支剪掉了。所以为了保证我们能得到全局最小，动态规划Dynamic Programming还是不二之选啊。其实这道题和 [Dungeon Game](http://www.cnblogs.com/grandyang/p/4233035.html) 非常的类似，都是用DP来求解的问题。那么其实我们可以不新建dp数组，而是直接复用triangle数组，我们希望一层一层的累加下来，从而使得 triangle[i][j] 是从最顶层到 (i, j) 位置的最小路径和，那么我们如何得到状态转移方程呢？其实也不难，因为每个结点能往下走的只有跟它相邻的两个数字，那么每个位置 (i, j) 也就只能从上层跟它相邻的两个位置过来，也就是 (i-1, j-1) 和 (i-1, j) 这两个位置，那么状态转移方程为：

triangle[i][j] = min(triangle[i - 1][j - 1], triangle[i - 1][j])

我们从第二行开始更新，注意两边的数字直接赋值上一行的边界值，那么最终我们只要在最底层找出值最小的数字，就是全局最小的路径和啦，代码如下：



解法一：

```
class Solution {
public:
    int minimumTotal(vector<vector<int>>& triangle) {
        for (int i = 1; i < triangle.size(); ++i) {
            for (int j = 0; j < triangle[i].size(); ++j) {
                if (j == 0) {
                    triangle[i][j] += triangle[i - 1][j];
                } else if (j == triangle[i].size() - 1) {
                    triangle[i][j] += triangle[i - 1][j - 1];
                } else {
                    triangle[i][j] += min(triangle[i - 1][j - 1], triangle[i - 1][j]);
                }
            }
        }
        return *min_element(triangle.back().begin(), triangle.back().end());
    }
};
```



这种方法可以通过OJ，但是毕竟修改了原始数组triangle，并不是很理想的方法。在网上搜到一种更好的DP方法，这种方法复制了三角形最后一行，作为用来更新的一位数组。然后逐个遍历这个DP数组，对于每个数字，和它之后的元素比较选择较小的再加上面一行相邻位置的元素做为新的元素，然后一层一层的向上扫描，整个过程和冒泡排序的原理差不多，最后最小的元素都冒到前面，第一个元素即为所求。代码如下：



解法二： 

```
class Solution {
public:
    int minimumTotal(vector<vector<int>>& triangle) {
        vector<int> dp(triangle.back());
        for (int i = (int)triangle.size() - 2; i >= 0; --i) {
            for (int j = 0; j <= i; ++j) {
                dp[j] = min(dp[j], dp[j + 1]) + triangle[i][j];
            }
        }
        return dp[0];
    }
};
```



下面我们来看一个例子，对于输入数组：

     -1

    2   3

  1  -1  -3

5   3   -1   2

下面我们来看DP数组的变换过程（红色数字为每次dp数组中值改变的位置）：

DP：5  3  -1  2

DP：4  3  -1  2

DP：4  -2  -1  2

DP：4  -2  -4  2

DP：0  -2  -4  2

DP：0  -1  -4  2

DP：-2  -1  -4  2



参考资料：

[https://leetcode.com/problems/triangle/](https://leetcode.com/problems/triangle/)

[https://leetcode.com/problems/triangle/discuss/38730/DP-Solution-for-Triangle](https://leetcode.com/problems/triangle/discuss/38730/DP-Solution-for-Triangle)

[https://leetcode.com/problems/triangle/discuss/38918/C%2B%2B-top-down-and-bottom-up-solutions.](https://leetcode.com/problems/triangle/discuss/38918/C%2B%2B-top-down-and-bottom-up-solutions.)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












