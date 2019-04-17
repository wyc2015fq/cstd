# [LeetCode] Subarray Product Less Than K 子数组乘积小于K - Grandyang - 博客园







# [[LeetCode] Subarray Product Less Than K 子数组乘积小于K](https://www.cnblogs.com/grandyang/p/7753959.html)







Your are given an array of positive integers `nums`.

Count and print the number of (contiguous) subarrays where the product of all the elements in the subarray is less than `k`.

**Example 1:**

**Input:** nums = [10, 5, 2, 6], k = 100
**Output:** 8
**Explanation:** The 8 subarrays that have product less than 100 are: [10], [5], [2], [6], [10, 5], [5, 2], [2, 6], [5, 2, 6].
Note that [10, 5, 2] is not included as the product of 100 is not strictly less than k.

**Note:**

- `0 < nums.length <= 50000`.
- `0 < nums[i] < 1000`.
- `0 <= k < 10^6`.




这道题给了我们一个数组和一个数字K，让我们求子数组且满足乘积小于K的个数。既然是子数组，那么必须是连续的，所以肯定不能给数组排序了，这道题好在限定了输入数字都是正数，能稍稍好做一点。博主刚开始用的是暴力搜索的方法来做的，就是遍历所有的子数组算乘积和K比较，两个for循环就行了，但是OJ不答应。于是上网搜大神们的解法，思路很赞。相当于是一种滑动窗口的解法，维护一个数字乘积刚好小于k的滑动窗口，用变量left来记录其左边界的位置，右边界i就是当前遍历到的位置。遍历原数组，用prod乘上当前遍历到的数字，然后进行while循环，如果prod大于等于k，则滑动窗口的左边界需要向右移动一位，删除最左边的数字，那么少了一个数字，乘积就会改变，所以用prod除以最左边的数字，然后左边右移一位，即left自增1。当我们确定了窗口的大小后，就可以统计子数组的个数了，就是窗口的大小。为啥呢，比如[5 2 6]这个窗口，k还是100，右边界刚滑到6这个位置，这个窗口的大小就是包含6的子数组乘积小于k的个数，即[6], [2 6], [5 2 6]，正好是3个。所以窗口每次向右增加一个数字，然后左边去掉需要去掉的数字后，窗口的大小就是新的子数组的个数，每次加到结果res中即可，参见代码如下：



```
class Solution {
public:
    int numSubarrayProductLessThanK(vector<int>& nums, int k) {
        if (k <= 1) return 0;
        int res = 0, prod = 1, left = 0;
        for (int i = 0; i < nums.size(); ++i) {
            prod *= nums[i];
            while (prod >= k) prod /= nums[left++];
            res += i - left + 1;
        }
        return res;
    }
};
```



讨论：这道题其实可有很多种变形，比如当数组的数字有负数和0该怎么做？或者求的不是子数组，而是子序列该怎么做，子序列的话就可以排序啦，当然还是需要都是正数，才有排序的意义，博主觉得如果有负数和0，是不是只能暴力破解了，或者使用[Maximum Product Subarray](http://www.cnblogs.com/grandyang/p/4028713.html)中的方法？再有一种的变形就是求子数组或子序列乘积刚好等于k，这就跟[Subarray Sum Equals K](http://www.cnblogs.com/grandyang/p/6810361.html)和[Maximum Size Subarray Sum Equals k](http://www.cnblogs.com/grandyang/p/5336668.html)这两题中使用的方法类似吧，建立子数组和其乘积之间的映射来快速找到。

欢迎大家在评论区留言讨论！



类似题目：

[Maximum Product Subarray](http://www.cnblogs.com/grandyang/p/4028713.html)

[Subarray Sum Equals K](http://www.cnblogs.com/grandyang/p/6810361.html)

[Maximum Size Subarray Sum Equals k](http://www.cnblogs.com/grandyang/p/5336668.html)



参考资料：

[https://leetcode.com/problems/subarray-product-less-than-k/](https://leetcode.com/problems/subarray-product-less-than-k/)

[https://leetcode.com/problems/subarray-product-less-than-k/discuss/108830/C%2B%2B-concise-solution-O(n)](https://leetcode.com/problems/subarray-product-less-than-k/discuss/108830/C%2B%2B-concise-solution-O(n))

[https://leetcode.com/problems/subarray-product-less-than-k/discuss/108861/JavaC%2B%2B-Clean-Code-with-Explanation](https://leetcode.com/problems/subarray-product-less-than-k/discuss/108861/JavaC%2B%2B-Clean-Code-with-Explanation)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












