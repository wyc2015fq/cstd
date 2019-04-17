# [LeetCode] Majority Element II 求大多数之二 - Grandyang - 博客园







# [[LeetCode] Majority Element II 求大多数之二](https://www.cnblogs.com/grandyang/p/4606822.html)







Given an integer array of size *n*, find all elements that appear more than `⌊ n/3 ⌋` times.

Note: The algorithm should run in linear time and in O(1) space.

Example 1:
Input: [3,2,3]
Output: [3]
Example 2:
Input: [1,1,1,3,3,2,2,2]
Output: [1,2]


这道题让我们求出现次数大于n/3的数字，而且限定了时间和空间复杂度，那么就不能排序，也不能使用哈希表，这么苛刻的限制条件只有一种方法能解了，那就是摩尔投票法 Moore Voting，这种方法在之前那道题 [Majority Element](http://www.cnblogs.com/grandyang/p/4233501.html) 中也使用了。题目中给了一条很重要的提示，让我们先考虑可能会有多少个这样的数字，经过举了很多例子分析得出，任意一个数组出现次数大于n/3的数最多有两个，具体的证明我就不会了，我也不是数学专业的。那么有了这个信息，我们使用投票法的核心是找出两个候选数进行投票，需要两遍遍历，第一遍历找出两个候选数，第二遍遍历重新投票验证这两个候选数是否为符合题意的数即可，选候选数方法和前面那篇 [Majority Element](http://www.cnblogs.com/grandyang/p/4233501.html) 一样，由于之前那题题目中限定了一定会有大多数存在，故而省略了验证候选众数的步骤，这道题却没有这种限定，即满足要求的大多数可能不存在，所以要有验证。代码如下：



```
class Solution {
public:
    vector<int> majorityElement(vector<int>& nums) {
        vector<int> res;
        int m = 0, n = 0, cm = 0, cn = 0;
        for (auto &a : nums) {
            if (a == m) ++cm;
            else if (a ==n) ++cn;
            else if (cm == 0) m = a, cm = 1;
            else if (cn == 0) n = a, cn = 1;
            else --cm, --cn;
        }
        cm = cn = 0;
        for (auto &a : nums) {
            if (a == m) ++cm;
            else if (a == n) ++cn;
        }
        if (cm > nums.size() / 3) res.push_back(m);
        if (cn > nums.size() / 3) res.push_back(n);
        return res;
    }
};
```



类似题目：

[Majority Element](http://www.cnblogs.com/grandyang/p/4233501.html)



参考资料：

[https://leetcode.com/problems/majority-element-ii/](https://leetcode.com/problems/majority-element-ii/)

[https://leetcode.com/discuss/42806/boyer-moore-majority-vote-algorithm-generalization](https://leetcode.com/discuss/42806/boyer-moore-majority-vote-algorithm-generalization)

[https://leetcode.com/discuss/42771/java-o-n-time-o-1-space-simplified-solution](https://leetcode.com/discuss/42771/java-o-n-time-o-1-space-simplified-solution)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












