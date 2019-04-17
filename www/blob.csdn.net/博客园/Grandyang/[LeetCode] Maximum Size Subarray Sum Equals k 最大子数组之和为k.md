# [LeetCode] Maximum Size Subarray Sum Equals k 最大子数组之和为k - Grandyang - 博客园







# [[LeetCode] Maximum Size Subarray Sum Equals k 最大子数组之和为k](https://www.cnblogs.com/grandyang/p/5336668.html)







Given an array *nums* and a target value *k*, find the maximum length of a subarray that sums to *k*. If there isn't one, return 0 instead.

**Example 1:**



Given *nums* = `[1, -1, 5, -2, 3]`, *k* = `3`,

return `4`. (because the subarray `[1, -1, 5, -2]` sums to 3 and is the longest)


**Example 2:**



Given *nums* = `[-2, -1, 2, 1]`, *k* = `1`,

return `2`. (because the subarray `[-1, 2]` sums to 1 and is the longest)


**Follow Up:**

    Can you do it in O(*n*) time?




这道题给我们一个一维数组nums，让我们求和为k最大子数组，默认子数组必须连续，题目中提醒我们必须要在O(n)的时间复杂度完成，我试了下brute force无法通过OJ，那么根据题目中的提示标签，我们需要用哈希表和累积和来做，关于累积和的用法可以参看我之前的博客[Range Sum Query - Immutable](http://www.cnblogs.com/grandyang/p/4952464.html)，那么建立累积和的好处显而易见，如果当前累积和正好等于k，那么从开头到此位置的子数组就是一个符合要求的解，但不一定是最长的子数组，而使用哈希表来建立累积和和其坐标之间的映射，我们就从题目中给的例子进行分析：

nums: [1, -1, 5, -2, 3], k = 3

sums: [1, 0, 5, 3, 6]

我们可以看到累积和的第四个数字为3，和k相同，则说明前四个数字就是符合题意的一个子数组，再来看第二个例子：

nums: [-2, -1, 2, 1], k = 1

sums: [-2, -3, -1, 0]

我们发现累积和中没有数字等于k，但是我们知道这个例子的答案是[-1, 2]，那么我们看累积和数组的第一和第三个数字，我们是否能看出一些规律呢，没错，第三个数字-1减去k，得到第一个数字，这就是规律，这也是累积和求区间和的方法，但是由于累计和数组中可能会有重复数字，而哈希表的关键字不能相同，比如下面这个例子：

nums: [1, 0, -1], k = -1

sums: [1, 1, 0]

我们发现累积和数组的第一个和第二个数字都为1，那么如何建立映射呢，我想的是用一个一维数组将其都存起来，然后比较的话就比较数组中的第一个数字，当我们建立完哈希表后，开始遍历这个哈希表，当累积和跟k相同时，我们更新res，不相同的话我们检测当前值减去k得到的值在哈希表中存不存在，如果存在就更新结果，参见代码如下：



解法一：

```
class Solution {
public:
    int maxSubArrayLen(vector<int>& nums, int k) {
        if (nums.empty()) return 0;
        int res = 0;
        unordered_map<int, vector<int>> m;
        m[nums[0]].push_back(0);
        vector<int> sum = nums;
        for (int i = 1; i < nums.size(); ++i) {
            sum[i] += sum[i - 1];
            m[sum[i]].push_back(i);
        }
        for (auto it : m) {
            if (it.first == k) res = max(res, it.second.back() + 1);
            else if (m.find(it.first - k) != m.end()) {
                res = max(res, it.second.back() - m[it.first - k][0]);
            }
        }
        return res;
    }
};
```



然而当我上网看大神们的解法时，才发现我图样图森破，根本不需要我写的那么复杂，我们不需要另外创建一个累积和的数组，而是直接用一个变量sum边累加边处理，而且我们哈希表也完全不用建立和一维数组的映射，只要保存第一个出现该累积和的位置，后面再出现直接跳过，这样算下来就是最长的子数组，对于想出这解法的人，博主只想说，阁下何不随风起，扶摇直上九万里～参见代码如下：



解法二：

```
class Solution {
public:
    int maxSubArrayLen(vector<int>& nums, int k) {
        int sum = 0, res = 0;
        unordered_map<int, int> m;
        for (int i = 0; i < nums.size(); ++i) {
            sum += nums[i];
            if (sum == k) res = i + 1;
            else if (m.count(sum - k)) res = max(res, i - m[sum - k]);
            if (!m.count(sum)) m[sum] = i;
        }
        return res;
    }
};
```



类似题目：

[Minimum Size Subarray Sum](http://www.cnblogs.com/grandyang/p/4501934.html)

[Range Sum Query - Immutable](http://www.cnblogs.com/grandyang/p/4952464.html)



参考资料：

[https://leetcode.com/discuss/77879/o-n-super-clean-9-line-java-solution-with-hashmap](https://leetcode.com/discuss/77879/o-n-super-clean-9-line-java-solution-with-hashmap)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












