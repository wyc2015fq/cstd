# [LeetCode] Longest Continuous Increasing Subsequence 最长连续递增序列 - Grandyang - 博客园







# [[LeetCode] Longest Continuous Increasing Subsequence 最长连续递增序列](https://www.cnblogs.com/grandyang/p/7608976.html)







Given an unsorted array of integers, find the length of longest `continuous` increasing subsequence.

Example 1:
Input: [1,3,5,4,7]
Output: 3
Explanation: The longest continuous increasing subsequence is [1,3,5], its length is 3. 
Even though [1,3,5,7] is also an increasing subsequence, it's not a continuous one where 5 and 7 are separated by 4. 



Example 2:
Input: [2,2,2,2,2]
Output: 1
Explanation: The longest continuous increasing subsequence is [2], its length is 1. 



Note: Length of the array will not exceed 10,000.



这道题让我们求一个数组的最长连续递增序列，由于有了连续这个条件，跟之前那道[Number of Longest Increasing Subsequence](http://www.cnblogs.com/grandyang/p/7603903.html)比起来，其实难度就降低了很多。我们可以使用一个计数器，如果遇到大的数字，计数器自增1；如果是一个小的数字，则计数器重置为1。我们用一个变量cur来表示前一个数字，初始化为整型最大值，当前遍历到的数字num就和cur比较就行了，每次用cnt来更新结果res，参见代码如下：



解法一：

```
class Solution {
public:
    int findLengthOfLCIS(vector<int>& nums) {
        int res = 0, cnt = 0, cur = INT_MAX;
        for (int num : nums) {
            if (num > cur) ++cnt;
            else cnt = 1;
            res = max(res, cnt);
            cur = num;
        }
        return res;
    }
};
```



下面这种方法的思路和上面的解法一样，每次都和前面一个数字来比较，注意处理无法取到钱一个数字的情况，参见代码如下：



解法二：

```
class Solution {
public:
    int findLengthOfLCIS(vector<int>& nums) {
        int res = 0, cnt = 0, n = nums.size();
        for (int i = 0; i < n; ++i) {
            if (i == 0 || nums[i - 1] < nums[i]) res = max(res, ++cnt);
            else cnt = 1;
        }
        return res;
    }
};
```



类似题目：

[Number of Longest Increasing Subsequence](http://www.cnblogs.com/grandyang/p/7603903.html)



参考资料：

[https://discuss.leetcode.com/topic/102999/java-c-clean-solution](https://discuss.leetcode.com/topic/102999/java-c-clean-solution)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












