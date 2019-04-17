# [LeetCode] Binary Trees With Factors 带因子的二叉树 - Grandyang - 博客园







# [[LeetCode] Binary Trees With Factors 带因子的二叉树](https://www.cnblogs.com/grandyang/p/10223109.html)








Given an array of unique integers, each integer is strictly greater than 1.

We make a binary tree using these integers and each number may be used for any number of times.

Each non-leaf node's value should be equal to the product of the values of it's children.

How many binary trees can we make?  Return the answer modulo 10 ** 9 + 7.

Example 1:
Input: `A = [2, 4]`
Output: 3
Explanation: We can make these trees: `[2], [4], [4, 2, 2]`
Example 2:
Input: `A = [2, 4, 5, 10]`
Output: `7`
Explanation: We can make these trees: `[2], [4], [5], [10], [4, 2, 2], [10, 2, 5], [10, 5, 2]`.
Note:
- `1 <= A.length <= 1000`.
- `2 <= A[i] <= 10 ^ 9`.




这道题给了我们一些不相同的数字，每个都大于1，数字可以重复使用且可以部分使用，问我们可以建立多少棵二叉树使得每个非叶结点的值等于其左右子结点值的乘积。并提示了结果可能会很大，让我们把结果对一个超大数取余。看到这里，刷题老司机们应该瞬间反应过来了吧，应该是用动态规划Dynamic Programming来做，为啥呢，不可能去递归遍历所有的情况阿，这么大的数，机子都要爆了。好吧，既然选定了DP，两个难点，定义dp表达式跟推导状态转移方程。怎么简单怎么来呗，我们用一个一维dp数组，其中dp[i]表示值为i的结点做根结点时，能够形成的符合题意的二叉树的个数。这样我们将数组A中每个结点的dp值都累加起来就是最终的结果了。好了，有了定义式，接下来就是最大的难点了，推导状态转移方程。题目中的要求是根结点的值必须是左右子结点值的乘积，那么根结点的dp值一定是跟左右子结点的dp值有关的，是要加上左右子结点的dp值的乘积的，为啥是乘呢，比如有两个球，一个有2种颜色，另一个有3种颜色，问两个球放一起总共能有多少种不同的颜色组合，当然是相乘啦。每个结点的dp值初始化为1，因为就算是当个光杆司令的叶结点，也是符合题意的，所以至少是1。然后就要找其左右两个子结点了，怎么找，有点像 [Two Sum](http://www.cnblogs.com/grandyang/p/4130379.html) 的感觉，先确定一个，然后在HashMap中快速定位另一个，想到了这一层的话，我们的dp定义式就需要做个小修改，之前说的是用一个一维dp数组，现在看来就不太合适了，因为我们需要快速查找某个值，所以这里我们用一个HashMap来定义dp。好，继续，既然要先确定一个结点，由于都是大于1的正数，那么这个结点肯定要比根结点值小，为了遍历方便，我们想把小的放前面，那么我们就需要给数组A排个序，这样就可以遍历之前较小的数字了，那么如何快速定位另一个子结点呢，我们只要用根结点值对遍历值取余，若为0，说明可以整除，然后再在HashMap中查找这个商是否存在，在的话，说明存在这样的两个结点，其结点值之积等于结点A[i]，然后我们将这两个结点值之积加到dp[A[i]]中即可，注意还要对超大数取余，防止溢出。最后当所有结点的dp值都更新完成了，将其和算出来返回即可，参见代码如下：



```
class Solution {
public:
    int numFactoredBinaryTrees(vector<int>& A) {
        long res = 0, M = 1e9 + 7;
        unordered_map<int, long> dp;
        sort(A.begin(), A.end());
        for (int i = 0; i < A.size(); ++i) {
            dp[A[i]] = 1;
            for (int j = 0; j < i; ++j) {
                if (A[i] % A[j] == 0 && dp.count(A[i] / A[j])) {
                    dp[A[i]] = (dp[A[i]] + dp[A[j]] * dp[A[i] / A[j]]) % M;
                }
            }
        }
        for (auto a : dp) res = (res + a.second) % M;
        return res;
    }
};
```



类似题目：

[Two Sum](http://www.cnblogs.com/grandyang/p/4130379.html)



参考资料：

[https://leetcode.com/problems/binary-trees-with-factors/](https://leetcode.com/problems/binary-trees-with-factors/)

[https://leetcode.com/problems/binary-trees-with-factors/discuss/125794/C%2B%2BJavaPython-DP-solution](https://leetcode.com/problems/binary-trees-with-factors/discuss/125794/C%2B%2BJavaPython-DP-solution)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












