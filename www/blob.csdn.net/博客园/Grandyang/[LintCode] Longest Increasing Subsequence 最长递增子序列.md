# [LintCode] Longest Increasing Subsequence 最长递增子序列 - Grandyang - 博客园







# [[LintCode] Longest Increasing Subsequence 最长递增子序列](https://www.cnblogs.com/grandyang/p/4891500.html)







Given a sequence of integers, find the longest increasing subsequence (LIS).

You code should return the length of the LIS.

Have you met this question in a real interview?





**Example**

For [5, 4, 1, 2, 3], the LIS  is [1, 2, 3], return 3

For [4, 2, 4, 5, 3, 7], the LIS is [4, 4, 5, 7], return 4



**Challenge**

Time complexity O(n^2) or O(nlogn)



**Clarification**

What's the definition of longest increasing subsequence?

    * The longest increasing subsequence problem is to find a subsequence of a given sequence in which the subsequence's elements are in sorted order, lowest to highest, and in which the subsequence is as long as possible. This subsequence is not necessarily contiguous, or unique.  

    * https://en.wikipedia.org/wiki/Longest_common_subsequence_problem



我们先来看一种类似Brute Force的方法，这种方法会找出所有的递增的子序列，并把它们都保存起来，最后再找出里面最长的那个，时间复杂度为O(n2)，参见代码如下：



```
class Solution {
public:
    /**
     * @param nums: The integer array
     * @return: The length of LIS (longest increasing subsequence)
     */
    int longestIncreasingSubsequence(vector<int> nums) {
        vector<vector<int> > solutions;
        longestIncreasingSubsequence(nums, solutions, 0);
        int res = 0;
        for (auto &a : solutions) {
            res = max(res, (int)a.size());
        }
        return res;
    }
    void longestIncreasingSubsequence(vector<int> &nums, vector<vector<int> > &solutions, int curIdx) {
        if (curIdx >= nums.size() || curIdx < 0) return;
        int cur = nums[curIdx];
        vector<int> best_solution;
        for (int i = 0; i < curIdx; ++i) {
            if (nums[i] <= cur) {
                best_solution = seqWithMaxLength(best_solution, solutions[i]);
            }
        }
        vector<int> new_solution = best_solution;
        new_solution.push_back(cur);
        solutions.push_back(new_solution);
        longestIncreasingSubsequence(nums, solutions, curIdx + 1);
    }
    vector<int> seqWithMaxLength(vector<int> &seq1, vector<int> &seq2) {
        if (seq1.empty()) return seq2;
        if (seq2.empty()) return seq1;
        return seq1.size() < seq2.size() ? seq2 : seq1;
    }  
};
```



还有两种方法，(未完待续。。)



参考资料：

[http://www.cnblogs.com/lishiblog/p/4190936.html](http://www.cnblogs.com/lishiblog/p/4190936.html)

[http://blog.xiaohuahua.org/2015/01/26/lintcode-longest-increasing-subsequence/](http://blog.xiaohuahua.org/2015/01/26/lintcode-longest-increasing-subsequence/)














